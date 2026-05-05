#include<iostream>
#include<algorithm>
using namespace std;




//最长上升子序列1
//O(n^2)，dp



const int N=1010,INF=1e9;
//注意如果是字母，-1e9存入char可能就不是负无穷了。
//注意数据类型
int a[N];
int f[N];
//f[i]:以a[i]结尾的最大上升子序列长度
    //集合：以a[i]结尾的所有上升子序列
    //属性：max
int main()
{
//法一：逆向枚举
    // int n;
    // cin>>n;
    // for(int i=1;i<=n;i++)cin>>a[i];
    // a[0]=-INF;
    // int res=0;
    // //f[0]=0,f[1]=1,……
    // for(int i=1;i<=n;i++)
    // {
    //     //对集合进行划分：
    //     //f[i]除了末尾的a[i],
    //         //刚好包含到a[i-1]，
    //         //刚好包含到a[i-2]，，
    //         //……
    //         //刚好包含到a[1]，
    //         //前面一个都不包含。
    //     //条件就是a[i]>a[j]        
        
    //     for(int j=i-1;j>=0;j--)
    //         if(a[i]>a[j])f[i]=max(f[i],f[j]+1);    
    //     res=max(res,f[i]);
    // }
    // //错的，子序列完全有可能不是以n结尾，cout<<f[n];
    // cout<<res;
    
    // //简洁版
    // int n;
    // cin>>n;
    // for(int i=1;i<=n;i++)cin>>a[i];
    // a[0]=-INF;
    // int res=0;
    // for(int i=1;i<=n;i++)
    // {

    //     for(int j=i-1;j>=0;j--)
    //         if(a[i]>a[j])f[i]=max(f[i],f[j]+1);    
    //     res=max(res,f[i]);
    // }
    // cout<<res;

//法二：正向枚举  （初始化不同）
    int n;
    cin>>n;
    for(int i=1;i<=n;i++)cin>>a[i];
    
    int res=0;
    for(int i=1;i<=n;i++)
    {
        f[i]=1;
        for(int j=1;j<=i-1;j++)
            if(a[i]>a[j])f[i]=max(f[i],f[j]+1);    
        res=max(res,f[i]);
    }
    cout<<res;
    return 0;
}
