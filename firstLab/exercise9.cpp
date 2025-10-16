#include <iostream>
#include <omp.h>
#include <vector>
using namespace std;

static bool readSize(int& n){
    if(!(cin >> n)) return false;
    return n > 1 && n <= 10;
}

static bool readMatrix(vector<double>& M, int n){
    M.assign(n * n, 0.0);
    for(int i = 0; i < n * n; i++){
        if(!(cin >> M[i])) return false;
    }
    return true;
}

static void matmul(const vector<double>& A, const vector<double>& B, vector<double>& C, int n){
    C.assign(n*n, 0.0);
    #pragma omp parallel for schedule(runtime)
    for(int i = 0; i < n; i++){
        for(int j = 0; j < n; j++){
            double s = 0.0;
            for(int k = 0; k < n; k++){
                s += A[i * n + k] * B[k * n + j];
            }
            C[i * n + j] = s;
        }
    }
}

static void printMatrix(const vector<double>M, int n){
    for(int i = 0; i < n; i++){
        for(int j = 0; j < n; j++){
            if(j) cout << " ";
            cout << M[i * n + j];
        }
        cout << "\n";
    }
}
int main(){
    int n;
    if(!readSize(n)) return 1;

    vector<double> A, B, C;
    if(!readMatrix(A,n)) return 1;
    if(!readMatrix(B,n)) return 1;

    matmul(A,B,C,n);
    printMatrix(C, n);
    return 0;
}