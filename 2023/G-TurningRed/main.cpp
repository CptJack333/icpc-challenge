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

    vector<vector<int>> connected_components;
    int light_cnt=0;
    vector<bool> visited_li(l+1,false);
    while(light_cnt<l){
        int i=1;
        for(;i<=l;i++)if(!visited_li[i])break;
        vector<int> comp;
        //需要设置ulimit -s unlimited 允许无限栈大小
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
        if(cc.size()==1 ){
            if(light_related_button[cc.front()].empty() && lights[cc.front()]!=0) {
                cout<<"impossible"<<endl;
                return 0;
            }
            int press_times = (3 - lights[cc.front()]) % 3;
            ans+=press_times;
            continue;
        }

        auto l=cc.front();
        set<int> visited;

        std::function<bool(int)> recur=[&](int l)->bool{
            //每个节点进来,先判断是不是所有的路径的解都求出来了
            //如果求出来了,就判断是否符合按压次数
            //如果没求出来,求解
            //无论是否求解了,都从所有路径扩散出去
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

            if(p1!=-1 && p2 !=-1){
                if((p1+p2)%3!=press_times)return false;
            }else{
                auto& sol=p2==-1?solution[but2]:solution[but1];
                if(p1==-1)swap(p1,p2);
                p2=press_times-p1;
                while(p2<0)p2+=3;
                sol=p2;
            }

            for(auto l2:button_control_lights[but1])
                if(!recur(l2))return false;
            if(but2!=-1)
            for(auto l2:button_control_lights[but2])
                if(!recur(l2))return false;

            return true;
        };

        int min_tot=0x7FFFFFFF;
        bool found_sol=false;
        for(int i=0;i<=2;++i)
            for(int j=0;j<=2;++j){
                solution=vector<int>(solution.size(),-1);
                auto related_button=light_related_button[l];
                if(related_button.size()>=1)
                    solution[*related_button.begin()]=i;
                if(related_button.size()==2)
                    solution[*related_button.rbegin()]=j;
                visited.clear();
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
            cout<<"impossible"<<endl;
            return 0;
        }
        ans+=min_tot;
    }

    cout<<ans<<endl;
    return 0;
}