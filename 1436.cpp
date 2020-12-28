#include <iostream>
#include <string>

using namespace std;

int main(){
    int num;
    scanf("%d",&num);
    string match = "666";
    int name = 665;
    int count = 0;
    while(1){
        string name_str = to_string(name);
        if(name_str.find(match) < (int)name_str.length()){
            count++;
        }
        if(count == num) break;
        name++;
    }
    printf("%d\n",name);

}