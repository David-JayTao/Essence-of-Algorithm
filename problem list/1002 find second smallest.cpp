#include<iostream>
#include<algorithm>
using namespace std;

const int N=1010;
int n;
int q[N];

int findSecond()
{
    //只维护这两个数据，遇到<最小的，便更新最小的x1；
    //遇到>=最小,<第二小的，便更新第二小的x2
    int x1=min(q[0],q[1]);//最小的
    int x2=max(q[0],q[1]);//第2小的,x1<=x2
    for(int i=2;i<n;i++)
        if(q[i]<x1)
            //x1=q[i];//此处考虑不完整，当更新最小值时，x2=pre(x1)
            x2=x1,x1=q[i];
        else if(q[i]<x2)
            x2=q[i];
    return x2;
}

int main()
{
    int m;
    cin>>m;
    while(m--)
    {
        cin>>n;
        for(int i=0;i<n;i++)cin>>q[i];
        cout<<findSecond()<<endl;
    }

    return 0;
}