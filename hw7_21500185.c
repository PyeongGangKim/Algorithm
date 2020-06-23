#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>
#include <time.h>


typedef struct Node{
    int location;
    int dist;
    struct Node* next;
}node;


typedef struct heap_Node{
    int location;
    int key;
}heap_node;

typedef struct City{
    char* city_name;
}city;
FILE* fp_w;
city* cities[40];
node* cityNode[40];
int size=0;
heap_node heap[40];
int heapSize=0;
int **dijkstra_result;
struct timeval start_time;
struct timeval end_time;



//----heap
int** init_result(int ** result){
    result = (int**)malloc(sizeof(int*)*(size+1));
    for(int i = 0; i <= size; i++){
        result[i] = (int*)malloc(sizeof(int)*(size+1));
    }
    return result;
}
void swap(int a, int b){//change the node's position 
    heap_node temp= heap[a];
    heap[a]=heap[b];
    heap[b]=temp;
}
void decreaseKey(int i,int key){//increase the value of the selected index. 
    if(key>heap[i].key){
        printf("new key is larger than current key.\n");
        return;
    }
    heap[i].key=key;
    while(i>1&&(heap[i/2].key)>(heap[i].key)){//execute when index size is less than 1 and current key value is greater than parent's key value.
        swap(i/2,i);//change position with parent's node.
        i=i/2;
    }
}
void insert (int location, int newKey){//insert new node
    if(heapSize==30){
        printf("queue is full.\n");
        return;
    }
    char newElement[50];
    heap_node* newNode=(heap_node*)malloc(sizeof(heap_node));
    newNode->location = location;
    newNode->key = 1000000;
    heapSize++;
    heap[heapSize] = *newNode;//first insert the new node at the end of array.
    decreaseKey(heapSize,newKey);//move the new node to the correct position in the heap.
}
void minHeapify(int i){//maintain the max-heap property
   int lChild=i*2;//left child index
   int rChild=i*2+1;//right child indx
   int smallest;
   if(lChild<=heapSize&&heap[lChild].key<heap[i].key) smallest=lChild;//if the left child exists and is bigger than parent, then put it into the largest.
   else smallest=i;
   if(rChild<=heapSize&&heap[rChild].key<heap[smallest].key) smallest=rChild;//if the right child exists and is bigger than largest, then put it into the largest. 
   if(smallest!=i){ 
       swap(i,smallest);
       minHeapify(smallest);//repeat until max-heap is reached
   }
}
heap_node* delete(){
    if(heapSize<1){
        printf("heap underflow.\n");
        return NULL;
    }
    heap_node* min=(heap_node*)malloc(sizeof(heap_node));
    min -> key = heap[1].key;
    min -> location = heap[1].location;
    heap[1]=heap[heapSize];//replace the first node with the last one.
    heapSize--;
    minHeapify(1);
    return min;
}
heap_node* pop(){
    return delete();
}
int find_heap(int location){
    int i;
    for( i = 1; i <= heapSize; i++){
        if(heap[i].location == location) break;
    }
    return i;
}
void change_heap(int location, int key){
    int index = find_heap(location);
    decreaseKey(index,key);
}
//-----여기까지 heap
void init_node(char* newCity, int num){
    city* ct = (city*)malloc(sizeof(city));
    ct -> city_name = (char*)malloc(sizeof(char)*40);
    strcpy(ct -> city_name,newCity);
    node* root = (node*)malloc(sizeof(node));    
    root -> location = num;
    root -> dist = 0;
    root -> next = NULL;
    cityNode[num] = root;
    cities[num] = ct;

}

void add_node(int num, int location, int curNode){
    if(location == curNode) return;
    node* newNode = (node*)malloc(sizeof(node));
    newNode -> location = location;
    newNode -> dist = num;
    newNode -> next = NULL;
    node* cur = cityNode[curNode];
    while(cur->next!=NULL){
        cur = cur->next;
    }
    cur-> next = newNode;
}
void print(int ** result){
    fprintf(fp_w,"\t\t");
    for(int i = 1; i <= size; i++){
        fprintf(fp_w,"%-10s",cities[i]->city_name);
    }
    fprintf(fp_w,"\n");
    for(int i = 1; i <= size; i++){
        fprintf(fp_w,"%-10s",cities[i]->city_name);
        for(int j = 1; j <= size; j++){
            fprintf(fp_w,"%-10d",result[i][j]);
        }
        fprintf(fp_w,"\n");
    }
}

void dijkstra_init(int n){
    for(int i = 1; i <= size; i++){
        cityNode[i] -> dist = 1000000;
    }
    cityNode[n] -> dist = 0;
}
void dijkstra_calculate(int location){
    node* cur = cityNode[location]->next;
    int value = cityNode[location] -> dist;
    while(cur != NULL){
        int sum = (value + cur->dist);
        if(cityNode[cur->location]->dist > sum){
            cityNode[cur->location]->dist = sum;
            change_heap(cur->location,sum);
        }
        cur = cur->next;
    }

}

void _dijkstra(int n){
    dijkstra_init(n);
    for(int i = 1; i <= size; i++){
        insert(cityNode[i] -> location,cityNode[i] -> dist);
    }
    while(heapSize>0){
        heap_node* temp = pop();
        dijkstra_result[n][temp->location] = temp -> key;
        //printf("pop temp: (%d,%d)\n",temp->location,temp->key);
        dijkstra_calculate(temp->location);
    }
    dijkstra_result[n][n] = 0;
}
void dijkstra(){
    gettimeofday(&start_time, NULL);
    for(int i = 1; i <= size; i++){
        _dijkstra(i);
    }
    gettimeofday(&end_time, NULL);
}
//------------dijkstra 끝-----------
//----------floyd 시작-------
int **floyd_source_odd;
int **floyd_source_even;
void init_floyd(){
    for(int i = 1; i <= size; i++){
        for(int j = 1; j <= size; j++){
            floyd_source_odd[i][j] = 1000000;
        }
    }
    for(int i = 1; i<=size; i++){
        node* cur = cityNode[i] -> next;
        while(cur != NULL){
            floyd_source_odd[i][cur->location] = cur -> dist;
            cur = cur -> next;
        }
    }
}
int** floyd_calculate(){
    int** floyd_s;
    int** floyd_r;
    for(int k = 1; k <= size; k++){
        if(k%2 == 0){
            floyd_s = floyd_source_even;
            floyd_r = floyd_source_odd;
        }else{
            floyd_s = floyd_source_odd;
            floyd_r = floyd_source_even;
        }
        for(int i = 1; i <= size; i++){
            for(int j = 1; j <= size; j++){
                if(i==j){ 
                    floyd_r[i][j] = 0;
                    continue;
                }
                if(floyd_s[i][k] + floyd_s[k][j] < floyd_s[i][j]){
                    floyd_r[i][j] = floyd_s[i][k] + floyd_s[k][j];
                }
                else{
                    floyd_r[i][j] = floyd_s[i][j];
                }
            }
        }
    }
    return floyd_r;
}
int** floyd(){
    gettimeofday(&start_time, NULL);
    init_floyd();
    gettimeofday(&end_time, NULL);
    return(floyd_calculate());
    
}
int negative_weight_check(){
    for(int i = 1; i <= size; i++){
        node* cur = cityNode[i]->next;
        while(cur != NULL){
            if(cur -> dist < 0) return 0;
            cur = cur -> next;
        }
    }
    return 1;
}


int main(){
    FILE* fp;
    fp = fopen("./hw7.data", "r");
    fp_w = fopen("output.txt","w");
    char buf[1000];
    char tCity[40];
    memset(tCity,'\0',40);
    int k = 0;
    int t = 0;
    int c = 0;
    char temp[5];
    int num = 0;
    int tSize = 0;
    fgets(buf,sizeof(buf),fp);
    while(1){
        if(buf[k] == '\n' || buf[k] == '\0'){
           if((buf[k-1]>=97&&buf[k-1]<=122) || (buf[k-1]>=65&&buf[k-1]<=90)){
               size++;
               tCity[c] = '\0';
               init_node(tCity,size);
               c = 0;
               memset(tCity,'\0',40);
           }
           break;
        }
        if(buf[k] == ' ' || buf[k] == '\t'){
            if((buf[k-1]>=97&&buf[k-1]<=122) || (buf[k-1]>=65&&buf[k-1]<=90)){
               size++;
               tCity[c] = '\0';
               init_node(tCity,size);
               c = 0;
               memset(tCity,'\0',40);
           }
        }
        if((buf[k]>=97&&buf[k]<=122) || (buf[k]>=65&&buf[k]<=90)){
            tCity[c] = buf[k];
            c++;
        }
        k++;
    }
    
    dijkstra_result = init_result(dijkstra_result);
    floyd_source_odd = init_result(floyd_source_odd);
    floyd_source_even = init_result(floyd_source_even);
    
    int cur_node = 1;
    memset(temp,0,5);
    while(fgets(buf,sizeof(buf),fp) != NULL){//숫자가 처음 받는 거 하기.
        k=0;
        tSize = 0;
        while(1){
            if(buf[k]=='\n'|| buf[k] == '\0'){//한줄 맨끝
                if(buf[k-1]>=48 && buf[k-1]<=57) {//전이 숫자면 숫자로 변환
                    temp[t+1] = '\0';
                    num = atoi(temp);
                    add_node(num,tSize,cur_node);
                    memset(temp,0,5);
                    tSize++;
                    t=0;
                }
                break;
            }
            if(buf[k] == ' ' || buf[k] == '\t'){//중간마다 체크
                if(buf[k-1]>=48 && buf[k-1]<=57){//전이 숫자면 숫자로 변환
                    temp[t+1] = '\0';
                    num = atoi(temp);
                    add_node(num,tSize,cur_node);
                    memset(temp,0,5);
                    tSize++;
                    t=0;
                }
                else if((buf[k-1]>=97&&buf[k-1]<=122) || (buf[k-1]>=65&&buf[k-1]<=90)){//숫자가 아니고 문자면 그냥 location 위치만 변경
                    tSize++;
                }

            }
            if((buf[k]>=48 && buf[k]<=57) || buf[k] == 45){//현재가 숫자면 temp에 저장하고 temp index 한칸 올리기.
                temp[t] = buf[k];
                t++;
            }
            k++;
        }   
        cur_node++;
    }
    double dijkstra_time = 0;
    double floyd_time = 0;
    int check = 0;
    check = negative_weight_check();
    if(check == 1){
        dijkstra();
        dijkstra_time = (double)(end_time.tv_sec - start_time.tv_sec ) + ((double)( end_time.tv_usec - start_time.tv_usec ) / 1000000.0);
        fprintf(fp_w,"It took %lf seconds to compute shortest path between cities with Dijkstra's algorithm as follows\n",dijkstra_time);
        print(dijkstra_result);
    }
    else{
        printf("graph has negative weight\n");
    }

    
    int ** floyd_result = floyd();
    
    floyd_time = (double)( end_time.tv_sec - start_time.tv_sec ) + ((double)( end_time.tv_usec - start_time.tv_usec ) / 1000000.0);
    printf("start_time.tv_sec: %d start_time.tv_usec : %d\n",start_time.tv_sec,start_time.tv_usec);
    printf("end_time.tv_sec: %d end_time.tv_usec : %d \n",end_time.tv_sec,end_time.tv_usec);
    fprintf(fp_w,"It took %lf seconds to compute shortest path between cities with Dijkstra's algorithm as follows\n",floyd_time);
    print(floyd_result);
}