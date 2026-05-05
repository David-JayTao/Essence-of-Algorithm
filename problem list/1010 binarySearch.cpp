#include <iostream>
#include<cstring>
using namespace std;

const int N = 20010;

int a[N];//递增有序数组

int main()
{
    int m;
    cin>>m;
    while(m--)
    {
        //memset(a,0,sizeof a);
        //没有必要，因为只会用到下标在0~n-1的部分
        int n,x;
        cin>>n>>x;
        for(int i=0;i<n;i++)
            cin>>a[i];
        int l=0,r=n-1;
        int fa=-1;
        int mid;
        while(l<=r)
        {
             mid=l+r>>1;
     
            if(a[mid]>x)r=mid-1;
            else if(a[mid]<x) l=mid+1;
            else break;
            
            fa=mid;
        }
        //默认题意是不会出现第一次命中的情况，不然fa=-1访问越界
        if(a[mid]==x)cout<<"success, father is "<<a[fa]<<endl;
        else cout<<"not found, father is "<<a[fa]<<endl;

    }

    return 0;
}





