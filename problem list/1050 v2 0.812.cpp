#include <bits/stdc++.h>
using namespace std;

// 外卖订单派送问题 - 插入贪心多策略版
// 核心改进：不再只能把订单追加到某个骑手末尾，而是尝试插入到任意位置。

const int INF = 1000000000;

int n, e, r, x;
int S;  // 矩阵行宽 = n + 1

vector<int> distMat;
vector<uint16_t> nxtMat;  // 0 表示不可达；顶点编号从 1 开始

inline size_t ID(int i, int j) {
    return 1ULL * i * S + j;
}

inline int& distAt(int i, int j) {
    return distMat[ID(i, j)];
}

inline uint16_t& nxtAt(int i, int j) {
    return nxtMat[ID(i, j)];
}

struct Order {
    int p, d;      // p: 取货点 pickup, d: 送达点 delivery
    int te, tl;    // [te, tl]: 最早/最晚送达时间
    int id;        // 原始订单编号
};

vector<Order> orders;

struct Eval {
    bool ok = true;
    int endTime = 0;      // 该序列最后一单真正 serve 的时间
    int drive = 0;        // 总行驶时间，不含等待
    int minSlack = INF;   // 所有订单中最小的 tl - finish，越大越安全
};

struct Rider {
    vector<int> seq;  // 订单编号序列
    Eval ev;          // 当前序列的模拟结果
};

struct Result {
    vector<Rider> riders;
    int served = 0;
    int totalDrive = 0;
    int sumEnd = 0;
};

// 恢复 u -> v 的最短路径，返回包含起点和终点的点序列
vector<int> getPath(int u, int v) {
    vector<int> path;
    if (u < 1 || u > n || v < 1 || v > n) return path;
    if (nxtAt(u, v) == 0) return path;

    path.push_back(u);
    int guard = 0;

    while (u != v) {
        u = nxtAt(u, v);
        if (u == 0) {
            path.clear();
            return path;
        }

        path.push_back(u);

        // 防止 next-hop 记录异常导致死循环
        if (++guard > n + 5) {
            path.clear();
            return path;
        }
    }

    return path;
}

// 输出 goto 操作。若 u == v，不需要移动，也就不输出 goto。
void printGoto(int u, int v) {
    if (u == v) return;

    vector<int> path = getPath(u, v);
    if (path.empty()) return;  // 理论上合法方案不会出现这种情况

    cout << "goto " << path.size();
    for (int node : path) cout << ' ' << node;
    cout << '\n';
}

// 模拟一个订单序列是否合法。
// 关键规则：第一单特殊——骑手时刻 0 默认就在第一单的取货点。
Eval evalSeq(const vector<int>& seq) {
    Eval res;
    if (seq.empty()) return res;

    int tim = 0;
    int cur = -1;
    int drive = 0;
    int minSlack = INF;

    for (int i = 0; i < (int)seq.size(); i++) {
        const Order& o = orders[seq[i]];

        int pd = distAt(o.p, o.d);
        if (pd >= INF) {
            res.ok = false;
            return res;
        }

        int arrive;

        if (i == 0) {
            // 第一单：时刻 0 已经在该单取货点
            arrive = pd;
            drive += pd;
        } else {
            int toPickup = distAt(cur, o.p);
            if (toPickup >= INF) {
                res.ok = false;
                return res;
            }

            arrive = tim + toPickup + pd;
            drive += toPickup + pd;
        }

        // 提前到达送达点，需要等待到 te 才能 serve
        int finish = max(arrive, o.te);

        if (finish > o.tl) {
            res.ok = false;
            return res;
        }

        minSlack = min(minSlack, o.tl - finish);
        tim = finish;
        cur = o.d;
    }

    res.ok = true;
    res.endTime = tim;
    res.drive = drive;
    res.minSlack = minSlack;
    return res;
}

// 模拟：把 oid 插入 seq 的 pos 位置后是否合法。
// 不真正修改 seq，避免每次候选都复制 vector。
Eval evalWithInsert(const vector<int>& seq, int pos, int oid) {
    Eval res;

    int len = (int)seq.size();
    int tim = 0;
    int cur = -1;
    int drive = 0;
    int minSlack = INF;

    for (int idx = 0; idx <= len; idx++) {
        int realOid;

        if (idx == pos) {
            realOid = oid;
        } else {
            int oldIdx = idx;
            if (idx > pos) oldIdx--;
            realOid = seq[oldIdx];
        }

        const Order& o = orders[realOid];

        int pd = distAt(o.p, o.d);
        if (pd >= INF) {
            res.ok = false;
            return res;
        }

        int arrive;

        if (idx == 0) {
            // 插入后新的第一单，也享受“时刻 0 已在取货点”的规则
            arrive = pd;
            drive += pd;
        } else {
            int toPickup = distAt(cur, o.p);
            if (toPickup >= INF) {
                res.ok = false;
                return res;
            }

            arrive = tim + toPickup + pd;
            drive += toPickup + pd;
        }

        int finish = max(arrive, o.te);

        if (finish > o.tl) {
            res.ok = false;
            return res;
        }

        minSlack = min(minSlack, o.tl - finish);
        tim = finish;
        cur = o.d;
    }

    res.ok = true;
    res.endTime = tim;
    res.drive = drive;
    res.minSlack = minSlack;
    return res;
}

struct Cand {
    bool ok = false;
    int rider = -1;
    int pos = -1;
    Eval nev;
    int deltaDrive = 0;
    int deltaEnd = 0;
    int oldLen = 0;
};

bool betterCand(const Cand& a, const Cand& b, int mode) {
    if (!b.ok) return true;
    if (!a.ok) return false;

    // mode 0：偏向完成时间早 + 路线均衡，通常更愿意启用空骑手
    if (mode == 0) {
        if (a.nev.endTime != b.nev.endTime) return a.nev.endTime < b.nev.endTime;
        if (a.oldLen != b.oldLen) return a.oldLen < b.oldLen;
        if (a.deltaDrive != b.deltaDrive) return a.deltaDrive < b.deltaDrive;
        if (a.nev.minSlack != b.nev.minSlack) return a.nev.minSlack > b.nev.minSlack;
    }
    // mode 1：偏向最小新增行驶时间，通常更愿意把路线压紧
    else if (mode == 1) {
        if (a.deltaDrive != b.deltaDrive) return a.deltaDrive < b.deltaDrive;
        if (a.deltaEnd != b.deltaEnd) return a.deltaEnd < b.deltaEnd;
        if (a.nev.endTime != b.nev.endTime) return a.nev.endTime < b.nev.endTime;
        if (a.nev.minSlack != b.nev.minSlack) return a.nev.minSlack > b.nev.minSlack;
    }
    // mode 2：偏向保留时间窗余量，减少后续被卡死
    else {
        if (a.nev.minSlack != b.nev.minSlack) return a.nev.minSlack > b.nev.minSlack;
        if (a.deltaEnd != b.deltaEnd) return a.deltaEnd < b.deltaEnd;
        if (a.deltaDrive != b.deltaDrive) return a.deltaDrive < b.deltaDrive;
        if (a.nev.endTime != b.nev.endTime) return a.nev.endTime < b.nev.endTime;
    }

    if (a.rider != b.rider) return a.rider < b.rider;
    return a.pos < b.pos;
}

// sortType 控制订单处理顺序；mode 控制候选插入位置的选择标准。
Result buildSolution(int sortType, int mode) {
    vector<int> ids;
    ids.reserve(r);

    for (int i = 1; i <= r; i++) {
        if (distAt(orders[i].p, orders[i].d) < INF) {
            ids.push_back(i);
        }
    }

    sort(ids.begin(), ids.end(), [&](int a, int b) {
        const Order& A = orders[a];
        const Order& B = orders[b];

        int da = distAt(A.p, A.d);
        int db = distAt(B.p, B.d);

        if (sortType == 0) {
            // 最晚送达早的优先：基础策略
            if (A.tl != B.tl) return A.tl < B.tl;
            if (A.te != B.te) return A.te < B.te;
            return da < db;
        } else if (sortType == 1) {
            // 最晚出发时间早的优先：tl - pickup_to_delivery
            int la = A.tl - da;
            int lb = B.tl - db;

            if (la != lb) return la < lb;
            if (A.tl != B.tl) return A.tl < B.tl;
            return A.te < B.te;
        } else if (sortType == 2) {
            // 时间窗窄的优先
            int wa = A.tl - A.te;
            int wb = B.tl - B.te;

            if (wa != wb) return wa < wb;
            if (A.tl != B.tl) return A.tl < B.tl;
            return da > db;
        } else if (sortType == 3) {
            // 最早送达时间早的优先
            if (A.te != B.te) return A.te < B.te;
            if (A.tl != B.tl) return A.tl < B.tl;
            return da < db;
        } else if (sortType == 4) {
            // 长单优先
            if (da != db) return da > db;
            if (A.tl != B.tl) return A.tl < B.tl;
            return A.te < B.te;
        } else {
            // 短单优先
            if (da != db) return da < db;
            if (A.tl != B.tl) return A.tl < B.tl;
            return A.te < B.te;
        }
    });

    vector<Rider> riders(x + 1);

    for (int oid : ids) {
        Cand best;

        for (int rid = 1; rid <= x; rid++) {
            const vector<int>& seq = riders[rid].seq;

            for (int pos = 0; pos <= (int)seq.size(); pos++) {
                Eval nev = evalWithInsert(seq, pos, oid);
                if (!nev.ok) continue;

                Cand cur;
                cur.ok = true;
                cur.rider = rid;
                cur.pos = pos;
                cur.nev = nev;
                cur.deltaDrive = nev.drive - riders[rid].ev.drive;
                cur.deltaEnd = nev.endTime - riders[rid].ev.endTime;
                cur.oldLen = (int)seq.size();

                if (betterCand(cur, best, mode)) {
                    best = cur;
                }
            }
        }

        if (best.ok) {
            vector<int>& seq = riders[best.rider].seq;
            seq.insert(seq.begin() + best.pos, oid);
            riders[best.rider].ev = best.nev;
        }
    }

    Result res;
    res.riders = move(riders);

    for (int i = 1; i <= x; i++) {
        res.served += (int)res.riders[i].seq.size();
        res.totalDrive += res.riders[i].ev.drive;
        res.sumEnd += res.riders[i].ev.endTime;
    }

    return res;
}

bool betterResult(const Result& a, const Result& b) {
    if (a.served != b.served) return a.served > b.served;
    if (a.totalDrive != b.totalDrive) return a.totalDrive < b.totalDrive;
    return a.sumEnd < b.sumEnd;
}

void floyd() {
    for (int k = 1; k <= n; k++) {
        for (int i = 1; i <= n; i++) {
            int dik = distAt(i, k);
            if (dik >= INF) continue;

            for (int j = 1; j <= n; j++) {
                int dkj = distAt(k, j);
                if (dkj >= INF) continue;

                int nd = dik + dkj;

                if (nd < distAt(i, j)) {
                    distAt(i, j) = nd;

                    // 正确 next-hop 更新：
                    // i -> j 现在走 i -> k -> j，
                    // 所以 i 到 j 的第一步 = i 到 k 的第一步。
                    nxtAt(i, j) = nxtAt(i, k);
                }
            }
        }
    }
}

void outputAnswer(const Result& ans) {
    for (int rid = 1; rid <= x; rid++) {
        const vector<int>& seq = ans.riders[rid].seq;

        // 空骑手不输出
        if (seq.empty()) continue;

        cout << "solution " << rid << '\n';

        int cur = -1;

        for (int k = 0; k < (int)seq.size(); k++) {
            int oid = seq[k];
            const Order& o = orders[oid];

            if (k == 0) {
                // 第一单：默认已经在取货点，所以不输出 goto 到取货点
                cur = o.p;
            } else {
                // 从上一单送达点走到这一单取货点
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

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    cout.tie(nullptr);

    int T;
    cin >> T;

    for (int tc = 1; tc <= T; tc++) {
        cin >> n >> e >> r >> x;

        // 不同测试用例之间空一行；
        // 同一测试用例内 solution 块之间绝不额外空行。
        if (tc > 1) cout << '\n';

        S = n + 1;
        size_t totalSize = 1ULL * S * S;

        distMat.assign(totalSize, INF);
        nxtMat.assign(totalSize, 0);

        for (int i = 1; i <= n; i++) {
            distAt(i, i) = 0;
            nxtAt(i, i) = i;
        }

        for (int i = 0; i < e; i++) {
            int a, b, w;
            cin >> a >> b >> w;

            // 无向图；若有重边，只保留最短边
            if (w < distAt(a, b)) {
                distAt(a, b) = distAt(b, a) = w;
                nxtAt(a, b) = b;
                nxtAt(b, a) = a;
            }
        }

        orders.assign(r + 1, Order());

        for (int i = 1; i <= r; i++) {
            cin >> orders[i].p >> orders[i].d >> orders[i].te >> orders[i].tl;
            orders[i].id = i;
        }

        floyd();

        // 多策略跑几遍，取完成订单数最多的一组。
        // 不使用随机数，保证每次提交输出稳定。
        vector<pair<int, int> > plans;

        plans.push_back(make_pair(0, 0));
        plans.push_back(make_pair(0, 1));
        plans.push_back(make_pair(0, 2));
        plans.push_back(make_pair(1, 0));
        plans.push_back(make_pair(1, 1));
        plans.push_back(make_pair(2, 0));
        plans.push_back(make_pair(2, 2));
        plans.push_back(make_pair(3, 0));
        plans.push_back(make_pair(4, 1));
        plans.push_back(make_pair(5, 0));

        Result best;
        best.served = -1;

        for (int pi = 0; pi < (int)plans.size(); pi++) {
            int sortType = plans[pi].first;
            int mode = plans[pi].second;

            Result cur = buildSolution(sortType, mode);

            if (betterResult(cur, best)) {
                best = move(cur);
            }
        }

        outputAnswer(best);
    }

    return 0;
}