#include<iostream>
#include<cstring>
using namespace std;
const int N=90,mod=1000007;

//int a[N];
//int f[N][N];
//本质为：多重背包，同种可以放多件，但有数量限制
//状态定义f[i][j]
    //集合：前i种摆件，共摆放j件的所有方案
    //属性：方案数量
    //ans=f[n][m]
//状态计算：
    //枚举第i种摆件的数量：
    //f[i][j]=f[i-1][j]+f[i-1][j-1]+f[i-1][j-2]+……+f[i-1][j-ai]
    //暴力枚举O(n^3)
    
int f[N];

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int t;
    cin>>t;
    while(t--)
    {
        // //由于f要进行+=操作，每组数据必须清空
        // memset(f,0,sizeof f);        
        // int n,m;
        // cin>>n>>m;
        // for(int i=1;i<=n;i++)cin>>a[i];
    
        // for(int i=0;i<=n;i++)f[i][0]=1;
        // //手动初始化j=0
        // //所以不能配套j从0开始循环，导致f[1][0]=f[1][0]+f[0][0]=2!=1
        // //j从1开始即可
        
        // //f[0][0]=1;//这样配套j从0开始，有了f[0][0]这个根，其他的全部从这里长出去
        
        // for(int i=1;i<=n;i++)//前i种
        //     for(int j=1;j<=m;j++)//总共摆j件
        //         for(int k=0;k<=a[i]&&k<=j;k++)
        //             f[i][j]=(f[i][j]+f[i-1][j-k])%mod;
        // cout<<f[n][m]<<endl;
        
        
        //优化为1维数组：
        memset(f,0,sizeof f);  
        int n,m,a;
        cin>>n>>m;
        f[0]=1;
        for(int i=1;i<=n;i++)
        {
            cin>>a;
            for(int j=m;j>=0;j--)//总共摆j件
                for(int k=1;k<=a&&k<=j;k++)
                //k不能从0开始枚举，否则f[j]=f[j]+f[j-0]
                //在进入k循环之前，f[j] 里原本存的值，就已经等价于f[i-1][j]了
                //（因为它继承自上一轮循环，还没被更新）
                    f[j]=(f[j]+f[j-k])%mod;
        }
        cout<<f[m]<<endl;
    }
    

    return 0;
}