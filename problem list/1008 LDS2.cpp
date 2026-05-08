#include<iostream>
#include<vector>
#include<cstring>
using namespace std;
//抽象问题，找出本质

//背关键结论：用不上升子序列覆盖整个序列，所需最少个数=最长上升子序列长度

//一二小问是完全独立，
    //一小问只有一套，求最长非升子序列，
    //二小问有多套系统，把整个序列拆成尽量少的不上升子序列。
        //从特例出发，一组严格上升的子序列，
            //a[i1] < a[i2] < a[i3] < ... < a[ik]
            //至少需要k套
            
            //那么能否取等呢？
            
        //结论：用最少的不上升子序列覆盖整个序列num=最长上升子序列长度l
            //证明：min{num}=l
                //1、num>=l
                //2、证明num可以取等l；
                    //如果最长上升子序列长度是 L，那么最多只有 1 ~ L 这些组。
                    //接下来证明：每一组内部一定是不上升的。
                    //f[i] = 以 a[i] 结尾的最长严格上升子序列长度
                    // f[i] = 1 的放第 1 组
                    // f[i] = 2 的放第 2 组
                    // f[i] = 3 的放第 3 组
                    //……
                        //反证法：
                        //已知f[i]=f[j],i<j,假设a[i]<a[j]
                        //则f[j]>=[i]+1
                        //矛盾
                    
        //从前往后看，第一颗子弹先配备一套，在这之后只要递减均能拦截
        //但凡遇到一个比这个高的，一定需要另一个系统……



//最长非降子序列
const int N=110;
int a[N],f[N],n,m;
//f[i]:以a[i]为结尾的最长非降子序列

// int pre[N];//pre[i]=k,a[i]接在尽可能长的子序列后面，结尾为a[k]
// bool st[N];//回溯中是否被遍历过
// vector<int>v;
int main()
{
    cin>>m;
    while(m--)
    {
        // v.clear();
        cin>>n;
        for(int i=0;i<n;i++)cin>>a[i];
        int res1=0,res2=0;
        
        //最长非升子序列
        for(int i=0;i<n;i++)
        {
            f[i]=1;//以a[i]结尾
            //枚举前面所有可能接上a[i]的
            for(int j=0;j<=i-1;j++)
                if(a[j]>=a[i])
                    f[i]=max(f[i],f[j]+1);
            res1=max(res1,f[i]);
        }
        cout<<res1<<" ";
        
        //
        //最长上升子序列
        for(int i=0;i<n;i++)
        {
            f[i]=1;//以a[i]结尾
            //枚举前面所有可能接上a[i]的
            for(int j=0;j<=i-1;j++)
                if(a[j]<a[i])
                    f[i]=max(f[i],f[j]+1);
            res2=max(res2,f[i]);
        }
        cout<<res2<<endl;
        
        
        
        
        
        
        
        
        // for(int round=1;round<=n;round++)//剔除的轮数
        // {
        //     memset(pre,-1,sizeof pre);
        //     memset(f,0,sizeof f);
            
        //     int index=-1;//每次剔除从哪个下标开始
             
        //     for(int i=0;i<(int)v.size();i++)
        //     {
        //         f[i]=1;//以v[i]结尾
        //         //枚举前面所有可能接上v[i]的
        //         for(int j=0;j<=i-1;j++)
        //             if(v[j]>=v[i])
        //                 if(f[i]<=f[j]+1)
        //                 {
        //                     f[i]=f[j]+1;
        //                     pre[i]=j;
        //                 }
        //         if(res1<f[i])
        //         {
        //             res1=f[i];
        //             index=i;
        //         }
        //     }
        //     if(round==1)cout<<res1<<" ";
            
        //     //最长的那组以index结尾,把这一组剔除
        //     for(int i=index;i!=-1;i=pre[i])
        //         v.erase(v.begin()+i);
            
        //     res2++;
            
        //     if(v.empty())break;
        // }
        // cout<<res2<<endl;
        
        //此法有问题，接在了重复部分后面，应该剔除某最长列再往后面做
        //使用pre[i]进行全局回溯，从后往前，看总共能剔除多少组子序列，其中每条都是以a[i]结尾的最长非升子序列
        // memset(st,0,sizeof st);       
        // for(int i=n-1;i>=0;i--)
        // {
        //     bool flag2=false;
        //     for(int j=i;j!=-1&&!st[j];j=pre[j])
        //     {
        //         st[j]=true;
        //         flag2=true;
        //     }
        //     if(flag2)res2++;
            
        //     bool flag=true;//是否已经遍历完
        //     for(int i=0;i<n;i++)
        //         if(!st[i])flag=false;
        //     if(flag)break;
        // }
        
        
    }
    return 0;
}