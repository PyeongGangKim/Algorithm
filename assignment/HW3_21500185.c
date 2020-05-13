#include<stdio.h>

long long recursionC(int n,int k){
    if(n==k) return 1;
    else if(k==0) return 1;
    else if(n==0&&k>0) return 0;
    else return recursionC(n-1,k-1) + recursionC(n-1,k);
}
long long dpC(int n, int k){
    long long m[100][100];
    for(int i=0; i<=n;i++){
        for(int j=0;j<=k;j++){
            if(i==j) m[i][j]=1;
            else if(j==0) m[i][j]=1;
            else if(i==0 && j>0) m[i][j]=0;
            else m[i][j] = m[i-1][j-1]+m[i-1][j];
        }
    }
    return m[n][k];
}
int main(){
    int n,k;
    long long rResult,dpResult;
    while(1){
        printf("put the two integer n,k. ");
        scanf("%d %d",&n,&k);
        if(n>=k) break;
        else printf("k must be less than n.\n");
    }
    dpResult=dpC(n,k);
    printf("dp result : %lld\n",dpResult);
    rResult=recursionC(n,k);
    printf("recursion result: %lld\n",rResult);
    
    return 0;
}