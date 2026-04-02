#include "bits/stdc++.h"

//、、先算出所有分数的gcd，然后算出这组分数的佛尼列斯数
//然后就可以知道哪个范围内的分数可以凑出来了，并且是gcd的倍数
//然后 用数位dp，对每一个数字，测试最长能达到的长度
//对6和9特殊一下处理

using namespace std;

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


int main(){
    int m,n;
    cin>>m>>n;
    vector<int> p(n);
    for(int i=0;i<n;++i)
        cin>>p[i];
    int gcd= vector_gcd(p);


}


