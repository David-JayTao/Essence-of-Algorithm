#include<iostream>
using namespace std;

#include<cstring>//包含memset

const int N=510;
//n个点，m条边；
int g[N][N];//邻接矩阵存储图
int dist[N];//dist[i],从起点1出发，到终点i的最短距离
//最大是5e6
//0x3f3f3f3f=10亿！满足INF+INF未越界int(20亿)仍为INF
int n,m;
//dijkstra：贪心

//1到其他所有点初始距离+INF

//从起点出发，

//while(未访问完，n次)
//{
//   每次将1个距离1“最近”的点i加入已定圈内，

//   for()
//   利用该点更新其他所有邻接点到新加点的距离，（如果更小则更新）

//   标记st[i]为true 
//}

bool st[N];//状态矩阵，表示节点

int dijkstra()
{
    g[1][1]=0;//从起点1出发
    dist[1]=0;
    
    int k=n;
   while(k--)//n次循环，每次加入一个点
    {
        //t:找到已定圈外离起点最近的点t，加入已定圈，并更新其他所有点的距离
        
        int t=-1;
        for(int j=1;j<=n;j++)
            if(!st[j]&&(t==-1||dist[t]>dist[j]))
                t=j;
        //这样是错的，t应该赋给的是最小的j，不能直接初始化为1      
        // int t=1;
        // for(int j=1;j<=n;j++)
        //     if(!st[j]&&(dist[t]>dist[j]))
        //         t=j;
                
        st[t]=true;
        
        for(int j=1;j<=n;j++)
            if(!st[j])
                dist[j]=min(dist[t]+g[t][j],dist[j]);
    }
    //
    return dist[n]==0x3f3f3f3f?-1:dist[n];
}

int main()
{
    //注意要处理重边和自环
    cin>>n>>m;
    
    memset(g,0x3f,sizeof g);//初始所有点间均不可达，INF
    memset(dist,0x3f,sizeof dist);//初始化1到所有其他点的最小距离为INF
    
    while(m--)
    {
        int x,y,z;
        cin>>x>>y>>z;
        //开始连线
        //处理重边
        if(x!=y)//可以保证g[i][i]为INF。处理自环
            g[x][y]=min(g[x][y],z);
    }
    
    cout<<dijkstra();
    
    return 0;
}