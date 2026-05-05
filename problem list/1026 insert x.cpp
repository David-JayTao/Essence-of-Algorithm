#include<iostream>
#include<cstring>
#include<algorithm>
using namespace std;

const int N=30;

typedef long long LL;

LL f[N][N][N];

//max product为线性dp
//（运算顺序固定从左至右，左端点固定，枚举右端点）

//插入乘号，区间dp
//（运算顺序未定，可以任意乘号，于是需要算出某一段子结构的最大值，考虑采用区间枚举）

//区间dp枚举左右端点是错的，最后一步是将小区间合并为一个大区间，所以枚举完起点后，应当从小到大枚举长度，保证算每个更长的区间时，都已经把需要的小区间算完了
//f[i][j][k]:i~j,共k个乘号
//f[1][n][k]
//枚举起点i:1~n-1;
    //枚举终点j：i~n;
    //**定了区间a[i~j]
        //枚举乘号个数k:1~num
            //**最后肯定是两个子区间合并起来的，+或*
            //枚举间断点mid（第一个区间结束的位置）:i~j-1; 
            //  a[i~mid]，  a[mid+1~j]
                    //最后一步+，枚举左边的乘号个数a:(0~mid-i);右边的乘号：k-a
                        //f[i][j][k]=max(f[i][j][k],f[i][mid][a]+f[mid+1][j][k-a])
                    //最后一步*，枚举左边的乘号个数a:0~(mid-i);右边的乘号：k-1-a
                        //f[i][j][k]=max(f[i][j][k],f[i][mid][a]*f[mid+1][j][k-1-a])
                //f[i][j][k]=max(f[i][j][k],f[i][mid][a]*f[mid+1][j][k-1-a],f[i][mid][a]+f[mid+1][j][k-a])
LL q[N];
int n,num;  

int main()
{
    int m;
    cin>>m;
    while(m--)
    {
        cin>>n>>num;
        for(int i=1;i<=n;i++)cin>>q[i];

        memset(f,0,sizeof (f));

        for(int i=1;i<=n;i++)
                f[i][i][0]=q[i];

        // //区间dp核心错误2：不能按照左右边界i,j来枚举，因为最后一步是将两个短区间合并为一个长区间，
        // //f[1][5]会用到f[4][5]
        // //所以第二步必须按照区间长度枚举
        // for(int i=1;i<=n-1;i++)//假设至少有两个数
        //     for(int j=i;j<=n;j++)
        //         for(int k=0;k<=num;k++)//区间dp核心错误1：k=1开始，假设至少有一个乘号,错的，某一个区间完全可以只有+,没有*
        //             for(int mid=i;mid<=j-1;mid++)
        //                 for(int a=0;a<=mid-i;a++)
        //                 {
        //                     if(k-a>=0)f[i][j][k]=max(f[i][j][k],f[i][mid][a]+f[mid+1][j][k-a]);
        //                     if(k-1-a>=0)f[i][j][k]=max(f[i][j][k],f[i][mid][a]*f[mid+1][j][k-1-a]);
        //                 }
        
        //区间dp核心错误3：要先从小到大枚举所有区间长度，再枚举起点
        //f[1][5]会用到f[4][5]，不然的话只枚举了从1开始的，从4开始的就还没有
        for(int len=1;len<=n;len++)
            //for(int i=1;i<=n-1;i++)//假设至少有两个数
            for(int i=1;i+len-1<=n;i++)//枚举起点时，结合区间长度len，保证终点j有意义
            //j=i+len-1
                for(int k=0;k<=num;k++)//区间dp核心错误1：k=1开始，假设至少有一个乘号,错的，某一个区间完全可以只有+,没有*
                    for(int mid=i;mid<=i+len-1-1;mid++)//+！=-
                        for(int a=0;a<=min(k,mid-i);a++)
                        {
                            int j=i+len-1;
                            //if(j<=n)
                            
                                if(k-a>=0)f[i][j][k]=max(f[i][j][k],f[i][mid][a]+f[mid+1][j][k-a]);
                                if(k-1-a>=0)f[i][j][k]=max(f[i][j][k],f[i][mid][a]*f[mid+1][j][k-1-a]);
                            
                        }
        cout<<f[1][n][num]<<endl;
    }

    
    return 0;
}