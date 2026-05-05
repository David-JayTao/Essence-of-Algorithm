#include<iostream>
using namespace std;

const int N=1e3+10;
int a[N];
int temp[N];
int res[N],p=0;

void mergeSort(int a[],int l,int r,int dep)
{
    if(l>=r)return;
    

    int mid=l+r>>1;
    mergeSort(a,l,mid,dep+1),mergeSort(a,mid+1,r,dep+1);
    
    //合并
    int i=l,j=mid+1,k=0;
    while(i<=mid&&j<=r)
    {
        if(a[i]<a[j])temp[k++]=a[i++];
        else temp[k++]=a[j++];
    }
    while(i<=mid)temp[k++]=a[i++];
    while(j<=r)temp[k++]=a[j++];
    //物归原主
    for(int i=0,j=l;j<=r;j++,i++)
        a[j]=temp[i];
        
    if(dep==3)
        for(int i=l;i<=r;i++)
            res[p++]=a[i];
}
int main()
{
    int m;
    cin>>m;
    while(m--)
    {
        //记得每一组的res对应的起始p清零
        p=0;
        
        int n;
        cin>>n;
        for(int i=0;i<n;i++)
            cin>>a[i];
        mergeSort(a,0,n-1,1);
        for(int i=0;i<n;i++)
            cout<<res[i]<<" ";  
        cout<<endl;
    }
    return 0;
}