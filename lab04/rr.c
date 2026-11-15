#include "scheduler.h"

/* All arrivals are zero; input order is the initial ready-queue order. */
int main(void)
{
    printf("Enter the number of processes: ");
    int count = (int)read_number(1, MAX_PROCESSES);
    printf("Enter quantum time: ");
    long long quantum = read_number(1, MAX_TIME);
    Process *p = read_processes(count, 0);
    long long now = 0;
    int completed = 0;

    puts("Gantt chart:");
    puts("Process\tStart\tStop");
    while (completed < count) {
        for (int i = 0; i < count; ++i) {
            if (p[i].remaining == 0)
                continue;
            if (p[i].start == -1)
                p[i].start = now;
            long long duration = p[i].remaining < quantum
                               ? p[i].remaining : quantum;
            printf("%s\t%lld\t%lld\n", p[i].name, now, now + duration);
            now += duration;
            p[i].remaining -= duration;
            if (p[i].remaining == 0) {
                p[i].finish = now;
                ++completed;
            }
        }
    }

    print_results(p, count);
    free(p);
    return EXIT_SUCCESS;
}
