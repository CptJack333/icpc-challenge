#include "bits/stdc++.h"

using namespace std;

vector<bool> gen_prime(int n){
    vector<bool> isPrime(n + 1, true);
    isPrime[0] = isPrime[1] = false;

    for (int i = 2; i * i <= n; i++) {
        if (isPrime[i]) {
            // 把 i 的倍数全部标记为非质数
            for (int j = i * i; j <= n; j += i) {
                isPrime[j] = false;
            }
        }
    }
    return isPrime;
}

int main(){
    int t;
    cin>>t;
    for(int case_i=0;case_i<t;++case_i){
        int n;
        cin>>n;
        auto isPrime= gen_prime(n);

    }
}