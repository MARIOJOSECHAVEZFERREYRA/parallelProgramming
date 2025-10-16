#include <iostream>
#include <cstdio>
#include <omp.h>
using namespace std;

int main(){
    int k;
    cin >> k;
    int totalSum = 0;
#pragma omp parallel num_threads(2) reduction(+:totalSum)
    {

        int tid = omp_get_thread_num();
        int partialSum = 0;
        if(tid == 0){
            for(int i = 1; i <= k/2; i++){
                partialSum += i;
            }
        } else {
            for(int i = k/2 + 1; i <= k; i++){
                partialSum += i;
            }
        }

        totalSum += partialSum;
        printf("[%d]: Sum = %d\n", tid, partialSum);
    }
    printf("Sum = %d\n", totalSum);
    return 0;
}