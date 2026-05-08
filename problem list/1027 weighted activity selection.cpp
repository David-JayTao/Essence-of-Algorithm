#include<iostream>
#include<algorithm>
using namespace std;
const int N=1e4+10;
int n;
struct range
{
    int s,f,v;
    bool operator<(const range& t)const
    {
        return f<t.f;
    }
}ranges[N];
int dp[N];//dp[i],包含1~i个活动的max ∑vj
//dp[n];
int find(int l,int r,int i)
{
    while(l<r)
    {
        int mid=l+r+1>>1;
        if(ranges[mid].f<=ranges[i].s)l=mid;
        else r=mid-1;
    }
    return l;
}
int main()
{
    int m;
    cin>>m;
    while(m--)
    {
        cin>>n;
        for(int i=1;i<=n;i++)
        {
            int s,f,v;
            cin>>s>>f>>v;
            ranges[i]={s,f,v};
        }
        sort(ranges+1,ranges+n+1);
        for(int i=1;i<=n;i++)
        {
            //int k=find(0,i,i);
            int k=find(0,i-1,i);
            //1、不包含活动i
            //2、包含活动i，找到最后一个不重叠的区间
                //k为最后一个终点ranges[k].f<=ranges[i].s的，（最大数k）
            dp[i]=max(dp[i-1],dp[k]+ranges[i].v);
        }
        cout<<dp[n]<<endl;
    }
    return 0;
}