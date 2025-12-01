#include <bits/stdc++.h>
using namespace std;

int h,w;

void tilt_wrap(int dir, vector<vector<int>>& g) {//dir: 0=left, 1=up, 2=right, 3=down
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

void tilt(direction d,vector<vector<int>>& g){
    return tilt_wrap(dmap[d],g);
}

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

int contain(vector<int> a,vector<int>b) {
    if (b.size() > a.size()) return -1;

    for (size_t i = 0; i <= a.size() - b.size(); ++i) {
        bool found = true;
        for (size_t j = 0; j < b.size(); ++j) {
            if (a[i + j] != b[j]) {
                found = false;
                break;
            }
        }
        if (found)
            return i;
    }
    return -1;
}

// 使用欧几里得算法计算最大公约数
int gcd(int a, int b) {
    // 转换为 long long 并取绝对值，避免溢出
    long long n1 = std::llabs(static_cast<long long>(a));
    long long n2 = std::llabs(static_cast<long long>(b));

    // 处理特殊情况：两个数都是0
    if (n1 == 0 && n2 == 0) {
        return 0;
    }

    // 欧几里得算法
    while (n2 != 0) {
        long long temp = n2;
        n2 = n1 % n2;
        n1 = temp;
    }

    return static_cast<int>(n1);
}

pair<int,int> match(vector<int> a, vector<int>b){// residue module
    if(a.size()!=b.size())return {0,0};
    vector<int>aa=a;
    aa.reserve(2*a.size());
    for(auto i:a)aa.push_back(i);
    auto r=contain(aa,b);
    if(r==-1)return {0,0};

    auto equal=[&](int m){
        for (auto i=0;i<a.size()-m;i++)
            if(a[i]!=a[i+m])return false;
        return true;
    };

    auto m = r ? r : 1;
    for (; m < a.size(); m++) {
        if (a.size() % m == 0 && equal(m)) break;//要找最小周期
    }
    return{r,m};
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

    vector<direction> dirs={::left,up,::right,down};
    for (int sd = 0; sd < 4; sd++)
        for (int dd = 1; dd < 4; dd += 2) {
            auto tg1 = g1;
            for (int i = 0, d8 = sd; i <= 6; i++, d8 = (d8 + dd) % 4) //0=left, 1=up, 2=right, 3=down
            {//4x2中间状态枚举
                auto d=dirs[d8];
                if (tg1 == g2)
                    goto can_match;
                if(i>=2){

                    bool same_outline = true;
                    for (int i = 0; i < h; i++)
                        for (int j = 0; j < w; j++)
                            if ((!!tg1[i][j]) != (!!g2[i][j]))//轮廓不一样
                                goto no_match;

                    //获取转换路径
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
                    for (int j = 0; j < 4; j++)//沿着这一方向转动四次
                        tilt(dirs[(d+j)%4], tg3);
                    map<int, int> lmap;//转了一圈的路径映射
                    for (int k = 1; k <= cnt; ++k) {
                        for (int i = 0; i < h; i++)
                            for (int j = 0; j < w; j++)
                                if (tg3[i][j] == k) {
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
                            num_set.erase(b);
                            b = lmap[b];
                        }
                        index_chain.push_back(chain);
                    }

                    bool can_match = true;
                    vector<pair<int, int>> res_mod;
                    for (auto ic: index_chain) {
                        vector<int> s1, s2;
                        for (auto i: ic) {
                            auto [x, y] = cm[i];
                            s1.push_back(tg1[x][y]);
                        }
                        for (auto i: ic) {
                            auto [x, y] = cm[i];
                            s2.push_back(g2[x][y]);
                        }
                        //判断S2有没有可能由S1旋转得来
                        auto [r, m] = match(s1, s2);
                        if (m == 0)
                            goto no_match;
                        if (m == 1)
                            continue;
                        //判断线性同余方程组是否有解
                        for (auto [r2, m2]: res_mod) {
                            auto g = gcd(m, m2);
                            if (r % g != r2 % g)
                                goto no_match;
                        }
                        if (!can_match)break;
                        res_mod.push_back({r, m});

                    }

                    if (can_match)
                        goto can_match;
                }
                no_match:;
                tilt(d, tg1);
            }
        }

    cout<<"no"<<endl;
    return 0;

can_match:
    cout<<"yes"<<endl;
    return 0;
}