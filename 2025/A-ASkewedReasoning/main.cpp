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
        auto lsize=calc_tree_size(l), rsize=calc_tree_size(r);
        if(lsize==-1||rsize==-1)
            return -1;
        if(0==lsize&&rsize>0)
            return -1;
        return tree_size[root]=1+lsize+rsize;
    };
    if(calc_tree_size(1)==-1){
        cout<<"impossible"<<endl;
        return 0;
    }

    int root=1;// 题目条件，ℓi and ri (i < ℓi ≤ n or ℓi = 0; i < ri ≤ n or ri = 0)，说明root是1
    int min_spine=0x7FFFFFFF;
//    从左边的脊梁骨里面选出一个最小/大的节点，并且不能有右子树
    while(child[root].first || child[root].second){
        if(child[root].second && !child[root].first){
            cout<<"impossible"<<endl;
            return 0;
        }
        if(!child[root].second)
            min_spine=min(root,min_spine);
        root=child[root].first;

    }
}