/**
 * Problem E: Delivery Service
 * Solution by Gemini
 * * Logic:
 * 1. Split each city u into u_out (1..n) and u_in (n+1..2n).
 * 2. Courier u->v implies edge (u_out, v_in).
 * 3. Maintain connected components using DSU.
 * 4. A city 'x' contributes to component A if find(x_out) == A or find(x_in) == A.
 * 5. Pairs = Sum(nC2(size of Component)) - Sum(nC2(size of Intersection of Comp A & B)).
 * 6. Use Small-to-Large merging to maintain intersections.
 */

#include <iostream>
#include <vector>
#include <numeric>
#include <map>
#include <algorithm>

using namespace std;

typedef long long ll;

const int MAXN = 200005; // Max cities
const int MAXM = 400005; // Max couriers

// DSU structures
int parent[MAXN * 2];
int comp_sz[MAXN * 2]; // Number of unique cities touching this component

// overlaps[root] stores a map {other_root -> count}
// count is the number of cities that have one port in 'root' and the other in 'other_root'
map<int, int> overlaps[MAXN * 2];

ll current_ans = 0;

ll nC2(ll n) {
    if (n < 2) return 0;
    return n * (n - 1) / 2;
}

int find_set(int v) {
    if (v == parent[v])
        return v;
    return parent[v] = find_set(parent[v]);
}

void merge_sets(int a, int b) {
    a = find_set(a);
    b = find_set(b);
    if (a != b) {
        // Heuristic: Merge smaller map into larger map
        if (overlaps[a].size() < overlaps[b].size())
            swap(a, b);

        // We are merging B into A.
        // Formula Logic:
        // NewAns = OldAns
        //          - Pairs(A) - Pairs(B) + Pairs(New)  <-- Component sizes contribution
        //          + Pairs(Overlap(A,B))               <-- Because A and B are now same, their mutual overlap is internal,
        //                                                  so we stop subtracting it (add it back).

        // 1. Remove old component contributions
        current_ans -= nC2(comp_sz[a]);
        current_ans -= nC2(comp_sz[b]);

        // 2. Handle the intersection between A and B
        int intersection_ab = 0;
        if (overlaps[a].count(b)) {
            intersection_ab = overlaps[a][b];
            // Since A and B are merging, this overlap is no longer "between two components".
            // It was subtracted before, so we add it back.
            current_ans += nC2(intersection_ab);

            // Remove from maps (cleanup)
            overlaps[a].erase(b);
            overlaps[b].erase(a);
        }

        // 3. Calculate new size
        // Size(A U B) = Size(A) + Size(B) - Intersection(A, B)
        int new_size = comp_sz[a] + comp_sz[b] - intersection_ab;

        // 4. Merge overlaps from B into A
        // We iterate over neighbors of B (let's call a neighbor K)
        for (auto const& [k, count] : overlaps[b]) {
            // k is a component that had 'count' overlap with B.
            // Now those cities will have overlap with A.

            // Remove old subtraction terms for (A, K) and (B, K)
            if (overlaps[a].count(k)) {
                current_ans += nC2(overlaps[a][k]);
            }
            current_ans += nC2(count);


            // Apply new subtraction term for (A, K)
            current_ans -= nC2(overlaps[a][k]+count);

            // Update map A
            overlaps[a][k] += count;
            // Update back-reference in K
            // K used to know about B, now it must know about A
            overlaps[k].erase(b);
            overlaps[k][a] = overlaps[a][k];
        }

        // Clear B's map to free memory/state
        overlaps[b].clear();

        // 5. Finalize DSU merge
        parent[b] = a;
        comp_sz[a] = new_size;

        // Add new component contribution
        current_ans += nC2(new_size);
    }
}

int main() {
    // Optimize I/O
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int n, m;
    if (!(cin >> n >> m)) return 0;

    // Initialization
    // Nodes 1..n are OUT nodes (Home)
    // Nodes n+1..2n are IN nodes (Dest)
    for (int i = 1; i <= 2 * n; ++i) {
        parent[i] = i;
        comp_sz[i] = 1; // Initially, each port is its own component containing 1 city
    }

    // Initialize overlaps
    // Each city i connects i_out and i_in via its existence.
    // So initially, component i (out) and component i+n (in) have overlap of 1 (city i itself).
    for (int i = 1; i <= n; ++i) {
        int u = i;
        int v = i + n;
        overlaps[u][v] = 1;
        overlaps[v][u] = 1;
        // Initial Answer calc:
        // Sum nC2(size) is 0 (all sizes 1).
        // Sum nC2(overlap) is 0 (all overlaps 1).
        // Ans = 0.
    }

    for (int i = 0; i < m; ++i) {
        int u, v;
        cin >> u >> v;

        // Courier connects u_out -> v_in
        // u_out is node u
        // v_in is node v + n
        merge_sets(u, v + n);

        cout << current_ans << "\n";
    }

    return 0;
}