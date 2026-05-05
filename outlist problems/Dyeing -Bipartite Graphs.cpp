#include<iostream>
#include<cstring>
using namespace std;
//染色法判断二分图
const int N=1e5+10,M=2*N;
int n,m;
int h[N],e[M],ne[M],idx;//因为是无向图，所以每次直接插入2条边，同时插入两个临界点，e、ne要开两倍
//节点编号i:0~n-1，h[i]指向的是下一个相邻节点的地址idx，

int color[N];
void add(int u,int v)
{
    e[idx]=v,ne[idx]=h[u],h[u]=idx++;
}

//如果染色过程出现矛盾，则返回false；
bool dfs(int x,int c)
{
    color[x]=c;//
    //开始递归染色看有没有矛盾
    for(int i=h[x];i!=-1;i=ne[i])
    {
        int j=e[i];

        //注意不要只顾着书写偷懒哈哈哈，
        //else只能匹配最近的if，这种情况简写会出现语义问题
        // if(!color[j])
        //     if(!dfs(j,-c))//1，2，3-c
        //         return false;
        // else if(color[j]==c)
        //         return false;

        if(!color[j])
        {
            if(!dfs(j,-c))//1，2，3-c
                return false;
        }
        else if(color[j]==c)//自环一定不是二分图（奇数环）
                 //这个地方判断了自环
                 //重边不影响，只是多几次遍历
                return false;
    }
    return true;
}
int main()
{
//在处理输入初始化之前先把其他需要的前置初始化完成再说！h[]=-1
    memset(h,-1,sizeof h);
    cin>>n>>m;
    while(m--)
    {
        int u,v;
        cin>>u>>v;
        add(u,v),add(v,u);//无向图
        //重边、自环好像不影响？
    }
//染色法判断二分图；
//如果染色过程出现矛盾（二分图当且仅当不含奇数环<=>相邻节点颜色相同）
//于是递归地对一点染为1，再把相邻的所有节点染为-1


//很关键的一点，可能是不连通图
//于是必须对所有非连通子块均进行染色排查
//当第一次访问到一个子块的第一个点后，会把整个子块均递归地全部染色
//于是若当前点未被染色时便染为1（都是第一次染所以没问题,如果染过了自然会跳过）
    bool flag=true;

    for(int i=1;i<=n;i++)//题目要求编号1~n!!!!!!!
        //将i号节点染色
        if(!color[i])
            if(!dfs(i,1))
            {
                flag=false;
                break;
            }

    if(flag)puts("Yes");
    else puts("No");
    return 0;
}