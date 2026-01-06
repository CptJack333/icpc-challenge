#include <iostream>
#include <vector>
#include <algorithm>
#include <set>
#include <iterator>

using namespace std;

const int INF = 1e9;

void solve() {
    int n, m;
    if (!(cin >> n >> m)) return;

    // cats_at_pot[j] 存储目标在花盆 j 的所有猫的喜好列表
    vector<vector<vector<int>>> cats_at_pot(m + 1);

    // release_time[plant]: 植物最早可以被放置的位置
    // 默认为 1。如果猫目标是 p 且喜欢该植物，则该植物不能在 < p 的位置使用。
    vector<int> release_time(m + 1, 1);

    for (int i = 0; i < n; ++i) {
        int p, k;
        cin >> p >> k;
        vector<int> likes(k);
        for (int j = 0; j < k; ++j) {
            cin >> likes[j];
            // 更新该植物的释放时间
            if (p > release_time[likes[j]]) {
                release_time[likes[j]] = p;
            }
        }
        cats_at_pot[p].push_back(likes);
    }

    // Req[j]: 花盆 j 必须放置的植物集合（所有目标在此的猫的喜好交集）
    // is_fixed[j]: 标记花盆 j 是否有猫
    vector<vector<int>> Req(m + 1);
    vector<bool> is_fixed(m + 1, false);
    bool possible = true;

    // 计算每个花盆的需求交集
    for (int j = 1; j <= m; ++j) {
        if (!cats_at_pot[j].empty()) {
            is_fixed[j] = true;
            // 取第一个列表作为初始交集
            vector<int> current_intersection = cats_at_pot[j][0];
            sort(current_intersection.begin(), current_intersection.end());

            // 依次与后续列表取交集
            for (size_t i = 1; i < cats_at_pot[j].size(); ++i) {
                vector<int> next_list = cats_at_pot[j][i];
                sort(next_list.begin(), next_list.end());
                vector<int> temp;
                set_intersection(current_intersection.begin(), current_intersection.end(),
                                 next_list.begin(), next_list.end(),
                                 back_inserter(temp));
                current_intersection = temp;
                if (current_intersection.empty()) break;
            }

            if (current_intersection.empty()) {
                possible = false;
            } else {
                Req[j] = current_intersection;
            }
        }
    }

    if (!possible) {
        cout << "no" << endl;
        return;
    }

    // Freq[y]: 植物 y 在未来的固定花盆中出现的次数
    vector<int> freq(m + 1, 0);
    for (int j = 1; j <= m; ++j) {
        if (is_fixed[j]) {
            for (int plant : Req[j]) {
                freq[plant]++;
            }
        }
    }

    // Available set: 存储 {future_frequency, plant_id}
    // 使用 set 自动排序，方便取 min frequency
    set<pair<int, int>> available;

    // 辅助数组，快速判断植物是否在 available 集合中
    vector<bool> is_available(m + 1, false);

    // 将植物按释放时间分组，方便在遍历花盆时加入
    vector<vector<int>> plants_by_release(m + 1);
    for (int plant = 1; plant <= m; ++plant) {
        if (release_time[plant] <= m) {
            plants_by_release[release_time[plant]].push_back(plant);
        }
        // 如果 release_time > m，说明该植物被限制得无法放在任何位置（理论上不可能，除非逻辑矛盾）
    }

    vector<bool> used(m + 1, false);

    // 主循环：遍历每个花盆填坑
    for (int j = 1; j <= m; ++j) {
        // 1. 将当前位置解锁的植物加入可用集合
        for (int plant : plants_by_release[j]) {
            available.insert({freq[plant], plant});
            is_available[plant] = true;
        }

        // 2. 更新频率：当前花盆 j 的需求已经不再是“未来”的需求了
        // 我们需要减少 Req[j] 中所有植物的 freq 计数
        if (is_fixed[j]) {
            for (int plant : Req[j]) {
                if (!used[plant]) {
                    // 如果植物在 available 集合中，需要先移除再插入以更新排序键值
                    if (is_available[plant]) {
                        available.erase({freq[plant], plant});
                        freq[plant]--;
                        available.insert({freq[plant], plant});
                    } else {
                        // 如果还没解锁，直接减计数
                        freq[plant]--;
                    }
                }
            }
        }

        int chosen = -1;

        // 3. 选择植物
        if (is_fixed[j]) {
            // 固定点：必须从 Req[j] 且 Available 的植物中选
            // 贪心策略：选 Freq 最小的
            int best_plant = -1;
            int min_f = INF;

            // 因为 Req[j] 通常较小，遍历 Req[j] 比遍历 Available 更快
            for (int plant : Req[j]) {
                if (is_available[plant]) {
                    if (freq[plant] < min_f) {
                        min_f = freq[plant];
                        best_plant = plant;
                    }
                }
            }

            if (best_plant == -1) {
                possible = false; // 需要的植物还没解锁或已被用掉
                break;
            }
            chosen = best_plant;
        } else {
            // 自由点：从 Available 中选 Freq 最小的
            if (available.empty()) {
                possible = false;
                break;
            }
            chosen = available.begin()->second;
        }

        // 4. 标记为已使用并从集合移除
        available.erase({freq[chosen], chosen});
        is_available[chosen] = false;
        used[chosen] = true;
    }

    if (possible) cout << "yes" << endl;
    else cout << "no" << endl;
}

int main() {
    // 优化 I/O 速度
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    int t;
    if (cin >> t) {
        while(t--) {
            solve();
        }
    }
    return 0;
}