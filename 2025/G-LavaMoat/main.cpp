#include "bits/stdc++.h"

using namespace std;

int main(){
    int t;
    cin>>t;
    for(int ti=0;ti<t;++ti){
        int w,l,n,m;
        cin>>w>>l>>n>>m;
        vector<int> x(n+1),y(n+1),z(n+1);
        for(int i=1;i<=n;++i)
            cin>>x[i]>>y[i]>>z[i];
        vector<int> a(m+1),b(m+1),c(m+1);
        map<pair<int,int>,vector<pair<int,int>>> connect;
        set<pair<int,int>> start_seg, seg_end;
        for(int i=0;i<m;++i){
            cin>>a[i]>>b[i]>>c[i];
//            auto k1= make_pair(a[i],b[i]),k2= make_pair(a[i],c[i]),k3= make_pair(b[i],c[i]);
            pair<int,int> ks[3]={make_pair(a[i],b[i]),make_pair(a[i],c[i]), make_pair(b[i],c[i])};
            auto adjust_key=[&](pair<int,int>& k){if(k.first>k.second)swap(k.first,k.second);};
            for(auto & k:ks)adjust_key(k);
            for(int j=0;j<2;++j){
                connect[ks[j]].push_back(ks[(j+1)%3]);
                connect[ks[j]].push_back(ks[(j+2)%3]);
                auto& k=ks[j];
                if(x[k.first]==0 && x[k.second]==0)
                    start_seg.insert(k);
                if(x[k.first]==w&&x[k.second]==w)
                    seg_end.insert(k);
            }
        }

        //            从start seg开始遍历整个图，直到end seg
        set<pair<int,int>> visited_seg;
        vector<pair<int,int>> seg_route;
        vector<vector<pair<int,int>>> possible_route;
        function<void(pair<int,int>)> recur=[&](pair<int,int> v){
            if(visited_seg.count(v))return;
            visited_seg.insert(v);
            seg_route.push_back(v);
            if(seg_end.count(v))
                possible_route.push_back(seg_route);
            for(auto nv:connect[v])
                recur(nv);
            seg_route.pop_back();
            visited_seg.erase(v);
        };
        for(auto start:start_seg) {
            recur(start);
        }


//        for(auto start:start_seg){
//            seg_route.push_back(start);
//            visited_seg.insert(start);
//            for(auto v:connect[seg_route.back()]){
//                if(!visited_seg.count(v))
//                    seg_route.push_back(v);
//            }
//        }
    }
}