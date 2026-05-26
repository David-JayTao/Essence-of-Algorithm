#include <bits/stdc++.h>
using namespace std;

const int INF = 1000000000;
const double TIME_LIMIT_SEC = 28.0;   // 防止局部搜索过头，给输出留一点时间

int n, e, r, x;
int S;

vector<int> distMat;
vector<uint16_t> nxtMat;

inline size_t ID(int i, int j) {
    return 1ULL * i * S + j;
}

inline int& distAt(int i, int j) {
    return distMat[ID(i, j)];
}

inline uint16_t& nxtAt(int i, int j) {
    return nxtMat[ID(i, j)];
}

inline bool timeOK() {
    return (double)clock() / CLOCKS_PER_SEC < TIME_LIMIT_SEC;
}

struct Order {
    int p, d;      // pickup, delivery
    int te, tl;    // earliest, latest
    int id;
};

vector<Order> orders;

struct Eval {
    bool ok = true;
    int endTime = 0;       // 最后一单 serve 的时间
    int drive = 0;         // 总行驶时间，不含等待
    int minSlack = INF;    // 所有订单中最小 tl - finish
};

struct Rider {
    vector<int> seq;
    Eval ev;
};

struct Result {
    vector<Rider> riders;
    int served = 0;
    int totalDrive = 0;
    int sumEnd = 0;
};

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

        if (++guard > n + 5) {
            path.clear();
            return path;
        }
    }

    return path;
}

void printGoto(int u, int v) {
    if (u == v) return;

    vector<int> path = getPath(u, v);
    if (path.empty()) return;

    cout << "goto " << path.size();
    for (int node : path) cout << ' ' << node;
    cout << '\n';
}

// 模拟整个订单序列是否合法。
// 第一单特殊：骑手在时刻 0 默认已经位于第一单取货点。
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

// 把 oid 插入 seq 的 pos 位置后，模拟是否合法。
// 不直接复制 seq，减少一点常数。
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
            // 插入后如果它变成第一单，也享受“时刻 0 已在取货点”规则
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

    if (mode == 0) {
        // 偏向完成时间早，更容易均摊给空骑手
        if (a.nev.endTime != b.nev.endTime) return a.nev.endTime < b.nev.endTime;
        if (a.oldLen != b.oldLen) return a.oldLen < b.oldLen;
        if (a.deltaDrive != b.deltaDrive) return a.deltaDrive < b.deltaDrive;
        if (a.nev.minSlack != b.nev.minSlack) return a.nev.minSlack > b.nev.minSlack;
    } else if (mode == 1) {
        // 偏向最小新增路程，更容易把路线压紧
        if (a.deltaDrive != b.deltaDrive) return a.deltaDrive < b.deltaDrive;
        if (a.deltaEnd != b.deltaEnd) return a.deltaEnd < b.deltaEnd;
        if (a.nev.endTime != b.nev.endTime) return a.nev.endTime < b.nev.endTime;
        if (a.nev.minSlack != b.nev.minSlack) return a.nev.minSlack > b.nev.minSlack;
    } else {
        // 偏向保留时间窗余量
        if (a.nev.minSlack != b.nev.minSlack) return a.nev.minSlack > b.nev.minSlack;
        if (a.deltaEnd != b.deltaEnd) return a.deltaEnd < b.deltaEnd;
        if (a.deltaDrive != b.deltaDrive) return a.deltaDrive < b.deltaDrive;
        if (a.nev.endTime != b.nev.endTime) return a.nev.endTime < b.nev.endTime;
    }

    if (a.rider != b.rider) return a.rider < b.rider;
    return a.pos < b.pos;
}

bool betterResult(const Result& a, const Result& b) {
    if (a.served != b.served) return a.served > b.served;
    if (a.totalDrive != b.totalDrive) return a.totalDrive < b.totalDrive;
    return a.sumEnd < b.sumEnd;
}

void recomputeStats(Result& res) {
    res.served = 0;
    res.totalDrive = 0;
    res.sumEnd = 0;

    for (int i = 1; i <= x; i++) {
        res.served += (int)res.riders[i].seq.size();
        res.totalDrive += res.riders[i].ev.drive;
        res.sumEnd += res.riders[i].ev.endTime;
    }
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

                    // next-hop 正确写法：
                    // i -> j 变成 i -> k -> j，第一步应等于 i -> k 的第一步。
                    nxtAt(i, j) = nxtAt(i, k);
                }
            }
        }
    }
}

bool orderLessByType(int a, int b, int sortType) {
    const Order& A = orders[a];
    const Order& B = orders[b];

    int da = distAt(A.p, A.d);
    int db = distAt(B.p, B.d);

    if (sortType == 0) {
        // 最晚送达早的优先
        if (A.tl != B.tl) return A.tl < B.tl;
        if (A.te != B.te) return A.te < B.te;
        return da < db;
    } else if (sortType == 1) {
        // 最晚出发时间早的优先：tl - 取送距离
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
        // 最早送达早的优先
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
}

vector<int> makeOrderList(int sortType) {
    vector<int> ids;
    ids.reserve(r);

    for (int i = 1; i <= r; i++) {
        if (distAt(orders[i].p, orders[i].d) < INF) {
            ids.push_back(i);
        }
    }

    sort(ids.begin(), ids.end(), [&](int a, int b) {
        return orderLessByType(a, b, sortType);
    });

    return ids;
}

bool tryInsertOne(vector<Rider>& riders, int oid, int mode) {
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

    if (!best.ok) return false;

    vector<int>& seq = riders[best.rider].seq;
    seq.insert(seq.begin() + best.pos, oid);
    riders[best.rider].ev = best.nev;
    return true;
}

Result buildSolution(int sortType, int mode) {
    Result res;
    res.riders.assign(x + 1, Rider());

    vector<int> ids = makeOrderList(sortType);

    for (int oid : ids) {
        if (!timeOK()) break;
        tryInsertOne(res.riders, oid, mode);
    }

    recomputeStats(res);
    return res;
}

vector<int> getUnservedOrders(const Result& res) {
    vector<char> used(r + 1, 0);

    for (int rid = 1; rid <= x; rid++) {
        for (int oid : res.riders[rid].seq) {
            used[oid] = 1;
        }
    }

    vector<int> unserved;

    for (int i = 1; i <= r; i++) {
        if (!used[i] && distAt(orders[i].p, orders[i].d) < INF) {
            unserved.push_back(i);
        }
    }

    return unserved;
}

struct RemoveCand {
    int rid;
    int pos;
    int oid;
    int saving;
    int score;
};

int calcRemovalSaving(const vector<int>& seq, int pos) {
    int len = (int)seq.size();
    int oid = seq[pos];

    const Order& o = orders[oid];
    int pd = distAt(o.p, o.d);

    if (len == 1) {
        return pd;
    }

    if (pos == 0) {
        const Order& nxt = orders[seq[1]];
        return pd + distAt(o.d, nxt.p);
    }

    const Order& pre = orders[seq[pos - 1]];

    if (pos == len - 1) {
        return distAt(pre.d, o.p) + pd;
    }

    const Order& nxt = orders[seq[pos + 1]];

    int oldCost = distAt(pre.d, o.p) + pd + distAt(o.d, nxt.p);
    int newCost = distAt(pre.d, nxt.p);

    return oldCost - newCost;
}

vector<RemoveCand> collectRemoveCandidates(const Result& base, int limit) {
    vector<RemoveCand> cands;

    for (int rid = 1; rid <= x; rid++) {
        const vector<int>& seq = base.riders[rid].seq;
        if (seq.empty()) continue;

        for (int pos = 0; pos < (int)seq.size(); pos++) {
            int oid = seq[pos];
            const Order& o = orders[oid];

            // 这里不用真的 erase + eval：
            // 最短路满足三角不等式，删除一个订单不会让后面的订单更晚，
            // 所以可行序列删掉一单后仍然可行。这里直接估算它对 drive 的贡献。
            int saving = calcRemovalSaving(seq, pos);

            int pd = distAt(o.p, o.d);
            int window = o.tl - o.te;

            // score 越大，越像“占路线/卡时间窗”的坏订单，优先尝试删它。
            int score = saving * 100 + pd * 10 + max(0, 144 - window);

            RemoveCand rc;
            rc.rid = rid;
            rc.pos = pos;
            rc.oid = oid;
            rc.saving = saving;
            rc.score = score;

            cands.push_back(rc);
        }
    }

    sort(cands.begin(), cands.end(), [](const RemoveCand& a, const RemoveCand& b) {
        if (a.score != b.score) return a.score > b.score;
        if (a.saving != b.saving) return a.saving > b.saving;
        return a.oid < b.oid;
    });

    if ((int)cands.size() > limit) cands.resize(limit);

    return cands;
}

// 删除一个已服务订单，再把“未服务订单 + 被删订单”重新插入。
// 这是 V3 的主要新增：允许局部反悔。
Result repairOnce(const Result& base, int removeLimit, int poolLimit, int sortType, int mode) {
    Result best = base;

    vector<int> unserved = getUnservedOrders(base);

    sort(unserved.begin(), unserved.end(), [&](int a, int b) {
        return orderLessByType(a, b, sortType);
    });

    if ((int)unserved.size() > poolLimit) {
        unserved.resize(poolLimit);
    }

    vector<RemoveCand> cands = collectRemoveCandidates(base, removeLimit);

    for (int ci = 0; ci < (int)cands.size(); ci++) {
        if (!timeOK()) break;

        const RemoveCand& rc = cands[ci];
        Result cur = base;

        vector<int>& seq = cur.riders[rc.rid].seq;

        if (rc.pos < 0 || rc.pos >= (int)seq.size()) continue;
        if (seq[rc.pos] != rc.oid) continue;

        // 1. 删除一个“坏订单”
        seq.erase(seq.begin() + rc.pos);
        cur.riders[rc.rid].ev = evalSeq(seq);

        // 2. 候选池：未完成订单 + 被删除的订单
        vector<int> pool = unserved;
        pool.push_back(rc.oid);

        sort(pool.begin(), pool.end(), [&](int a, int b) {
            return orderLessByType(a, b, sortType);
        });

        // 3. 重新插入
        for (int oid : pool) {
            if (!timeOK()) break;
            tryInsertOne(cur.riders, oid, mode);
        }

        recomputeStats(cur);

        if (betterResult(cur, best)) {
            best = move(cur);
        }
    }

    return best;
}

Result improveByRepair(Result res) {
    // 数据越大，局部搜索参数越保守，防 TLE。
    int lightRemove = min(30, max(10, r / 20));
    int lightPool   = min(120, max(40, r / 8));

    int strongRemove = min(90, max(30, r / 10));
    int strongPool   = min(350, max(100, r / 4));

    vector<pair<int, int> > configs;

    configs.push_back(make_pair(0, 1));
    configs.push_back(make_pair(1, 1));
    configs.push_back(make_pair(2, 2));
    configs.push_back(make_pair(3, 0));

    // 先轻量 repair 两轮，有提升才继续吃掉这个提升。
    for (int round = 0; round < 2; round++) {
        bool changed = false;

        for (int i = 0; i < (int)configs.size(); i++) {
            if (!timeOK()) break;

            Result nxt = repairOnce(res, lightRemove, lightPool,
                                    configs[i].first, configs[i].second);

            if (betterResult(nxt, res)) {
                res = move(nxt);
                changed = true;
            }
        }

        if (!changed) break;
    }

    // 最后对当前最好方案做一次稍强 repair。
    if (timeOK()) {
        Result nxt = repairOnce(res, strongRemove, strongPool, 0, 1);
        if (betterResult(nxt, res)) res = move(nxt);
    }

    if (timeOK()) {
        Result nxt = repairOnce(res, strongRemove, strongPool, 1, 1);
        if (betterResult(nxt, res)) res = move(nxt);
    }

    return res;
}

void outputAnswer(const Result& ans) {
    if ((int)ans.riders.size() <= x) return;

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
                // 第一单不用 goto 到取货点
                cur = o.p;
            } else {
                printGoto(cur, o.p);
                cur = o.p;
            }

            cout << "pick " << oid << '\n';

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

        // 同一组数据内 solution 之间不空行；不同测试用例之间留一个空行。
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

        vector<pair<int, int> > basePlans;

        basePlans.push_back(make_pair(0, 0));
        basePlans.push_back(make_pair(0, 1));
        basePlans.push_back(make_pair(0, 2));
        basePlans.push_back(make_pair(1, 0));
        basePlans.push_back(make_pair(1, 1));
        basePlans.push_back(make_pair(2, 0));
        basePlans.push_back(make_pair(2, 2));
        basePlans.push_back(make_pair(3, 0));
        basePlans.push_back(make_pair(4, 1));
        basePlans.push_back(make_pair(5, 0));

        Result best;
        best.served = -1;

        // 先多排序插入贪心取最优 base。
        for (int i = 0; i < (int)basePlans.size(); i++) {
            if (!timeOK()) break;

            Result cur = buildSolution(basePlans[i].first, basePlans[i].second);

            // 对每个 base 只做一次轻量 repair，避免漏掉明显可改的方案。
            if (timeOK()) {
                Result repaired = repairOnce(cur, 20, 80,
                                             basePlans[i].first,
                                             basePlans[i].second);

                if (betterResult(repaired, cur)) {
                    cur = move(repaired);
                }
            }

            if (betterResult(cur, best)) {
                best = move(cur);
            }
        }

        if (best.served < 0) {
            best.riders.assign(x + 1, Rider());
            recomputeStats(best);
        }

        // 再对全局最好方案做更强的删除-重插 repair。
        if (best.served >= 0 && timeOK()) {
            Result repairedBest = improveByRepair(best);

            if (betterResult(repairedBest, best)) {
                best = move(repairedBest);
            }
        }

        outputAnswer(best);
    }

    return 0;
}