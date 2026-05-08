#include<iostream>
using namespace std;

const int N=5e4+10;
int x[N];
int temp[N];
int n;
int my_merge(int l,int r)//返回逆序对数
{
    if(l==r)return 0;
    //int mid=l+r>>1;
    int mid=(l+r)/2;
    int res=my_merge(l,mid)+my_merge(mid+1,r);
    //l~mid,mid+1~r均排好序了，将这两端合并在一起
    int k=0,i=l,j=mid+1;
    while(i<=mid&&j<=r)
    {
        if(x[i]>x[j])
        {
            temp[k++]=x[j++];
            res+=mid-i+1;
        }
        else temp[k++]=x[i++];
    }
    while(i<=mid)temp[k++]=x[i++];
    while(j<=r)temp[k++]=x[j++];
    //物归原主
    for(int k=0,i=l;i<=r;i++,k++)
        x[i]=temp[k];
    return res;
}
int main()
{
    int m;
    cin>>m;
    while(m--)
    {
        cin>>n;
        for(int i=0;i<n;i++)cin>>x[i];
        
        //逆序对数量
        //归并排序
        cout<<my_merge(0,n-1)<<endl;
    }
    return 0;
}