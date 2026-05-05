#include<cstring>
//const int maxN,maxM;
class Graph {
private:
    int cnt;
    int Head[maxN];
    int Next[maxM];
    int W[maxM];
    int V[maxM];//编号e[]
    int Depth[maxN];//Depth[u]：在当前残量网络里，从 s 到 u 的最短边数层次
    int cur[maxN];//cur就是记录当前点u循环到了哪一条边
public:
    int s,t;
    void init() {
        cnt=-1;
        memset(Head,-1,sizeof(Head));
        memset(Next,-1,sizeof(Next));
    }
    void _Add(int u,int v,int w) {
        cnt++;
        Next[cnt]=Head[u];
        Head[u]=cnt;
        V[cnt]=v;
        W[cnt]=w;
    }
    void Add_Edge(int u,int v,int w) {
        _Add(u,v,w);
        _Add(v,u,0);
    }
    int dfs(int u,int flow) {
        if (u==t)
            return flow;
        for (int& i=cur[u];i!=-1;i=Next[i]) { //注意这里的&符号，这样i增加的同时也能改变cur[u]的值，达到记录当前弧的目的
            if ((Depth[V[i]]==Depth[u]+1)&&(W[i]!=0)) {
                int di=dfs(V[i],min(flow,W[i]));
                if (di>0) {
                    W[i]-=di;
                    W[i^1]+=di;//最低位异或操作，cnt应从0开始，相当于连接的两个_Add操作（正反边操作）
                    return di;
                }
            }
        }
        return 0;
    }
    int bfs() {
        queue<int> Q;
        while (!Q.empty())
            Q.pop();
        memset(Depth,0,sizeof(Depth));
        Depth[s]=1;
        Q.push(s);
        do {
            int u=Q.front();
            Q.pop();
            for (int i=Head[u];i!=-1;i=Next[i])
                if ((Depth[V[i]]==0)&&(W[i]>0)) {
                    Depth[V[i]]=Depth[u]+1;
                    Q.push(V[i]);
                }
        }
        while (!Q.empty());
        if (Depth[t]>0)
            return 1;
        return 0;
    }
    int Dinic() {
        int Ans=0;
        while (bfs()) {
            for (int i=1;i<=n;i++)//每一次建立完分层图后都要把cur置为每一个点的第一条边 
                cur[i]=Head[i];
            while (int d=dfs(s,inf))
            {
                Ans+=d;
            }
        }
        return Ans;
    }
};
