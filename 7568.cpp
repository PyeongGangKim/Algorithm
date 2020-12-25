#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;


int main(){
    int num;
    scanf("%d",&num);
    vector<pair<int,int> >v(num);
    int n1,n2;
    for(int i = 0 ; i < num; i++){
        scanf("%d %d",&n1,&n2);
        v[i].first = n1;
        v[i].second = n2;
    }
    for(int i = 0 ; i < num; i++){
        int rank = 1;
        for(int j = 0; j < num; j++){
            if(v[i].first < v[j].first && v[i].second < v[j].second){
                rank++;
            }
        }
        printf("%d ",rank);
    }
    return 0;
}