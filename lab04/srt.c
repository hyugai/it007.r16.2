#include "scheduler.h"

/* Preemptive Shortest Remaining Time, reconsidered at each arrival. */
int main(void)
{
    printf("Enter the number of processes: ");
    int count = (int)read_number(1, MAX_PROCESSES);
    Process *p = read_processes(count, 1);
    long long now = 0;
    int completed = 0;

    while (completed < count) {
        int chosen = -1;
        long long next_arrival = -1;
        for (int i = 0; i < count; ++i) {
            if (p[i].remaining == 0)
                continue;
            if (p[i].arrival > now) {
                if (next_arrival == -1 || p[i].arrival < next_arrival)
                    next_arrival = p[i].arrival;
                continue;
            }
            /* Equal remaining time: earlier arrival, then input order. */
            if (chosen == -1 || p[i].remaining < p[chosen].remaining ||
                (p[i].remaining == p[chosen].remaining &&
                 p[i].arrival < p[chosen].arrival))
                chosen = i;
        }
        if (chosen == -1) {
            now = next_arrival;
            continue;
        }
        if (p[chosen].start == -1)
            p[chosen].start = now;
        long long duration = p[chosen].remaining;
        if (next_arrival != -1 && next_arrival - now < duration)
            duration = next_arrival - now;
        now += duration;
        p[chosen].remaining -= duration;
        if (p[chosen].remaining == 0) {
            p[chosen].finish = now;
            ++completed;
        }
    }

    print_results(p, count);
    free(p);
    return EXIT_SUCCESS;
}
