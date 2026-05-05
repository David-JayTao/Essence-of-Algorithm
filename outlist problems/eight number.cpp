#include<iostream>
#include<string>
#include<queue>
#include<unordered_map>
using namespace std;
//将一个状态抽象为一个点，从一个状态经过一步变为另一状态，设置步长为1，直到到达终点状态为止
//等权图求最短路径，BFS宽搜

//队列里要存储每一个状态，每次据此状态迅速能得到移动后的下一状态
//用一个字符串存储，

unordered_map<string,int>dist;

int bfs(string start)
{
    queue<string> q;
    q.push(start);
    //需要存储每一个状态（string）对应距离起始状态的距离，
    //每次从队列弹出装入相邻状态时需要dist[邻]=distance+1
    //用一个哈希表：dist<string,int>
    //每个状态只存最开始出现的那一次，dist.count(t),可以判断t状态是否出现过
    dist[start]=0;

    int dx[4]={-1,1,0,0},dy[4]={0,0,-1,1};
    //四个方向

    //t="12345678x"

    //队列换成栈就变成了DFS
    while(q.size())//此队列会遍历完所有从t出发能到达的状态
    {
        string t=q.front();//注意，queue.front,没有top一说
        q.pop();
//终止条件
        //如果到达终点状态则停止
        if(t=="12345678x")return dist[t];

        int distance=dist[t];
//状态转移
        //拿到t状态中x对应原图的位置[0~2][0~2]
        int k=t.find('x');
        int tx=k/3,ty=k%3;
        for(int i=0;i<4;i++)
        {
            //x移动之后的原图中的位置
            int nx=tx+dx[i],ny=ty+dy[i];

            //将这个新位置的数与之前的x的位置交换（如果没有越界）
            if(nx>=0&&nx<=2&&ny>=0&&ny<=2)
            {
                //据此得出新的状态
                swap(t[k],t[3*nx+ny]);

                //让新的未进入过的相邻状态“入队”，如果此状态在之前已通过更少的步数到达了，那么后面再到就不用管了
                //可以用一个st哈希表映射该字符串是否进来过，但是不用
                //直接复用dist是否出现过t即可


                if(!dist.count(t))
                {
                    dist[t]=distance+1;//这是为什么之前的distance必须保留
                    q.push(t);//这个别忘了嗷宝贝
                }  

                //换回去继续进行下一个方向的遍历，因为这个新的状态已经用哈希表存储进去了，不用再保留新的t了
                 swap(t[k],t[3*nx+ny]);               
            }
        }
    }
    return -1;
}

int main()
{
    string start;
    for(int i=0;i<9;i++)
    {
        char c;
        cin>>c;
        start+=c;
    }

    cout<<bfs(start);

    return 0;;

}