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
    int pr=sr , pc=sc;
    for(auto p:path){
//        if (order=="NESW")
//            asm("int $0x3");
        int npr=pr+(p=='S'?1:(p=='N'?-1:0));
        int npc=pc+(p=='E'?1:(p=='W'?-1:0));
        auto filter_orders=[&](){
            for (int oi = 0; oi < 24; ++oi) {
                auto order = orders[oi];
                // auto matched=false;
                for (auto o: order) {
                    int nr = pr + (o == 'S' ? 1 : (o == 'N' ? -1 : 0));
                    int nc = pc + (o == 'E' ? 1 : (o == 'W' ? -1 : 0));
                    if (nr >= r || nr < 0 || nc >= c || nc < 0)continue;
                    if (g[nr][nc] == '#')continue;
                    if (npr != nr || npc != nc) {
                        valid_orders[oi] = false;
                    }
                    break;
                }
            }
        };
        filter_orders();
        bool no_valid_order=true;
        for(auto b:valid_orders)
            if(b){
                no_valid_order=false;
                break;
            }
        if(no_valid_order){
            ++hit_times;
            valid_orders=vector<bool>(24,true);
            filter_orders();
        }
        pr=npr;
        pc=npc;
    }
    cout<<hit_times<<endl;
    return 0;
}