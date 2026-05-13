#include<iostream>
#include<cstring>
#include<iomanip>
using namespace std;

const int N=510;
//每棵二叉树又左子树、根节点、右子树构成，
    //最优子结构：左右子树均最佳
    
//给定N个整数关键字，编号1~N，则还有N+1个空隙区间,编号0~n;
//最终问题是构建一颗最优的二叉搜索树，使其具有最优的结构，使得总代价最小
//与上司的树形dp不同（树结构已定，根据父子关系推进），
//此题采用区间dp：树的结构未定，需要自己构造最优的，且重叠的子问题为一段一段的区间，
//区间长度从小到大枚举

//求f[0][n],key[1~n],p[1~n],q[0~n],w[0~n][0~n]
//(w[i][j],第i、j个空隙和其间所有key构成的权值和，用于状态转移时的均下沉一层操作)

//初始化：
    //w[i][i]=q[i],f[i][i]=0(只有一个空间隙，无key，连树都还没有构成，何谈代价@o@)
    //w[i][j]=w[i][j-1]+p[j]+q[j];(j>=i+1)
//状态表示：
    //集合：f[i][j]空隙 q[i]~q[j] 之间夹着的那些key（key[i+1],key[j]），所有树形结构
    //属性：min，最小代价
//集合计算
    //集合划分：
        //从左至右枚举根节点，
    //状态转移方程：枚举根节点k号key，k:i+1~j,
            //左子树：i+1~k-1号key
            //root:k
            //右子树：k+1~j;
        //f[i][j]=(f[i][k-1]+w[i][k-1])+p[k]+(f[k][j]+w[k][j])
        //f[i][j]=min(f[i][j],f[i][k-1]+f[k][j]+w[i][j]);
        
double f[N][N];
int n,m;
int key[N];
double p[N],q[N],w[N][N];

int main()
{
    scanf("%d",&m);
        //printf("%d\n",-1);
    //cin>>m;
    while(m--)
    {
        scanf("%d",&n);
        //memset错误，不能初始化doumble，memset(f,0x3f,sizeof f);
        //memset(0x3f)只能用于初始化无穷型int
        
        for(int i=1;i<=n;i++)scanf("%d",&key[i]);
        for(int i=1;i<=n;i++)scanf("%lf",&p[i]);//i号key的概率
        for(int i=0;i<=n;i++)scanf("%lf",&q[i]);//i号区间的概率
        
        for(int i=0;i<=n;i++)w[i][i]=q[i],f[i][i]=0;
        //保证从f[i][i]转移过去，此时只有一个空区间，不含键，len=0
        //慢慢转移，包含越来越多的键
        
        for(int i=0;i<=n-1;i++)
            for(int j=i+1;j<=n;j++)
                w[i][j]=w[i][j-1]+p[j]+q[j];
            
        for(int len=1;len<=n;len++)//枚举区间长度(包含键的个数)
            for(int i=0;i+len<=n;i++)//枚举区间编号（从第一个空隙开始枚举）i：0~j，包含键i+1~j
            {
                int j=i+len;//已定区间i~j（空隙编号）,含key号i+1~j
                f[i][j]=1e9;
                for(int k=i+1;k<=j;k++)
                    f[i][j]=min(f[i][j],f[i][k-1]+f[k][j]+w[i][j]);
            }
        printf("%.6f\n",f[0][n]);
       // cout<<fixed<<setprecision(6)<<f[0][n]<<endl;
    }
    return 0;
}

