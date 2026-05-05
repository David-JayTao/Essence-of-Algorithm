
//yxc 版本是 Hoare 双指针分区


//只有这两种取中点的方法不会超时，

// x=q[(r+l+1)/2]
// quick_sort(q,l,i-1);
// quick_sort(q,i,r);

// x=q[(r+l)/2]
// quick_sort(q,l,j);
// quick_sort(q,j+1,r);

#include<iostream>
using namespace std;

int n;
const int N=1e6+10;
int q[N];

void quick_sort(int*q,int l,int r)
{
    if(l>=r)return;
    //分治，以x=q[]的值为划分基准，左边的都小于该数，右边的都大于该数，如果左右各自也已排好，则全部就都排好咯，于是左右分开递归再次进行更细的划分
    int x=q[(r+l+1)/2],i=l-1,j=r+1;
    while(i<j)
    {
        do i++;while(q[i]<x);

        //@o@手误手误：注意应该是j--哈；do j++;while(q[j]>x);
        do j--;while(q[j]>x);

        //@o@错误：漏写交换条件：swap(q[i],q[j]);

        if(i<j)swap(q[i],q[j]);
    };
    quick_sort(q,l,i-1);
    quick_sort(q,i,r);

  //注意
    //以i分区间长度
    // quick_sort(q,l,i-1)
    // quick_sort(q,i,r);不能x=q[l]，不能x=q[(r+l)/2]死循环
    // i，从l左向右
    // (r+l+1)/2-->r;一半向上取整及以上,

    //以j分区间长度
    // quick_sort(q,l,j)
    // quick_sort(q,j+1,r);不能不能x=q[(r+l+1)/2],x=q[r]，死循环
    // j，从r右向左
    // l-->(r+l)/2;一半向下取整及以下
}

int main()
{
    scanf("%d",&n);

    for(int i=0;i<n;i++)scanf("%d",&q[i]);

    quick_sort(q,0,n-1);

    for(int i=0;i<n;i++)
    //@o@真逆天呀，少了一个空格："%d"！="%d "，printf("%d",q[i]);
    printf("%d ",q[i]);

    return 0;
}
