#include<iostream>

using namespace std;

const int N=110;
int a[N],f[N],n,m;
//f[i]:以a[i]为结尾的最长非降子序列
int main()
{
    cin>>m;
    while(m--)
    {
        cin>>n;
        for(int i=0;i<n;i++)cin>>a[i];
        //最长非降子序列：
        int res=0;
        for(int i=0;i<n;i++)
        {
            f[i]=1;//以a[i]结尾
            //枚举前面所有可能接上a[i]的
            for(int j=0;j<=i-1;j++)
                if(a[j]>=a[i])
                    f[i]=max(f[i],f[j]+1);
            res=max(res,f[i]);
        }
        cout<<res<<endl;
    }
    return 0;
}