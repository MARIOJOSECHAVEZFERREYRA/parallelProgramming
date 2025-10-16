#include <iostream>
#include <omp.h>
using namespace std;

int main(){
    #pragma omp parallel for num_threads(4)
    for(int i = 0; i < 4; i++){
            cout << "Hello World!" << endl;
    }
    return 0;
}