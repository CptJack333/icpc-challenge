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

void tilt2(int direction,vector<vector<int>>& board) {//dir: 0=left, 1=up, 2=right, 3=down
    int dr, dc;
    auto h=board.size(),w=board[0].size();
    if (direction == up) {
        dr = -1; dc = 0;
    }
    else if (direction == down) {
        dr = 1; dc = 0;
    }
    else if (direction == ::left) {
        dr = 0; dc = -1;
    }
    else if (direction == ::right) {
        dr = 0; dc = 1;
    }

    bool moved = true;
    while (moved) {  // 多次扫描直到所有滑块都停止
        moved = false;
        for (int r=0;r<h;++r)
            for (int c=0;c<w;++c)
                if (board[r][c]) {  // 找到滑块
                    auto chess = board[r][c];
                    int nr = r + dr, nc = c + dc;
                    // 检查是否可以移动
                    if (nr >= 0 && nr < h && nc >= 0 && nc < w && board[nr][nc] == 0) {
                        board[nr][nc] = chess;
                        board[r][c] = 0;
                        moved = true;
                    }
                }
    }

}

bool contain(vector<int> a,vector<int>b) {
    if (b.empty()) return true;
    if (b.size() > a.size()) return false;

    for (size_t i = 0; i <= a.size() - b.size(); ++i) {
        bool found = true;
        for (size_t j = 0; j < b.size(); ++j) {
            if (a[i + j] != b[j]) {
                found = false;
                break;
            }
        }
        if (found) return true;
    }
    return false;
}

bool is_rotation(vector<int> a,vector<int>b){
    if(a.size()!=b.size())return false;
    vector<int>aa=a;
    aa.reserve(2*a.size());
    for(auto i:a)aa.push_back(i);
    return contain(aa,b);
}

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

        for(auto d2:dset) {
            if (d == d2 || notd[d] == d2)continue;
            tilt(dmap[d2], tg1);
            if (tg1 == g2)
                goto can_match;

            for (int i = 0; i < h; i++)
                for (int j = 0; j < w; j++)
                    if ((!!tg1[i][j]) != (!!g2[i][j]))//轮廓不一样
                        continue;

            //获取转换路径
            vector<direction> vd = {notd[d], notd[d2], d, d2};
            auto tg2 = tg1;
            int cnt;
            cnt = 0;
            map<int, pair<int, int>> cm;
            for (int i = 0; i < h; i++)
                for (int j = 0; j < w; j++)
                    if (tg1[i][j] != 0) {
                        tg2[i][j] = ++cnt;
                        cm[cnt] = make_pair(i, j);
                    }
            auto tg3 = tg2;
            for (auto d: vd) {
                tilt(dmap[d], tg3);
            }
            map<int, int> lmap;//转了一圈的路径映射
            for (int k = 1; k <= cnt; ++k) {
                for (int i = 0; i < h; i++)
                    for (int j = 0; j < w; j++) {
                        if (tg3[i][j] == k)
                            lmap[k] = tg2[i][j];
                        goto double_break;
                    }
                double_break:
                int a = 1;
            }
            //已经得到了路径,然后要判断g1和g2的对应位置,看看这个转换有没有可能得到
            //找出始末图的字符串，然后看看这两个字符串是不是可以通过旋转得到的
            vector<vector<int>> index_chain;
            set<int> num_set;
            for (int k = 1; k <= cnt; ++k)
                num_set.insert(k);
            while (!num_set.empty()) {
                auto a = *num_set.begin();
                num_set.erase(a);
                vector<int> chain;
                chain.push_back(a);
                auto b = lmap[a];
                while (b != a) {
                    chain.push_back(b);
                    b = lmap[b];
                }
                index_chain.push_back(chain);
            }
            vector<int> s1, s2;
            for (auto i: index_chain.front()) {
                auto [x, y] = cm[i];
                s1.push_back(tg1[x][y]);
            }
            for (auto i: index_chain.front()) {
                auto [x, y] = cm[i];
                s2.push_back(g2[x][y]);
            }
            //判断S2有没有可能由S1旋转得来
            if(is_rotation(s1,s2))
                goto can_match;
        }
    }

no_match:
    cout<<"no"<<endl;
    return 0;

can_match:
    cout<<"yes"<<endl;
    return 0;
}