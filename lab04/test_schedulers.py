"""Run after `make`: python3 test_schedulers.py (standard library only)."""

from collections import deque
from pathlib import Path
import random
import subprocess

ROOT = Path(__file__).resolve().parent


def reference(algorithm, jobs, quantum):
    remaining = [burst for arrival, burst in jobs]
    starts = [None] * len(jobs)
    finishes = [None] * len(jobs)
    now = 0
    current = None
    queue = deque(range(len(jobs)))
    chart = []
    while any(remaining):
        if algorithm == "rr":
            current = queue.popleft()
            duration = min(quantum, remaining[current])
            chart.append((f"P{current}", now, now + duration))
        else:
            ready = [i for i, (arrival, _) in enumerate(jobs)
                     if arrival <= now and remaining[i]]
            if not ready:
                now += 1
                continue
            if algorithm == "srt" or current is None:
                current = min(ready, key=lambda i: (remaining[i], jobs[i][0], i))
            duration = 1  # Deliberately tick-based to check event-based code.
        if starts[current] is None:
            starts[current] = now
        now += duration
        remaining[current] -= duration
        if remaining[current] == 0:
            finishes[current] = now
            current = None
        elif algorithm == "rr":
            queue.append(current)
    rows = []
    for i, (arrival, burst) in enumerate(jobs):
        turnaround = finishes[i] - arrival
        rows.append((f"P{i}", starts[i] - arrival, turnaround - burst, turnaround))
    return rows, chart


def check(algorithm, jobs, quantum=4):
    data = [str(len(jobs))]
    if algorithm == "rr":
        data.append(str(quantum))
    for i, (arrival, burst) in enumerate(jobs):
        data.append(f"P{i} {burst}" if algorithm == "rr"
                    else f"P{i} {arrival} {burst}")
    result = subprocess.run([str(ROOT / algorithm)], input="\n".join(data),
                            text=True, capture_output=True, check=True)
    expected, chart = reference(algorithm, jobs, quantum)
    actual = []
    actual_chart = []
    for line in result.stdout.splitlines():
        fields = line.split()
        if fields and fields[0].startswith("P") and fields[0][1:].isdigit():
            row = (fields[0], *map(int, fields[1:]))
            (actual if len(fields) == 4 else actual_chart).append(row)
    assert actual == expected, (algorithm, jobs, actual, expected)
    assert actual_chart == chart, (algorithm, actual_chart, chart)
    waiting = sum(row[2] for row in expected) / len(jobs)
    turnaround = sum(row[3] for row in expected) / len(jobs)
    assert f"Average waiting time: {waiting:.2f}" in result.stdout
    assert f"Average turnaround time: {turnaround:.2f}" in result.stdout


def main():
    rng = random.Random(4)
    checks = 0
    for algorithm in ("sjf", "srt", "rr"):
        fixed = [[(0, 8), (1, 4), (2, 9), (3, 5)], [(5, 2)],
                 [(9, 1), (0, 2), (6, 2)], [(0, 3)] * 4,
                 [(0, 5), (2, 3), (2, 1)]]
        random_jobs = [[(rng.randrange(15), rng.randrange(1, 15))
                        for _ in range(rng.randrange(1, 9))] for _ in range(100)]
        for jobs in fixed + random_jobs:
            if algorithm == "rr":
                jobs = [(0, burst) for _, burst in jobs]
            check(algorithm, jobs, rng.randrange(1, 20))
            checks += 1
        invalid = ["0", "-1", "10001", "abc", "1", "1 P 0 0"]
        if algorithm == "rr":
            invalid += ["1 0", "1 -2", "1 4 P 0"]
        else:
            invalid += ["1 P -1 3", "1 P 0 -1", "1 P 0 2x"]
        for data in invalid:
            result = subprocess.run([str(ROOT / algorithm)], input=data,
                                    text=True, capture_output=True)
            assert result.returncode != 0, (algorithm, data)
            checks += 1
    print(f"Passed {checks} scheduling and invalid-input cases.")


if __name__ == "__main__":
    main()
