//my program works for greedy and DP and B&B solution.
//gcc 6.3.0에서 정상작동합니다
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
FILE* fp;
typedef struct item{//item struct
    int benefit;
    int weight; 
}item;
typedef struct Bbnode{//branch and bound node struct
    int benefit;
    int weight;
    double bound;
    int level;
}bbnode;
void swap(item *items, int a, int b){//use this function when sorting the item
    item temp = items[a];
    items[a] = items[b];
    items[b] = temp;
}
int quick_partition(item *item, int left, int right){//quick sort
    float pivot = (float)item[left].benefit/item[left].weight;
    int low = left+1;
    int high = right;

    while(low<=high){
        while(low<= right && pivot<= (float)item[low].benefit/item[low].weight){
            low++;
        }
        while(high >= (left+1) && pivot >= (float)item[high].benefit/item[high].weight){
            high--;
        }
        if(low<=high){
            swap(item,low,high);
        }
    }
    swap(item,left,high);
    return high;
}
void quickSort(item *item, int left, int right){//quick sort
    if(left<=right){
        int pivot = quick_partition(item,left,right);
        quickSort(item,left,pivot-1);
        quickSort(item,pivot+1,right);
    }
}
void greedy(item *item, int numItem, int maxW){//greedy program
    quickSort(item,1,numItem);//quick sort
    clock_t begin,end,middle; 
    double middle_time=0;
    begin= clock();
    int total_weight=0;
    double total_benefit=0;
    int i=1;
    while(i<=numItem){//get the benefit
        middle=clock();
        middle_time= ((double)middle - begin) / CLOCKS_PER_SEC;
        if(middle_time>=900) break;
        if(total_weight+item[i].weight>=maxW){//the case is that the sum of current item weight and total weight is greater than or equal to max weight 
            if(total_weight+item[i].weight==maxW) {// equal to max weight
                total_benefit+= item[i].benefit;
                total_weight+= item[i].weight;
            }
            else{//total weight is greater than max weight
                double fraction = (double)item[i].benefit/item[i].weight;//get the fractional value
                for(int j=0;j<item[i].weight;j++){
                    if(total_weight+1<=maxW){
                        total_weight+=1;
                        total_benefit+=fraction;
                    }
                    else break;
                }
            }
            break;
        }
        else{
            total_benefit+=item[i].benefit;
            total_weight+=item[i].weight;
        }
        i++;
    }
    double maxB= total_benefit;
    end=clock();
    double time=0;
    char greedy[50];
    time= ((double)end - begin) / CLOCKS_PER_SEC * 1000;
    if(middle_time>=10){
        sprintf(greedy,"%.3fms / $0 ",middle_time);
        fprintf(fp,"%-30s ",greedy);
        return;
    }
    sprintf(greedy,"%.3fms / $%.3f ",time,maxB);
    fprintf(fp,"%-30s ",greedy);
}   
void dp_calculate(item *item,int **b,int checknum,int bChecknum,int maxW,int inum){
    for(int j=1;j<=maxW;j++){
                if(item[inum].weight<=j){
                    if(item[inum].benefit+b[bChecknum][j-item[inum].weight]>b[bChecknum][j]) b[checknum][j]=item[inum].benefit+b[bChecknum][j-item[inum].weight];
                    else b[checknum][j]=b[bChecknum][j];
                }
                else b[checknum][j]=b[bChecknum][j];
                }
}
void dp(item *item,int numItem, int maxW){//dynamic programing space를 줄이기 위해 아이템 행을 두개로 줄여서 번갈아 가면서 사용함.
    clock_t begin =clock();
    clock_t end,middle;
    double middle_time=0;
    int checkNum=0;
    int **b=(int**)malloc(sizeof(int*)*2);
    for(int i=0;i<2;i++){
        b[i]=(int*)malloc(sizeof(int)*(maxW+1));
    }
    for(int i=0;i<2;i++){
        b[i][0]=0;
    }
    for(int j=0;j<=maxW;j++){
        b[0][j]=0;
    }
    for(int i=1;i<=numItem;i++){
        middle=clock();
        middle_time= ((double)middle - begin) / CLOCKS_PER_SEC;
        if(middle_time>=900) break;
        if(i%2==0){
            checkNum=0;
            dp_calculate(item,b,checkNum,1,maxW,i);
        }
        else{
            checkNum=1;
            dp_calculate(item,b,checkNum,0,maxW,i);
        }
    }
    int maxB= b[checkNum][maxW];
    for(int i=0;i<2;i++){
        free(b[i]);
    }
    free(b);
    end=clock();
    double time;
    time= ((double)end - begin) / CLOCKS_PER_SEC * 1000;
    char dp[50];
    if(middle_time>=900){
        sprintf(dp,"%.3fms / $0 ",middle_time);
        fprintf(fp,"%-30s ",dp);
        return;
    }
    sprintf(dp,"%.3fms / $%d ",time,maxB);
    fprintf(fp,"%-30s ",dp);
}
void calculate_bound (item *item,bbnode *node,int numItem,int maxW,int tot_weight){//calculate the bound of the node, level을 통해 현재 위치를 파악.
    for(int i=1;(node->level)+i<=numItem;i++){
        if(tot_weight+item[(node->level)+i].weight>=maxW){
            if(tot_weight+item[(node->level)+i].weight==maxW){
                tot_weight += item[(node->level)+i].weight;
                node->bound += (double)item[(node->level)+i].benefit;
            }
            else{
                double fraction = (double)item[(node->level)+i].benefit/item[(node->level)+i].weight;
                for(int j=0;j<item[(node->level)+i].weight;j++){
                    if(tot_weight+1<=maxW){
                        tot_weight+=1;
                        node->bound += fraction;
                    }
                    else break;
                }
            }
            break;
        }
        else{
            tot_weight += item[(node->level)+i].weight;
            node->bound += (double)item[(node->level)+i].benefit;
        }
    }
}
void expand(item *item,bbnode *pnode,bbnode *cnode,int numItem,int maxW,int choose){//expand and calculate the weight and benefit of current node
    int tot_weight;
    cnode->level = pnode->level+1;
    if(choose==1){
        cnode->weight = pnode->weight+item[cnode->level].weight;
        cnode->benefit = pnode->benefit+item[cnode->level].benefit;
        
    }
    else{
        cnode->weight = pnode->weight;
        cnode->benefit = pnode->benefit;
        
    }
    tot_weight=cnode->weight;
    cnode->bound=(double)cnode->benefit;
    if(tot_weight>maxW) {
        cnode->bound=0;
        return;
    }
    calculate_bound(item,cnode,numItem,maxW,tot_weight);
    
}
void heapSwap(bbnode* heap[], int a, int b){
    bbnode* temp= heap[a];
    heap[a]=heap[b];
    heap[b]=temp;
}
void makeHeap(bbnode* heap[],int i){//make the heap
    if(i==1) return;
    while(i>1&&(heap[i/2]->bound)<(heap[i]->bound)){
        heapSwap(heap,i/2,i);
        i=i/2;
    }
}
void insert (bbnode* heap[], bbnode* node,int* size){//insert the promising node
    *size+=1;
    heap[*size]=node;
    makeHeap(heap,*size);
    
}
void maxHeapify(bbnode* heap[],int i,int* size){
   int lChild=i*2;
   int rChild=i*2+1;
   int largest;
   if(lChild<=*size&&heap[lChild]->bound>=heap[i]->bound) largest=lChild;
   else largest=i;
   if(rChild<=*size&&heap[rChild]->bound>=heap[largest]->bound) largest=rChild;
   if(largest!=i){ 
       heapSwap(heap,i,largest);
       maxHeapify(heap,largest,size);
   }
}
bbnode* delete(bbnode* heap[],int* size){//pop the top bound node and delete it.
    if(*size<1){
        return NULL;
    }
    bbnode *max=heap[1];
    heap[1]=heap[*size];
    *size-=1;
    maxHeapify(heap,1,size);
    return max;
}
void bAndb(item* item,int numItem, int maxW){//branch and bound
    bbnode* heap[100000];
    quickSort(item,1,numItem);
    clock_t begin, end,middle;
    double middle_time=0;
    begin=clock();
    int num=0;
    int *size=&num;
    int max_benefit=0;
    bbnode *node =(bbnode*)malloc(sizeof(bbnode));
    node->level=0;
    node->weight=0;
    node->benefit=0;
    node->bound=0;
    calculate_bound(item,node,numItem,maxW,0);
    insert(heap,node,size);
    while(1){
        middle=clock();
        middle_time= ((double)middle - begin) / CLOCKS_PER_SEC;
        if(middle_time>=900) break;
        bbnode* node = delete(heap,size);
        if(node->bound<=max_benefit||node==NULL) break;
        bbnode *lnode =(bbnode*)malloc(sizeof(bbnode));
        bbnode *rnode =(bbnode*)malloc(sizeof(bbnode));
        expand(item,node,lnode,numItem,maxW,1);
        expand(item,node,rnode,numItem,maxW,0);
        if(lnode->benefit>max_benefit&&lnode->weight<=maxW) max_benefit=lnode->benefit;
        if(rnode->benefit>max_benefit&&rnode->weight<=maxW) max_benefit=rnode->benefit;
        if(lnode->bound>=max_benefit) insert(heap,lnode,size);
        else free(lnode);
        if(rnode->bound>=max_benefit) insert(heap,rnode,size);
        else free(rnode);
        free(node);
    }
    end=clock();
    double time=0;
    time= ((double)end - begin) / CLOCKS_PER_SEC * 1000;
    char bb[50];
    if(middle_time>=900){
        sprintf(bb,"%.3fms / $0 ",middle_time);
        fprintf(fp,"%-30s ",bb);
        return;
    }
    sprintf(bb,"%.3fms / $%d ",time,max_benefit);
    fprintf(fp,"%s\n",bb);
}
int main(){
    clock_t wholeTimeB,WholeTimeE;
    wholeTimeB=clock();
    double wholeTime=0;
    item item[10001];
    int maxW=0;
    srand(time(NULL));
    item[0].weight=item[0].benefit=0;
    for(int j=1;j<10001;j++){
            int wRandom= (rand()%100)+1;
            int bRandom= (rand()%300)+1;
            item[j].weight=wRandom;
            item[j].benefit=bRandom;
    }
    fp = fopen("output.txt","w");
    if(fp==NULL){
        printf("file open error!");
        exit(0);
    }
    fprintf(fp,"Number of\t\t Processing time in milliseconds / Maximum benefit value\n");
    fprintf(fp,"%-15s %-30s %-30s %s  \n","items","Greedy","D.P","B.& B.");
    int numitems[9]={10,100,500,1000,3000,5000,7000,9000,10000};
    for(int i=0;i<9;i++){
        fprintf(fp,"%-15d ",numitems[i]);
        maxW=numitems[i]*40;
        greedy(item,numitems[i],maxW);
        dp(item,numitems[i],maxW);
        bAndb(item,numitems[i],maxW);
        WholeTimeE=clock();
        wholeTime= ((double)WholeTimeE - wholeTimeB) / CLOCKS_PER_SEC;
        if(wholeTime>=4500) break;
    }

}
