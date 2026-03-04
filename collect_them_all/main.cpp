#include "bits/stdc++.h"

using namespace std;

int wid=6,hei=6;

vector<vector<int>> board;

const vector<int> colors={'r','g','b','p','o'};

void initBoard(){
    board.resize(wid,vector<int>(hei));
    for(int i=0;i<wid;++i){
        for(int j=0;j<hei;++j){
//            rand使用当前时间作为种子，确保每次运行结果不同
            srand(time(0)+i*wid+j);
            board[i][j]=colors[rand()%colors.size()];
        }
    }
}

void printBoard(){
    cout<<"***********************************"<<endl;
    for(int i=0;i<wid;++i){
        for(int j=0;j<hei;++j) {
            cout<<char(board[i][j]);
        }
        cout<<endl;
    }
    cout<<"***********************************"<<endl;
}

bool collect(vector<pair<int,int>> route){
    auto fr=route.front();
    auto c=board[fr.first][fr.second];
//    检验每个格子颜色是否相同
    for(auto p :route){
        if(board[p.first][p.second]!=c) return false;
    }
    //    检验route上的每个格子，是否在九宫格范围内相邻
    for(int i=1;i<route.size();++i){
        auto p=route[i];
        if(abs(p.first-route[i-1].first)>1||abs(p.second-route[i-1].second)>1) return false;
    }
    return true;
}



int main(){
    initBoard();
    printBoard();
    //读取
}