


//稠密图，邻接矩阵存储
#include<iostream>
#include<cstring>
using namespace std;

const int N=510,INF=0x3f3f3f3f;//最大值的定义前后一定要一致！！！！！！！
//后面dist,g均用0x3f填充的
int g[N][N];
int dist[N];

//prim总体和dijkstra很像，但是dij是距离起点0的最短路
//prim是每次加入其他点距离集合的最近的点，最小生成树

//点i距离当前最小生成树的集合的距离
//（距离其中所有点的距离最小值）
int st[N];//
int n,m;

int prim()
{
    //1~n号节点
    //起点先设为1吧，然后开始一圈一圈往外拓展
    //每次均新加距离当前集合最近的点，
    //res记录总的最小生成树的总权重
    
    //起点为1，更新所有其他点距离当前集合的最小距离，并将最小的加进来
    
    
    ////哎呀，其实可以统一写，只是判断一下是否是第一次加入，第一次便不用更新res
    
    
    //假设当前集合为空，每次加入一个点，总共n次
    int res=0; 
    
    //int cnt=n;
    //while(cnt--)
    for(int i=0;i<n;i++)//利用轮数计数i对不同轮区别对待
    {
        int t=-1;//当前遍历找到的集合外距离集合最进的点
        
        for(int j=1;j<=n;j++)
        //dijkstra此处是循环n-1次，因为已经定了距离起点0的距离
            if(!st[j]&&(t==-1||dist[t]>dist[j]))
                t=j;
            
        if(i&&dist[t]==INF)return INF;
        
        st[t]=true;//勿忘
        
         if(i)res+=dist[t];  
         
        //用新加入的点更新其他点距离当前集合的最短距离
        for(int j=1;j<=n;j++)
            dist[j]=min(dist[j],g[j][t]);
    }
    return res;
}

int main()
{
    int T;
    cin>>T;
    while(T--)
    {
        cin>>n>>m;
        //初始化：
        memset(dist,0x3f,sizeof dist);
        memset(g,0x3f,sizeof g);
        memset(st,0,sizeof st);
        //别忘了咱st数组宝贝嗷~
        while(m--)
        {
            int u,v,w;
            cin>>u>>v>>w;
            //无向图，重边,自环
            if(u!=v)g[u][v]=g[v][u]=min(g[v][u],w);
        }
        int t=prim();
        if(t==INF)cout<<"-1\n";
        else cout<<t<<endl;
    }
}