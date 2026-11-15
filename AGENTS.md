# Repository guidance

## Scope and purpose

This file applies to the entire `lab04` directory. This repository contains
C solutions to the three CPU scheduling assignments in section 4.5 of
`Lab 4.pdf`. Use the PDF as the assignment specification and `README.md`
for input formats and worked examples.

## Files

- `sjf.c`: non-preemptive Shortest Job First.
- `srt.c`: preemptive Shortest Remaining Time.
- `rr.c`: Round Robin with all arrival times equal to zero.
- `scheduler.h`: shared process structure, input validation, and reporting.
- `test_schedulers.py`: independent scheduling reference and input checks.
- `Makefile`: builds the three executables with a C11 compiler.

## Implementation conventions

- Use `bat --paging=never --style=plain <file>` when viewing source code.
  Use `rg` for searching file contents and locating files.
- Keep the solutions in portable C11 with standard library dependencies only.
  Use Python's standard library for the verification script.
- Keep each assignment executable independently through its existing source
  file and the shared header. Favor straightforward code suitable for a lab.
- Preserve unrelated user edits. Do not edit the assignment PDF or commit
  generated executables (`sjf`, `srt`, and `rr`).
- Use `long long` for times and accumulated timing statistics; preserve input
  validation and document any changes to accepted ranges.
- Keep the README consistent with input, output, and scheduling changes.

## Scheduling requirements

- SJF selects the shortest ready job and runs it to completion.
- SRT selects the ready job with the shortest remaining time and reconsiders
  that choice at arrivals and completions.
- For SJF and SRT, break equal scheduling priorities by earlier arrival,
  then input order. Handle unsorted arrivals and idle CPU intervals.
- RR starts with every process arriving at zero, uses input order for the
  initial ready queue, and prints each slice's process, start, and stop.
  Section 4.3's RR example has different arrivals; follow section 4.5 for
  assignment 3.
- Report response as first start minus arrival, turnaround as completion
  minus arrival, and waiting as turnaround minus burst.
- Keep result rows in input order and print averages to two decimal places.
  Assume zero context-switch overhead.

## Verification

After changing C code or the shared header, rebuild and run:

```sh
make CFLAGS='-std=c11 -O2 -Wall -Wextra -Wpedantic -Werror'
python3 test_schedulers.py
```

If changing only compiler flags, use `make -B` to force recompilation.
Check relevant cases such as preemption, ties, idle gaps, a single process,
invalid input, and RR quanta larger than a burst. Add focused checks when
fixing behavior not already covered. Documentation-only edits need no
compiler or scheduling test run.

## Editor diagnostics

These are C sources; use `-std=c11`. If clangd reports that `-std=c++20`
is invalid with C, inspect editor and global clangd flags. Do not convert
the sources to C++ to resolve an editor configuration issue.
