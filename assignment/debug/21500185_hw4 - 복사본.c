#include <stdio.h>
#include <stdlib.h>
#include <time.h>
typedef struct item{
    int benefit;
    int weight; 
}item;
typedef struct Bbnode{
    int benefit;
    int weight;
    double bound;
    int level;
}bbnode;
void swap(item *item, int a, int b){
    int temp = item[a].weight;
    item[a].weight = item[b].weight;
    item[b].weight = temp;
    temp = item[a].benefit;
    item[a].benefit = item[b].benefit;
    item[b].benefit = temp;
}
int quick_partition(item *item, int left, int right){
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
void quickSort(item *item, int left, int right){
    if(left<=right){
        int pivot = quick_partition(item,left,right);
        quickSort(item,left,pivot-1);
        quickSort(item,pivot+1,right);
    }
}
double greedy(item *item, int numItem, int maxW){
    quickSort(item,1,numItem);
    clock_t begin,end; 
    begin= clock();
    int total_weight=0;
    double total_benefit=0;
    int i=1;
    while(i<=numItem){
        if(total_weight+item[i].weight>=maxW){
            if(total_weight+item[i].weight==maxW) {
                total_benefit+= item[i].benefit;
                total_weight+= item[i].weight;
            }
            else{
                double fraction = (double)item[i].benefit/item[i].weight;
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
    time=(long double)(((end-begin)/CLOCKS_PER_SEC)*1000);
    printf("greedy time: %f\n",time);
    return maxB;
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
int dp(item *item,int numItem, int maxW){
    clock_t begin =clock();
    clock_t end;
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
    time=(double)(((end-begin)/CLOCKS_PER_SEC)*1000);
    printf("dp time: %f\n",time);
    return maxB;
}
void calculate_bound (item *item,bbnode *node,int numItem,int maxW,int tot_weight){
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
    printf("last bound: %.3f\n", node->bound);
}
void expand(item *item,bbnode *pnode,bbnode *cnode,int numItem,int maxW,int choose){
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
void heapSwap(bbnode* heap[], int a, int b){//change the node's position 
    bbnode* temp= heap[a];
    heap[a]=heap[b];
    heap[b]=temp;
}
void makeHeap(bbnode* heap[],int i){//increase the value of the selected index. 
    if(i==1) return;
    while(i>1&&(heap[i/2]->bound)<(heap[i]->bound)){//execute when index size is less than 1 and current key value is greater than parent's key value.
        heapSwap(heap,i/2,i);
        i=i/2;
    }
}
void insert (bbnode* heap[], bbnode* node,int* size){//insert new node
    *size+=1;
    //bbnode* temp =heap;
    //heap =(bbnode*)realloc(heap,sizeof(bbnode)*(1+(*size)));
    printf("hellodd\n");
    heap[*size]=node;
    printf("hellodd22\n");
    //free(temp);
    makeHeap(heap,*size);
    printf("hellodd33\n");
    //return heap;
    
}
void maxHeapify(bbnode* heap[],int i,int* size){//maintain the max-heap property
   int lChild=i*2;//left child index
   int rChild=i*2+1;//right child indx
   int largest;
   if(lChild<=*size&&heap[lChild]->bound>=heap[i]->bound) largest=lChild;//if the left child exists and is bigger than parent, then put it into the largest.
   else largest=i;
   if(rChild<=*size&&heap[rChild]->bound>=heap[largest]->bound) largest=rChild;//if the right child exists and is bigger than largest, then put it into the largest. 
   if(largest!=i){ 
       heapSwap(heap,i,largest);
       maxHeapify(heap,largest,size);//repeat until max-heap is reached
   }
}
bbnode* delete(bbnode* heap[],int* size){
    if(*size<1){
        return NULL;
    }
    bbnode *max=heap[1];
    heap[1]=heap[*size];//replace the first node with the last one.
    *size-=1;
    printf("hi111\n");
    maxHeapify(heap,1,size);
    printf("hi2221\n");
    return max;
}
int bAndb(item* item,int numItem, int maxW){
    bbnode** heap=(bbnode**)malloc(sizeof(bbnode*));
    quickSort(item,1,numItem);
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
        bbnode* node = delete(heap,size);
        if(node->bound<=max_benefit||node==NULL) break;
        bbnode *lnode =(bbnode*)malloc(sizeof(bbnode));
        bbnode *rnode =(bbnode*)malloc(sizeof(bbnode));
        expand(item,node,lnode,numItem,maxW,1);
        expand(item,node,rnode,numItem,maxW,0);
        printf("hello4\n");
        if(lnode->benefit>max_benefit&&lnode->weight<=maxW) max_benefit=lnode->benefit;
        if(rnode->benefit>max_benefit&&rnode->weight<=maxW) max_benefit=rnode->benefit;
        if(lnode->bound>=max_benefit) {
            printf("hello5\n");
            insert(heap,lnode,size);
            printf("hello6\n");
        }
        else free(lnode);
        if(rnode->bound>=max_benefit) {
            printf("hello7\n");
            insert(heap,rnode,size);
            printf("hello8\n");
        }
        else free(rnode);
        free(node);
    }
    return max_benefit;
}
int main(){
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
    int numitems[9]={10,100,500,1000,3000,5000,7000,9000,10000};
    for(int i=0;i<9;i++){
        maxW=numitems[i]*40;
        int dpResult=dp(item,numitems[i],maxW);
        double gResult=greedy(item,numitems[i],maxW);  
        int bbResult=bAndb(item,numitems[i],maxW);
        printf("greedy: %.3f dp: %d bb:%d\n",gResult,dpResult,bbResult);
    }

}
/*
*/
/*
typedef struct Bbnode{
    int benefit;
    int weight;
    float bound;
    int level;
    struct Bbnode *next;
}bbnode;
typedef struct _list{
    bbnode * head;
}LinkedList;
void init (LinkedList* plist){
    bbnode *dummy = (bbnode*)malloc(sizeof(bbnode));
    dummy -> next= NULL;
    plist -> head = dummy;
}
void add (LinkedList *plist,bbnode* newNode){
    bbnode *cur= plist->head;
    bbnode *temp;
    while(cur->next !=NULL &&newNode->bound<=cur->next->bound){//Null이지 않고, 다음꺼의 bound가 새로운 bound 보다 크면 계속
        cur = cur->next;
    }
    if(cur->next==NULL) {
        cur->next = newNode;
        return;
    }
    temp=cur->next;
    cur->next = newNode;
    newNode->next=temp;
}
bbnode* getTop(LinkedList *plist){
    bbnode *temp;
    bbnode *cur= plist->head;
    temp=cur->next;
    cur->next=temp->next;
    return temp;
}

void deleteAllnode(bbnode* node){
    bbnode *cur=node;
    bbnode *next;
    while(cur !=NULL){
        next=cur->next;
        free(cur);
        cur=next;
    }
}
void checkNonPromising(LinkedList *plist, int max_benefit){
    bbnode *cur=plist->head;
    while(cur->next!=NULL&&cur->next->bound>=max_benefit){
        cur = cur->next;
    }
    if(cur->next==NULL) return;
    deleteAllnode(cur->next);
    cur->next=NULL;
    
}
int bAndb(item* item,int numItem, int maxW){
    LinkedList *plist=(LinkedList*)malloc(sizeof(LinkedList));
    init(plist);
    quickSort(item,1,numItem);
    int max_benefit=0;
    bbnode *root =(bbnode*)malloc(sizeof(bbnode));
    root->level=0;
    root->weight=0;
    root->benefit=0;
    root->next=NULL;
    calculate_bound(item,root,numItem,maxW,0);
    add(plist,root);
    while(1){
        bbnode* node = getTop(plist);
        if(node->bound<=max_benefit||node==NULL) break;
        bbnode *lnode =(bbnode*)malloc(sizeof(bbnode));
        lnode->next=NULL;
        bbnode *rnode =(bbnode*)malloc(sizeof(bbnode));
        rnode->next=NULL;
        expand(item,node,lnode,numItem,maxW,1);
        expand(item,node,rnode,numItem,maxW,0);
        if(lnode->benefit>max_benefit&&lnode->weight<=maxW) max_benefit=lnode->benefit;
        if(rnode->benefit>max_benefit&&rnode->weight<=maxW) max_benefit=rnode->benefit;
        if(lnode->bound>=max_benefit) add(plist,lnode);
        else free(lnode);
        if(rnode->bound>=max_benefit) add(plist,rnode);
        else free(rnode);
        free(node);
        checkNonPromising(plist,max_benefit);
    }
    return max_benefit;
}*/