#include "bits/stdc++.h"
using namespace std;

int main(){
    int t,n,m,p,k;
    cin>>t;
    for(int tc=0;tc<t;++tc){
        cin>>n>>m;
        vector<int> plant_after(m+1,0);
        vector<vector<set<int>>> pot_should_plant(m+1);
        for(int ri=0;ri<n;++ri){
            cin>>p>>k;
            set<int> plants;
            for(int rj=0;rj<k;++rj) {
                int plant_index;
                cin>>plant_index;
                plants.insert(plant_index);
                plant_after[plant_index]=max(plant_after[plant_index],p);
            }
            pot_should_plant[p].push_back(plants);
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
        if(!can_do) {
            cout << "no" << endl;
            continue;
        }
        for(int pi=1;pi<=m;++pi){
            if(pot_should_plant[pi].empty())continue;
            auto set1=pot_should_plant[pi].front();
            for(auto ss:pot_should_plant[pi]) {
                std::set<int> temp_intersection; // 临时变量存储本次交集结果
                std::set_intersection(
                        set1.begin(), set1.end(),
                        ss.begin(), ss.end(),
                        std::inserter(temp_intersection, temp_intersection.begin())
                );
                set1 = std::move(temp_intersection); // 将本次交集结果赋给 set1，用于下次计算
                if (set1.empty()) { // 提前终止，交集已为空
                    break;
                }
            }
            if (set1.empty()) {
                can_do = false;
                break;
            }
        }
        if(!can_do) {
            cout << "no" << endl;
            continue;
        }
        cout<<"yes"<<endl;
    }
}