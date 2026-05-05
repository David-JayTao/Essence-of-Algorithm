#include<iostream>
#include<cstring>
#include<algorithm>
using namespace std;

const int N=510;

//最长距离dist=5e6,
int dist[N];//起点s到点i的最小距离
int g[N][N];
int st[N];
int n,E,s,t;//起点为s（不一定是1！！！）

int dijkstra()
{
    int k=n;
    while(k--)
    {
        //坑2：t已经全局用过了
        int find=-1;
        for(int j=1;j<=n;j++)
            if(!st[j]&&(find==-1||dist[j]<dist[find]))
                find=j;
                
        st[find]=true;
        
        for(int j=1;j<=n;j++)
            if(!st[j])
                dist[j]=min(dist[find]+g[find][j],dist[j]);
    }
    //return dist[n]==0x3f3f3f3f?-1:dist[n];
    //坑三：错了，不要思维惯性，起点为s，终点为t！！！
    return dist[t]==0x3f3f3f3f?-1:dist[t];
    //又错了哈哈哈，t局部声明会导致覆盖全局声明！！！换一个
}

int main()
{
    int T;
    cin>>T;
    while(T--)
    {
        //坑一：对于每组数据要全部初始化
        memset(g,0x3f,sizeof g);
        memset(dist,0x3f,sizeof dist);
        memset(st,0,sizeof st);
        cin>>n>>E>>s>>t;
        g[s][s]=0;
        dist[s]=0;        
        int u,v,w;
        while(E--)
        {
            cin>>u>>v>>w;//坑4：有重边、自环
            if(u!=v)
            {
                g[u][v]=min(g[u][v],w);
                g[v][u]=min(g[v][u],w);           
            }
            //坑5：无向图哈哈哈哈哈哈
        }
        cout<<dijkstra()<<endl;
    }
    return 0;
}