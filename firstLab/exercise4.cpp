#include <iostream>
#include <cstdio>
#include <omp.h>
using namespace std;

int main(){
    int k;
    bool ok = (cin >> k) && (k > 0);
    if (ok){
        #pragma omp parallel num_threads(k) default(none) shared(k)
        {
        int rank = omp_get_thread_num();
        printf("I am %d thread.\n", rank);
        }
    }
    return 0;
}