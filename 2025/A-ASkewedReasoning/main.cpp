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
//    甚至可能不用算每个子树size，拔除的过程中判断就行了
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
    int cur=root;
    bool find_min=true;
    vector<int> ret;
    for(auto rep=0;rep<2;++rep){
        auto child_copy=child;
        auto parent_copy=parent;
        while (root) {
//        root可拔除，且仅有root可拔除
            if (!child[root].second) {
//            auto tmp=root;
                ret.push_back(root);
                parent[child[root].first] = 0;
                child[root].first = 0;
                root = child[root].first;
            }
            auto p = root;
            //找到left spine上第一个右子树为空 的节点，这个节点可以拔出
            while (child[p].second) {
                p = child[p].first;
            }
//        找到了一个叶子结点的父节点，叶子节点也可以拔除,在找最小序列的时候拔叶子
            if (find_min && child[p].first && !child[child[p].first].first) {
                auto leaf = child[p].first;
                parent[leaf] = 0;
                child[p].first = child[p].second;
                child[p].second = 0;
                ret.push_back(leaf);
            } else {
//            非叶子或者，最大序列的时候，拔枝干上的节点
                auto pparent = parent[p];
                auto lchild = child[p].first;
                parent[lchild] = pparent;
                child[pparent].first = lchild;
                parent[p] = 0;
                child[p].first = 0;
                ret.push_back(p);
            }
        }
        reverse(ret.begin(),ret.end());
        for(auto i:ret)cout<<i<<' ';
        cout<<endl;
        find_min=!find_min;
        child=child_copy;
        parent=parent_copy;
    }
    return 0;
}