#include "bits/stdc++.h"

using namespace std;

long long factorial(int x) {
    if(x==0||x==1)return 1;
    long long result = 1;
    for (int i = 2; i <= x; ++i) {
        result *= i;
    }
    return result;
}

int comb(int k){
    return factorial(k-1)/ factorial((k+1)/2)/ factorial(k/2-1);
}

int main(){
    int n,w;
    cin>>n>>w;
    int k=2;
    while(comb(k)<n)
        ++k;
    if(k>w) {
        cout <<"infinity"<<endl;
        return 0;
    }
    vector<int> permu;
    for(int i=0;i<k-1;++i){
        if(i<(k+1)/2)
            permu.push_back(1);
        else
            permu.push_back(0);
    }
    vector<vector<int>> sched;
    for(int i=0;i<n;++i){
        vector<int> sce{0};
        sce.insert(sce.end(),permu.begin(),permu.end());
        while(sce.size()<w)
            sce.insert(sce.end(),sce.begin(),sce.begin()+min((unsigned long)k,w-sce.size()));
        next_permutation(permu.begin(),permu.end());
        sched.push_back(sce);
    }
    cout<<k<<endl;
    for(int i=0;i<w;++i) {
        for (int j = 0; j < n; ++j)
            cout<<sched[j][i]+1;
        cout<<endl;
    }
    return 0;
}