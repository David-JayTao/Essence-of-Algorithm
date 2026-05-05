#include<iostream>
#include<algorithm>
#include<string>
using namespace std;

const int N=510;

//char a[N],b[N];
int f[N][N];//f[i][j]:a[1~i],b[1~j],此时的最大公共子序列长度
int main()
{
    int cnt;
    cin>>cnt;
    cin.ignore();    
    while(cnt--)
    {
        //处理多组数据，while循环内部每组一定都要重新初始化！！！

    string a,b;
    //cin遇到空格会停止输入，
    //getline读取整行输入，包括空格，直到遇到换行符\n为止
    
    getline(cin, a);
    getline(cin, b);
// 法一//如果想要字符串数组从1开始
//     a = " " + a;
//     b = " " + b;
//     //哟呵，妙哉，getline定死了必须从[0]开始往后读，那就读完后在最前面补一个空格就ok啦
//     //holly shit!!,补了前导0那就必须要把有效长度也减1
//     int n=a.size()-1,m=b.size()-1;


//法二（推荐，适配于getline只能从0开始读入）：如果直接从0开始ab,那就把下面改为a[i-1]b[j-1]即可，一一对应
    //a[0]为第一个字母，a[i-1]为字符串中第i个字母，适配了dp从下标1开始（需要用到下标0）
    int n=a.size(),m=b.size();
    
    for(int i=1;i<=n;i++)
        for(int j=1;j<=m;j++)
        {
            f[i][j]=0;
        //处理多组数据，while循环内部每组一定都要重新初始化！！！
            //if (a[i]==b[j])f[i][j]=f[i-1][j-1]+1;
            if (a[i-1]==b[j-1])f[i][j]=f[i-1][j-1]+1;
            //else f[i][j]=max(f[i-1][j-1],max(f[i][j],max(f[i-1][j],f[i][j-1])));
            else f[i][j]=max(f[i][j],max(f[i-1][j],f[i][j-1]));
        }
    cout<<f[n][m]<<endl;
    }
    return 0;
}
