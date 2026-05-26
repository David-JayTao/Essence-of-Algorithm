//核心贪心逻辑：
    // 优先处理最紧迫的订单（结束时间最早，按结束时间排序预处理）
    // 对于每个订单：分配给完成时间最早的骑手
#include<iostream>
#include<cstring>
#include<vector>
#include<algorithm>
using namespace std;
const int N=5010,INF=1e9;
int dist[N][N];
int ne[N][N];
//ne[i][j]=k1,i->j的最短路上,i的下一个点是k1;
//再访问ne[k1][j]=k2,得到路径上的下一个点……直至走到j
vector<int>getPath(int i,int j)
{
    vector<int>path;
    if(-1==ne[i][j])return path;
    path.push_back(i);
    while(i!=j)
    {
        path.push_back(ne[i][j]);
        i=ne[i][j];
    } 
    return path;
}       
//从 u 到 v，如果不是同一个点，就输出一条最短路径
void printGoto(int u, int v)
{
    if(u == v) return;
    vector<int> path = getPath(u, v);
    if(path.empty()) return;
    cout << "goto " << path.size();
    for(int node : path) cout << ' ' << node;
    cout << '\n';
}
int n,e,r,x;
struct Order
{
    int p, d;     // p: pickup, d: delivery
    int te, tl;   // earliest, latest
    int id;       // 原始订单编号
};
struct Rider
{
    bool used = false;   // 是否已经接过单
    int cur = -1;        // 当前所在区域
    int tim = 0;         // 当前时间
    vector<int> seq;     // 这个骑手送的订单编号序列
};
//这个骑手接某单，送完的具体时间
int calcFinish(const Rider& rider, const Order& order)
{
    int arrive;
    // 第一单：题目说骑手时刻0已经在第一单取货点
    if(!rider.used)
        arrive=dist[order.p][order.d];
    else
        arrive=rider.tim+ dist[rider.cur][order.p]+dist[order.p][order.d];
    int finish = max(arrive, order.te);
    if(finish > order.tl) return INF;
    return finish;
}
int main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int T;
    cin>>T;
    while(T--)
    {
        // memset(dist,0,sizeof dist);
        // memset(ne,-1,sizeof ne);

        cin>>n>>e>>r>>x;
        for(int i=1;i<=n;i++)
            for(int j=1;j<=n;j++)
                if(j==i)dist[i][j]=0,ne[i][j]=i;
                else dist[i][j]=INF,ne[i][j]=-1;
        //无向图加双边：
        while(e--)
        {
            int i,j,w;
            cin>>i>>j>>w;
            if(w<dist[i][j])
            {
                dist[j][i]=dist[i][j]=w;  
                ne[i][j]=j,ne[j][i]=i;                
            }
        }
        //读订单
        vector<Order> orders(r + 1);        
        for(int i=1;i<=r;i++)
        {
            cin>>orders[i].p>>orders[i].d>>orders[i].te>>orders[i].tl;
            orders[i].id =i;
        }
        //floyd:
        //以后我每次直接访问dist[i][j]便可得到最短距离与对应的路径
        //并且如果更新了，要记录下到目前最短路的路径！
        for(int k=1;k<=n;k++)
            for(int i=1;i<=n;i++)
                for(int j=1;j<=n;j++)
                    if(dist[i][j]>dist[i][k]+dist[k][j])
                    {
                        dist[i][j]=dist[i][k]+dist[k][j];
                        ne[i][j]=ne[i][k];
                        //经过1~k-1中转点集合的最短路已确定，
                        //现在已确定新增k可以更新i->j的最短路上
                        //正解：i->j的最短路的第一步=i->k的最短路的第一步
                        //错误：记录路径：i先走到ne[i][j](k)，再走到ne[k][j]……
                    }
        //贪心分配：订单按最晚结束时间排序
        vector<int> ids;
        for(int i = 1; i <= r; i++) ids.push_back(i);
        sort(ids.begin(), ids.end(), [&](int a, int b)
        {
            if(orders[a].tl != orders[b].tl) return orders[a].tl < orders[b].tl;
            return orders[a].te < orders[b].te;
        });
        //对每个订单找最合适的骑手：
        vector<Rider> riders(x + 1);
        for(int oid : ids)
        {
            Order& o = orders[oid];
            int bestRider = -1;
            int bestFinish = INF;
            for(int i = 1; i <= x; i++)
            {
                int finish = calcFinish(riders[i], o);
                if(finish < bestFinish)
                    bestFinish = finish,bestRider = i;
            }
            if(bestRider != -1)
            {
                riders[bestRider].used = true;
                riders[bestRider].cur = o.d;
                riders[bestRider].tim = bestFinish;
                riders[bestRider].seq.push_back(o.id);
            }
        } 

        for(int i = 1; i <= x; i++)
        {
            if(riders[i].seq.empty()) continue;
            cout << "solution " << i << '\n';
            int cur = -1;
            for(int k = 0; k < riders[i].seq.size(); k++)
            {
                int oid = riders[i].seq[k];
                Order& o = orders[oid];
                if(k == 0)
                {
                    // 第一单：题目说骑手时刻 0 已经在第一单取货点
                    cur = o.p;
                }
                else
                {
                    // 从上一单送达点，走到这一单取货点
                    printGoto(cur, o.p);
                    cur = o.p;
                }
                cout << "pick " << oid << '\n';
                // 从取货点走到送达点
                printGoto(o.p, o.d);
                cur = o.d;
                cout << "serve " << oid << '\n';
            }
        }
        // for(int i = 1; i <= x; i++)
        // {
        //     cout << "solution " << i << '\n';
        //     if(riders[i].seq.empty())
        //     {
        //         cout << '\n';
        //         continue;
        //     }
        //     int cur = -1;
        //     for(int k = 0; k < riders[i].seq.size(); k++)
        //     {
        //         int oid = riders[i].seq[k];
        //         Order& o = orders[oid];
        //         if(k == 0)
        //         {
        //             cur = o.p;
        //         }
        //         else
        //         {
        //             printGoto(cur, o.p);
        //             cur = o.p;
        //         }
        //         cout << "pick " << oid << '\n';
        //         printGoto(o.p, o.d);
        //         cur = o.d;
        //         cout << "serve " << oid << '\n';
        //     }
        // }
    }
    return 0;
}