#include<iostream>
#include <unordered_map>
using namespace std;

const int N=5e4+10;
int q[N];
int n,x;

void easy_sol()
{
    for(int i=0;i<n;i++)
    {
        for(int j=i+1;j<n;j++)
        {
            if(q[i]+q[j]==x)
            {
                cout<<"yes\n";
                return;
            }        
        }
    }
    cout<<"no\n";
}
void hash_sol()
{
    //在for外部创建全局哈希表复杂度O（1）,ordered_map为红黑树，复杂度O(logn)
    unordered_map<int,int> num_map;
    for(int i=0;i<n;i++)
    {
        //unordered_map<int,int> num_map;
            if(num_map.find(x-q[i])!=num_map.end())
            {
                cout<<"yes\n";
                return;
            }      
            num_map[q[i]]=1;  
    }
    cout<<"no\n";
}
int main()
{
    int m;
    cin>>m;
    while(m--)
    { 
        cin>>n>>x;
        for(int i=0;i<n;i++)scanf("%d",&q[i]);

        //easy_sol(); 
        hash_sol();
    }

    return 0;
}