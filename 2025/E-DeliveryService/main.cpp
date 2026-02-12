#include "bits/stdc++.h"

using namespace std;

long long Cn2(long long n){
    if(n<2)return 0;
    return n*(n-1)/2;
}

vector<int> componets;//每个城市所属的连通分量
vector<int> component_size;

int find_componet_head(int c){
    if(componets[c]==c)return c;
    componets[c]= find_componet_head(componets[c]);
    return componets[c];
}

long long current_city_pairs=0;
vector<map<int,int>> intersecting_component;//与下标的分量有共同城市节点的分量，value是共同城市节点的数目

void merge_componets(int a, int b){
    a= find_componet_head(a);
    b= find_componet_head(b);

    auto a_size=component_size[a];
    auto b_size=component_size[b];
    auto ab_intersect_size=intersecting_component[a].count(b)? intersecting_component[a][b]:0;


    //todo 交换ab，使得size小的往大的merge

    current_city_pairs-= Cn2(a_size);
    current_city_pairs-=Cn2(b_size);
    current_city_pairs+=Cn2(ab_intersect_size);//容斥原理
    auto merged_componet_size=a_size+b_size-ab_intersect_size;
    current_city_pairs+=Cn2(merged_componet_size);

    //检查b的intersect compo，进行处理
    for(auto [c,b_inter_c_size]:intersecting_component[b]){
        auto a_inter_c_size=intersecting_component[a].count(c)?intersecting_component[a][c]:0;

        //根据容斥原理重新调整merge后的新a和c的城市对
        current_city_pairs-= Cn2(a_inter_c_size+b_inter_c_size);//减去current_city_pairs+=Cn2(merged_componet_size)里面多计算了的
        current_city_pairs+=Cn2(a_inter_c_size);//这两个是上面double count了的，加回来
        current_city_pairs+=Cn2(b_inter_c_size);

//        调整数据结构，达到merge效果
        intersecting_component[a][c]+=b_inter_c_size;
        intersecting_component[c].erase(b);
        intersecting_component[c][a]=intersecting_component[a][c];
    }

//        调整数据结构，达到merge效果
    intersecting_component[a].erase(b);
    intersecting_component[b].clear();
    componets[b]=a;
    component_size[a]+=component_size[b];
    component_size[b]=0;
}

int main(){
    int n,m;
    cin>>n>>m;
    componets.resize(n+1);
    component_size.resize(n + 1);
    intersecting_component.resize(n+1);
    for(int i=1;i<=n;++i)
        componets[i]=i;

    for(int i=1;i<=m;++i){
        int dep,arr;
        cin>>dep>>arr;

        merge_componets(dep,arr);

        cout<<current_city_pairs<<endl;
    }

    return 0;
}