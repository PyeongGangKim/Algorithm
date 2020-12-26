#include <iostream>
#include <string>

using namespace std;


string black[8] = {
    "BWBWBWBW",
    "WBWBWBWB",
    "BWBWBWBW",
    "WBWBWBWB",
    "BWBWBWBW",
    "WBWBWBWB",
    "BWBWBWBW",
    "WBWBWBWB"
};
string white[8] = {
    "WBWBWBWB",
    "BWBWBWBW",
    "WBWBWBWB",
    "BWBWBWBW",
    "WBWBWBWB",
    "BWBWBWBW",
    "WBWBWBWB",
    "BWBWBWBW"
};
string chess[50];
int black_check(int x,int y){
    int diff = 0;
    for(int i = x; i < x + 8; i++){
        for(int j = y; j < y + 8; j++){
            if(chess[i][j] != black[i-x][j-y]) diff++;
        }
    }
    return diff;
}
int white_check(int x,int y){
    int diff = 0;
    for(int i = x; i < x + 8; i++){
        for(int j = y; j < y + 8; j++){
            if(chess[i][j] != white[i-x][j-y]) diff++;
        }
    }
    return diff;
}
int main(){
    int min = 1000000;
    int row,col;
    scanf("%d %d",&row,&col);
    for(int i = 0; i < row; i++){
        cin >>chess[i];
    }
    for(int i = 0; i <= row - 8; i++){
        for(int j = 0; j <= col - 8; j++){
            int black_num = black_check(i,j);
            int white_num = white_check(i,j);
            if(min > black_num) min = black_num;
            if(min > white_num) min = white_num; 
        }
    }
    printf("%d\n",min);
}