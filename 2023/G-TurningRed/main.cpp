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
        node(int t,int i):type(t),index(i){}
    };

    vector<int> lvisited(l+1,false);
    vector<int> bvisited(b+1,false);

    for(int li=1;li<=l;++li){
        if(lvisited[li])continue;

        int min_tot=0x7FFFFFFF;
        bool found_sol=false;
        vector<int> connected_componet;

        for(int i=0;i<=2;++i)
            for(int j=0;j<=2;++j){
            connected_componet.clear();

            std::function<bool(node)> recur = [&](node n)->bool {
                if (n.type == 1) {
                    if (lvisited[n.index])return true;
                    lvisited[n.index] = true;
                    connected_componet.push_back(n.index);
                } else {
                    if (bvisited[n.index])return true;
                    bvisited[n.index] = true;
                }

                if(n.type==2){
                    for(auto l3:button_control_lights[n.index])
                        if(!lvisited[l3] && !recur(node(1,l3)))
                            return false;
                    return true;
                }

                int press_times = (3 - lights[n.index]) % 3;
                //获取这个灯关联的两个按钮
                auto related_button = light_related_button[li];
                auto but1 = *related_button.begin();
                auto but2 = related_button.size() == 2 ? *related_button.rbegin() : -1;
                //获取两个按钮按下的次数
                auto p1 = solution[but1];
                auto p2 = but2 != -1 ? solution[but2] : 0;//如果只有一个按钮,虚拟按钮次数为0

                if (p1 != -1 && p2 != -1) {
                    if ((p1 + p2) % 3 != press_times)return false;
                } else {
                    auto &sol = p2 == -1 ? solution[but2] : solution[but1];
                    if (p1 == -1)swap(p1, p2);
                    p2 = press_times - p1;
                    while (p2 < 0)p2 += 3;
                    sol = p2;
                }

                for(auto b:light_related_button[n.index])
                    if(!bvisited[b] && !recur(node(2,b)))
                        return false;

                return true;
            };

            solution=vector<int>(solution.size(),-1);
            auto related_button=light_related_button[li];
            if(related_button.size()>=1)
                solution[*related_button.begin()]=i;
            if(related_button.size()==2)
                solution[*related_button.rbegin()]=j;
            if(recur(node(1,li))){
                found_sol=true;
                int tot=0;
                for(int k : solution)
                    if(k!=-1)
                        tot+=k;
                min_tot=min(tot,min_tot);
            }
            for(auto c:connected_componet)lvisited[c]=false;
        }
        if(!found_sol){
            cout<<"impossible"<<endl;
            return 0;
        }
        for(auto c:connected_componet)lvisited[c]=true;
        connected_componet.clear();
        ans+=min_tot;
    }

    cout<<ans<<endl;
    return 0;
}