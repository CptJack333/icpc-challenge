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
map<pair<int,int>,int> memo;
long long m;

int dfs(int index, int remainder, bool limit, int desired_digit,int leading_zeros){//return best_count, best_num_str
    if(index==n_len){
        if( remainder==0)
            return 0;
        else
            return -1;
    }
    auto key =std::make_pair(index,remainder);
    if (!limit && memo.count(key)){
        return memo[key];
    }
    int max_digit=limit? s_limit[index]-'0':9;
    auto best_count=-1;
    for(int d=0;d<=max_digit;++d){
        auto next_limit=limit && (d==max_digit);
        auto next_rem=(remainder*10+d)%divisor;
        auto next_lzeros=leading_zeros;
        if(d==0 && leading_zeros==index)
            ++next_lzeros;
        auto count=dfs(index+1,next_rem,next_limit,desired_digit,next_lzeros);
        if(count!=-1){
            auto current_count=count;
            if(desired_digit!=6&&desired_digit!=9) {
                if(desired_digit==0){
                    if(d==0 && next_lzeros<index+1)//当前的0不是由开头连到这个位置的
                        ++current_count;
                } else if (d == desired_digit)
                    ++current_count;
            }else{
                if(d==6||d==9)
                    ++current_count;
            }
            best_count=std::max(best_count,current_count);
        }
    }
    if(best_count==-1)
        return -1;
    if(!limit)
        memo[key]=best_count;
    return best_count;
}

vector<long long> get_reachable_score_under_frobenius_number(vector<int> coins){
     if(coins.size()==1){
        return {0,std::min((int)m,coins.front())};
    }

    if(divisor!=1){
        for(auto& c :coins)
            c/=divisor;
    }
    auto max_coin=*max_element(coins.begin(), coins.end());
    auto upper=std::min((long long)max_coin*max_coin,m+1);
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
            bool capped=false;
            if(score>m){
                capped=true;
                score=m;
                if(ret.back()!=score)
                    ret.push_back(score);
            }else
                ret.push_back(score);
            if(capped)
                break;
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
    auto scores= get_reachable_score_under_frobenius_number(p);
//如果m< Frobenius数，只需要处理小面值
    if(scores.back()>=m){
        vector<int> digit_count_max(9,0);
        for(auto s :scores) {
            vector<int> digit_count(9,0);

            if(s==0){
                digit_count_max[0]=1;
                continue;
            }

            if (s > m)
                break;
            while(s){
                auto d=s%10;
                if(d==9)
                    d=6;
                s/=10;
                ++digit_count[d];
            }
            for(int i=0;i<=8;++i)
                digit_count_max[i]=std::max(digit_count[i],digit_count_max[i]);
        }
        for(int d=0;d<=8;++d){
            if(digit_count_max[d]>0)
                std::cout<<d<<" "<<digit_count_max[d]<<std::endl;
        }
    }else{
//否则，用数位dp，对每一个数字，测试最长能达到的长度
        for(int d=0;d<=8;++d){
            memo.clear();
            auto ret=dfs(0,0,true,d,0);
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
            if(ret>0 && d!=9){
                std::cout<<d<<" "<<ret<<std::endl;
            }
        }
    }

    return 0;
}


