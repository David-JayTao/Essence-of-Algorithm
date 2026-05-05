//此题最关键的点：最终结果的数据范围！！数据很大！！，使用long long 类型

#include<iostream>
using namespace std;

typedef long long LL;

const int N=1e5+10;
int n;
int q[N];
int temp[N];

LL myMerge(int q[],int l,int r)
{
    //递归终止条件
    if(l==r)return 0;
    
    int mid=r+l>>1;
    LL res=myMerge(q,l,mid)+myMerge(q,mid+1,r);
    
    int i=l,j=mid+1;
    int t=0;
    while(i<=mid&&j<=r)
    {
        if(q[i]<=q[j])temp[t++]=q[i++];
        else 
        {
            temp[t++]=q[j++];
            res+=mid-i+1;
        }
    }
    
    //将剩余的最大的部分合并进temp
    while(i<=mid)temp[t++]=q[i++];
    while(j<=r)temp[t++]=q[j++];
    
    //物归原主
    for(int i=0,j=l;j<=r;j++,i++)q[j]=temp[i];
    
    return res;
    
}

int main()
{
    int m;
    cin>>m;
    while(m--)
    {
        cin>>n;
        for(int i=0;i<n;i++)scanf("%d",&q[i]);
        //scanf输入勿漏传入地址&
        cout<<myMerge(q,0,n-1)<<endl;
    }

    return 0;
}