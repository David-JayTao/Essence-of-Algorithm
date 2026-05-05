//以第一个元素为基准的单向扫描分区，接近 Lomuto 
#include <algorithm>
#include<iostream>
using namespace std;

int res[1000];
int a[1000];
int n;
//第1个元素a[low]作为划分基准, 进行一次扫描实现划分
//a[low]
// a[low+1...p]    < pivot,
// a[p+1...high]   >= pivot

// swap(a[low], a[p]);

// a[low...p-1]    < pivot
// a[p]            pivot
// a[p+1...high]   >= pivot

//a[low]<->a[p],已经归位
// quicksort(low, p - 1, depth + 1);
// quicksort(p + 1, high, depth + 1);

void quicksort(int low, int high, int depth) 
{
    if(low>=high)return;
    
    int pivot=a[low];
    int p=low;//指向<pivot的这一串的最后一个数
    for(int i=low;i<=high;i++)
    {
        if(a[i]<pivot)
        {
            p++;
            swap(a[i],a[p]);          
        }
    }
    swap(a[low],a[p]);
    
    if(depth==1)
        res[p]=a[p];
    
    if(depth==2)
        for(int i=low;i<=high;i++)
            res[i]=a[i];
    
    quicksort(low,p-1,depth+1);
    quicksort(p+1,high,depth+1);
}


int main() {
    
    int m;
    scanf("%d", &m);
    
    while(m--) 
    {
        cin>>n;
        for(int i=0;i<n;i++)cin>>a[i];
        quicksort(0,n-1,1);
        for(int i=0;i<n;i++)cout<<res[i]<<" ";
        cout<<endl;
    }
    
    return 0;
}
