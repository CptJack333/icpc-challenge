#include "bits/stdc++.h"

//先算出所有分数的gcd，然后算出这组分数的Frobenius数
//列出Frobenius数以下的，可以拼出的面值
//然后就可以知道可以拼出的所有面值了，比Frobenius数大的面值，是gcd的倍数
//如果m< Frobenius数，只需要处理小面值
//否则，用数位dp，对每一个数字，测试最长能达到的长度
//对6和9特殊一下处理

using std::string;
using std::cin;
using std::cout;
using std::endl;
using std::vector;
using std::map;
using std::pair;
using std::make_pair;

int vector_gcd(const std::vector<int>& nums) {
    if (nums.empty()) return 0; // 空向量自定义处理

    return std::reduce(
            nums.begin(),
            nums.end(),
            nums[0],
            [](int a, int b) {
                return std::gcd(a, b);
            }
    );
}

int divisor;
long long upper_bound;
string s_limit;
int n_len;
map<pair<int,int>,pair<int,string>> memo;

pair<int, string> dfs(int idx, int rem, bool limit, int desired_digit) {
    memo.clear();
    // 1. 基本情况
    if (idx == n_len) {
        if (rem == 0) {
            return {0, ""}; // 找到解，返回计数0和空字符串
        } else {
            return {-1, ""}; // 无效解
        }
    }

    // 2. 记忆化查询
    auto key = make_pair(idx, rem);
    if (!limit && memo.count(key)) {
        return memo[key];
    }

    // 3. 确定当前位上限
    int max_digit = limit ? (s_limit[idx] - '0') : 9;

    int best_count = -1;
    string best_num_str = "";

    // 4. 枚举数字
    for (int d = 0; d <= max_digit; ++d) {
        bool next_limit = limit && (d == max_digit);
        int next_rem = (rem * 10 + d) % divisor;

        // 递归调用
        auto result = dfs(idx + 1, next_rem, next_limit, desired_digit);
        int count = result.first;
        string suffix = result.second;

        if (count != -1) {
            // 修正：忠实还原 Python 的计数逻辑
            int current_count = count + (d == desired_digit ? 1 : 0);

            // 修正：还原 Python 的更新逻辑
            // 只有当计数严格大于当前最优时才更新
            // 这样配合 d 从 0->9 的循环，保证了 count 相同时保留字典序较小的解
            if (current_count > best_count) {
                best_count = current_count;
                best_num_str = std::to_string(d) + suffix;
            }
        }
    }

    // 5. 存储并返回
    if (best_count == -1) {
        return {-1, ""};
    }

    if (!limit) {
        memo[key] = {best_count, best_num_str};
    }

    return {best_count, best_num_str};
}

int main(){
    long long m;
    int n;
    cin>>m>>n;
    vector<int> p(n);
    for(int i=0;i<n;++i)
        cin>>p[i];
    int gcd= vector_gcd(p);

    divisor= gcd;
    cout<<"gcd "<<gcd<<endl;
    upper_bound=m;
    s_limit=std::to_string(upper_bound);
    cout<<"s_limit "<<s_limit<<endl;
    n_len=s_limit.size();

    for(int d=0;d<=9;++d){
        auto [ret,ss]=dfs(0,0,true,d);
        if(ret>0){
            std::cout<<d<<" "<<ret<<" "<<ss<<std::endl;
        }
    }

    return 0;
}


