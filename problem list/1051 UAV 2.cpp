#include <bits/stdc++.h>
using namespace std;

const int N = 1010;
const double eps = 1e-10;

int n;
double D;
double L[N], R[N], Tm[N];
double s[N];  // 前缀和，s[i] = t1 + ... + ti

struct Segment
{
    double l, r, tau;
    // 原坐标区间 [l, r] 使用时间密度 tau
    // tau = 1 / v
};

vector<Segment> ans;

// 第二关功率函数
double P(double v)
{
    return 0.3 * v * v * v + 0.13 * v * v - 150 * v + 1160;
}

// 单位距离能耗 = P(v) / v
double cost(double v)
{
    return P(v) / v;
}

// 三分求最省单位距离能耗的巡航速度 v0
double get_v0()
{
    double l = 1e-9, r = 10000;

    for (int i = 0; i < 200; i++)
    {
        double m1 = l + (r - l) / 3;
        double m2 = r - (r - l) / 3;

        if (cost(m1) < cost(m2)) r = m2;
        else l = m1;
    }

    return (l + r) / 2;
}

// 加入一段答案，并且顺手合并相邻且速度相同的段
void add_segment(double l, double r, double tau)
{
    if (l + eps >= r) return;

    if (ans.size())
    {
        Segment &last = ans.back();

        if (fabs(last.r - l) < 1e-8 && fabs(last.tau - tau) < 1e-8)
        {
            last.r = r;
            return;
        }
    }

    ans.push_back({l, r, tau});
}

/*
    solve(l, r, A, B)

    当前要安排传感器 l ~ r；
    当前这些传感器只能使用原坐标区间 [A, B]。

    为什么会有 A, B？
    因为 YDS 每次会把一个最大密度区间拿出来固定速度，
    剩下左边任务只能在左边做，右边任务只能在右边做。
*/
void solve(int l, int r, double A, double B, double tau0)
{
    if (A + eps >= B) return;

    // 如果这一段里已经没有任务了，那就是纯飞行，使用最省巡航速度
    if (l > r)
    {
        add_segment(A, B, tau0);
        return;
    }

    double best = -1;
    double best_l = -1, best_r = -1;
    int bi = -1, bj = -1;

    /*
        枚举连续传感器块 i ~ j。

        因为必须按编号顺序采集，所以瓶颈一定是一段连续任务块。
        i ~ j 的可用空间区间是：

            [max(A, L[i]), min(B, R[j])]

        它们总共需要：

            t[i] + ... + t[j]

        所以密度为：

            sum / length
    */
    for (int i = l; i <= r; i++)
    {
        for (int j = i; j <= r; j++)
        {
            double left = max(A, L[i]);
            double right = min(B, R[j]);

            double len = right - left;
            if (len <= eps) continue;

            double sum = s[j] - s[i - 1];
            double density = sum / len;

            double old_len = best_r - best_l;

            // 选最大密度块；密度相同则选更长的，输出更稳定，也更容易合并
            if (density > best + eps ||
                (fabs(density - best) < eps && len > old_len + eps))
            {
                best = density;
                best_l = left;
                best_r = right;
                bi = i;
                bj = j;
            }
        }
    }

    /*
        如果最大密度都不超过 tau0，
        说明最省巡航速度已经能提供足够采集时间，
        没必要为了“刚好采够”而飞得更快。
    */
    if (best <= tau0 + eps)
    {
        add_segment(A, B, tau0);
        return;
    }

    /*
        否则，bi ~ bj 是当前最紧的连续传感器块。

        它们必须在 [best_l, best_r] 内完成，
        且这段区间的最优时间密度就是 best。

        对应速度：
            v = 1 / best
    */

    // 左边传感器 l ~ bi-1 只能在 [A, best_l] 中完成
    solve(l, bi - 1, A, best_l, tau0);

    // 当前最大密度瓶颈区间，速度被锁死
    add_segment(best_l, best_r, best);

    // 右边传感器 bj+1 ~ r 只能在 [best_r, B] 中完成
    solve(bj + 1, r, best_r, B, tau0);
}

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    cout << fixed << setprecision(6);

    int Case;
    cin >> Case;

    while (Case--)
    {
        cin >> D;
        cin >> n;

        ans.clear();

        for (int i = 1; i <= n; i++)
        {
            cin >> L[i] >> R[i] >> Tm[i];
            s[i] = s[i - 1] + Tm[i];
        }

        double v0 = get_v0();
        double tau0 = 1.0 / v0;

        solve(1, n, 0, D, tau0);

        // 防止浮点误差导致最后一个点不是 D
        if (ans.size()) ans.back().r = D;

        cout << ans.size() << '\n';

        for (int i = 0; i < (int)ans.size(); i++)
        {
            if (i) cout << ' ';
            cout << ans[i].r;
        }
        cout << '\n';

        for (int i = 0; i < (int)ans.size(); i++)
        {
            if (i) cout << ' ';
            cout << 1.0 / ans[i].tau;
        }
        cout << '\n';
    }

    return 0;
}