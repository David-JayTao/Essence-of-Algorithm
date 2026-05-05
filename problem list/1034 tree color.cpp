#include<iostream>
#include<cstring>
using namespace std;

const int N=50010;
// 和“上司的舞会”本质一样，状态转移一摸一样
// 只是上司题的父子关系天然给定，有一个定了的根（有向图）
// 而这题的父子关系需要通过 DFS 人为确定，根自己随便定一个（无向图）

//树的本质：连通且没有环的无向图，
//采用邻接表存储，能够直接找到以i号为根节点所有子节点（向下直接相邻的所有子节点）,
//k为l的直接，除开最上层根节点，其他n-1个点都有一个直接父节点，共n-1条边

int h[2*N],e[2*N],ne[2*N],idx;//无向图空间要开够，两倍点的数量
int n,m;
bool visited[N];
//这是与上司的核心区别，因为是无向图，双向边，
//每次遍历所有指出去的边，保证不要指向父节点
void add(int u,int v)
{
    e[idx]=v,ne[idx]=h[u],h[u]=idx++;
}

//状态表示：
//对于每个局部根节点来说，均有选或不选m_root两种，
//f[u][0]，根节点u为白色,f[u][0]=∑max(f[j][0],f[j][1]);
//f[u][1]，根节点u为黑色,f[u][1]=1+∑f[j][0];

int f[N][2];

void dfs(int u)//求解以u为根节点的树的f[u][0],f[u][1]
{
    visited[u]=true;
    f[u][1]=1;//自己染成了黑色
    for(int i=h[u];i!=-1;i=ne[i])//注意邻接表的遍历写法，不是i++
    {
        int j=e[i];
        if(!visited[j])
        {
            dfs(j);
            f[u][0]+=max(f[j][0],f[j][1]);
            f[u][1]+=f[j][0];
        }
    }
}
int main()
{
    cin>>m;
    while(m--)
    {
        cin>>n;
        //每组数据必须初始化完整，邻接表初始化idx=0，h[i]=-1
        //visited,f都必须要重新初始化（f[][0]是在原基础上叠加）
        idx=0;
        memset(h,-1,sizeof h);
        memset(f,0,sizeof f);
        memset(visited,0,sizeof visited);
        
        for(int i=1;i<=n-1;i++)
        {
            int u,v;
            cin>>u>>v;
            add(u,v),add(v,u);
        }
    
        int root=1;
        dfs(root);
        
        cout<<max(f[root][0],f[root][1])<<endl;
    }
    
    return 0;
}
