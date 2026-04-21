#include "bits/stdc++.h"

using namespace std;

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
    return isPrime;
}

// round: 0 first 1 second
bool dfs(int num, int n,int round, const vector<bool>& isPrime, vector<bool>& used){
    vector<int> available_move;
    for(int p=2;p*num<=n;++p){
        if(isPrime[p] && !used[p*num])
            available_move.push_back(p*num);
    }
    for(int p=2;p<num;++p)
        if(isPrime[p] && num%p==0 && !used[num/p])
            available_move.push_back(num/p);

    if(available_move.empty())
        return round==0;

    if(round==0){
        bool all_opponent_move_can_be_handled = true;
        for (auto m: available_move) {
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
//        n=12;
        auto isPrime= gen_prime(n);

        if(n<176){
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
            for (auto p = n / 4; p <= n / 3; ++p) {
                if (!isPrime[p])continue;
                pri.push_back(p);
                if (pri.size() >= 3)break;
            }
            if (pri.size() >= 3)
                cout<<"first "<<pri.front()*2<<endl;
            else
                cout<<"second";
        }
    }
}