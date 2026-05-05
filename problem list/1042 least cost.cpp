#include<iostream>
#include<algorithm>
using namespace std;

const int N=370;

int days[N];
//days[n],共旅行1<=n<=365天，
//days[i]=j,1<=i<=n,整个旅行中的第i天是一年中的第j天,1<=j<=365
int n;
//int f[N];//f[i]包含旅行前i天的方案中最低消费，
int f[N];//f[i]一年中前i天旅行完后的方案中最低消费，
int cost[3];

int get(int x) // 找到离得最近的小于等于这个数的位置，也就是反向找第一个覆盖不到的位置
{  
    int l = 0, r = n;
    while (l < r) 
    {
        int mid = (l + r + 1) >> 1;
        if(days[mid] <= x) l = mid;
        else r = mid - 1;
    }
    return l;
}

int main()
{
    int nums;
    cin>>nums;
    while(nums--)
    {
        cin>>n;
        for(int i=1;i<=n;i++)cin>>days[i];
        for(int i=0;i<=2;i++)cin>>cost[i];
        
        //法一，O(30*n)~O(n),n<365;
        //暴力从 i 往前扫（最多30个），找第一个不被票覆盖的位置。
        
        // for(int i=1;i<=n;i++)//f[i]包含旅行前i天的方案中最低消费，最后一天为day[i]
        // //于是周票、月票都以i结尾往前推即可，完全没有必要包含i之后的，最优子结构f[i-k]同理，所以f[i-k]+cost[2]对
        // {
        //     f[i]=1e9;
        //     f[i]=min(f[i],f[i-1]+cost[0]);

        //         int k=0;
        //         for(int j=i;j>=1&&days[j]>=max(1,days[i]-6);j--)
        //             k++;
        //         f[i]=min(f[i],f[i-k]+cost[1]);
        //         //f[i-7]+cost[1]不对，
        //         //7日票，day[i]-6~days[i],已经包含进来了，设包含了k个，
        //         //应该等于在1~days[i]-7这段时间内旅行的总费用+cost[1];
        //         //f[i-k]+cost[1]

        //         k=0;
        //         for(int j=i;j>=1&&days[j]>=max(1,days[i]-29);j--)
        //             k++;
        //         f[i]=min(f[i],f[i-k]+cost[2]);            
        // }
 
        //法二：O(n)
        //基于days[i]序列，i、j指针移动的单调性，可以用双指针优化
            //j7：最后一个不被 7 日票覆盖的旅行日编号
            //j30：最后一个不被 30 日票覆盖的旅行日编号
        //在i从左往右扫的过程中，不同层的i的j7、j30只增不退
        
        //错的
        // for(int i=1,j7=0,j30=0;i<=n;i++)
        // {
        //     while(j7<n&&days[j7]+7<days[i])j7++;//不需要j7<n，且这样逻辑不对，会被days[0]干扰
        //     while(j30<n&&days[j30]+30<days[i])j30++;
        //     f[i]=min({f[i-1]+cost[0],f[j7]+cost[1],f[j30]+cost[2]});//
        // }
        
        // for(int i=1,j7=0,j30=0;i<=n;i++)
        // {
        //     while(days[j7+1]+7<=days[i])j7++;//判断条件为:如果移动之后满足，则移动j++，这样逻辑更对
        //     while(days[j30+1]+30<=days[i])j30++;
        //     f[i]=min({f[i-1]+cost[0],f[j7]+cost[1],f[j30]+cost[2]});//
        // }
          
        //法三:O(nlogn),此题差不多
        //基于days[i]序列值与下标的有序性（通常为值的单调性，但二分的本质是性质），可以使用二分查找最大的不被票覆盖的位置。
        
        for (int i = 1; i <= n; i ++ ) 
        {
            f[i] = f[i - 1] + cost[0];
            if (days[i] >= 7) f[i] = min(f[i], f[get(days[i] - 7)] + cost[1]);
            else f[i] = min(f[i], cost[1]);//一开始就买周卡
            if (days[i] >= 30) f[i] = min(f[i], f[get(days[i] - 30)] + cost[2]);
            else f[i] = min(f[i], cost[2]);
    
            // 这写法和上面一样，如果上面不加else就错了，因为下面这个写法考虑了直接一开始就买周卡或者月卡
            // int a = 0, b = 0;
            // if (days[i] >= 7) a = get(days[i] - 7);
            // if (days[i] >= 30) b = get(days[i] - 30);
            // f[i] = min(f[i], f[a] + cost[1]);
            // f[i] = min(f[i], f[b] + cost[2]);
        }     
        
        //总结：二分法vs双指针
        //在有序性序列中搜索某值，除了用暴力线性枚举，可能可以二分或双指针，可能都能用，可能只能用一种，因为基于的性质不同
        //双指针一般是用于n^2利用指针单调性优化到n,此题不明显，但需要有这个充分利用性质的优化思想
        cout<<f[n]<<endl;
    }
    
    return 0;
}