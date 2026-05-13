#include <iostream>
#include <algorithm>
#include <vector>
using namespace std;

//法二，Graham：找到最下面的点作为基座点，极角排序，栈 + 左转判断
typedef pair<int, int> PII;
typedef long long LL;

const int N = 110;
int n;
PII p[N];
PII p0;

LL cross(PII a, PII b, PII c)
{
    // (b - a) x (c - a)
    return 1LL * (b.first - a.first) * (c.second - a.second)
         - 1LL * (b.second - a.second) * (c.first - a.first);
}

LL dist2(PII a, PII b)
{
    LL dx = a.first - b.first;
    LL dy = a.second - b.second;
    return dx * dx + dy * dy;
}

// 按相对于 p0 的极角排序
bool cmp_angle(PII a, PII b)
{
    LL c = cross(p0, a, b);

    // c > 0 说明 p0 -> a -> b 是左转
    // 也就是 a 的极角小于 b 的极角
    if (c != 0) return c > 0;

    // 如果和 p0 在同一条射线上，要距离近的排前面
    // 这样后面 cross <= 0 时，近的会被弹掉，保留远的端点
    return dist2(p0, a) < dist2(p0, b);
}

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int m;
    cin >> m;

    for (int round = 1; round <= m; round++)
    {
        cout << "case " << round << ":\n";

        cin >> n;

        for (int i = 0; i < n; i++)
        {
            int x, y;
            cin >> x >> y;
            p[i] = {x, y};
        }

        // 1. 先排序去重
        sort(p, p + n);
        n = unique(p, p + n) - p;

        // 特判：只有一个点
        if (n == 1)
        {
            cout << p[0].first << " " << p[0].second << "\n";
            continue;
        }

        // 特判：只有两个点
        if (n == 2)
        {
            // 从最下最左点开始输出
            if (p[1].second < p[0].second ||
               (p[1].second == p[0].second && p[1].first < p[0].first))
            {
                swap(p[0], p[1]);
            }

            cout << p[0].first << " " << p[0].second << "\n";
            cout << p[1].first << " " << p[1].second << "\n";
            continue;
        }

        // 2. 找 p0：最下最左点
        int k = 0;

        for (int i = 1; i < n; i++)
        {
            if (p[i].second < p[k].second ||
               (p[i].second == p[k].second && p[i].first < p[k].first))
            {
                k = i;
            }
        }

        swap(p[0], p[k]);
        p0 = p[0];

        // 3. 其余点按照相对于 p0 的极角排序
        sort(p + 1, p + n, cmp_angle);

        // 4. Graham 扫描
        vector<PII> stk;

        stk.push_back(p[0]);
        stk.push_back(p[1]);

        for (int i = 2; i < n; i++)
        {
            while (stk.size() >= 2 &&
                   cross(stk[stk.size() - 2], stk[stk.size() - 1], p[i]) <= 0)
            {
                stk.pop_back();//不能凹进去
            }

            stk.push_back(p[i]);
        }

        // 5. stk 里面就是凸包点，且从 p0 开始逆时针
        for (auto x : stk)
        {
            cout << x.first << " " << x.second << "\n";
        }
    }

    return 0;
}