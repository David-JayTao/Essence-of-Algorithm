#include<iostream>
#include<algorithm>
using namespace std;

const int N=510;
const int INF=1e9;
//01背包：背包必须刚好装满,
//此时f[i][c]定义为刚好装满c时的最大价值，
    //故只有f[0][0]初始化为0，其他全为-INF（不可达）。
    //则所有可达的均有f[0][0]转移过去
int c,n;
int v[N],s[N];//第i颗宝石大小为si，价值为vi
int f[N][N];//f[i][c],在前i颗宝石里选，体积<=c时的最大价值;
int main()
{
    int m;
    cin>>m;



    while(m--)
    {
        cin>>n>>c;//n颗宝石，背包体积为c
        for(int i=1;i<=n;i++)
            cin>>s[i]>>v[i];
        
        //每一组数据初始化一定要在内部！！！
        for(int i=0;i<=n;i++)
            for(int j=0;j<=c;j++)
                f[i][j]=-INF;
        f[0][0]=0; 
        
        //递推，从小到大递推
        //f[0~i][0~c]
        for(int i=1;i<=n;i++)//f[i][0~c]
            for(int j=0;j<=c;j++)
            {
                f[i][j]=f[i-1][j];
                if(j>=s[i])f[i][j]=max(f[i-1][j],f[i-1][j-s[i]]+v[i]);
            }
        //0
        if(f[n][c]<0)cout<<0<<endl;
        else {cout<<f[n][c]<<"\n";}
    }
    return 0;
}