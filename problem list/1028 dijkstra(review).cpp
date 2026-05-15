#include<iostream>
#include<cstring>
using namespace std;

const int N=510;
int g[N][N];//g[i][j]
int dist[N];//dist[i]
bool st[N];
int n;
int dijkstra()
{
    dist[1]=0;
    
    for(int round=1;round<=n;round++)//每次加1个点
    {
        //贪心选择未确定最短距离的最近点，并用此点更新到其他所有点的距离
        int t=-1;    
        for(int i=1;i<=n;i++)
            if(!st[i]&&(t==-1||dist[i]<dist[t]))
                t=i;
        st[t]=true;
        for(int i=1;i<=n;i++)
            dist[i]=min(dist[i],dist[t]+g[t][i]);
    }
    if(dist[n]==0x3f3f3f3f)return -1;
    else return dist[n];
}
int main()
{
    int m;
    cin>>n>>m;
    memset(g,0x3f,sizeof g);
    memset(dist,0x3f,sizeof dist);
    while(m--)
    {
        int x,y,z;
        cin>>x>>y>>z;
        g[x][y]=min(g[x][y],z);
    }
    cout<<dijkstra();
}