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

    while(true){
        int i=1;
        for (;i<=l;++i)
            if(light_related_button[i].size()==1)
                break;
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
                solution[but] = press_times;
                for(auto li:button_control_lights[but]){
                    light_related_button_org[li].erase(but);
                    lights[li]=(lights[li]+press_times)%3;
                }
                continue;
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