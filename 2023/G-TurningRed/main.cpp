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

    vector<set<int>> light_related_button_org=light_related_button;
//    for(auto s:light_related_button){
//        set<int> new_set;
//        for(auto v:s){
//            new_set.insert(v);
//        }
//        light_related_button_org.push_back(new_set);
//    }

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