#include<iostream>
#include<algorithm>
using namespace std;

const int N=1e5+10,INF=1e9+10;
int h[N],s;
int n;

// void down(int i)
// {
//     ///把节点i沉下去！
    
//     //if(i>n/2)return;//写错了，n!=s
//     if(i>s/2)return;
//     //必须要有这行，不只是说1~n/2才需要往下down
//     //更是定义了递归终止条件，一层一层往下沉，i沉到哪个地方为止
//     //idx>n/2表示已经沉到底了
//     //自己在根节点，左孩子，右孩子；三者最小的调整到根结点处
//     int t,idx;
    
//     if(2*i+1>s)h[2*i+1]=INF;
    
//     t=min(h[2*i],h[2*i+1]);
//     idx=(t==h[2*i]?2*i:2*i+1);
//     //有个问题，可能只有左孩子，没有右孩子
//     if(h[i]>t)//大了就往下沉，把最小的浮上去
//     {
//         swap(h[i],h[idx]);
//         down(idx);//i沉到了idx处
//     }

// }

 //有个更优雅的写法,随时判断所有孩子是否存在，没有了递归自然就终止了
void down(int i)
{
    int t=i;
    if(2*i<=s&&h[2*i]<h[i])t=2*i;
    if(2*i+1<=s&&h[2*i+1]<h[t])t=2*i+1;
    if(t!=i)
    {
        swap(h[t],h[i]);
        down(t);
    }
}
int main()
{

    int m;
    cin>>m;
    while(m--)
    {
        //此处读入整型数组不能偷懒，读字符串可以偷懒（字符数组char a[N]）scanf("%d%d%s",&n,&m,a+1);
        scanf("%d",&n);
        for(int i=1;i<=n;i++)cin>>h[i];
        
        //存储从编号1开始，1~n,i,2i为左子，2i+1为右子，i/2为父
        s=n;
        
        //最小堆
        //建堆方式1，依次插入n个数到末尾，再up，O(nlogn);
        //建堆方式2，O(n),先直接所有数读入，从n/2开始down直至1（最后一层节点本来就只有一个点，不用再down了）
        for(int i=n/2;i;i--)down(i);
        
        for(int i=1;i<=n;i++)cout<<h[i]<<" ";
            cout<<endl;
    }
    return 0;
}