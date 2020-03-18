#include<stdio.h>
#include<stdlib.h>
#include<string.h>

typedef struct Node{
    char element[22];
    int key;
}node;
void swap(node* heap, int a, int b){//change the node's position 
    node temp= heap[a];
    heap[a]=heap[b];
    heap[b]=temp;
};
void increaseKey(node* heap,int i,int key){//increase the value of the selected index. 
    if(key<heap[i].key){
        printf("new key is smaller than current key.\n");
        return;
    }
    heap[i].key=key;
    while(i>1&&(heap[i/2].key)<(heap[i].key)){//execute when index size is less than 1 and current key value is greater than parent's key value.
        swap(heap,i/2,i);//change position with parent's node.
        i=i/2;
    }
};
int insert (node* heap, int size){//insert new node
    if(size==30){
        printf("queue is full.\n");
        return size;
    }
    int newKey;
    char newElement[22];
    node* newNode=(node*)malloc(sizeof(node));
    while(1){//get a new element name
        printf("Enter name of element: ");
        scanf("%s",newElement);
        if(strlen(newElement)<=20) break;
        else printf("enter name less than 20 characters\n");
    }
    while(1){//get a new key value
        printf("Enter key value of element: ");
        scanf("%d", &newKey);
        if(newKey>=1&&newKey<=10) break;
        else printf("enter a number between 1 and 10.\n");
    }
    strcpy(newNode->element,newElement);
    newNode->key=-1;
    size++;
    heap[size]=*newNode;//first insert the new node at the end of array.
    increaseKey(heap,size,newKey);//move the new node to the correct position in the heap.
    printf("[%s, %d]\n",newNode->element,newKey);
    return size;
};
void maxHeapify(node* heap,int i,int size){//maintain the max-heap property
   int lChild=i*2;//left child index
   int rChild=i*2+1;//right child indx
   int largest;
   if(lChild<size&&heap[lChild].key>=heap[i].key) largest=lChild;//if the left child exists and is bigger than parent, then put it into the largest.
   else largest=i;
   if(rChild<size&&heap[rChild].key>=heap[largest].key) largest=rChild;//if the right child exists and is bigger than largest, then put it into the largest. 
   if(largest!=i){ 
       swap(heap,i,largest);
       maxHeapify(heap,largest,size);//repeat until max-heap is reached
   }
   return;
};
int delete(node* heap,int size){
    if(size<1){
        printf("heap underflow.\n");
        return size;
    }
    node max=heap[1];
    heap[1]=heap[size];//replace the first node with the last one.
    size--;
    maxHeapify(heap,1,size);
    printf("[%s, %d]\n",max.element,max.key);
    return size;
};
void printAll(node* heap,int size){//print all node in max-heap
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
        switch (menu){//execute the correct option
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
                    if(heapSize==0) break;
                    while(1){
                        printf("Enter index of element: ");
                        scanf("%d",&i);
                        if(i>0 && i<=heapSize) break;
                        else printf("enter a index between 1 and %d\n",heapSize);
                    }
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
