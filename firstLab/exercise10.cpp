#include <iostream>
#include <omp.h>
using namespace std;

static bool readK(int& k){
    if(!(cin >> k)) return false;
    return k > 1;
}

static void runSections(const int& k){
    #pragma omp parallel num_threads(k)
    {
        int tid = omp_get_thread_num();
        cout << "[" << tid << "]: parallel region\n";
        #pragma omp sections
        {
            #pragma omp section
            {
                cout << "[" << omp_get_thread_num() << "]: came in section 1\n";
            }
            #pragma omp section
            {
                cout << "[" << omp_get_thread_num() << "]: came in section 2\n";
            }
            #pragma omp section
            {
                cout << "[" << omp_get_thread_num() << "]: came in section 3\n";
            }
        }
    }
}

int main(){
    int k;
    if(!readK(k)) return 1;
    runSections(k);
    return 0;
}