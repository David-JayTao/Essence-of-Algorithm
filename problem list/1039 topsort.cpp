#include<iostream>
#include<cstring>
#include<queue>
using namespace std;
const int N=45010;
int n,m;
int h[N],e[N],ne[N],idx;//一定要手动初始化h[]
int d[N];//编号1~n
int res[N],num;

//也可以手搓堆，同时利用所有原有数据
//pop:q[hh++],push:q[++tt],empty():tt<=hh
void add(int x,int y)
{
    e[idx]=y,ne[idx]=h[x],h[x]=idx++;
}

void topsort()
{
    queue<int>q;
    for(int i=1;i<=n;i++)
        if(d[i]==0)
            q.push(i);
    while(q.size())
    {
        int t=q.front();
        q.pop();
        res[num++]=t;
        
        for(int i=h[t];i!=-1;i=ne[i])
        {
            int j=e[i];
            d[j]--;
            if(d[j]==0)q.push(j);
        }
    }
    if(num==n)
    {
        for(int i=0;i<num;i++)
                cout<<res[i]<<" ";
        cout<<endl;
    }
    else cout<<0<<endl;
}

int main()
{
    int T;
    cin>>T;
    while(T--)
    {
//在进行输入初始化前，先进行所有前置初始化！！！！比如h[]=-1,
        memset(h,-1,sizeof h);
        idx=0;
        num=0;
        memset(d,0,sizeof d);
        
        cin>>n>>m;
        while(m--)
        {
            int x,y;
            cin>>x>>y;
            add(x,y);
            d[y]++;
        }
        
        topsort();
    }
        
    return 0;
}
