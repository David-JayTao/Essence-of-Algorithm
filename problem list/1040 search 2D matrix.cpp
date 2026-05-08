#include<iostream>
using namespace std;
const int N = 1e3 + 10;
int a[N][N];
int main()
{
    //整个面为一个斜坡，右下角高，左上角低
    //一、暴力O(n^2)
    //二、每一列二分查找O(nlogn),
        //只利用了每一列的单调性，未用行单调性，还能再优化
    //三、O(m+n)充分利用行列单调性，从左下角（或右上角）开始搜索;
        //右上角为例：
        //i=1,j=5
        //if(a[i][j]<target)i++;//排除第i行，
            //因为a[i][1~j-1]<a[i][j]<target
        //else(a[i][j]>target)j--;//排除第j列
            //a[i+1~n][j]>a[i][j]>target
        //else 
        // {
        //     flag=true;
        //     break;
        // };
    int nums;
    cin>>nums;
    while(nums--)
    {
        int m,n,target;
        cin>>m>>n>>target;
        for(int i=1;i<=m;i++)
            for(int j=1;j<=n;j++)
                cin>>a[i][j];
        bool flag=false;
        int i=1,j=n;
        while(i>=1&&i<=m&&j>=1&&j<=n)
        {
            if(a[i][j]<target)i++;//排除第i行，
                //因为a[i][1~j-1]<a[i][j]<target
            else if(a[i][j]>target)j--;//排除第j列
                //a[i+1~n][j]>a[i][j]>target
            else 
            {
                flag=true;
                break;
            };
        }
        if(flag)cout<<"true"<<endl;
        else cout<<"false"<<endl;
    }
    return 0;
}