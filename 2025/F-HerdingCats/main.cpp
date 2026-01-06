#include "bits/stdc++.h"
using namespace std;

set<int> set_intersect(vector<set<int>> sets){
    auto set1=sets.front();
    for(auto ss:sets) {
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
    return set1;
}

int main(){
    int t,n,m,p,k;
    cin>>t;
    for(int tc=0;tc<t;++tc){
        bool cout_test_case= false;
        cin>>n>>m;
        if(cout_test_case)
            cout<<"test case "<<tc<<endl<<n<<" "<<m<<endl;
        vector<int> catnip_lower_bound(m + 1, 0);
        set<int> key_pots;
        vector<vector<set<int>>> pot_should_plant(m+1);
//        读入数据，计算猫草lb
        for(int ri=0;ri<n;++ri){
            cin>>p>>k;
            key_pots.insert(p);
            if(cout_test_case)
                cout<<p<<" "<<k<<" ";
            set<int> plants;
            for(int rj=0;rj<k;++rj) {
                int plant_index;
                cin>>plant_index;
                if(cout_test_case)
                    cout<<plant_index<<" ";
                plants.insert(plant_index);
                catnip_lower_bound[plant_index]=max(catnip_lower_bound[plant_index], p);
            }
            pot_should_plant[p].push_back(plants);
            if(cout_test_case)
                cout<<endl;
        }
//        对关键盆计算应种草集合
        vector<int> after_count(m+1,0);
        bool can_do=true;
        //如果一个盆需要多个猫停在这，那需要种一颗对这些猫都吸引的艹
//        对关键盆计算应种草集合
        vector<bool> catnip_planted(m+1,false);
        for(auto pi :key_pots){
            auto set1=set_intersect(pot_should_plant[pi]);
//           过滤应种草集合，去除lb过高的
            set<int> set2;
            for(auto ci:set1)
                if(catnip_lower_bound[ci]<=pi)
                    set2.insert(ci);

            if(set2.empty()){
                can_do= false;
                goto finish;
            }
        }
//        对猫草按lb排序
        sort(catnip_lower_bound.begin(),catnip_lower_bound.end());
        for(int ii=1;ii<=m;++ii)
            if(catnip_lower_bound[ii]>ii) {
                can_do= false;
                goto finish;
            }
finish:
        cout<<(can_do?"yes":"no")<<endl;
//        cout<<"tc shit "<<tc<<" " <<(can_do?"yes":"no")<<endl;
    }
}