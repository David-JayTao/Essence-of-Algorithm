#include <iostream>
#include <cstring>
#include <queue>
#include <algorithm>

using namespace std;

const int N = 110, M = 10010, INF = 0x3f3f3f3f;

int n, m;              // n 个男嘉宾，m 个女嘉宾
int S, T;              // 源点、汇点

int h[N], e[M], ne[M], f[M], w[M], idx;
// h[u]：u 的第一条边编号
// e[i]：第 i 条边指向的点
// ne[i]：第 i 条边的下一条边
// f[i]：第 i 条边剩余容量
// w[i]：第 i 条边费用
// idx：当前边编号

int dist[N];           // dist[x]：从 S 到 x 的最小费用距离
int pre[N];            // pre[x]：最短路中，走到 x 的那条边编号
int incf[N];           // incf[x]：从 S 到 x 这条路径上的最小剩余容量
bool st[N];            // SPFA 判重，表示是否在队列里

int cost[60][60];      // cost[女][男]：女嘉宾和男嘉宾牵手要收的钱

void add(int a, int b, int c, int d)
{
    // 正向边：a -> b，容量 c，费用 d
    e[idx] = b, f[idx] = c, w[idx] = d, ne[idx] = h[a], h[a] = idx++;

    // 反向边：b -> a，容量 0，费用 -d
    // 反向边的意义：允许之后“反悔”，重新调整匹配方案
    e[idx] = a, f[idx] = 0, w[idx] = -d, ne[idx] = h[b], h[b] = idx++;
}

bool spfa()
{
    memset(dist, 0x3f, sizeof dist);
    memset(st, false, sizeof st);

    queue<int> q;

    dist[S] = 0;
    incf[S] = INF;
    q.push(S);
    st[S] = true;

    while (q.size())
    {
        int t = q.front();
        q.pop();
        st[t] = false;

        for (int i = h[t]; i != -1; i = ne[i])
        {
            int j = e[i];

            // 只有剩余容量 > 0 的边才能走
            if (f[i] && dist[j] > dist[t] + w[i])
            {
                dist[j] = dist[t] + w[i];
                pre[j] = i;
                incf[j] = min(incf[t], f[i]);

                if (!st[j])
                {
                    q.push(j);
                    st[j] = true;
                }
            }
        }
    }

    return dist[T] != INF;
}

void min_cost_max_flow(int& flow, int& fee)
{
    flow = 0;
    fee = 0;

    // 每次找一条从 S 到 T 的最小费用增广路
    while (spfa())
    {
        int t = incf[T];

        flow += t;
        fee += t * dist[T];

        // 沿着这条增广路修改残量网络
        for (int i = T; i != S; i = e[pre[i] ^ 1])
        {
            f[pre[i]] -= t;        // 正向边容量减少
            f[pre[i] ^ 1] += t;    // 反向边容量增加
        }
    }
}

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int C;
    cin >> C;

    while (C--)
    {
        cin >> n >> m;

        memset(h, -1, sizeof h);
        idx = 0;

        S = 0;
        T = n + m + 1;

        // cost[i][j]：第 i 个女嘉宾和第 j 个男嘉宾牵手的费用
        // 输入是 m 行，每行 n 个数
        for (int i = 1; i <= m; i++)
            for (int j = 1; j <= n; j++)
                cin >> cost[i][j];

        // 源点 -> 每个男嘉宾
        // 每个男嘉宾最多匹配一次
        for (int i = 1; i <= n; i++)
            add(S, i, 1, 0);

        // 每个女嘉宾 -> 汇点
        // 每个女嘉宾最多匹配一次
        for (int i = 1; i <= m; i++)
            add(n + i, T, 1, 0);

        // 男嘉宾 -> 他喜欢的女嘉宾
        for (int i = 1; i <= n; i++)
        {
            int k;
            cin >> k;

            while (k--)
            {
                int girl;
                cin >> girl;

                // 男 i 喜欢 女 girl
                // 牵手费用是 cost[girl][i]
                add(i, n + girl, 1, cost[girl][i]);
            }
        }

        int flow, fee;
        min_cost_max_flow(flow, fee);

        cout << flow << ' ' << fee << '\n';
    }

    return 0;
}