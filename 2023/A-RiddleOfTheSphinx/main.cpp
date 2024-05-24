#include "bits/stdc++.h"

using namespace std;

int main(){
    vector<vector<int>> A={
            {1,0,0},
            {0,1,0},
            {0,0,1},
            {1,1,1},
            {1,2,3},
    };

    vector<int> ans(5,0);

    int a,b,c,d,e;
    cout<<"1 0 0"<<endl;
    cin>>a;
    cout<<"0 1 0"<<endl;
    cin>>b;
    cout<<"0 0 1"<<endl;
    cin>>c;
    cout<<"1 1 1"<<endl;
    cin>>d;
    cout<<"1 2 3"<<endl;
    cin>>e;
    //最后两个问题其中之一撒谎，代表前面三个问题是诚实的。另外一个方程判断不等，其实不是必须的。
    if( a+b+c==d // && a+2*b+3*c!=e
        ||
        a+2*b+3*c==e // && a+b+c!=d
    ) {
        cout << a << " " << b << " " << c << endl;
        return 0;
    }
    //前面三个问题其中之一撒谎，则剩下四个等式成立，分别去验证
    if(d-b-c+2*b+3*c==e)
        cout << a << " " << b << " " << c << endl;
    else if(d-a-c+a+3*c==e)
        cout << a << " " << b << " " << c << endl;
    else if(d-b-a+2*b+a==e)
        cout << a << " " << b << " " << c << endl;
    return 0;
}