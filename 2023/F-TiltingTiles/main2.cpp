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
//void tilt(direction dir, vector<vector<char>>& g) {
//
//    for(auto y=0;y<h;++y) {
//        int cnt = 0;
//        for (auto x = 0; x < w; ++x)
//            if (g[y][x])
//                g[y][cnt] = g[y][x];
//        for(int c=0;c<cnt;++c)
//            g[y][cnt+c]=0;
//
//    }
//}


int main(){
    cin>>h>>w;
    vector<vector<char>> g1(h, vector<char>(w)),g2(h, vector<char>(w));
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
    for

}