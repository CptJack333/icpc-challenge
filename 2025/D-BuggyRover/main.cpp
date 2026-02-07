#include "bits/stdc++.h"

using namespace std;

int main(){
    int r,c;
    cin>>r>>c;
    vector<vector<char>> g(r,vector<char>(c));
    int si,sj;
    for(int i=0;i<r;++i)
        for(int j=0;j<c;++j){
            cin>>g[i][j];
            if(g[i][j]=='S')
                si=i,sj=j;
        }
    string path;
    cin>>path;
    string order="ENSW";
    vector<string> orders;
    do{
        orders.push_back(order);
    }while(next_permutation(order.begin(),order.end()));
    int min_hit_times=0x7FFFFFFF;
    int ii=si , jj=sj;
    for(auto order:orders){
        int hit_times=0;
        for(auto s:path){
            // auto matched=false;
            for(auto o:order){
                int ni=ii+(o=='E'?1:(o=='W'?-1:0)), nj=jj+(o=='S'?1:(o=='N'?-1:0));
                if(ni>=c ||ni<0 || nj>=r || nj<0)continue;
                if(g[ni][nj]=='.'){
                    if(o!=s)
                        ++hit_times;
                    // matched=true;
                    break;
                }
            }

        }
        min_hit_times=min(min_hit_times,hit_times);
    }
    cout<<min_hit_times<<endl;
    return 0;
}