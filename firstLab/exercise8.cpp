#include <iostream>
#include <iomanip>
#include <omp.h>

using namespace std;

static double compute_pi(long long N) {
    if (N <= 0) return 0.0;

    const double step = 1.0 / static_cast<double>(N);
    double sum = 0.0;

    #pragma omp parallel
    {
        double local_sum = 0.0;

        #pragma omp for schedule(runtime)
        for (long long i = 0; i < N; ++i) {
            const double x = (i + 0.5) * step;
            local_sum += 4.0 / (1.0 + x * x);
        }

        #pragma omp critical
        {
            sum += local_sum;
        }
    }

    return step * sum;
}

int main() {
    long long N;
    if (!(cin >> N)) {
        cerr << "not an integer N\n";
        return 1;
    }

    const double t0 = omp_get_wtime();
    const double pi = compute_pi(N);
    const double t1 = omp_get_wtime();

    cout << fixed << setprecision(8) << pi << '\n';
    cerr << "N=" << N << ", threads=" << omp_get_max_threads()
         << ", time=" << fixed << setprecision(3) << (t1 - t0) << "s\n";
    return 0;
}
