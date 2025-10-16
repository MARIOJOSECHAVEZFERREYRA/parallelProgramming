#include <iostream>
#include <cstdio>
#include <omp.h>
using namespace std;

int main(){
    int k,n;
    cin >> k;
    cin >> n;
    int totalSum = 0;
#pragma omp parallel num_threads(k) reduction(+:totalSum)
    {

        int tid = omp_get_thread_num();
        int partialSum = 0;
        #pragma omp for
        for(int i = 1; i <= n; i++){
            partialSum += i;
        }
        totalSum += partialSum;
        printf("[%d]: Sum = %d\n", tid, partialSum);
    }
    printf("Sum = %d\n", totalSum);
    return 0;
}