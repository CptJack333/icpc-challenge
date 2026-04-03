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
long long m;

map<std::tuple<int, int, bool, bool>, pair<int, string>> memo0;

pair<int, string> dfs0(int idx, int rem, bool limit, bool started) {
    // 基本情况：处理完所有位数
    if (idx == n_len) {
        // 如果构成了有效数字（非前导零状态）且能被整除
        if (started && rem == 0) {
            return {0, ""};
        }
        return {-1, ""}; // 无效解
    }

    // 记忆化检查
    // 注意：只有在不受limit约束时才能使用缓存，因为limit=true的情况路径唯一
    // started状态也必须包含在key中，或者通过逻辑处理（这里为了严谨包含进去）
    auto key = std::make_tuple(idx, rem, limit, started);
    if (!limit && memo0.count(key)) {
        return memo0[key];
    }

    int max_digit = limit ? (s_limit[idx] - '0') : 9;

    int best_zeros = -1;
    string best_num = "";

    // 从大到小枚举，优先找大数，在0数量相同时自动保留数值较大的解
    for (int d = max_digit; d >= 0; --d) {
        bool next_limit = limit && (d == max_digit);
        bool next_started = started || (d != 0);

        // 剪枝：如果还是前导零，且选了0，直接跳过（不计数）
        if (!started && d == 0) {
            auto res = dfs0(idx + 1, rem, next_limit, false);
            if (res.first > best_zeros) {
                best_zeros = res.first;
                best_num = res.second; // 前导零不加到字符串里，或者加到最后处理
                // 这里为了方便，我们在最终输出时处理前导零，或者逻辑上认为空字符串代表前导零部分
                // 但为了统一，这里我们让返回的字符串不包含前导零
            }
        } else {
            int next_rem = (rem * 10 + d) % divisor;
            auto res = dfs0(idx + 1, next_rem, next_limit, true);

            if (res.first != -1) {
                int current_zeros = res.first + (d == 0 ? 1 : 0);
                if (current_zeros > best_zeros) {
                    best_zeros = current_zeros;
                    best_num = std::to_string(d) + res.second;
                }
            }
        }
    }

    // 存储结果
    if (!limit) {
        memo0[key] = {best_zeros, best_num};
    }

    return {best_zeros, best_num};
}

map<pair<int, int>, pair<int, string>> memo;

// 返回值: pair<最大计数, 对应的数字字符串>
// 返回值: pair<最大计数, 对应的数字字符串>
pair<int, string> dfs(int idx, int rem, bool limit, int desired_digit) {
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
            int current_count = count;
            if(desired_digit!=6&&desired_digit!=9) {
                if (d == desired_digit)
                    ++current_count;
            }else{
                if(d==6||d==9)
                    ++current_count;
            }

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



pair<vector<long long>,long long> get_reachable_score_under_frobenius_number(vector<int> coins){
     if(coins.size()==1){
        return {{0,std::min((int)m,coins.front())}, -1};
    }

    if(divisor!=1){
        for(auto& c :coins)
            c/=divisor;
    }
    auto max_coin=*max_element(coins.begin(), coins.end());
    auto upper=max_coin*max_coin;
    vector<long long> dp(upper,false);
    dp[0]=true;
    for(int i=0;i<upper;++i){
        if(dp[i])
            for(auto c :coins)
                if(i+c<upper)
                    dp[i+c]=true;
    }

    vector<long long>ret;
    for(int i=0;i<upper;++i){
        if(dp[i]){
            auto score=i*divisor;
            ret.push_back(score);
        }
    }
    return ret;
}

int main(){
    int n;
    cin>>m>>n;
    vector<int> p(n);
    for(int i=0;i<n;++i)
        cin>>p[i];

    divisor= vector_gcd(p);
//    cout<<"gcd "<<gcd<<endl;
    upper_bound=m;
    s_limit=std::to_string(upper_bound);
//    cout<<"s_limit "<<s_limit<<endl;
    n_len=s_limit.size();

//列出Frobenius数以下的，可以拼出的面值
    auto r_frob= get_reachable_score_under_frobenius_number(p);
    auto [scores,frob]= r_frob;
//如果m< Frobenius数，只需要处理小面值
     if(frob>=m){
        vector<int> digit_count_max(9,0);
        for(auto s :scores) {
            vector<int> digit_count(9,0);
            //处理score 0的特殊情况
            if(s==0){
                digit_count_max[0]=1;
                continue;
            }
//处理超出m的截断分数
            bool capped=false;
            if (s > m){
                s=m;
                capped=true;
            }
//            按数位分解统计数字个数
            while(s){
                auto d=s%10;
                if(d==9)
                    d=6;
                s/=10;
                ++digit_count[d];
            }
            for(int i=0;i<=8;++i)
                digit_count_max[i]=std::max(digit_count[i],digit_count_max[i]);
//后面的分数都是m，截断
            if(capped)
                break;
        }
//         输出各个数字的最多值
        for(int d=0;d<=8;++d){
            if(digit_count_max[d]>0)
                std::cout<<d<<" "<<digit_count_max[d]<<std::endl;
        }
    }else{
//否则，用数位dp，对每一个数字，测试最长能达到的长度
        for(int d=0;d<=8;++d){
            memo.clear();
            int ret;
//            深度优先数位dp
            if(d!=0) {
                auto result = dfs(0, 0, true, d);
                ret=result.first;
            } else {
//            寻找0的最大长度需要另外处理，因为前导0的存在
                auto result = dfs0(0, 0, true, false);
                ret=result.first;
            }
//            不要忘了有截断的情况存在
            auto mm=m;
            auto cnt=0;
            while(mm){
                auto dd=mm%10;
                if(d!=6) {
                    if (dd == d)
                        ++cnt;
                }else{
                    if(dd==6||dd==9)
                        ++cnt;
                }

                mm/=10;
            }
            ret=std::max(ret,cnt);
//输出结果
            if(ret>0 && d!=9){
                std::cout<<d<<" "<<ret<<std::endl;
            }
        }
    }

    return 0;
}


