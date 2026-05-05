#include<iostream>
#include<cmath>
using namespace std;

const int N=1e5+10;
int q[N];
//法一分治法，O(nlogn)，将大问题划分为多个不重叠的小问题，求解子问题，合并子问题的解得到原问题的解
//Divid and Conquer,分治

//左边

//右边

//包含中间点，左边一部分，右边一部分
//<=>左边从右端点往左尽可能延申，右边同理
int Merge(int q[],int l,int r)
{
    if(l==r)return q[l];
    
    int mid=r+l>>1;
    //从中间点劈开，区间对半分

    int x=Merge(q,l,mid);//1、子数组全部在左边
    int y=Merge(q,mid+1,r);//2、子数组全部在右边

    //3、第三种情况：左边的有部分+右边的左部分
    //左边的右端点l~mid，右边的左端点mid+1~r

    //注意可能全为负数，此时返回最大的负数
    //故将端点初始化为q[mid]，而不是0
    int sum_left=q[mid],sum_right=q[mid+1];
    int max_left=sum_left,max_right=sum_right;

//每一层O(n)，用mid分层，共logn层
    for(int i=mid-1;i>=l;i--)
    {
        sum_left+=q[i];
        max_left=max(max_left,sum_left);
    }
    for(int i=mid+2;i<=r;i++)
    {
        sum_right+=q[i];
        max_right=max(max_right,sum_right);
    }
    return max(max(x,y),max_left+max_right);
}

//法二：贪心法、动态规划：
//O(n),只扫描一遍
//动规在哪？
//f[i],必须以第 i 个数结尾的最大连续子数组和
//dp[i] = max(dp[i - 1] + q[i], q[i]);

//每次扫描到第i个数了，要不要加上前面的连续部分的数pre[i]
    //pre[i]>0,则加
    //pre[i]<=,则不加，自己作为新的起点
int best(int q[],int n)
{
    int res=q[0],pre=q[0];
    
    for(int i=1;i<n;i++)
    {
        if(pre>0)res=max(res,pre+q[i]),pre=pre+q[i];
        else pre=q[i],res=max(res,pre);
        //每走一步便更新res,else里勿忘更新，防止均为负数时（-2）res未更新为最小的负数
    }
    return res;
}
//更简洁的写法
int best_best(int q[],int n)
{
    int res=q[0],pre=q[0];
    
    for(int i=1;i<n;i++)
    {
        // 核心：要么带着前面的（pre + q[i]），要么从头开始（q[i]）
        pre=max(pre+q[i],q[i]);
        res=max(res,pre);
    }
    return res;
}


int main()
{
    int m,n;
    cin>>m;
    while(m--)
    {
        cin>>n;
        for(int i=0;i<n;i++)scanf("%d",&q[i]);
        //scanf输入勿漏传入地址&

        cout<<Merge(q,0,n-1)<<endl;
        //cout<<best(q,n)<<endl;
        //cout<<best_best(q,n)<<endl;
    }
    return 0;
}