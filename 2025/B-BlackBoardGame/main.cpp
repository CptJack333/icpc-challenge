#include "bits/stdc++.h"

using namespace std;

vector<vector<int>> graph;

vector<bool> gen_prime(int n){
    vector<bool> isPrime(n + 1, true);
    isPrime[0] = isPrime[1] = false;

    for (int i = 2; i * i <= n; i++) {
        if (isPrime[i]) {
            // 把 i 的倍数全部标记为非质数
            for (int j = i * i; j <= n; j += i) {
                isPrime[j] = false;
            }
        }
    }

    if(n<118)
        for (int x = 1; x <= n; x++) {
            for (int y = 1; y <  x; y++)
                if (x%y == 0 && isPrime[x/y])
                    graph[x].push_back(y);
            for (int y = x; y <= n; y++)
                if (y%x == 0 && isPrime[y/x])
                    graph[x].push_back(y);
        }

    return isPrime;
}

// round: 0 first 1 second

int n;
vector<int> match_to;
vector<bool> visited;

// 匈牙利算法 DFS 实现：尝试给 u 找匹配
bool dfs(int u) {
    // 遍历 u 的所有邻接点
    for (int v : graph[u]) {
        if (!visited[v]) {
            visited[v] = true;
            // 如果 v 未匹配，或匹配它的节点能找到新的匹配。 如果 v 没人要，或者 占有 v 的人能找到别人 → 我要了
            if (match_to[v] == 0 || dfs(match_to[v])) {
                match_to[v] = u;  // 更新匹配关系
                return true;
            }
        }
    }
    return false;
}

//匈牙利算法返回最大匹配的数目
int hungarianAssignment(){
    match_to.clear();
    match_to.resize(n+1, 0);
    int res = 0;
    for (int i = 1; i <= n; ++i) {
        visited.clear();
        visited.resize(n+1, false);
        if (dfs(i)) res++;
    }
    return res;
}

int main(){
    int t;
    cin>>t;
//    t=1;
    for(int case_i=0;case_i<t;++case_i){
        cin>>n;
//        n=138;

        auto isPrime= gen_prime(n);

        if(n<118){
            graph.clear();
            graph.resize(n+1);
            for (int x = 1; x <= n; x++) {
                for (int y = 1; y <  x; y++) if (x%y == 0 && isPrime[x/y])
                    graph[x].push_back(y);
                for (int y = x; y <= n; y++) if (y%x == 0 && isPrime[y/x])
                    graph[x].push_back(y);
            }

            bool first_win = false;
            auto strategy = 2;
            for (; strategy <= n; strategy += 2) {
                int cnt = 0;
                auto match_num=hungarianAssignment();

                auto tmp=graph[strategy];
                graph[strategy].clear();
                auto match_num2=hungarianAssignment();
                graph[strategy]=tmp;

                if (match_num2 == match_num) break;
            }
            if (strategy<=n)
                cout<<"first "<<strategy;
            else
                cout << "second" << endl;
        }else{
            vector<int> pri;
            for (auto p = n / 3; ; --p)
                if (isPrime[p]) {
                    cout << "first " << p * 2 << endl;
                    break;
                }
        }
    }
}