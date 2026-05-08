#include<iostream>
#include<algorithm>
#include<cstring>
using namespace std;
typedef long long LL;
typedef pair<LL ,int>PII;
const int N = 5e4 + 10;
PII w[N];//<pi,di>;
bool st[N];//当前是否空闲
int pa[N];//pa[i] 本质上是并查集里的父节点指针。
//真正有意义的是：find(i)它才表示：当前不超过 i 的最晚空闲位置。
int find(int x)
{
    if(pa[x]!=x)pa[x]=find(pa[x]);
    return pa[x];
}
int main()
{
    //每个作业耗时都一样，都是 1
    //同一个位置肯定放高收益更好
    //贪心：
    //每次将p最大的放在前面空闲的最晚的地方，局部->全局
    //sort
    
    //法一：O(nlogn+n*d)->O(n^2)暴力枚举空闲的最晚的地方
    
    //法二:O(nlogn+n)=O(nlogn)并查集优化：
        //核心：利用并查集的路径压缩替换暴力扫描，可以直接O(1)跳转到正确位置
        //它不是传统那种“判断两个点是否连通”的并查集。
        //这里用的是并查集的一个变形：
            //快速找到当前点左边最近的可用位置。
            
        //动态删除 + 查询前驱:
            //不断删除一些位置，同时查询 <= x 的最大未删除位置。
        //并查集刚好可以处理这种“只往一个方向合并”的情况。
        //find(x)找到时间点x前空闲的最晚的地方
        
        //
    int nums;
    scanf("%d",&nums);
    while(nums--)
    {
        int n;
        scanf("%d",&n);
        int max_d=0;
        for(int i=1;i<=n;i++)
        {
            LL p;
            int d;
            scanf("%d%lld",&d,&p);
            w[i]={p,d};
            max_d=max(max_d,d);
        }
        //将work从的p(w[i].first)从大到小排序
        sort(w+1,w+1+n,greater<PII>());
        //优先选择p大的，且放在空闲的最晚的位置
        
        LL res=0;        
        
        memset(st,1,sizeof st);
        
        for(int i=1;i<=max_d;i++)pa[i]=i;//初始化为i
        //pa[0]=0
        //如果第i个时间点放上作业了，pa[i]=find(i-1),
        //从第i个时间点往前找的最晚空闲位置
        //<=>
        //从第i-1个时间点往前找的最晚空闲位置
        
        for(int i=1;i<=n;i++)
        {
            LL p=w[i].first;
            int d=w[i].second;
            // for(int j=d;j>=1;j--)
            //     if(st[j])
            //     {
            //         st[j]=false;
            //         res+=p;
            //         break;//勿忘
            //     }
            
            int j=find(d);
            if(j)
            {
                res+=p;
                pa[j]=find(j-1);
            }
        }
        //cout<<res<<endl;
        printf("%lld\n", res);
    }
    return 0;
}