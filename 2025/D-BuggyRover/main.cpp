#include "bits/stdc++.h"

using namespace std;

int main(){
    int r,c;
    cin>>r>>c;
    vector<vector<char>> g(r,vector<char>(c));
    int sr,sc;
    for(int i=0;i<r;++i)
        for(int j=0;j<c;++j){
            cin>>g[i][j];
            if(g[i][j]=='S')
                sr=i,sc=j;
        }
    string path;
    cin>>path;
    string first_order="ENSW";
    vector<string> orders;
    do{
        orders.push_back(first_order);
    }while(next_permutation(first_order.begin(), first_order.end()));
    int min_hit_times=0x7FFFFFFF;
    int rr=sr , cc=sc;
    for(auto order:orders){
        int hit_times=0;
        for(auto s:path){
            // auto matched=false;
            for(auto o:order){
                int nr=rr+(o=='S'?1:(o=='N'?-1:0)), nc=cc+(o=='E'?1:(o=='W'?-1:0));
                if(nr>=r ||nr<0 || nc>=c || nc<0)continue;
                if(g[nr][nc]=='.'){
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