#include "bits/stdc++.h"

//、、先算出所有分数的gcd，然后算出这组分数的佛尼列斯数
//然后就可以知道哪个范围内的分数可以凑出来了，并且是gcd的倍数
//然后 用数位dp，对每一个数字，测试最长能达到的长度
//对6和9特殊一下处理

using std::string;
using std::cin;
using std::vector;
using std::map;
using std::pair;

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

int dfs(int index, int remainder, bool limit, int desired_digit){//return best_count, best_num_str
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
        auto count=dfs(index+1,next_rem,next_limit,desired_digit);
        if(count!=-1){
            auto current_count=count;
            if(desired_digit!=6&&desired_digit!=9) {
                if (d == desired_digit)
                    ++current_count;
            }else{
                if(d==6||d==9)
                    ++current_count;
            }
            best_count=std::max(best_count,current_count);
        }
    }
//    if(best_count==-1)
//        return -1;
    if(!limit)
        memo[key]=best_count;
    return best_count;
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
    upper_bound=m;
    s_limit=std::to_string(upper_bound);
    n_len=s_limit.size();

    for(int d=0;d<=8;++d){
        auto ret=dfs(0,0,true,d);
        if(ret>0){
            std::cout<<d<<" "<<ret<<std::endl;
        }
    }

    return 0;
}


