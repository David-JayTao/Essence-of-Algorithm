#include <bits/stdc++.h>
using namespace std;

const int N = 310;
const double eps = 1e-10;

struct Job
{
    double l, r, t;
    // 当前压缩坐标系下：
    // 传感器可采区间 [l, r]
    // 还需要采集 t 秒
};

struct Seg
{
    double l, r;
    // 当前剩余坐标轴上的一段，在原坐标系中的位置 [l, r]
    // 注意：当前压缩坐标不是直接存的，而是通过 seg 的长度累计出来
};

struct Ans
{
    double l, r, tau;
    // 原坐标区间 [l, r] 使用时间密度 tau
    // tau = 1 / v
};

vector<Job> jobs;
vector<Seg> segs;
vector<Ans> ans;

double D;

double P(double v)
{
    return 0.07 * v * v * v + 0.0391 * v * v - 13.196 * v + 390.95;
}

// 单位距离能耗：P(v) / v
double cost_per_meter(double v)
{
    return P(v) / v;
}

// 三分求最省单位距离能耗的巡航速度 v0
// 也就是 minimize P(v) / v
double get_v0()
{
    double l = 1e-9, r = 10000;

    for (int i = 0; i < 200; i++)
    {
        double m1 = l + (r - l) / 3;
        double m2 = r - (r - l) / 3;

        if (cost_per_meter(m1) < cost_per_meter(m2)) r = m2;
        else l = m1;
    }

    return (l + r) / 2;
}

// 把当前压缩坐标系中的区间 [L, R] 映射回原坐标，加入答案
void add_answer(double L, double R, double tau)
{
    double cur = 0;

    for (auto s : segs)
    {
        double len = s.r - s.l;
        double a = cur, b = cur + len;

        double x = max(a, L);
        double y = min(b, R);

        if (x + eps < y)
        {
            // 压缩坐标 [x, y] 落在当前 seg 的 [a, b] 内
            // 因为 seg 内部没有被删除过，所以可以线性映射回原坐标
            double ol = s.l + (x - a);
            double orr = s.l + (y - a);

            ans.push_back({ol, orr, tau});
        }

        cur = b;
    }
}

// 从当前剩余坐标轴中删除压缩坐标区间 [L, R]
void delete_segment(double L, double R)
{
    vector<Seg> ns;
    double cur = 0;

    for (auto s : segs)
    {
        double len = s.r - s.l;
        double a = cur, b = cur + len;

        // 当前原坐标段 [s.l, s.r] 和删除区间 [L, R] 没交集
        if (b <= L + eps || a >= R - eps)
        {
            ns.push_back(s);
        }
        else
        {
            // 左边剩余部分
            if (a + eps < L)
            {
                double nl = s.l;
                double nr = s.l + (L - a);
                if (nl + eps < nr) ns.push_back({nl, nr});
            }

            // 右边剩余部分
            if (R + eps < b)
            {
                double nl = s.l + (R - a);
                double nr = s.r;
                if (nl + eps < nr) ns.push_back({nl, nr});
            }
        }

        cur = b;
    }

    segs = ns;
}

// 删除压缩坐标区间 [L, R] 后，一个点 x 在新压缩坐标系中的位置
double trans_point(double x, double L, double R)
{
    double len = R - L;

    if (x <= L) return x;
    if (x >= R) return x - len;

    // x 落在被删除区间内部，删除后会被压到 L 这个位置
    return L;
}

// 当前剩余总长度
double get_total_len()
{
    double res = 0;
    for (auto s : segs) res += s.r - s.l;
    return res;
}

void solve_case()
{
    double v0 = get_v0();
    double tau0 = 1.0 / v0;

    while (jobs.size())
    {
        vector<double> xs;

        for (auto j : jobs)
        {
            xs.push_back(j.l);
            xs.push_back(j.r);
        }

        sort(xs.begin(), xs.end());
        xs.erase(unique(xs.begin(), xs.end(), [](double a, double b) {
            return fabs(a - b) < eps;
        }), xs.end());

        double best = -1;
        double best_l = -1, best_r = -1;

        // 枚举候选区间 [xs[i], xs[j]]
        // 最大密度区间一定可以由某些任务端点组成
        for (int i = 0; i < (int)xs.size(); i++)
        {
            for (int j = i + 1; j < (int)xs.size(); j++)
            {
                double L = xs[i], R = xs[j];
                double len = R - L;
                if (len <= eps) continue;

                double sum = 0;

                // 统计完全被包含在 [L, R] 中的任务
                // 这些任务无法逃出该区间，必须全部在这里采完
                for (auto job : jobs)
                {
                    if (job.l + eps >= L && job.r <= R + eps)
                        sum += job.t;
                }

                double density = sum / len;

                // 选最大密度区间
                // 若密度几乎相同，取更长的区间，减少碎片，稳定一些
                if (density > best + eps ||
                    (fabs(density - best) < eps && len > best_r - best_l))
                {
                    best = density;
                    best_l = L;
                    best_r = R;
                }
            }
        }

        // 如果剩下所有区间的最大密度都不超过 tau0，
        // 说明最省巡航速度已经能提供足够采集时间。
        if (best <= tau0 + eps)
        {
            for (auto s : segs)
                ans.push_back({s.l, s.r, tau0});

            jobs.clear();
            segs.clear();
            break;
        }

        // 当前最大密度区间是瓶颈区间。
        // 它必须以 tau = best 飞行，也就是速度 v = 1 / best。
        add_answer(best_l, best_r, best);

        vector<Job> njobs;

        for (auto job : jobs)
        {
            // 完全包含在最大密度区间内的任务，已经在这里采完，删除
            if (job.l + eps >= best_l && job.r <= best_r + eps)
                continue;

            // 其他任务保留，但要根据删除 [best_l, best_r] 后的新坐标更新 l, r
            double nl = trans_point(job.l, best_l, best_r);
            double nr = trans_point(job.r, best_l, best_r);

            if (nl + eps < nr)
                njobs.push_back({nl, nr, job.t});
        }

        jobs = njobs;

        // 当前瓶颈区间已经固定速度，从剩余问题中剪掉
        delete_segment(best_l, best_r);
    }

    // 理论上如果没有任务了，但还有未覆盖的剩余路段，也应该用 tau0 巡航
    if (segs.size())
    {
        for (auto s : segs)
            ans.push_back({s.l, s.r, tau0});

        segs.clear();
    }

    // 整理答案：按原坐标排序，并合并相邻且速度相同的区间
    sort(ans.begin(), ans.end(), [](Ans a, Ans b) {
        if (fabs(a.l - b.l) > eps) return a.l < b.l;
        return a.r < b.r;
    });

    vector<Ans> res;

    for (auto x : ans)
    {
        if (x.l + eps >= x.r) continue;

        if (res.empty())
        {
            res.push_back(x);
        }
        else
        {
            Ans &last = res.back();

            // 如果两段相邻，且 tau 一样，就合并
            if (fabs(last.r - x.l) < 1e-8 && fabs(last.tau - x.tau) < 1e-8)
                last.r = x.r;
            else
                res.push_back(x);
        }
    }

    // 防御性处理：最后一个端点应该是 D
    if (res.size()) res.back().r = D;

    cout << res.size() << '\n';

    for (int i = 0; i < (int)res.size(); i++)
    {
        if (i) cout << ' ';
        cout << res[i].r;
    }
    cout << '\n';

    for (int i = 0; i < (int)res.size(); i++)
    {
        if (i) cout << ' ';
        cout << 1.0 / res[i].tau;
    }
    cout << '\n';
}

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    cout << fixed << setprecision(10);

    int T;
    cin >> T;

    while (T--)
    {
        int n;
        cin >> n;
        cin >> D;

        jobs.clear();
        segs.clear();
        ans.clear();

        for (int i = 1; i <= n; i++)
        {
            double l, r, t;
            cin >> l >> r >> t;
            jobs.push_back({l, r, t});
        }

        // 初始时，整个 [0, D] 都还没有被确定速度
        segs.push_back({0, D});

        solve_case();
    }

    return 0;
}