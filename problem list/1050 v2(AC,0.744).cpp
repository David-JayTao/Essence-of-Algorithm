// 外卖订单派送问题
//
// 当前算法：
// 1. Floyd 求任意两点最短路
// 2. 用 next-hop 矩阵恢复 goto 路径
// 3. 订单按最晚送达时间 tl 从小到大排序
// 4. 每个订单分配给“能完成它且完成时间最早”的骑手
//
// 注意：
// 这是一个贪心启发式算法，不保证全局最优。
// 但它能构造合法方案，是这类特殊判题题目的基础版本。

#include <bits/stdc++.h>
using namespace std;

const int INF = 1e9;

int n, e, r, x;
int S;  // 矩阵行宽，S = n + 1

// distMat[i][j]：i 到 j 的最短距离
// 用一维数组模拟二维数组，避免写死 N
vector<int> distMat;

// nxtMat[i][j]：从 i 到 j 的最短路上，i 的下一步走到哪个点
// 由于顶点编号通常不大于几千，这里用 uint16_t 省内存
// 0 表示不可达；因为题目顶点编号从 1 开始，所以 0 可以当作 -1
vector<uint16_t> nxtMat;

// 二维下标转一维下标
inline size_t ID(int i, int j)
{
    return 1ULL * i * S + j;
}

inline int& distAt(int i, int j)
{
    return distMat[ID(i, j)];
}

inline uint16_t& nxtAt(int i, int j)
{
    return nxtMat[ID(i, j)];
}

struct Order
{
    int p, d;      // p: 取货点 pickup, d: 送达点 delivery
    int te, tl;    // te: 最早送达时间, tl: 最晚送达时间
    int id;        // 原始订单编号
};

struct Rider
{
    bool used = false;    // 是否已经接过至少一单
    int cur = -1;         // 当前所在区域
    int tim = 0;          // 当前时间
    vector<int> seq;      // 该骑手的订单序列
};

// 恢复 u -> v 的最短路径
// 返回路径上的所有节点，例如 1 -> 2 -> 3 返回 {1,2,3}
vector<int> getPath(int u, int v)
{
    vector<int> path;

    if (u < 1 || u > n || v < 1 || v > n) return path;
    if (nxtAt(u, v) == 0) return path;

    path.push_back(u);

    // 防止路径记录异常导致死循环
    int guard = 0;

    while (u != v)
    {
        u = nxtAt(u, v);

        if (u == 0)
        {
            path.clear();
            return path;
        }

        path.push_back(u);

        guard++;
        if (guard > n + 5)
        {
            path.clear();
            return path;
        }
    }

    return path;
}

// 输出 goto 操作
// 如果 u == v，不需要输出 goto
void printGoto(int u, int v)
{
    if (u == v) return;

    vector<int> path = getPath(u, v);

    // 正常情况下，被调度的路径一定可达
    // 如果 path 为空，说明前面可行性判断出了问题
    if (path.empty()) return;

    cout << "goto " << path.size();

    for (int node : path)
    {
        cout << ' ' << node;
    }

    cout << '\n';
}

// 计算 rider 如果接 order，送完后的时间
// 如果不能在时间窗内送达，返回 INF
int calcFinish(const Rider& rider, const Order& order)
{
    int pd = distAt(order.p, order.d);

    // 取货点到送达点不可达
    if (pd >= INF) return INF;

    int arriveDelivery;

    if (!rider.used)
    {
        // 第一单：
        // 题目规定，每个骑手在时刻 0 位于自己第一单的取货点
        arriveDelivery = pd;
    }
    else
    {
        int toPickup = distAt(rider.cur, order.p);

        // 当前点到新订单取货点不可达
        if (toPickup >= INF) return INF;

        arriveDelivery = rider.tim + toPickup + pd;
    }

    // 如果提前到达，需要等到最早送达时间 te 才能 serve
    int finish = max(arriveDelivery, order.te);

    // 超过最晚送达时间，说明这单不能接
    if (finish > order.tl) return INF;

    return finish;
}

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    cout.tie(nullptr);

    int T;
    cin >> T;

    for (int tc = 1; tc <= T; tc++)
    {
        cin >> n >> e >> r >> x;

        // 不同测试用例之间空一行
        // 同一测试用例内的 solution 块之间不加空行
        if (tc > 1) cout << '\n';

        S = n + 1;
        size_t totalSize = 1ULL * S * S;

        // 动态分配矩阵
        distMat.assign(totalSize, INF);
        nxtMat.assign(totalSize, 0);

        // 初始化
        for (int i = 1; i <= n; i++)
        {
            for (int j = 1; j <= n; j++)
            {
                if (i == j)
                {
                    distAt(i, j) = 0;
                    nxtAt(i, j) = i;
                }
                else
                {
                    distAt(i, j) = INF;
                    nxtAt(i, j) = 0;
                }
            }
        }

        // 读入无向边
        for (int cnt = 0; cnt < e; cnt++)
        {
            int a, b, w;
            cin >> a >> b >> w;

            // 可能存在重边，只保留最短边
            if (w < distAt(a, b))
            {
                distAt(a, b) = w;
                distAt(b, a) = w;

                // 直接边：
                // 从 a 到 b 的下一步是 b
                // 从 b 到 a 的下一步是 a
                nxtAt(a, b) = b;
                nxtAt(b, a) = a;
            }
        }

        // 读入订单
        vector<Order> orders(r + 1);

        for (int i = 1; i <= r; i++)
        {
            cin >> orders[i].p >> orders[i].d >> orders[i].te >> orders[i].tl;
            orders[i].id = i;
        }

        // Floyd 求任意两点最短路
        for (int k = 1; k <= n; k++)
        {
            for (int i = 1; i <= n; i++)
            {
                if (distAt(i, k) >= INF) continue;

                for (int j = 1; j <= n; j++)
                {
                    if (distAt(k, j) >= INF) continue;

                    int nd = distAt(i, k) + distAt(k, j);

                    if (nd < distAt(i, j))
                    {
                        distAt(i, j) = nd;

                        // i -> j 的最短路变成 i -> k -> j
                        // 所以 i 到 j 的第一步，等于 i 到 k 的第一步
                        nxtAt(i, j) = nxtAt(i, k);
                    }
                }
            }
        }

        // 订单编号数组
        vector<int> ids;
        ids.reserve(r);

        for (int i = 1; i <= r; i++)
        {
            ids.push_back(i);
        }

        // 按紧迫程度排序
        sort(ids.begin(), ids.end(), [&](int a, int b)
        {
            if (orders[a].tl != orders[b].tl)
                return orders[a].tl < orders[b].tl;

            if (orders[a].te != orders[b].te)
                return orders[a].te < orders[b].te;

            return distAt(orders[a].p, orders[a].d) < distAt(orders[b].p, orders[b].d);
        });

        vector<Rider> riders(x + 1);

        // 贪心分配订单
        for (int oid : ids)
        {
            Order& o = orders[oid];

            int bestRider = -1;
            int bestFinish = INF;

            for (int i = 1; i <= x; i++)
            {
                int finish = calcFinish(riders[i], o);

                if (finish < bestFinish)
                {
                    bestFinish = finish;
                    bestRider = i;
                }
            }

            if (bestRider != -1)
            {
                riders[bestRider].used = true;
                riders[bestRider].cur = o.d;
                riders[bestRider].tim = bestFinish;
                riders[bestRider].seq.push_back(o.id);
            }
        }

        // 输出配送方案
        // 按截图样例：
        // solution 块之间不额外输出空行
        // 空骑手不输出
        for (int i = 1; i <= x; i++)
        {
            if (riders[i].seq.empty()) continue;

            cout << "solution " << i << '\n';

            int cur = -1;

            for (int k = 0; k < (int)riders[i].seq.size(); k++)
            {
                int oid = riders[i].seq[k];
                Order& o = orders[oid];

                if (k == 0)
                {
                    // 第一单：
                    // 骑手默认在该订单的取货点，所以不用 goto 到取货点
                    cur = o.p;
                }
                else
                {
                    // 从上一单送达点，走到当前订单取货点
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
    }

    return 0;
}