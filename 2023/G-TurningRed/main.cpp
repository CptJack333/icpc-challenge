#include "bits/stdc++.h"
using namespace std;

int l,b;
vector<char> lights;
vector<vector<int>> button_control_lights;
vector<set<int>> light_related_button;
vector<int> solution;

int main(){
    cin>>l>>b;
    //灯和按钮的下标都是1开始
    lights.resize(l+1);
    light_related_button.resize(l+1);
    map<char,char> lmap={{'R',0},{'G',1},{'B',2}};
    for(int i=1;i<=l;++i){
        char t;
        cin>>t;
        lights[i]=lmap[t];
    }

    button_control_lights.resize(b+1);
    solution.resize(b+1);
    for(int i=1;i<=b;++i)solution[i]=-1;
    for(int i=1;i<=b;++i){
        int k;
        cin>>k;
        for(int j=0;j<k;++j){
            int x;
            cin>>x;
            light_related_button[x].insert(i);
            button_control_lights[i].push_back(x);
        }
    }
    int ans =0;

    class node{
    public:
        int type;//1 light 2 button
        int index;
    };
    vector<int> lvisited(l+1,false);
    vector<int> bvisited(b+1,false);
    for(int i=1;i<=l;++i){
        if(lvisited[l])continue;
        int min_tot=0x7FFFFFFF;
        bool found_sol=false;
        for(int i=0;i<=2;++i)
            for(int j=0;j<=2;++j){
            vector<int> connected_componet;
            std::function<void(node)> recur = [&](node n) {
                if (n.type == 1) {
                    if (lvisited[n.index])return;
                    lvisited[n.index] = true;
                    connected_componet.push_back(n.index);
                } else {
                    if (bvisited[n.index])return;
                    bvisited[n.index] = true;
                }
                auto related_button = light_related_button[li];
                auto but1 = *related_button.begin();
                auto but2 = related_button.size() == 2 ? *related_button.rbegin() : -1;

            };
        }
    }

        int min_tot=0x7FFFFFFF;
        bool found_sol=false;
        for(int i=0;i<=2;++i)
            for(int j=0;j<=2;++j){
                solution=vector<int>(solution.size(),-1);
                auto related_button=light_related_button[lf];
                if(related_button.size()>=1)
                    solution[*related_button.begin()]=i;
                if(related_button.size()==2)
                    solution[*related_button.rbegin()]=j;
//                visited=vector<int>(l+1,0);
                if(recur(lf)){
                    found_sol=true;
                    int tot=0;
                    for(int k=0;k<solution.size();++k)
                        if(solution[k]!=-1)
                            tot+=solution[k];
                    min_tot=min(tot,min_tot);
                }
            }
        if(!found_sol){
            cout<<"impossible"<<endl;
            return 0;
        }
        ans+=min_tot;

    cout<<ans<<endl;
    return 0;
}