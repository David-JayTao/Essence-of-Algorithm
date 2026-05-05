#include<iostream>
#include<algorithm>
using namespace std;

const int N=510;

//01背包：背包可不用装满,
//f[i][j]定义为小于等于j时的最大价值（f[0][0~j]=0,表示允许啥也没装但价值可为0（有定义，可达））
    //f[][]全部初始化为0。

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
            
        //递推，从小到大递推
        //f[0~i][0~c]
        for(int i=1;i<=n;i++)//f[i][0~c]
            for(int j=0;j<=c;j++)
            {
                f[i][j]=f[i-1][j];
                if(j>=s[i])f[i][j]=max(f[i-1][j],f[i-1][j-s[i]]+v[i]);
            }
        cout<<f[n][c]<<"\n";
    }
    return 0;
}