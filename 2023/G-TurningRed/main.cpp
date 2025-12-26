#include "bits/stdc++.h"
using namespace std;

int l,b;
vector<char> lights;
vector<vector<int>> button_control_lights;
vector<set<int>> light_related_button;

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

    vector<int> li_visited(l + 1, false);
    vector<int> bu_visited(b + 1, false);
    //把灯和按钮画成连通图，分开各个连通分量处理
    vector<vector<node>> connected_componets;

    for(int li=1;li<=l;++li){
        if(li_visited[li])continue;

        //特殊情况处理, 孤立一盏灯, 且没有按钮控制
        if(light_related_button[li].empty()&& lights[li] != 0){
            cout<<"impossible"<<endl;
            return 0;
        }

        //特殊情况, 孤立的一盏灯,但有按钮可以控制. 这里如果不单独处理, 按照两个按钮不断试找最小的方法, 会导致很慢.
        bool single_light=true;
        for (auto b2:light_related_button[li])
            for(auto l2:button_control_lights[b2])
                if(l2!=li){
                    single_light= false;
                    break;
                }
        if(single_light){
            int press_times = (3 - lights[li]) % 3;
            ans+=press_times;
            continue;
        }

        int min_tot=0x7FFFFFFF;
        bool found_solution=false;
        vector<node> connected_componet;

        //两个按钮, 各按0-2次, 不断组合去试, 得出按的最少次数
        for(int i=0;i<=2;++i)
        for(int j=0;j<=2;++j){
            connected_componet.clear();
            vector<int> solution(b+1,-1);//每个按钮分别按多少次

            auto try_push=[&](node nn){
                stack<node> recur_stack;
                recur_stack.push(nn);
                while (!recur_stack.empty()) {
                    auto n =recur_stack.top();
                    recur_stack.pop();
                    //灯和按钮分开类型处理
                    if (n.type == 1) {
//                        if (light_related_button[n.index].empty() && lights[n.index] != 0) {
//                            return false;
//                        }

                        if (li_visited[n.index])continue;
                        li_visited[n.index] = true;
                    } else {
                        if (bu_visited[n.index])continue;
                        bu_visited[n.index] = true;
                    }
                    connected_componet.push_back(n);
                    //按钮只需要推栈
                    if (n.type == 2) {
                        for (auto l3: button_control_lights[n.index])
                            if (!li_visited[l3])
                                recur_stack.emplace(1, l3);
                        continue;
                    }

                    //灯需要判断按的次数是否可行
                    int press_times = (3 - lights[n.index]) % 3;
                    //获取这个灯关联的两个按钮
                    auto related_button = light_related_button[n.index];
                    auto but1 = *related_button.begin();
                    auto but2 = related_button.size() == 2 ? *related_button.rbegin() : -1;
                    //获取两个按钮按下的次数
                    auto p1 = solution[but1];
                    auto p2 = but2 != -1 ? solution[but2] : 0;//如果只有一个按钮,虚拟按钮次数为0

                    if (p1 != -1 && p2 != -1) {
                        if ((p1 + p2) % 3 != press_times)
                            return false;
                    } else {
                        auto &sol = p2 == -1 ? solution[but2] : solution[but1];
                        if (p1 == -1)swap(p1, p2);
                        p2 = press_times - p1;
                        while (p2 < 0)p2 += 3;
                        sol = p2;
                    }

                    for (auto b: light_related_button[n.index])
                        if (!bu_visited[b])
                            recur_stack.push(node(2, b));

                }
                return true;
            };

            auto related_button=light_related_button[li];
            if(related_button.size()>=1)
                solution[*related_button.begin()]=i;
            if(related_button.size()==2)
                solution[*related_button.rbegin()]=j;
            if(try_push(node(1, li))){
                if(!found_solution) {//第一次找到解决方案, 把当前的连通分量记下来
                    sort(connected_componet.begin(), connected_componet.end(),
                         [] (const node& a, const node& b) { return a.type < b.type || (a.type==b.type&& a.index<b.index); });
                    connected_componets.push_back(connected_componet);
                }
                found_solution=true;
                int tot=0;
                for(int k : solution)
                    if(k!=-1)
                        tot+=k;
                min_tot=min(tot,min_tot);
            }
            for(auto n:connected_componet)
                if(n.type==1)
                    li_visited[n.index]=false;
                else
                    bu_visited[n.index]=false;
        }
        if(!found_solution){
            cout<<"impossible"<<endl;
            return 0;
        }
        for(auto n:connected_componets.back())
            if(n.type==1)
                li_visited[n.index]=true;
            else
                bu_visited[n.index]=true;
        connected_componet.clear();
        ans+=min_tot;
    }

    cout<<ans<<endl;
    return 0;
}