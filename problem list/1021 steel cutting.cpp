#include<iostream>
#include<cstring>
using namespace std;
typedef pair<int,int> PII;
const int N=1e4+10;

int m,n,k;

int f[N];//f[i]长度为i的钢条的所有划分中的最大收益
//f[n]
//f[i]=max(f[i],f[i-a[j].first]+a[j].second);
PII a[N];//长度为a[j].first的钢条的收益a[j].second;

int main()
{
    cin>>m;
    while(m--)
    {
        cin>>n>>k;
        
        memset(f,0,sizeof f);
        
        for(int i=1;i<=k;i++)
            cin>>a[i].first>>a[i].second;
        
        //钢条的最后一步可以视为最后独立计费的一小段，再加上剩余长度的所有划分的最大收益（最有子结构）
        
        for(int i=1;i<=n;i++)//枚举钢条的总长度i：1~n
            for(int j=1;j<=k;j++)//枚举最后剩的一小段
                if(i-a[j].first>=0)
                    f[i]=max(f[i],f[i-a[j].first]+a[j].second);
            
        cout<<f[n]<<endl;
    }

    
    return 0;
}