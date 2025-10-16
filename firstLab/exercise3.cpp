#include <iostream>
#include <omp.h>
using namespace std;

int main(){
    int k;
    cin >> k;
    #pragma omp parallel  num_threads(k)
    {
    int tid = omp_get_thread_num();
    int nthreads = omp_get_num_threads();
    cout << "I am " << tid << " thread from " << nthreads << " threads!" << endl;
    }
    return 0;
}
