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
//计算距离平方，最后一次输出时再加sqrt，显著减少运算量
double dist2(PII p1,PII p2)
{
    double dx=p1.first-p2.first;
    double dy=p1.second-p2.second;
    return dx*dx+dy*dy;
}

// solve(l, r) 做两件事：

// 1. 求出 p[l ~ r] 这个点集内部的最近距离
// 2. 把 p[l ~ r] 按 y 坐标排好序

// p[] 这个数组在算法过程中，它的“排序意义”发生了变化：

// 一开始：p[1 ~ n] 按 x 排序
// 递归过程中：solve(l, r) 返回后，p[l ~ r] 被改成按 y 排序

// 这看起来很怪，但它是为了把合并阶段做到 O(n)。

// PPT 里的伪代码也强调：递归函数返回最近距离，同时把当前区间按 y 排序；
// 临界区 Yd 不需要重新排序，只需要扫描一遍得到。


double solve(int l,int r)//返回1号点~n号点的最短距离的平方，并且按照y排序号数组
{
    double res=1e14;
    
    //base case
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
    //找到
    int mid=l+r>>1;
    int midx=p[mid].first;
    //divide,conquer
    double d2=min(solve(l,mid),solve(mid+1,r));
    res=d2;
    double d=sqrt(d2);
    //combine:
        //此时左半部分、右半部分内每两点间距离一定<=d
        //且左半部分、右半部分内全部按照y的大小重新排序了
        //此后合并入条带时，直接O(n)merge即可
    //一左一右，且距离小于d，
    //一定在条带中，midx-d<=p[k].first<=midx+d
    //在这个条带中的点，并且按照y递增存储起来，
    //每个点只需要计算后面紧接的6个点距离即可，取min
    int i=l,j=mid+1,k=0;
    while(i<=mid&&j<=r)
    {
         if(p[i].second<p[j].second)temp[k++]=p[i++];
         else temp[k++]=p[j++];
    }
    while(i<=mid)temp[k++]=p[i++];
    while(j<=r)temp[k++]=p[j++];
    //物归原主
    for(int i=l,k=0;i<=r;i++,k++)p[i]=temp[k];
    //从 p[l ~ r] 中筛出 |x - midx|^2 < d 的点放进 strip
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
        //先按照x从左到右排序，x相同再按照y从小到大排序
        //返回为double
        printf("%.2f\n", sqrt(solve(1,n)));
        //cout<<fixed<<setprecision(2)<<solve(1,n)<<endl;//1号点~n号点的最短距离
    }
    return 0;
}