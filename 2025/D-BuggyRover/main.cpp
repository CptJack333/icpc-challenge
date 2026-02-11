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
    int current_r=sr , current_c=sc;
    for(auto p:path){
        int next_r= current_r + (p == 'S' ? 1 : (p == 'N' ? -1 : 0));
        int next_c= current_c + (p == 'E' ? 1 : (p == 'W' ? -1 : 0));

        auto filter_orders=[&](){
            for (int oi = 0; oi < 24; ++oi) {
                auto order = orders[oi];
                // auto matched=false;
                for (auto o: order) {
                    int order_r = current_r + (o == 'S' ? 1 : (o == 'N' ? -1 : 0));
                    int order_c = current_c + (o == 'E' ? 1 : (o == 'W' ? -1 : 0));
                    if (order_r >= r || order_r < 0 || order_c >= c || order_c < 0)continue;
                    if (g[order_r][order_c] == '#')continue;
                    if (next_r != order_r || next_c != order_c) {
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

        current_r=next_r;
        current_c=next_c;
    }

    cout<<hit_times<<endl;
    return 0;
}