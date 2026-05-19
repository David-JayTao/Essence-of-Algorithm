//本质：选最少的点覆盖所有区间
#include<iostream>
#include<algorithm>
#include<cmath>
using namespace std;
typedef pair<double,double>PDD;
const int N=1e4+10;
PDD points[N];//points[1]={x,y};
PDD ranges[N];//
//排序ranges

const double eps = 1e-10;//提高浮点运算容错率，考虑微小运算误差

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int m;
    cin>>m;
    while(m--)
    {
        int n,d;
        cin>>n>>d;
        for(int i=1;i<=n;i++)
        {
            double x,y;
            cin>>x>>y;
            points[i]={x,y};
            double t=sqrt(d*d-y*y);
            //double t=sqrt(d*d-y*y+eps);
            double l=x-t,r=x+t;
            ranges[i]={l,r};
        }
        //按照区间右端点排序
        sort(ranges+1,ranges+n+1,[](PDD p1,PDD p2)
        {
            return p1.second<p2.second;
        });
        //开始区间覆盖；
        double ed=-1e10;
        int res=0;
        for(int i=1;i<=n;i++)
        {
           //if(ranges[i].first>ed)
           if(ranges[i].first>ed+eps)
           {
               res++;
               ed=ranges[i].second;
           }
        }
        cout<<res<<endl;
    }
    return 0;
}