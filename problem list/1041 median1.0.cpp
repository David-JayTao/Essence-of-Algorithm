#include<iostream>
#include<algorithm>
#include<iomanip>
using namespace std;

const int N=1e3+10;
int x[N],y[N];

//注意平均值可能有0.5,返回double
double get_kth(int x[],int sx,int len_x,
         int y[],int sy,int len_y,
         int k)
         //包含sx、sy均往后多长的区间内找这个第k小的数
         //sx~sx+len-1
         //sy~sy+len-1 
         //数组剩余长度 ，lenx,leny 
         //k/2为往后看的步长
{
    //寻找两个区间合在一起的第k小
    //只处理x比y短，可以保证一定是短的x先被砍完
    //x[sx]->x[sx+len_x-1]
        //此处为了不越界，每次往后看的区间长度
        //step_x=min(n-sx,len_x/2)
        //step_y=min(n-sy,len_y/2)
    //递归终止：
        //sx==n-1,len_x==0,短的已经删完了，空了，直接返回长的第k小即可
        //k==1，return min(x[sx],y[sy]);
    //if(x[sx+step_x-1]<y[sy+step_y-1])
        //砍去短的左半小部分
        //sx=sx+step_x;
        //k-=step_x;
            //不用砍去y[sy+step_y-1]后面的数，
            //因为搜索长度每次砍一半，隐含不会递归到右半部分
    //else
        //砍去长的左半部分
        //sy=sy+step_y;

    //先保证x比y短，可以更好地定义边界情况
    //sx可能越界
    if(len_x>len_y)
        return get_kth(y,sy,len_y,x,sx,len_x,k);

    //先写递归终止条件
    if(len_x==0)
        return y[sy+k-1];   
    if(k==1)//此时x已确定未越界
        return min(x[sx],y[sy]);

    int step_x=min(len_x,k>>1);
    int step_y=min(len_y,k>>1);
    //更新还剩的长度

    if(x[sx+step_x-1]<y[sy+step_y-1])
        return get_kth(x,sx+step_x,len_x-step_x,y,sy,len_y,k-step_x);
        //注意在寻找第k小时，不能够删除右半部分，因为存在 step_x=min(len_x,k>>1),如果短的已经到头了，短的基准点左边完全有可能不足k/2;
        //return get_kth(x,sx+step_x,len_x-step_x,y,sy,step_y,k-step_x);
    else
        return get_kth(x,sx,len_x,y,sy+step_y,len_y-step_y,k-step_y);
        //return get_kth(x,sx,step_x,y,sy+step_y,len_y-step_y,k-step_y);  
}
int main()
{
    int nums;
    cin>>nums;
    //别忘了递减nhhhhhhh
    while(nums--)
    {
        int n,m,k;
        cin>>n>>m;
        k=m+n>>1;
        for(int i=0;i<=n-1;i++)
            cin>>x[i];
        for(int i=0;i<=m-1;i++)
            cin>>y[i];

        cout << fixed << setprecision(5);

        if((n+m)%2!=0)//总共奇数个，
            cout<<get_kth(x,0,n,y,0,m,(n+m+1)>>1)<<"\n";
        else//总共偶数个
            cout<<(get_kth(x,0,n,y,0,m,k)+get_kth(x,0,n,y,0,m,k+1))/2.0<<"\n";
    }
    return 0;
}