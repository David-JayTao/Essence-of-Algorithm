#include<iostream>
#include<cstring>
using namespace std;
typedef long long LL;
const int N=1e5+10;
LL c[N],y[N];

//答案不爆int不代表所有候选答案不爆int
//**,**题干说的是不保证结果不爆int，**

//总结一下dp方法：
//需求y[i],当前i月生产c[i]，还是之前生产存储到现在用？
//状态定义dp[i]:对于i月，最小的实际成本，
    //dp[i]=min(dp[i-1]+1,c[i]);


LL sum[N];
//sum[i]:第i月的成本总和sum[i]=y[i]*min(ci+j-i)，i<=j
//每个月自己独立地往前选择使当前成本最小的那个月来生产

// #include<queue>
// int cost[N];

LL dp[N];

LL res;
int main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int T;
    cin>>T;
    
    //暴力O(n^2)往前挨个找，
    //其实可以预处理每个月的实际最小生产成本，全部装入小根堆，于是第j月直接就输出sum[j],
        //heap:O(nlogn) 
    //dp:O(n)
    while(T--)
    {
        res=0;
        
        int n;
        cin>>n;
        for(int i=1;i<=n;i++)cin>>c[i];
        for(int i=1;i<=n;i++)cin>>y[i];
        
        //c[i] + j - i = (c[i] - i) + j
        //对于月份j，j为常数，heap里存放c[i] - i
        // priority_queue<int>heap;//存储第j个月及之前的所有成本，堆顶即为min{cost[j]}
        // heap.push(c[1]-1);
        // cost[1]=y[1];
        // for(int i=2;i<=n;i++)
        // {
        //     heap.push(c[i]-i);
        // }
    
        
        //有点想用dp,dp[i]:第i月的实际最低成本
        //集合划分：
            //当前月生产成本c[i]
            //之前生产，dp[i-1]+1,因为上一个月dp[i-1]已经是i-1月及之前的实际最低成本，只是再多存储一个月
        //O(n)
        dp[1]=c[1];
        for(int i=2;i<=n;i++)
            dp[i]=min(c[i],dp[i-1]+1);
        for(int i=1;i<=n;i++)
            res+=dp[i]*y[i];
        cout<<res<<endl;
    }

    // while(T--)//别把--泄露了哈嘿嘿嘿~
    // {
    //     memset(sum,0x3f,sizeof sum);
    //     res=0;
    //     int n;
    //     cin>>n;
    //     for(int i=1;i<=n;i++)cin>>c[i];
    //     for(int i=1;i<=n;i++)cin>>y[i];
        
    //     for(int j=1;j<=n;j++)//计算第j月的最低成本，枚举在第i月生产，去min
    //         for(int i=1;i<=j;i++)
    //             sum[j]=min(sum[j], y[j]*(c[i]+j-i));
        
    //     for(int i=1;i<=n;i++)res+=sum[i];
    //     cout<<res<<endl;
    // }
    return 0;
}