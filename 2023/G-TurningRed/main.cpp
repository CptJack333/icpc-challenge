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
    auto lights_copy=lights;

    vector<vector<int>> g(l+1,vector<int>(l+1,0));

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

    vector<set<int>> light_related_button_org=light_related_button;
//    for(auto s:light_related_button){
//        set<int> new_set;
//        for(auto v:s){
//            new_set.insert(v);
//        }
//        light_related_button_org.push_back(new_set);
//    }

    vector<vector<int>> connected_components;
    int light_cnt=0;
    vector<bool> visited_li(l+1,false);
    while(light_cnt<l){
        int i=1;
        for(;i<=l;i++)if(!visited_li[i])break;
        vector<int> comp;
        std::function<void(int)> recur=[&](int li){
            visited_li[li]=true;
            ++light_cnt;
            comp.push_back(li);
            for(auto bu :light_related_button[li])
                for(auto l2:button_control_lights[bu])
                    if(!visited_li[l2])
                        recur(l2);
        };
        recur(i);
        connected_components.push_back(comp);
    }

    int ans =0;

    for(auto cc:connected_components){
        if(cc.size()==1 && lights[cc.front()]!=0){
            cout<<"impossilbe"<<endl;
            return 0;
        }

        auto l=cc.front();
        set<int> visited;

        std::function<bool(int)> recur=[&](int l)->bool{
            if (visited.count(l))return true;
            visited.insert(l);

            int press_times = (3 - lights[l]) % 3;
            //获取这个灯关联的两个按钮
            auto related_button=light_related_button[l];
            auto but1=*related_button.begin();
            auto but2=related_button.size()==2?*related_button.rbegin():-1;
            //获取两个按钮按下的次数
            auto p1=solution[but1];
            auto p2=but2!=-1?solution[but2]:0;//如果只有一个按钮,虚拟按钮次数为0
            //如果只有一个按钮,这个按钮的sol必然为0/1/2
            //如果有两个按钮,则需要通过一个按钮的次数,计算另一个的次数
            if(related_button.size()==2){
                if(p1==-1)p1=press_times-p2;
                if(p2==-1)p2=press_times-p1;
            }
            //如果上面传入的按钮次数不合理，无解
            if(p1<0||p2<0)return false;
            if((p1+p2)%3!=press_times)return false;

            solution[but2]=press_times-solution[but1];

            //遍历两个按钮关联的其它灯
            if(but1!=-1)
            for(auto l2:button_control_lights[but1]){
                auto b=recur(l2);
                if(!b)return false;
            }
            if(but2!=-1)
            for(auto l2:button_control_lights[but2]){
                auto b=recur(l2);
                if(!b)return false;
            }
            return true;
        };

        int min_tot=(2>>31)-1;
        bool found_sol=false;
        for(int i=0;i<=2;++i)
            for(int j=0;j<=2;++j){
                for(int k=0;k<solution.size();++k)solution[k]=-1;
                auto related_button=light_related_button[l];
                if(related_button.size()>=1)
                    solution[*related_button.begin()]=i;
                if(related_button.size()==2)
                    solution[*related_button.rbegin()]=j;
                if(recur(l)){
                    found_sol=true;
                    int tot=0;
                    for(int k=0;k<solution.size();++k)
                        if(solution[k]!=-1)
                            tot+=solution[k];
                    min_tot=min(tot,min_tot);
                }
            }
        if(!found_sol){
            cout<<"impossilbe"<<endl;
            return 0;
        }
        ans+=min_tot;
    }
    for (int i = 1; i <= l; ++i){
        if(lights[i]!=0&&light_related_button_org[i].size()==2){
            int press_times = (3 - lights[i]) % 3;
            int but1=*light_related_button_org[i].begin(), but2=*light_related_button_org[i].rbegin();
            for(int p1=0;p1<=2;++p1)
                for(int p2=0;p2<=2;++p2){
                    if(p1+p2!=press_times)continue;
                    solution
                }
            try_but()
        }
    }

    //先解出只被一个按钮控制的灯的按压次数
    while(true){
        int i=1;
        for (;i<=l;++i) {
            if (light_related_button[i].size() == 0 && lights[i] != 0) {
                cout << "impossible" << endl;
                return 0;
            }
            if (light_related_button[i].size() == 1)
                break;
        }
        if(i>l)
            break;
        int but=*light_related_button[i].begin();
        if(solution[but]==-1){
            int press_times=(3-lights[i])%3;
            solution[but]=press_times;
            for(auto li:button_control_lights[but]){
                lights[li]=(lights[li]+press_times)%3;
                light_related_button[li].erase(but);
            }

        }else{
            cout<<"impossible"<<endl;
            return 0;
        }
    }

    //剩下的灯都是被两个按钮控制的


    set<int> travesered_light;
    while(true) {
        bool all_red=true;
        for (int i = 1; i <= l; ++i)
            if (lights[i] != 0) {
                all_red=false;

                if(travesered_light.count(i)){
                    cout << "impossible" << endl;
                    return 0;
                }

                travesered_light.insert(i);

                if (light_related_button_org[i].empty()) {
                    cout << "impossible" << endl;
                    return 0;
                }

                int press_times = (3 - lights[i]) % 3;
                int but = *light_related_button_org[i].begin();
                if (solution[but] != -1)
                    but = *light_related_button_org[i].rbegin();
                if(solution[but] != -1){
                    cout << "impossible" << endl;
                    return 0;
                }

                solution[but] = press_times;

                for(auto li:button_control_lights[but]){
                    lights[li]=(lights[li]+press_times)%3;
                }
            }
        if(all_red)break;
    }
    int tot=0;
    for(int i=1;i<=b;++i)
        if(solution[i]!=-1)
            tot+=solution[i];
    cout<<tot<<endl;
    return 0;
}