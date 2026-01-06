#include "bits/stdc++.h"
using namespace std;

int main(){
    int t,n,m,p,k;
    cin>>t;
    for(int tc=0;tc<t;++tc){
        bool cout_test_case=false;
        cin>>n>>m;
        if(cout_test_case)
            cout<<"test case "<<tc<<endl<<n<<m<<endl;
        vector<int> plant_after(m+1,0);
        vector<vector<set<int>>> pot_should_plant(m+1);
        for(int ri=0;ri<n;++ri){
            cin>>p>>k;
            if(cout_test_case)
                cout<<p<<k;
            set<int> plants;
            for(int rj=0;rj<k;++rj) {
                int plant_index;
                cin>>plant_index;
                if(cout_test_case)
                    cout<<plant_index;
                plants.insert(plant_index);
                plant_after[plant_index]=max(plant_after[plant_index],p);
            }
            pot_should_plant[p].push_back(plants);
            if(cout_test_case)
                cout<<endl;
        }
        vector<int> after_count(m+1,0);
        for(auto pot_index:plant_after)
            after_count[pot_index]++;
        //每盆艹可能被多个猫喜欢，检查多个猫中最大的盆，这个艹需要种在大于等于这个盆的位置
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
        //如果一个盆需要多个猫停在这，那需要种一颗对这些猫都吸引的艹
        vector<bool> catnip_planted(m+1,false);
        vector<bool> pot_planted(m+1,false);
        vector<set<int>> pot_possible_cn(m+1);
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
            int not_planted=set1.size();
            for(auto cn:set1){
                if(catnip_planted[cn])
                    --not_planted;
            }
            if (not_planted == 1) {
                catnip_planted[*set1.begin()] = true;
                pot_planted[pi] = true;
            }
            else
                pot_possible_cn[pi] = set1;
            if (set1.empty()||not_planted==0) {
                can_do = false;
                break;
            }
            if (set1.size() == 1) {
                catnip_planted[*set1.begin()] = true;
                pot_planted[pi] = true;
            }
        }
        if(!can_do) {
            cout << "no" << endl;
            continue;
        }
//        检查一颗艹有没有被重复种
//尝试把没种的艹种了
        int try_pot=1;
        vector<bool> try_planted(m+1,false);
        while(true){
            if(pot_planted[try_pot]){++try_pot;
                continue;}
            for(auto cn:pot_possible_cn[p]){
                try_planted[cn]=true;
            }
        }
        for(int pi=1;pi<=m;++pi){
            if(pot_planted[pi])continue;
//            用递归的方法去种，不断反复试验
            for(auto cn:pot_possible_cn[pi])
        }
        cout<<"yes"<<endl;
    }
}