#include "bits/stdc++.h"

using namespace std;

long long Cn2(long long n){
    if(n<2)return 0;
    return n*(n-1)/2;
}

vector<int> componets;//每个城市所属的连通分量
vector<int> componet_size;

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

    auto a_size=componet_size[a];
    auto b_size=componet_size[b];
    auto ab_intersect_size=intersecting_component[a].count(b)? intersecting_component[a][b]:0;


    //todo 交换ab，使得size小的往大的merge

    current_city_pairs-= Cn2(a_size);
    current_city_pairs-=Cn2(b_size);
    current_city_pairs+=Cn2(ab_intersect_size);//容斥原理
    auto merged_componet_size=a_size+b_size-ab_intersect_size;
    current_city_pairs+=Cn2(merged_componet_size);

    //todo 检查b的intersect compo，进行处理


    intersecting_component[a].erase(b);
    intersecting_component[b].clear();
    componets[b]=a;
}

int main(){
    int n,m;
    cin>>n>>m;
    componets.resize(n+1);
    componet_size.resize(n+1);
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