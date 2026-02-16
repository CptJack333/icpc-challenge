//#pragma GCC optimize("O3,unroll-loops")
//#pragma GCC target("avx2,bmi,bmi2,lzcnt,popcnt")
//#include <iostream>
//#include <vector>
//#include <cmath>
//#include <algorithm>
//#include <iomanip>
#include "bits/stdc++.h"
using namespace std;

struct TriEdge {
    int u, v, tri_idx;
    bool operator<(const TriEdge& o) const {
        if (u != o.u) return u < o.u;
        if (v != o.v) return v < o.v;
        return tri_idx < o.tri_idx;
    }
};

struct Edge {
    int u, v;
};

struct FastSeg {
    double mx, cx, my, cy;
};

void solve() {
    int n, m;
    long long w_coord, l_coord;
    if (!(cin >> w_coord >> l_coord >> n >> m)) return;

    vector<long long> X(n + 1), Y(n + 1), Z_orig(n + 1);
    int W0 = -1, W1 = -1;

    for (int i = 1; i <= n; ++i) {
        cin >> X[i] >> Y[i] >> Z_orig[i];
        if (X[i] == 0 && Y[i] == 0) W0 = i;
        if (X[i] == 0 && Y[i] == l_coord) W1 = i;
    }

    vector<vector<int>> triangles(m, vector<int>(3));
    vector<TriEdge> all_edges;
    all_edges.reserve(m * 3);

    int start_tri = -1;
    for (int i = 0; i < m; ++i) {
        bool has_W0 = false, has_W1 = false;
        for (int j = 0; j < 3; ++j) {
            cin >> triangles[i][j];
            if (triangles[i][j] == W0) has_W0 = true;
            if (triangles[i][j] == W1) has_W1 = true;
        }
        if (has_W0 && has_W1) start_tri = i;

        for (int j = 0; j < 3; ++j) {
            int u = triangles[i][j];
            int v = triangles[i][(j + 1) % 3];
            if (u > v) swap(u, v);
            all_edges.push_back({u, v, i});
        }
    }

    sort(all_edges.begin(), all_edges.end());
    vector<vector<int>> adj(m, vector<int>(3, -1));

    for (size_t i = 0; i < all_edges.size(); ) {
        size_t j = i + 1;
        while (j < all_edges.size() && all_edges[j].u == all_edges[i].u && all_edges[j].v == all_edges[i].v) {
            j++;
        }
        if (j - i == 2) {
            int t1 = all_edges[i].tri_idx;
            int t2 = all_edges[i + 1].tri_idx;
            for (int k = 0; k < 3; ++k) {
                int u = triangles[t1][k], v = triangles[t1][(k + 1) % 3];
                if (u > v) swap(u, v);
                if (u == all_edges[i].u && v == all_edges[i].v) { adj[t1][k] = t2; break; }
            }
            for (int k = 0; k < 3; ++k) {
                int u = triangles[t2][k], v = triangles[t2][(k + 1) % 3];
                if (u > v) swap(u, v);
                if (u == all_edges[i].u && v == all_edges[i].v) { adj[t2][k] = t1; break; }
            }
        }
        i = j;
    }

    auto get_adj = [&](int curr_tri, int u, int v) {
        int u_min = min(u, v), v_max = max(u, v);
        for (int k = 0; k < 3; ++k) {
            int tu = triangles[curr_tri][k], tv = triangles[curr_tri][(k + 1) % 3];
            if (min(tu, tv) == u_min && max(tu, tv) == v_max) return adj[curr_tri][k];
        }
        return -1;
    };

    vector<long long> Z_vals;
    for (int i = 1; i <= n; ++i) Z_vals.push_back(Z_orig[i]);
    sort(Z_vals.begin(), Z_vals.end());
    Z_vals.erase(unique(Z_vals.begin(), Z_vals.end()), Z_vals.end());
    int S = Z_vals.size() - 1;

    auto check = [&](int k, vector<pair<Edge, Edge>>* path_out = nullptr) {
        bool W0_above = Z_orig[W0] >= Z_vals[k + 1];
        bool W1_above = Z_orig[W1] >= Z_vals[k + 1];

        if (W0_above && W1_above) return -1;
        if (!W0_above && !W1_above) return 1;

        int curr_tri = start_tri;
        int u = W0, v = W1;
        int exit_boundary = -1;

        while (true) {
            int w = -1;
            for (int i = 0; i < 3; ++i) {
                if (triangles[curr_tri][i] != u && triangles[curr_tri][i] != v) {
                    w = triangles[curr_tri][i];
                    break;
                }
            }

            bool w_above = Z_orig[w] >= Z_vals[k + 1];
            bool u_above = Z_orig[u] >= Z_vals[k + 1];

            int next_u, next_v;
            if (w_above != u_above) { next_u = u; next_v = w; }
            else { next_u = v; next_v = w; }

            if (path_out) path_out->push_back({{u, v}, {next_u, next_v}});

            int next_tri = get_adj(curr_tri, next_u, next_v);
            if (next_tri == -1) {
                if (X[next_u] == w_coord && X[next_v] == w_coord) exit_boundary = 2; // EAST
                else if (Y[next_u] == 0 && Y[next_v] == 0) exit_boundary = 0;        // SOUTH
                else if (Y[next_u] == l_coord && Y[next_v] == l_coord) exit_boundary = 1; // NORTH
                break;
            }
            curr_tri = next_tri;
            u = next_u; v = next_v;
        }

        if (exit_boundary == 2) return 0; // EAST (Valid Path Found)
        if (exit_boundary == 0) return (Z_orig[W0] < Z_orig[W1]) ? -1 : 1;
        return (Z_orig[W0] < Z_orig[W1]) ? 1 : -1;
    };

    int low = 0, high = S - 1;
    int K_start = -1, K_end = -1;

    while (low <= high) {
        int mid = low + (high - low) / 2;
        int res = check(mid);
        if (res == 0) { K_start = mid; high = mid - 1; }
        else if (res < 0) { low = mid + 1; }
        else { high = mid - 1; }
    }

    if (K_start == -1) {
        cout << "impossible\n";
        return;
    }

    low = 0; high = S - 1;
    while (low <= high) {
        int mid = low + (high - low) / 2;
        int res = check(mid);
        if (res == 0) { K_end = mid; low = mid + 1; }
        else if (res < 0) { low = mid + 1; }
        else { high = mid - 1; }
    }

    double global_min = 1e18;
    double phi = (1.0 + sqrt(5.0)) / 2.0;
    double resphi = 2.0 - phi;

    for (int k = K_start; k <= K_end; ++k) {
        vector<pair<Edge, Edge>> path;
        check(k, &path);
        vector<FastSeg> fast_segs;

        for (const auto& p : path) {
            int u1 = p.first.u, v1 = p.first.v;
            int u2 = p.second.u, v2 = p.second.v;
            double inv_dz1 = 1.0 / (Z_orig[v1] - Z_orig[u1]);
            double inv_dz2 = 1.0 / (Z_orig[v2] - Z_orig[u2]);

            double mx1 = (X[v1] - X[u1]) * inv_dz1;
            double cx1 = X[u1] - Z_orig[u1] * mx1;
            double my1 = (Y[v1] - Y[u1]) * inv_dz1;
            double cy1 = Y[u1] - Z_orig[u1] * my1;

            double mx2 = (X[v2] - X[u2]) * inv_dz2;
            double cx2 = X[u2] - Z_orig[u2] * mx2;
            double my2 = (Y[v2] - Y[u2]) * inv_dz2;
            double cy2 = Y[u2] - Z_orig[u2] * my2;

            fast_segs.push_back({mx1 - mx2, cx1 - cx2, my1 - my2, cy1 - cy2});
        }

        auto eval_L = [&](double z) {
            double total_len = 0;
            for (const auto& fs : fast_segs) {
                double dx = fs.mx * z + fs.cx;
                double dy = fs.my * z + fs.cy;
                total_len += sqrt(dx * dx + dy * dy);
            }
            return total_len;
        };

        double a = Z_vals[k], b = Z_vals[k + 1];
        double c = a + resphi * (b - a), d = b - resphi * (b - a);
        double fc = eval_L(c), fd = eval_L(d);

        for (int iter = 0; iter < 60; ++iter) {
            if (fc < fd) {
                b = d; d = c; fd = fc;
                c = a + resphi * (b - a); fc = eval_L(c);
            } else {
                a = c; c = d; fc = fd;
                d = b - resphi * (b - a); fd = eval_L(d);
            }
        }
        global_min = min({global_min, fc, fd, eval_L(Z_vals[k]), eval_L(Z_vals[k + 1])});
    }

    cout << fixed << setprecision(9) << global_min << "\n";
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    int t;
    if (cin >> t) {
        while (t--) {
            solve();
        }
    }
    return 0;
}