#include<iostream>
#include<algorithm>
#include<iomanip>
using namespace std;

const int N=1e3+10;
int x[N],y[N];

double get_kth(int x[],int sx,int len_x,
               int y[],int sy,int len_y,
               int k)
{
    if(len_x>len_y)
        return get_kth(y,sy,len_y,x,sx,len_x,k);

    if(len_x==0)
        return y[sy+k-1];
    if(k==1)
        return min(x[sx],y[sy]);

    int step_x=min(len_x,k>>1);
    int step_y=min(len_y,k>>1);

    if(x[sx+step_x-1]<y[sy+step_y-1])
        return get_kth(x,sx+step_x,len_x-step_x,y,sy,len_y,k-step_x);
    else
        return get_kth(x,sx,len_x,y,sy+step_y,len_y-step_y,k-step_y);
}

int main()
{
    int nums;
    cin>>nums;

    cout << fixed << setprecision(5);

    while(nums--)
    {
        int n,m,k;
        cin>>n>>m;
        k=(m+n)>>1;

        for(int i=0;i<n;i++) cin>>x[i];
        for(int i=0;i<m;i++) cin>>y[i];

        if((n+m)%2!=0)
            cout<<get_kth(x,0,n,y,0,m,(n+m+1)>>1)<<"\n";
        else
            cout<<(get_kth(x,0,n,y,0,m,k)+get_kth(x,0,n,y,0,m,k+1))/2.0<<"\n";
    }
    return 0;
}