#include <bits/stdc++.h>
using namespace std;

int h,w;

void tilt(int dir, vector<vector<int>>& g) {//dir: 0=left, 1=up, 2=right, 3=down
    int X = g[0].size(), Y = g.size();
    if (dir&1) swap(X, Y);
    auto get = [&](int x, int y) {
        return dir == 0 ? &g[y][x] : dir == 1 ? &g[x][y] : dir == 2 ? &g[y][X-1-x] : &g[X-1-x][y];
    };
    for (int y = 0; y < Y; y++) {
        int x2 = 0;
        for (int x = 0; x < X; x++) if (*get(x, y)) {//妙喔，艹
                *get(x2++, y) = *get(x, y);
            }
        for (; x2 < X; x2++) *get(x2, y) = 0;
    }
}

enum direction{left, right,up,down};
map<direction,int> dmap={{::left, 0},{::right,2},{up,1},{down,3}};
map<direction,direction> notd={{::left, ::right},{::right,::left},{up,down},{down,up}};

int main(){
    cin>>h>>w;
    vector<vector<int>> g1(h, vector<int>(w)),g2(h, vector<int>(w));
    char ch;
    for (auto &row: g1)
        for (auto &v: row) {
            cin >> ch;
            if (ch != '.')
                v = ch - 'a' + 1;
        }
    for (auto &row: g2)
        for (auto &v: row) {
            cin >> ch;
            if (ch != '.')
                v = ch - 'a' + 1;
        }
    set<direction> dset={::left,::right};
    for(auto d :dset){
        auto tg1=g1;
        tilt(dmap[d],tg1);
        if (tg1==g2)
            goto can_match;

        for(auto d2:dset){
            if(d==d2||notd[d]==d2)continue;
            tilt(dmap[d2],tg1);
            if (tg1==g2)
                goto can_match;

            for(int i=0;i<h;i++)
                for(int j=0;j<w;j++)
                    if ((!!tg1[i][j])!=(!!g2[i][j]))
                        goto pppp;

            //获取转换路径
            vector<direction> vd={notd[d],notd[d2],d,d2};
            auto tg2=tg1;
            auto cnt=0;
            for(int i=0;i<h;i++)
                for(int j=0;j<w;j++)
                    if(tg1[i][j]!=0)
                        tg2[i][j]=++cnt;
            auto tg3=tg2;
            for(auto d :vd){
                tilt(dmap[d],tg3);
            }
            map<int,int> lmap;//转了一圈的路径映射
            for(int k=1;k<=cnt;++k){
                for(int i=0;i<h;i++)
                    for (int j = 0; j < w; j++) {
                        if (tg3[i][j] == k)
                            lmap[k] = tg2[i][j];
                        goto kkk;
                    }
                kkk:
                int a=1;
            }
            //已经得到了路径,然后要判断g1和g2的对应位置,看看这个转换有没有可能得到
            //找出始末图的字符串，然后看看这两个字符串是不是可以通过旋转得到的
        }
        pppp:

    }

can_match:
    cout<<"yes"<<endl;

}