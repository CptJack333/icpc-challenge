#include "bits/stdc++.h"
using namespace std;

int main(){
    int t,n,m,p,k;
    cin>>t;
    for(int tc=0;tc<t;++tc){
        cin>>n>>m;
        vector<int> plant_after(m+1,0);
        for(int ri=0;ri<n;++ri){
            cin>>p>>k;
            for(int rj=0;rj<k;++rj) {
                int plant_index;
                cin>>plant_index;
                plant_after[plant_index]=max(plant_after[plant_index],p);
            }
        }
        vector<int> after_count(m+1,0);
        for(auto pot_index:plant_after)
            after_count[pot_index]++;
        bool can_do=true;
        for(int pot_index=1; pot_index <= m; ++pot_index)
            if(after_count[pot_index] >m -pot_index+1){
                can_do=false;
                break;
            }
        if(can_do)
            cout<<"yes"<<endl;
        else
            cout<<"no"<<endl;
    }
}