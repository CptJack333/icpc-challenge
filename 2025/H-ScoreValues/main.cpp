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
long long frobenius;

// num：数字
// totalLen：补0后总长度
// pos：取第几位（从左 1 开始）
int getPaddedDigit(long long num, int totalLen, int pos) {
    // 转字符串
    string s = std::to_string(num);

    // 左侧补 0 到 totalLen 位
    if (s.size() < totalLen) {
        s = string(totalLen - s.size(), '0') + s;
    }

    // 返回第 pos 位数字
    return s[pos] - '0';
}


map<pair<int, int>, pair<int, string>> memo;

// 返回值: pair<最大计数, 对应的数字字符串>
// 返回值: pair<最大计数, 对应的数字字符串>
pair<int, string> dfs(int idx, int rem, bool upper_limit, bool lower_limit, int desired_digit, bool started0) {
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
    if (!upper_limit && !lower_limit && memo.count(key)) {
        return memo[key];
    }

    // 3. 确定当前位上限
    int max_digit = upper_limit ? (s_limit[idx] - '0') : 9;
    int min_digit= lower_limit? getPaddedDigit(frobenius+1,n_len,idx) :0;

    int best_count = -1;
    string best_num_str = "";

    // 4. 枚举数字
    for (int d = min_digit; d <= max_digit; ++d) {
        bool next_limit = upper_limit && (d == max_digit);
        bool next_limit2 = lower_limit && (d == min_digit);
        int next_rem = (rem * 10 + d) % divisor;
        bool next_started = d!=0 || started0;

        // 递归调用
        auto result = dfs(idx + 1, next_rem, next_limit,next_limit2, desired_digit,next_started);
        int count = result.first;
        string suffix = result.second;

        // 剪枝：如果还是前导零，且选了0，直接跳过（不计数）
        if (!started0 && d == 0) {
            auto res = dfs(idx + 1, rem, next_limit,next_limit2, 0, false);
            if (res.first > best_count) {
                best_count = res.first;
                best_num_str = res.second; // 前导零不加到字符串里，或者加到最后处理
                // 这里为了方便，我们在最终输出时处理前导零，或者逻辑上认为空字符串代表前导零部分
                // 但为了统一，这里我们让返回的字符串不包含前导零
            }
        } else {
            if (count != -1) {
                int current_count = count;
                if (desired_digit != 6 && desired_digit != 9) {
                    if (d == desired_digit)
                        ++current_count;
                } else {
                    if (d == 6 || d == 9)
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
    }

    // 5. 存储并返回
    if (best_count == -1) {
        return {-1, ""};
    }

    if (!upper_limit && !lower_limit) {
        memo[key] = {best_count, best_num_str};
    }

    return {best_count, best_num_str};
}



pair<vector<long long>,long long> get_reachable_score_under_frobenius_number(vector<int> coins){
     if(coins.size()==1){
        return {{0, coins.front()}, -1};
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

//# 找 Frobenius 数（最大不能凑的数）
    long long frob = -1;
    for (long long x=upper-1;x>=0;--x) {
        if( !dp[x]){
            frob=divisor*x;
            break;
        }
    }

    return {ret,frob};
}

bool debug =false;

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
    frobenius=frob;
    if(debug) {
        std::cout << "frobenius num " << frobenius << std::endl;
        std::cout << "gcd " << divisor << std::endl << std::endl;
    }

    vector<pair<int,string>> results(9);
//如果m< Frobenius数，只需要处理小面值
    for(auto s :scores) {
        vector<int> digit_count(9,0);
        //处理score 0的特殊情况
        if(s==0){
            results[0].first=1;
            results[0].second="0";
            continue;
        }
//处理超出m的截断分数
        bool capped=false;
        if (s > m){
            s=m;
            capped=true;
        }
        auto ss=s;
//            按数位分解统计数字个数
        while(s){
            auto d=s%10;
            if(d==9)
                d=6;
            s/=10;
            ++digit_count[d];
        }
        for(int d=0; d <= 8; ++d){
            if(digit_count[d] > results[d].first){
                results[d].first=digit_count[d];
                results[d].second=std::to_string(ss);
            }
        }
//后面的分数都是m，截断
        if(capped)
            break;
    }

//否则，用数位dp，对每一个数字，测试最长能达到的长度
    if(frobenius+divisor<m)
        for(int d=0;d<=8;++d){
            memo.clear();
    //            深度优先数位dp
            pair<int,string> result;
            result = dfs(0, 0, true,true, d,d!=0);
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
            if(cnt>result.first){
                result.first=cnt;
                result.second=std::to_string(m);
            }

            if(result.first>results[d].first)
                results[d]=result;
        }

    for(int d=0;d<=8;++d) {
        auto ret=results[d].first;
//输出结果
        if (ret > 0) {
            std::cout << d << " " << ret << std::endl;
        if(debug)
            std::cout<<results[d].second<<std::endl;
        }
    }

    return 0;
}


