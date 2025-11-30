#include <algorithm>
#include <cstring>
#include <iostream>
#include <string>
#include <vector>
#include <cstdint>

using namespace std;

template<typename T> constexpr T Gcd(const T& a, const T& b) { return b != 0 ? Gcd(b, a%b) : a < 0 ? -a : a; }

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

void tilt2(int direction,vector<vector<int>>& board) {//dir: 0=left, 1=up, 2=right, 3=down
    int dr, dc;
    auto h=board.size(),w=board[0].size();
    if (direction == 1) {
        dr = -1; dc = 0;
    }
    else if (direction == 3) {
        dr = 1; dc = 0;
    }
    else if (direction == 0) {
        dr = 0; dc = -1;
    }
    else if (direction == 2) {
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

pair<int64_t, int64_t> match(const string& s, const string& t) {
    // Who needs hashing/string algorithms?
    int64_t r, m;
    for (r = 0; r <= s.size(); r++) {
        if (r == s.size()) return {0, 0};
        if (memcmp(&s[r], &t[0], s.size()-r) == 0 && memcmp(&s[0], &t[s.size()-r], r) == 0) break;//找字符串能对上的位置
    }
    for (m = r ? r : 1; m < s.size(); m++) {//要找最小周期
        if (s.size() % m == 0 && memcmp(&s[0], &s[m], s.size()-m) == 0) break;
    }
    return {r, m};
}

int main() {
    int h, w;
    cin >> h >> w;
    vector<vector<int>> g(h, vector<int>(w)), g2 = g;
    char ch;
    for (auto &row: g)
        for (auto &v: row) {
            cin >> ch;
            if (ch != '.') v = ch - 'a' + 1;
        }
    for (auto &row: g2)
        for (auto &v: row) {
            cin >> ch;
            if (ch != '.') v = ch - 'a' + 1;
        }

    for (int sd = 0; sd < 4; sd++)
        for (int dd = 1; dd < 4; dd += 2) {//4x2中间状态枚举
            auto tg = g;
            for (int i = 0, d = sd; i <= 6; i++, d = (d + dd) % 4) {//0=left, 1=up, 2=right, 3=down
                if (tg == g2)
                    goto pass;
                if (i >= 2) {
                    auto ng = tg;
                    for (int y = 0; y < h; y++)//检查外廓形状是否一致
                        for (int x = 0; x < w; x++) {
                            if (!!g2[y][x] != !!ng[y][x])//整个grid里面的tiles的外廓形状不一致
                                goto nomatch;
                            if (ng[y][x])//给每个位置一个独立的数字编号
                                ng[y][x] = y * w + x + 1;
                        }
                    for (int j = 0; j < 4; j++)//沿着这一方向转动四次
                        tilt((d + j) % 4, ng);
                    vector<int64_t> residues, mods;
                    for (int y = 0; y < h; y++)
                        for (int x = 0; x < w; x++)
                            if (ng[y][x]) {
                                string s, t;//找出 转动的变换路径
                                //找出这个格子所属的变换路径
                                for (int *ptr = &ng[y][x]; *ptr;) {
                                    int x2 = (*ptr - 1) % w, y2 = (*ptr - 1) / w;
                                    *ptr = 0;
                                    s += tg[y2][x2];
                                    t += g2[y2][x2];
                                    ptr = &ng[y2][x2];
                                }

                                auto [residue, mod] = match(s, t);
                                if (mod == 0)
                                    goto nomatch;
                                if (mod == 1)
                                    continue;
                                for (int i = 0; i < mods.size(); i++) {//验证线性同余方程组是否有解
                                    int64_t g = Gcd(mod, mods[i]);
                                    if (residues[i] % g != residue % g) goto nomatch;
                                }
                                //cerr << "Adding r=" << residue << " m=" << mod << endl;
                                residues.push_back(residue);
                                mods.push_back(mod);
                            }
                    goto pass;
                }
                nomatch:;
                tilt(d, tg);
            }
        }

    fail:
    cout << "no" << endl;
    return 0;
    pass:
    cout << "yes" << endl;
}