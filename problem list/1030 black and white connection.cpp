#include<iostream>
#include<cstring>
using namespace std;
const int N=2e4+10;
int color[N];//0:白色,1:黑色
bool st[N];//是否已经访问过，是否已经连好线了，st[i]=true,
int main()
{
    //贪心+双指针，O(n),i、j指针间的单调性
    int m;
    cin>>m;
    while(m--)
    {
        int n;
        cin>>n;
        for(int i=1;i<=2*n;i++)cin>>color[i];
        memset(st,0,sizeof st);
        int res=0;
        int j=2;//j随着i的前进永不后退
        for(int i=1;i<=2*n-1;i++)
        {
            if(!st[i])
            {
                st[i]=true;
                //颜色为color[i],匹配1-color[i]
                while(j<=2*n)
                {
                    if(!st[j]&&color[j]==1-color[i])
                    {
                        st[j]=true;
                        res+=j-i;
                        break;//勿忘break
                    }
                    j++;
                }
            }
        }
        cout<<res<<endl;
    }

    
    return 0;
}