#include "bits/stdc++.h"

using namespace std;

int main(){
    int n;
    cin>>n;
    vector<int> tree_size(n+1,0);
    vector<pair<int,int>> child(n+1);
    vector<int> parent(n+1,0);
    for(int i=1;i<=n;++i){
        int l,r;
        cin>>l>>r;
        child[i]= make_pair(l,r);
        parent[l]=i;
        parent[r]=i;
    }
//    甚至可能不用算每个子树size，拔除的过程中判断就行了

    bool find_min=true;
    for(auto rep=0;rep<2;++rep){
        vector<int> ret;
        int root=1;// 题目条件，ℓi and ri (i < ℓi ≤ n or ℓi = 0; i < ri ≤ n or ri = 0)，说明root是1
        auto child_copy=child;
        auto parent_copy=parent;
        while (root) {
            auto take_out=[&](int p){
                auto ch =child[p];
                if(ch.second&&!ch.first){
                    cout<<"impossible"<<endl;
                    exit(0);
                }
                auto par=parent[p];
                child[par].first=child[p].first;
                parent[child[p].first]=par;
                child[p]={0,0};
                parent[p]=0;
                ret.push_back(p);
                p=par;
                while(p){
                    auto &ch = child[p];
                    swap(ch.first, ch.second);
                    p=parent[p];
                }
            };
//        root可拔除，还得看看是不是只剩最后两个了
            if (!child[root].second) {
                if(child[root].first&&!child[child[root].first].first&&find_min){
                    auto leaf=child[root].first;
                    take_out(leaf);
                    continue;
                }
                auto lch=child[root].first;
                take_out(root);
                root = lch;
                continue;
            }
            auto p = root;
            //找到left spine上第一个右子树为空 的节点，这个节点可以拔出
            while (child[p].second) {
                p = child[p].first;
            }
//        找到了一个叶子结点的父节点，叶子节点也可以拔除,在找最小序列的时候拔叶子
            if (find_min && child[p].first && !child[child[p].first].first) {
                auto leaf = child[p].first;
                take_out(leaf);
            } else {
//            非叶子或者，最大序列的时候，拔枝干上的节点
                take_out(p);
            }
        }
        reverse(ret.begin(),ret.end());
        stringstream ss;
        for(auto i:ret)ss<<i<<' ';
        auto st=ss.str();
        st.pop_back();
        cout<<st<<endl;
        find_min=!find_min;
        child=child_copy;
        parent=parent_copy;
    }
    return 0;
}