#include<iostream>
#include<algorithm>
#include<iomanip>
#include<cmath>
using namespace std;
typedef pair<int,int>PII;
const int N=5e4+10;
int n;
PII p[N];
PII temp[N];//用于条带内merge
PII strip[N];
double dist2(PII p1,PII p2)
{
    double dx=p1.first-p2.first;
    double dy=p1.second-p2.second;
    return dx*dx+dy*dy;
}
double solve(int l,int r)//返回1号点~n号点的最短距离的平方，并且按照y排序号数组
{
    double res=1e14;
    if(r-l+1<=3)
    {
        sort(p+l,p+r+1,[](PII p1,PII p2)
        {
            return p1.second<p2.second;
        });
        for(int i=l;i<=r-1;i++)
            for(int j=i+1;j<=r;j++)//注意不要算到自己和自己的距离
                res=min(res,dist2(p[i],p[j]));
        return res;
    };
    int mid=l+r>>1;
    int midx=p[mid].first;
    double d2=min(solve(l,mid),solve(mid+1,r));
    res=d2;
    double d=sqrt(d2);
    int i=l,j=mid+1,k=0;
    while(i<=mid&&j<=r)
    {
         if(p[i].second<p[j].second)temp[k++]=p[i++];
         else temp[k++]=p[j++];
    }
    while(i<=mid)temp[k++]=p[i++];
    while(j<=r)temp[k++]=p[j++];
    for(int i=l,k=0;i<=r;i++,k++)p[i]=temp[k];
    int s=0;
    for(int i=l,k=1;i<=r;i++)
    {
        double dx=p[i].first-midx;
        if(dx*dx<d2)
            strip[k++]=p[i],s++; 
    }
    for(int i=1;i<=s;i++)
        for(int j=1;j<=6&&i+j<=s;j++)
            res=min(res,dist2(strip[i],strip[i+j]));//注意不要算到自己和自己的距离
    return res;        
}
int main()
{
    int m;
    scanf("%d",&m);
    while(m--)
    {
        scanf("%d",&n);
        for(int i=1;i<=n;i++)
        {
            int x,y;  
            scanf("%d%d",&x,&y);     
            p[i]={x,y};
        }
        sort(p+1,p+1+n);
        printf("%.2f\n", sqrt(solve(1,n)));
    }
    return 0;
}//120ms

//更快！90ms
// #include <cstdio>
// #include <algorithm>
// #include <cmath>
// using namespace std;

// const int N = 100000 + 10;
// const double INF = 1e100;
// const int BRUTE = 16;

// struct Point {
//     int x, y;
// };

// Point p[N], tmp[N], strip[N];

// struct FastScanner {
//     static const int S = 1 << 20;
//     int idx, size;
//     char buf[S];

//     FastScanner() : idx(0), size(0) {}

//     inline char getChar() {
//         if (idx >= size) {
//             size = fread(buf, 1, S, stdin);
//             idx = 0;
//             if (size == 0) return EOF;
//         }
//         return buf[idx++];
//     }

//     bool readInt(int &x) {
//         char c;
//         int sign = 1;
//         x = 0;

//         c = getChar();
//         if (c == EOF) return false;

//         while (c != '-' && (c < '0' || c > '9')) {
//             c = getChar();
//             if (c == EOF) return false;
//         }

//         if (c == '-') {
//             sign = -1;
//             c = getChar();
//         }

//         while (c >= '0' && c <= '9') {
//             x = x * 10 + (c - '0');
//             c = getChar();
//         }

//         x *= sign;
//         return true;
//     }
// } fs;

// inline bool cmpX(const Point &a, const Point &b) {
//     if (a.x != b.x) return a.x < b.x;
//     return a.y < b.y;
// }

// inline bool cmpY(const Point &a, const Point &b) {
//     return a.y < b.y;
// }

// inline double dist2(const Point &a, const Point &b) {
//     double dx = (double)a.x - b.x;
//     double dy = (double)a.y - b.y;
//     return dx * dx + dy * dy;
// }

// // 返回 p[l ~ r] 的最近距离平方，并保证 p[l ~ r] 按 y 排序
// double solve(int l, int r) {
//     int len = r - l + 1;

//     if (len <= BRUTE) {
//         double ans = INF;

//         for (int i = l; i <= r; i++) {
//             for (int j = i + 1; j <= r; j++) {
//                 ans = min(ans, dist2(p[i], p[j]));
//             }
//         }

//         sort(p + l, p + r + 1, cmpY);
//         return ans;
//     }

//     int mid = (l + r) >> 1;
//     int midx = p[mid].x;

//     double ans = min(solve(l, mid), solve(mid + 1, r));

//     int i = l, j = mid + 1, k = l;

//     while (i <= mid && j <= r) {
//         if (p[i].y < p[j].y) tmp[k++] = p[i++];
//         else tmp[k++] = p[j++];
//     }

//     while (i <= mid) tmp[k++] = p[i++];
//     while (j <= r) tmp[k++] = p[j++];

//     for (int t = l; t <= r; t++) {
//         p[t] = tmp[t];
//     }

//     int cnt = 0;

//     for (int t = l; t <= r; t++) {
//         double dx = (double)p[t].x - midx;
//         if (dx * dx < ans) {
//             strip[cnt++] = p[t];
//         }
//     }

//     for (int a = 0; a < cnt; a++) {
//         int limit = min(cnt, a + 8);

//         for (int b = a + 1; b < limit; b++) {
//             double dy = (double)strip[b].y - strip[a].y;
//             if (dy * dy >= ans) break;

//             ans = min(ans, dist2(strip[a], strip[b]));
//         }
//     }

//     return ans;
// }

// int main() {
//     int T;
//     fs.readInt(T);

//     while (T--) {
//         int n;
//         fs.readInt(n);

//         for (int i = 1; i <= n; i++) {
//             fs.readInt(p[i].x);
//             fs.readInt(p[i].y);
//         }

//         sort(p + 1, p + n + 1, cmpX);

//         double ans2 = solve(1, n);

//         printf("%.2f\n", sqrt(ans2));
//     }

//     return 0;
// }