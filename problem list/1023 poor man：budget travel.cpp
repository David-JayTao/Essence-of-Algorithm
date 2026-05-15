//套堆优化dijkstra模板，隐式图中每个节点状态为（编号，cost）
//使用剪枝加速算法（第一次弹出t，直接break），减少不必要的计算


#include<iostream>
#include<queue>
#include<cstring>
using namespace std;
typedef pair<int,int>PII;
const int N1=510,N2=1e5+10;//无向边开两倍
int h[N1],e[N2],ne[N2],idx,w[N2],B[N2];//w[idx]指向idx的这条边的权重
int dist[N1][110];//dist[ver][cost]=distance,
//bool st[N1][110];//隐式图中的某点(编号+cost)的最短距离是否已经确定，是否为冗余备份，后面也可以采用懒删除
int n,E,s,t,M;
void add(int a,int b,int c)
{
    e[idx]=b,ne[idx]=h[a],w[idx]=c,h[a]=idx++;
}
struct Node//按照cost分层，虽然都是到达ver，但不同的策略dist、cost不同，也许当前dist小，但是cost大，不够，需要把dist大但是cost小的也存下来
{
    int d,v,c;//d：到达v号节点的当前最短距离，
    bool operator>(const Node&other)const
    {
        return d>other.d;
    }
}Nodes[N1];
//不是在跑原图的 Dijkstra，而是在跑一个“隐式分层图”的 Dijkstra。
int dijkstra_plus()
{
    dist[s][0]=0;//注意是编号1~n
    priority_queue<Node,vector<Node>,greater<Node>>heap;
    heap.push({0,s,0});//{dist[e[idx]],编号e[idx]}
    while(heap.size())
    {
        Node temp=heap.top();
        heap.pop();
        int v=temp.v,distance=temp.d,cost=temp.c;
        //优化：t第一次被弹出时，它就是所有可行到达 t 的状态中路程最短的。
        if(v==t)break;//剪枝，提前结束
        
        //堆的优化：懒删除
        if(distance!=dist[v][cost])continue;
        // if(st[v][cost])continue;
        // st[v][cost]=true;
        
        for(int i=h[v];i!=-1;i=ne[i])
        {
            int j=e[i];
            //int add=(j==s?0:B[j]);//其实这句话没必要，只是为了严格对照题意罢了，也可以B[s]=0
            //这个地方到到达的是j，不是v
            //int newcost=cost+add;
            int newcost=cost+B[j];
            if(newcost<=M&&dist[j][newcost]>distance+w[i])
            {
                dist[j][newcost]=distance+w[i];//此处松弛可以处理重边
                heap.push({dist[j][newcost],j,newcost});
            }
        }
    }
    //min(dist[t][0~M])
    int res=0x3f3f3f3f;
    for(int c=0;c<=M;c++)
        res=min(res,dist[t][c]);
    return res==0x3f3f3f3f?-1:res;
}
int main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int T;
    cin>>T;
    while(T--)
    {
        idx=0;
        memset(dist,0x3f,sizeof dist);
        memset(h,-1,sizeof h);
        //memset(st,0,sizeof st);        
        cin>>n>>E>>s>>t>>M;       
        for(int i=1;i<=n;i++)cin>>B[i];
        B[s]=0;
        while(E--)
        {
            int x,y,z;
            cin>>x>>y>>z;
            if(x!=y)add(x,y,z),add(y,x,z);//算法可以处理重边，且为无向图
        }
        cout<<dijkstra_plus()<<endl;
    }
    return 0;
}
