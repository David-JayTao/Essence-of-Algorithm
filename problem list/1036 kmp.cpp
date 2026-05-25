#include<iostream>
#include<cstring>
using namespace std;
const int N=1e5+10;
char s[N],t[N];
int ne[N];
int main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int T;
    cin>>T;
    while(T--)
    {
        int n,m;
        cin>>n>>m>>s+1>>t+1;
        //必须要加一行重新初始化ne[j]
        memset(ne,0,sizeof ne);
        int res=0;
        for(int i=2,j=0;i<=m;i++)
        {
            while(j&&t[i]!=t[j+1])j=ne[j];
            if(t[i]==t[j+1])
            {
                j++;
                ne[i]=j;
                //每组数据重新初始化ne时，只有匹配成功才会给ne赋值
                //故必须把上组数据赋的值清除
            }
        }
        for(int i=1,j=0;i<=n;i++)
        {
            while(j&&s[i]!=t[j+1])j=ne[j];
            if(s[i]==t[j+1])j++;
            if(j==m)
            {
                res++;
                j=ne[j];
            }
        }
        cout<<res<<endl;
    }
    return 0;
}