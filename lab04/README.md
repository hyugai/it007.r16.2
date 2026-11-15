# Lab 4: CPU scheduling

Solutions to the three assignments in section 4.5 of `Lab 4.pdf`.
Each C program uses the shared input and reporting helpers in `scheduler.h`.
No external libraries are needed.

Build and run:

```sh
make
./sjf
./srt
./rr
```

Verify the programs against an independent tick-based simulation and invalid
input cases with `python3 test_schedulers.py`.

## 1. Shortest Job First (`sjf.c`)

Input: process count, followed by `name arrival burst` for each process.
Whenever the CPU is free, select the shortest burst among processes that
have arrived, then run that process to completion (non-preemptive).
Ties use earlier arrival, then input order. If no process is ready, advance
the clock to the next arrival. Input need not be sorted.

Example input:

```text
4
P1 0 8
P2 1 4
P3 2 9
P4 3 5
```

Execution: `P1 [0,8), P2 [8,12), P4 [12,17), P3 [17,26)`.

| Process | Response | Waiting | Turnaround |
| --- | ---: | ---: | ---: |
| P1 | 0 | 0 | 8 |
| P2 | 7 | 7 | 11 |
| P3 | 15 | 15 | 24 |
| P4 | 9 | 9 | 14 |

Average waiting: **7.75**. Average turnaround: **14.25**.

## 2. Shortest Remaining Time (`srt.c`)

Input has the same format as SJF. At each arrival or completion, select
the ready process with the smallest remaining burst. A newly arrived
shorter process can preempt the running process. Ties use earlier arrival,
then input order. The simulation advances directly between events.

For the same example:
`P1 [0,1), P2 [1,5), P4 [5,10), P1 [10,17), P3 [17,26)`.

| Process | Response | Waiting | Turnaround |
| --- | ---: | ---: | ---: |
| P1 | 0 | 9 | 17 |
| P2 | 0 | 0 | 4 |
| P3 | 15 | 15 | 24 |
| P4 | 2 | 2 | 7 |

Average waiting: **6.50**. Average turnaround: **13.00**.

## 3. Round Robin (`rr.c`)

As required by the assignment, all arrivals are zero. Input: process
count, quantum, then `name burst` for each process. Visit unfinished
processes in input order repeatedly, executing at most one quantum each.
Print every time slice as a Gantt chart row with process name, start, and stop.

Example input:

```text
3
4
P1 24
P2 3
P3 3
```

Gantt chart:

```text
Process Start Stop
P1      0     4
P2      4     7
P3      7     10
P1      10    14
P1      14    18
P1      18    22
P1      22    26
P1      26    30
```

Average waiting: **5.67**. Average turnaround: **15.67**.
These differ from the PDF's RR example because that example uses arrivals
0, 1, 2, whereas assignment 3 explicitly requires all arrivals to be zero.

## Timing definitions and input limits

- Response = first start − arrival.
- Turnaround = completion − arrival.
- Waiting = turnaround − burst, including every wait after preemption.
- Each average is the sum of its per-process values divided by the process count.

Results are printed in input order, with averages to two decimal places.
There is no context-switch overhead. Times are integer units; process count
must be 1–10,000, arrivals 0–1,000,000,000, and bursts and quantum
1–1,000,000,000. Names are whitespace-free tokens of up to 63 bytes.
Invalid or incomplete input exits with an error. Use distinct names to make
output easy to identify. RR prints one row per slice, so small quanta with
large bursts can produce substantial output.
