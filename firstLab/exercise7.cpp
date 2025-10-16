#include <stdio.h>
#include <omp.h>

static void run_and_print(const char *title, omp_sched_t kind, int chunk, int k, int N) {
    int i;
    int owners[32]; /* suficiente para N=10 */

    printf("\n=== %s ===\n", title);

    for (i = 0; i <= N; i++) {
        owners[i] = -1;
    }

    omp_set_schedule(kind, chunk);

    #pragma omp parallel num_threads(k) shared(owners)
    {
        int tid = omp_get_thread_num();

    #pragma omp for schedule(runtime)
        for (int it = 1; it <= N; it++) {
            owners[it] = tid;
            printf("[%d]: calculation of the iteration number %d\n", tid, it);
        }
    }

    printf("Iteration :");
    for (i = 1; i <= N; i++) {
        printf(" %2d", i);
    }
    printf("\nThread   :");
    for (i = 1; i <= N; i++) {
        printf(" %2d", owners[i]);
    }
    printf("\n");
}

int main(void) {
    int k = 4;
    int N = 10;

    run_and_print("static",     omp_sched_static,  0, k, N);
    run_and_print("static, 1",  omp_sched_static,  1, k, N);
    run_and_print("static, 2",  omp_sched_static,  2, k, N);

    run_and_print("dynamic",    omp_sched_dynamic, 0, k, N);
    run_and_print("dynamic, 2", omp_sched_dynamic, 2, k, N);

    run_and_print("guided",     omp_sched_guided,  0, k, N);
    run_and_print("guided, 2",  omp_sched_guided,  2, k, N);

    return 0;
}
