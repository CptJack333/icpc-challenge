#include "bits/stdc++.h"
using namespace std;

struct Edge;

struct Link {
    int edge_index1, edge_index2;
    double fz, fc;
    Edge* edge1, *edge2;
    bool operator<(const Link& l) const { return false; }
    Link operator+(const Link& l) const {
        assert(edge_index2 == l.edge_index1);
        return Link{edge_index1, l.edge_index2, fz + l.fz, fc + l.fc,.edge1 = this->edge1,.edge2 = l.edge2};
    }
    Link rev() const { return Link{edge_index2, edge_index1, fz, fc,.edge1 = edge2,.edge2 = edge1}; }
};

struct Edge {
    int a, b, border;//1 ab都在西边界 2 ab都在东边界 0 无
    int64_t za,zb;
    vector<vector<Link>> skip{{}, {}};
};

int main() {
    int T, X, Y, N, M, A, B, C;
    for (cin >> T; T--;) {
        cin >> X >> Y >> N >> M;
        vector<int64_t> vx(N), vy(N), vz(N);// 三角形顶点坐标和高度
        for (int i = 0; i < N; i++) cin >> vx[i] >> vy[i] >> vz[i];

        vector<Edge> edges;
        map<pair<int,int>, int> edge_index;
        auto edge_idx = [&](int a, int b) {
            if (edge_index.count({a, b})) return edge_index[{a, b}];
            int ret = edge_index[{a, b}] = edges.size();
            edges.push_back(Edge{za:vz[a],zb:vz[b],a: a, b: b, border: vx[a] == 0 && vx[b] == 0 ? 1 : vx[a] == X && vx[b] == X ? 2 : 0});
            return ret;
        };

        vector<tuple<int64_t,bool,int,Link>> events; //触发高度、 操作类型、 关联顶点、 等高线片段
        for (int i = 0; i < M; i++) {
            cin >> A >> B >> C; A--; B--; C--;//每个三角形的顶点,下标从0开始
            while (vz[A] > vz[B] || vz[A] > vz[C]) { swap(A, B); swap(B, C); }//确保A的高度最低
            bool flip = false;
            if (vz[B] > vz[C]) { swap(B, C); flip = true; }//高度顺序ABC
            double mx = vx[A] + (vx[C]-vx[A])*(vz[B]-vz[A])/double(vz[C]-vz[A]);
            double my = vy[A] + (vy[C]-vy[A])*(vz[B]-vz[A])/double(vz[C]-vz[A]);
            double ml = hypot(mx-vx[B], my-vy[B]);//AC上一点连接B的等高线的长度

            for (int j = 0; j < 2; j++) {
                int lo = j?B:A, hi = j?C:B, zero=j?C:A;
                double fz = ml / (vz[B]-vz[zero]), fc = -fz * vz[zero];
                Link link{edge_idx(lo, hi), edge_idx(A, C), fz, fc};//ei1 ei2的顺序是怎么来的
                if (flip) swap(link.edge_index1, link.edge_index2);
                events.push_back({vz[lo], true , lo, link});//z从低到高，把每一个节点推入，还有对应的edge间的链接
                events.push_back({vz[hi], false, hi, link});
            }
        }
        sort(events.begin(), events.end());

        double ret = 1e18;
        int maxskip = 0;
        for (int ei = 0; ei < edges.size(); ei++) { //初始化跳链
            do {
                edges[ei].skip[0].push_back(Link{ei, -1, 0.0, 0.0,.edge1=&edges[ei]});
                edges[ei].skip[1].push_back(Link{ei, -1, 0.0, 0.0,.edge1=&edges[ei]});
            } while (rand()%2);//每个edge随机跳链高度
        }
        function<Link(int,int,int,int)> follow = [&](int ei, int dir, int h, int rep) {//如果不使用跳链，相当于每次h都是1，一直深度优先遍历到底
            auto const& s = edges[ei].skip[dir];
            maxskip = max<int>(maxskip, s.size());
            if (s[h].edge_index2 == rep)
                return Link{ei, ei, 1e50, 1e50,&edges[ei],&edges[ei]};  // cycle。 rep其实就是递归第一次进来的ei，后面递又找到了，说明是通过环找回来了
            while (h+1 < s.size() && s[h+1].edge_index2 != -1) {
                h++;
                rep = ei;
            } //找当前能跳的最远的
            while (h > 0 && s[h].edge_index2 == -1)
                h--;
            if (s[h].edge_index2 == -1)
                return Link{ei, ei, 0.0, 0.0,&edges[ei],&edges[ei]};
            return s[h] + follow(s[h].edge_index2, dir, h, rep);// 一跳接一跳
        };

        long long oldz=-1;
        for (int i = 0; i < events.size(); i++) {
            auto [z, add, zv, link] = events[i];

            if (oldz==-1 || z!=oldz) {//高度发生了变化，这个条件也是加速
                vector<double> border(3, 1e50);//所有长度置0，从当前点一直找到东西边界，然后计算长度
                if (vx[zv] == 0) border[1] = 0.0;//点在西边界
                if (vx[zv] == X) border[2] = 0.0;//点在东边界
                for (int j = i; j < events.size(); j++) {
                    auto [z2, add2, zv2, link2] = events[j];
                    if (z2 != z || add2) break;             //拿到所有同一个新高度的点。add2去重加速
                    for (int dir = 0; dir < 2; dir++) {             // 尝试从同一高度的link的两个方向出去
                        link2 = link2.rev();
                        if (edges[link2.edge_index2].a == zv || edges[link2.edge_index2].b == zv) continue;//去重加速
                        Link link3 = follow(link2.edge_index1, dir, 0, link2.edge_index1);
                        double& b = border[edges[link3.edge_index2].border];
                        if(edges[link3.edge_index2].border!=0)//妈的！其实是不需要border 0的！
                            b = min(b, link3.fz*z + link3.fc);
                    }
                }
                ret = min(ret, border[1]+border[2]);
                oldz=z;
            }

            maxskip = 0;
            follow(link.edge_index2, 1, 0, link.edge_index2);//得到当前的maxskip
            if (add) {// 更新跳链表
                for (int h = 0; h < maxskip; h++) {
                    while (link.edge_index1 != -1 && edges[link.edge_index1].skip[1].size() <= h)
                        link = edges[link.edge_index1].skip[0][h - 1].rev() + link;
                    while (link.edge_index2 != -1 && edges[link.edge_index2].skip[1].size() <= h)
                        link = link + edges[link.edge_index2].skip[1][h - 1];
                    if (link.edge_index1 == -1 || link.edge_index2 == -1) break;
                    edges[link.edge_index1].skip[1][h] = link;
                    edges[link.edge_index2].skip[0][h] = link.rev();
                }
            } else {
                for (int dir = 0; dir < 2; dir++) {
                    int ei = dir ? link.edge_index2 : link.edge_index1;
                    for (int h = 0; h < maxskip; h++) {
                        while (ei != -1 && edges[ei].skip[dir].size() <= h)
                            ei = edges[ei].skip[dir][h - 1].edge_index2;
                        if (ei == -1) break;
                        edges[ei].skip[!dir][h] = Link{ei, -1, 0.0, 0.0};
                    }
                }
            }
        }

        if (ret == 1e18) cout << "impossible" << endl; else cout << fixed << setprecision(10) << ret << endl;
    }
}
