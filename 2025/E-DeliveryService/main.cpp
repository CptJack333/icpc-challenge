#include "bits/stdc++.h"

using namespace std;

vector<int> components;//每个城市所属的连通分量
vector<int> component_size;

long long current_city_pairs=0;
vector<map<int,int>> intersecting_component;//与下标的分量有共同城市节点的分量，value是共同城市节点的数目

long long Cn2(long long n){
    if(n<2)return 0;
    return n*(n-1)/2;
}

int find_component_head(int c){
    if(components[c] == c)return c;
    components[c]= find_component_head(components[c]);
    return components[c];
}

void merge_components(int a, int b){
    a= find_component_head(a);
    b= find_component_head(b);

    if(a==b)return;

    //根据大小交换ab，使得size小的往大的merge，不做这步运行会很慢！
    if(intersecting_component[a].size()<intersecting_component[b].size())
        swap(a,b);

    auto a_size=component_size[a];
    auto b_size=component_size[b];
//    auto ab_intersect_size=intersecting_component[a].count(b)? intersecting_component[a][b]:0;
    auto ab_intersect_size=intersecting_component[a][b];

    current_city_pairs-= Cn2(a_size);
    current_city_pairs-=Cn2(b_size);
    current_city_pairs+=Cn2(ab_intersect_size);//容斥原理

    auto merged_component_size= a_size + b_size - ab_intersect_size;
    current_city_pairs+=Cn2(merged_component_size);

    intersecting_component[a].erase(b);
    intersecting_component[b].erase(a);

    //检查b的intersect compo，进行处理
    for(auto [c,b_inter_c_size]:intersecting_component[b]){
//        auto a_inter_c_size=intersecting_component[a].count(c)?intersecting_component[a][c]:0;
        auto a_inter_c_size=intersecting_component[a][c];

        //根据容斥原理重新调整merge后的新a和c的城市对
        current_city_pairs-= Cn2(a_inter_c_size+b_inter_c_size);//减去current_city_pairs+=Cn2(merged_component_size)里面多计算了的
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
    components[b]=a;
    component_size[a]+=merged_component_size;
    component_size[b]=0;
}

int main(){
    int n,m;
    cin>>n>>m;
    //下标1-n是出节点，下标n+1-2n是入节点
    components.resize(2*n + 1);
    component_size.resize(2*n + 1);
    intersecting_component.resize(2*n+1);
    for(int i=1;i<=2*n;++i)
        components[i]=i,
        component_size[i]=1;
    // Initialize overlaps
    // Each city i connects i_out and i_in via its existence.
    // So initially, component i (out) and component i+n (in) have overlap of 1 (city i itself).
    for(int i=1;i<=n;++i){
        intersecting_component[i][i+n]=1;
        intersecting_component[i+n][i]=1;
    }


    for(int i=1;i<=m;++i){
        int dep,arr;
        cin>>dep>>arr;

        merge_components(dep,arr+n);

        cout<<current_city_pairs<<endl;
    }

    return 0;
}