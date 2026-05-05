#include<iostream>
#include<cstring>
using namespace std;
typedef pair<int,int> PII;
const int N=510;
int m,n;
//本题两个矩阵A(r行s列)和B(s行t列)相乘, 乘法代价为r*s*t
//不该变所有矩阵的左右顺序，只是加括号，改变运算顺序，使总的代价最小
//最后一步可以看成左右两个矩阵乘在一起，按照间断点位置枚举
//最有子结构：两个小的都最小时，乘起来大的自然最小
PII a[N];//a[k]={i,j},i行j列
int f[N][N];//f[i][j]:a[i]*……*a[j]的最小代价
int main()
{
    cin>>m;
    while(m--)
    {
        cin>>n;
        for(int i=1;i<=n;i++)
            cin>>a[i].first>>a[i].second;
        
        //初始化
        //memset(f,1e9,sizeof f);
        //INF=0x3f3f3f3f,memset按字节赋值，0x3f为十六进制下的，转换为二进制：0011 1111，刚好8位，一个字节
        memset(f,0x3f,sizeof f);
        for(int i=1;i<=n;i++)f[i][i]=0;

        //n个矩阵
        for(int len=1;len<=n;len++)
            for(int i=1;i+len-1<=n;i++)
            {
                int j=i+len-1;
                //定下了区间：a[i]*……*a[j]
                //f[i][j]=1e9;
                //枚举最后一层相乘的断点mid(左边的最后一个位置)
                for(int mid=i;mid<=j-1;mid++)
                {
                    f[i][j]=min(f[i][j],f[i][mid]+f[mid+1][j]+a[i].first*a[mid].second*a[j].second);
                }
            }

        cout<<f[1][n]<<endl;
    }

    
    return 0;
}