#include<iostream>
using namespace std;

const int N=1e5+10;
int q[N];
//法一：暴力扫描法,O(n)，
int cyclic_num(int q[],int n)
{
   for(int i=0;i<=n-2;i++)  
         if(q[i]>q[i+1])
            return i+1;

    return 0;
}
//法二：二分法，O(logn)，
int binary_search(int q[],int n)
{
    int l=0,r=n-1;
    if(q[0]<q[n-1])return 0;
    
    while(l<r)
    {
        int mid=l+r+1>>1;
        if(q[mid]>q[n-1])l=mid;
        else r=mid-1;
    }  
    return l+1;
}
int main()
{
    int n;
    cin>>n;
    for(int i=0;i<n;i++)scanf("%d",&q[i]);
    
    cout<<cyclic_num(q,n);
    //cout<<binary_search(q,n);
    return 0;
}