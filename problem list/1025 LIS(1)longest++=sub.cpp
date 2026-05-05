#include<iostream>
#include<algorithm>
using namespace std;

//注意常数根据具体题目确定！！
const int N=10010,INF=1e9;
//注意如果是字母，-1e9存入char可能就不是负无穷了。
//注意数据类型
int a[N];
int f[N];
//f[i]:以a[i]结尾的最大上升子序列长度
    //集合：以a[i]结尾的所有上升子序列
    //属性：max
int main()
{
    //O(n^2)
    int m;
    //记得加上&
    scanf("%d",&m);
    while(m--)
    {
        int n;
        scanf("%d",&n);
        for(int i=1;i<=n;i++)scanf("%d",&a[i]);
        
        int res=0;
        for(int i=1;i<=n;i++)
        {
            f[i]=1;
            for(int j=1;j<=i-1;j++)
                if(a[i]>=a[j])f[i]=max(f[i],f[j]+1);    
            res=max(res,f[i]);
        }
        printf("%d\n",res);
    }
    return 0;
}
