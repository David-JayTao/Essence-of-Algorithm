#include<iostream>
#include<algorithm>
using namespace std;
//稀疏图kruscal:
//最开始全部不连通，每次加最短的一条边（只要这两点未连通，判断连通使用并查集）
//如果最终加了n-1条则连通；
const int N=510;
int n,m;
int p[N];//并查集O(1)，p[i]为i号节点的祖宗节点编号,1~n

//不需要使用复杂数据结构，直接用struct存所有无向边即可，每次取出满足要求的最短的
struct edge
{
    int u,v,w;//无向边
    //定义结构体比较规则：
    bool operator<(const edge&e)const
    {
        return w<e.w;
    }
}edges[40*N];

int find(int x)//“路径压缩”很重要！！！不然会因效率低下而过不了！！
{
    if(p[x]!=x)p[x]=find(p[x]);
    //1、注意此处递归必须传入p[x],传入x会导致死循环
    //2、必须更新x的祖宗p[x]的值，不能是x=find(),意思都不对
    //3、这个递归很妙，只有当x是根节点是才会执行到当前层的return返回自己的祖宗编号
    return p[x];
    //return x;语义也不对，应该是返回x的祖宗节点，这样会导致无线套娃，Memory Limit Exceeded       
}
////////////////////////////唯一写法////////////////////////////////////////////////////////
// int find(int x)
// {
//     //简记：函数体内全部用p[x]!!!(只有判断条件时会出现p[x]!=x)
//     if(p[x]!=x)p[x]=find(p[x]);
//     return p[x];
// }

void kruscal()
{
    int res=0,cnt=0;
    for(int i=0;i<m;i++)//共m条边，从其中选到n-1则成功连通！
    {
        int a=edges[i].u,b=edges[i].v,w=edges[i].w;
        
        int pa=find(a),pb=find(b);
        if(pa!=pb)//两点间不连通
        {
            res+=w;        
            cnt++;
            p[pb]=pa;
            //合并集合时必须要合并根节点，
            //如果只是合并p[b]=a;那么b集合中其他所有点都还是指向原来的根，而原来的根完全没有变化
            //如果更新根节点，那么这一串点全部都更新了
        }
    }
    if(cnt<n-1)cout<<"-1"<<endl;
    else cout<<res<<endl;
    // if(cnt==n-1)cout<<res<<endl;
    // else cout<<"impossible"<<endl;
}
int main()
{
    int T;
    cin>>T;
    while(T--)
    {
        cin>>n>>m;
        for(int i=0;i<m;i++)
        {
            int u,v,w;
            cin>>u>>v>>w;
            edges[i]={u,v,w};
        }
        //将所有边排序(注意需要自定义比较规则)
        sort(edges,edges+m);   
        
        for(int i=1;i<=n;i++) p[i]=i;//并查集初始化
    
        kruscal();
    }    
    return 0;
}