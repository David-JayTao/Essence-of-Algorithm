#include<iostream>
#include<algorithm>
using namespace std;

const int N=1010;
int n;
int q[N];

void popsort_1()
{
    for(int j=0;j<=n-2;j++)//最多比较a[j]a[j+1],j+1<=n-1
        if(q[j]>q[j+1])
            swap(q[j],q[j+1]);
}

int main()
{
    int m;
    cin>>m;
    while(m--)
    {
        cin>>n;
        for(int i=0;i<n;i++)cin>>q[i];
        popsort_1();
        for(int i=0;i<n;i++)cout<<q[i]<<" ";
        cout<<endl;
    }

    return 0;
}