#include<iostream>
#include<string>
#include<cstring>
#include<algorithm>
using namespace std;
typedef long long LL;
const int N=50;
int n,k;
LL value[N][N];//value[i][j]=a[j]+10a[j+1]+10^2*a[j+2]+……+10^(i-j)a[i]
LL f[N][N];


int main()
{
    int m;
    cin>>m;
    while(m--)
    {
        cin>>n>>k;

        LL x;
        cin>>x;
        string num_s="0"+to_string(x);        
        //从下标1开始," "+没用
        
        // memset(value, 0, sizeof(value));
         memset(f, 0, sizeof(f));  
        
        //预处理+线性dp（枚举最后一个*的位置）
        for(int i=1;i<=n;i++)
            for(int j=i;j<=n;j++)
               value[i][j]=stoll(num_s.substr(i,j-i+1));
        //预处理也可受挫，这样是严格O(n^2)
        // for (int l = 1; l <= n; l++)
        // {
        //     value[l][l] = num_s[l] - '0';
        //     for (int r = l + 1; r <= n; r++)
        //         value[l][r] = value[l][r - 1] * 10 + (num_s[r] - '0');
        // }
        
        
        for(int i=1;i<=n;i++)
            f[i][0]=value[1][i];
        
        //随时警惕下标哈哈哈哈
        for(int i=2;i<=n;i++)//枚举整个序列终点i的位置；枚举从2~i的范围，等价于枚举长度1~i
            for(int j=1;j<=k;j++)//枚举1~i这个序列中*的个数，为j
                for(int t=j;t<=i-1;t++)//1~t里有j-1个*(f[t][j-1])，在t后面有一个*，*value[t+1][i];枚举1~i这个序列的最后一个*的位置（出现在哪个数后面）
                    f[i][j]=max(f[i][j],f[t][j-1]*value[t+1][i]);
                    //所以必须每次都清零，应为max(先前上一组数据的f[i][j],)
        cout<<f[n][k]<<endl;
    }

    
    return 0;
}