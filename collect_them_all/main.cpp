#include "bits/stdc++.h"

using namespace std;

int wid=6,hei=6;

vector<vector<int>> board;

const vector<int> colors={'r','g','b','p','o'};

void initBoard(){
    board.resize(wid,vector<int>(hei));
    for(int i=0;i<wid;++i){
        for(int j=0;j<hei;++j){
            board[i][j]=colors[rand()%colors.size()];
        }
    }
}

