#include<iostream>
#include<algorithm>
#include<vector>
using namespace std;
typedef pair<int,int>PII;
typedef long long LL;
const int N=110;
int n;
PII p[N];
LL cross(PII a,PII b,PII c)
{
    //(b-a)x(c-a)
    return (b.first-a.first)*(c.second-a.second)-
           (b.second-a.second)*(c.first-a.first);
}
int main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int m;
    cin>>m;
    for(int round=1;round<=m;round++)
    {
        cout<<"case "<<round<<":\n";
        cin>>n;
        for(int i=0;i<n;i++)
        {
            int x,y;
            cin>>x>>y;
            p[i]={x,y};
        }
        sort(p,p+n);
        //将所有点从左至右x排序，x相同则y从小到大
        
        //一定要去重，勿忘，切记！！
        n=unique(p,p+n)-p;

        //vector:
        //  p.erase(unique(p.begin(), p.end()), p.end());
        //  n=p.size();

        //两个数组vector:lower,upper,
        vector<PII>lower,upper;
        
        //lower（凸包下边界）:从左端点一直扫到右端点，
        //（至少存在两点）前两个点与新点cross<=0,右转，则popback，
        //由于要知道倒数第一、倒数第二个点，所以用数组手写栈，直接能拿到
        
        //upper同理，最后去重端点拼在一起
        //再找到最下边的点start，使用(start+i)%size取余操作进行输出
        
        //如果最开始仅有一个或两个点，直接输出：
        if(n==1)
        {
            cout<<p[0].first<<" "<<p[0].second<<"\n";
            continue;
        }
        if(n==2)
        {
            if(p[1].second<p[0].second)
                swap(p[1],p[0]);
            for(int i=0;i<=1;i++)
                cout<<p[i].first<<" "<<p[i].second<<"\n";
            continue;
        }
        //构造lower
        for(int i=0;i<n;i++)
        {
            //弹出
            while(lower.size()>=2&&
            cross(lower[lower.size() - 2],lower[lower.size() - 1],p[i])<=0)
                lower.pop_back();
            //压入栈
            lower.push_back(p[i]);
        }
        for(int i=n-1;i>=0;i--)//++!=--
        {
            //弹出
            while(upper.size()>=2&&
            cross(upper[upper.size() - 2],upper[upper.size() - 1],p[i])<=0)
                upper.pop_back();
            //压入栈
            upper.push_back(p[i]);
        }
        lower.pop_back();
        upper.pop_back();
        
        //拼在一起
        vector<PII>res;
        for(int i=0;i<=lower.size()-1;i++)
            res.push_back(lower[i]);
        for(int i=0;i<=upper.size()-1;i++)
            res.push_back(upper[i]);
            
        int start=0;
        //n为去重后的总点数，res.size()为凸包点数，两者完全不等哈！！！！！！！！！
        for(int i=1;i<res.size();i++)
            if(res[start].second>res[i].second||res[start].second==res[i].second&&res[i].first<res[start].first)
                start=i;
        for(int i=0;i<res.size();i++)
            cout<<res[(start+i)%res.size()].first<<" "<<res[(start+i)%res.size()].second<<"\n";
    }

    
    return 0;
}