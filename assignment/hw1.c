#include<stdio.h>
#include<stdlib.h>
typedef struct Node{
    char element[20];
    int key;
}node;
void swap(node* heap, int a, int b){
    node temp= heap[a];
    heap[a]=heap[b];
    heap[b]=temp;
};
void increaseKey(node* heap,int i,int key){
    if(key<heap[i].key){
        printf("new key is smaller than current key\n");
        return;
    }
    heap[i].key=key;
    while(i>1&&(heap[i/2].key)<(heap[i].key)){
        swap(heap,i/2,i);
        i=i/2;
    }
};
int insert (node* heap, int size){
    if(size==30){
        printf("queue is full");
        return size;
    }
    int newKey;
    char* 
    node* newNode=(node*)malloc(sizeof(node));
    printf("Enter name of element: ");
    scanf("%s",newNode->element);
    printf("Enter key value of element: ");
    scanf("%d", &newKey);
    newNode->key=-1;
    size++;
    heap[size]=*newNode;
    increaseKey(heap,size,newKey);
    printf("[%s, %d]\n",newNode->element,newKey);
    return size;
};
void maxHeapify(node* heap,int i,int size){
   int lChild=i*2;
   int rChild=i*2+1;
   int largest;
   if(lChild<size&&heap[lChild].key>=heap[i].key) largest=lChild;
   else largest=i;
   if(rChild<size&&heap[rChild].key>=heap[largest].key) largest=rChild;
   printf("%d\n",largest);
   if(largest!=i){ 
       swap(heap,i,largest);
       maxHeapify(heap,largest,size);
   }
   return;
};
int delete(node* heap,int size){
    if(size<1){
        printf("heap underflow\n");
        return size;
    }
    node max=heap[1];
    heap[1]=heap[size];
    size--;
    maxHeapify(heap,1,size);
    printf("[%s, %d]\n",max.element,max.key);
    return size;
};
void printAll(node* heap,int size){
    for(int i=1;i<=size;i++){
        printf("[%s, %d] ",heap[i].element,heap[i].key);
    }
    printf("\n");
}
int main(){
	node heap[31];
    int heapSize=0;
    char menu;
    int i,newKey;
    while(1){
        printf("**********  ");
        printf("MENU  ");
        printf("**********\n");
        printf("I : Insert new element into queue.\n");
        printf("D : Delete element with largest key from queue.\n");
        printf("R : Retrieve element with largest key from queue.\n");
        printf("K : Increase key of element in queue.\n");
        printf("P : Print out all elements in queue.\n");
        printf("Q : Quit\n\n");
        printf("Chose menu: ");
        scanf("%s",&menu);
        switch (menu){
            case 'I' :
                    heapSize=insert(heap,heapSize);
                    break;
            case 'D' :   
                    heapSize=delete(heap,heapSize);
                    break;
            case 'R' :
                    printf("[%s, %d]\n",heap[1].element,heap[1].key);
                    break;
            case 'K' :
                    printf("Enter index of element: ");
                    scanf("%d",&i);
                    printf("Enter new key value: ");
                    scanf("%d",&newKey);
                    increaseKey(heap,i,newKey);
                    break;
            case 'P' :
                    printAll(heap,heapSize);
                    break;
            case 'Q' : 
                    printf("Thank you. Bye!\n");
                    break;
            default :
                    printf("You pressed the wrong button\n");
        }
        if(menu=='Q') break;
        
    }
    return 0;   
}
