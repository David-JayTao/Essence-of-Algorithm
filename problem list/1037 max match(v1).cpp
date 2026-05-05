#include<iostream>
#include<cstring>
using namespace std;

const int N=60,M=1010;
int n1,n2;

int h[N],e[M],ne[M],idx;
//h[1~n1];
//e[地址]：编号

bool st[N];//st[1~n2]
//对于每个左点集的男生，这一轮中是否已经考虑过右边编号为i的女生，保证不重复访问造成find(i)死循环

int match[N];//match[1~n2]=1~n1;

void add(int u,int v)
{
    e[idx]=v,ne[idx]=h[u],h[u]=idx++;
}

bool find(int x)
{
    //如果x号男生匹配成功，返回true，反之返回false,
    //一一考虑该男生的心动对象，
        //如果单身（match==0）,直接抢走，
        //如果match！=0，但现男友还有其他备胎，便喧宾夺主
    for(int i=h[x];i!=-1;i=ne[i])
    {
        int j=e[i];
        if(!st[j])
        {
            st[j]=true;//不管成功与否，我都已经考虑过这个女生了
            if(match[j]==0||find(match[j]))
            {
                match[j]=x;
                return true;
            }            
        }
    }
    return false;
}

int main()
{
    int T;
    cin>>T;
    while(T--)
    {
        int res=0;
        //初始化h[]数组全为-1，表示i号男生还没有任何心动关系
        memset(h,-1,sizeof h);
        memset(match,0,sizeof match);//match别忘了清空,初始化为0
        idx=0;
        
        cin>>n1>>n2;
        
        for(int i=1;i<=n1;i++)
        {
            int k;
            cin>>k;
            while(k--)
            {
                int m;
                cin>>m;//勿忘读进女生编号
                add(i,m);      
            }
        }

        for(int i=1;i<=n1;i++)
        {
            memset(st,false,sizeof st);//对于每个男生来说都还没有考虑过右边的所有女生
            if(find(i))res++;
        }
        cout<<res<<endl;
    }
    return 0;
}