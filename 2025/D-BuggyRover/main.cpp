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
    int hit_times=0;
    vector<bool> valid_orders(24,true);
    for(auto s:path){
        int rr=sr , cc=sc;
//        if (order=="NESW")
//            asm("int $0x3");
        for(int oi=0;oi<24;++oi){
            auto order=orders[oi];
            // auto matched=false;
            for(auto o:order){
                int nr=rr+(o=='S'?1:(o=='N'?-1:0)), nc=cc+(o=='E'?1:(o=='W'?-1:0));
                if(nr>=r ||nr<0 || nc>=c || nc<0)continue;
                if(g[nr][nc]=='.'){
                    if(o!=s)
                        valid_orders[oi]=false;
                    // matched=true;
                }
            }
        }
        bool no_valid_order=true;
        for(auto b:valid_orders)
            if(b){
                no_valid_order=false;
                break;
            }
        if(no_valid_order)
            ++hit_times;
            valid_orders=vector<bool>(24,true);
    }
    cout<<hit_times<<endl;
    return 0;
}