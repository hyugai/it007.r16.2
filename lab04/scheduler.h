#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

/* Limits keep all accumulated times safely within long long. */
#define MAX_PROCESSES 10000
#define MAX_TIME 1000000000LL

typedef struct {
    char name[64];
    long long arrival, burst, remaining, start, finish;
} Process;

static void fail(const char *message)
{
    fprintf(stderr, "Error: %s\n", message);
    exit(EXIT_FAILURE);
}

/* Read a whole whitespace-delimited token, rejecting oversized tokens. */
static void read_token(char *buffer, size_t capacity)
{
    int c;
    size_t length = 0;
    do {
        c = getchar();
    } while (c == ' ' || c == '\t' || c == '\n' || c == '\r' ||
             c == '\v' || c == '\f');
    if (c == EOF)
        fail("unexpected end of input");
    while (c != EOF && c != ' ' && c != '\t' && c != '\n' &&
           c != '\r' && c != '\v' && c != '\f') {
        if (length + 1 >= capacity)
            fail("input token is too long");
        buffer[length++] = (char)c;
        c = getchar();
    }
    buffer[length] = '\0';
}

static long long read_number(long long minimum, long long maximum)
{
    char token[64], *end;
    read_token(token, sizeof token);
    errno = 0;
    long long value = strtoll(token, &end, 10);
    if (errno != 0 || *end != '\0' || value < minimum || value > maximum)
        fail("integer is invalid or outside the documented range");
    return value;
}

static Process *read_processes(int count, int with_arrivals)
{
    Process *processes = calloc((size_t)count, sizeof *processes);
    if (processes == NULL)
        fail("could not allocate process storage");
    printf(with_arrivals ? "Enter name, arrival time, burst time for each process:\n"
                         : "Enter name and burst time for each process:\n");
    for (int i = 0; i < count; ++i) {
        read_token(processes[i].name, sizeof processes[i].name);
        processes[i].arrival = with_arrivals ? read_number(0, MAX_TIME) : 0;
        processes[i].burst = read_number(1, MAX_TIME);
        processes[i].remaining = processes[i].burst;
        processes[i].start = -1;
    }
    return processes;
}

static void print_results(const Process *processes, int count)
{
    long long total_waiting = 0, total_turnaround = 0;
    puts("Process\tResponse\tWaiting\tTurnaround");
    for (int i = 0; i < count; ++i) {
        long long turnaround = processes[i].finish - processes[i].arrival;
        long long waiting = turnaround - processes[i].burst;
        long long response = processes[i].start - processes[i].arrival;
        printf("%s\t%lld\t%lld\t%lld\n", processes[i].name,
               response, waiting, turnaround);
        total_waiting += waiting;
        total_turnaround += turnaround;
    }
    printf("Average waiting time: %.2f\n", (double)total_waiting / count);
    printf("Average turnaround time: %.2f\n", (double)total_turnaround / count);
}

#endif
