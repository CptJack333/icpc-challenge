#include <bits/stdc++.h>

using namespace std;

vector<long long> facts;

long long fact(int n){
    if(n==0 || n==1)return 1;
    int i=facts.size()-1;
    if(n<=i)
        return facts[n];
    long long tmp=n*fact(n-1);
    facts.push_back(tmp);
//    cout<<"fact "<<n<<" "<<tmp<<endl;
    return tmp;
}

long long factSum(int n){
    long long ret=0;
    for(int i=1;i<=n;++i){
        ret+=fact(i);
//        cout<<ret<<endl;
    }
    return ret;
}

int main(){
    facts.resize(2);
    facts[0]=1;
    facts[1]=1;
    int n=5;
    auto tmp=factSum(5);
    cout<<tmp<<endl;
    return 0;
}