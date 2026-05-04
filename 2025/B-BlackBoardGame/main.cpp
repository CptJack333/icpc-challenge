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
bool dfs(int num, int n,int round, const vector<bool>& isPrime, vector<bool>& used){
    vector<int> available_move=graph[num];

    if(available_move.empty())
        return round==0;

    if(round==0){
        bool all_opponent_move_can_be_handled = true;
        for (auto m: available_move) {
            if(used[m])
                continue;
            used[m] = true;
            auto b = dfs(m, n, (round + 1) % 2, isPrime, used);
            if (!b) {
                all_opponent_move_can_be_handled=false;
            }
            used[m] = false;

            if(!all_opponent_move_can_be_handled)
                break;
        }
        return all_opponent_move_can_be_handled;
    }else{
//        bool opponent_move_has_solution=false;
        for (auto m: available_move) {
            if(used[m])
                continue;
            used[m] = true;
            auto b = dfs(m, n, (round + 1) % 2, isPrime, used);
            used[m] = false;
            if(b)
                return true;
        }
        return false;
    }
}

int main(){
    int t;
    cin>>t;
//    t=1;
    for(int case_i=0;case_i<t;++case_i){
        int n;
        cin>>n;
//        n=138;

        graph.clear();
        graph.resize(n+1);
        auto isPrime= gen_prime(n);

        if(n<118){
            bool first_win = false;
            for (auto strategy = 2; strategy <= n; strategy += 2) {
                vector<bool> used(n + 1, false);
                used[strategy] = true;
                if (dfs(strategy, n, 0, isPrime, used)) {
                    cout << "first " << strategy << endl;
                    first_win = true;
                    break;
                }
            }
            if (!first_win)
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