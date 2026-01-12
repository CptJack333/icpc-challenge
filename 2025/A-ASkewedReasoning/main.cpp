#include "bits/stdc++.h"

using namespace std;

int main(){
    int n;
    cin>>n;
    vector<int> tree_size(n,0);
    vector<pair<int,int>> child(n);
    vector<int> parent(n,0);
    for(int i=1;i<=n;++i){
        int l,r;
        cin>>l>>r;
        child[n]= make_pair(l,r);
        parent[l]=i;
        parent[r]=i;
    }
    function<int(int)> calc_tree_size=[&](int root)->int{
        if(!root)return 0;
        auto [l,r]=child[root];
        return tree_size[root]=1+calc_tree_size(l)+calc_tree_size(r);
    };

}