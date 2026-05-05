#include<iostream>
#include<set>
#include<vector>
#include<algorithm>
using namespace std;

typedef pair<int ,int> PII;

const int N=5e4+10;
struct Event
{
    int x;
    int h;
    // bool operator<(const Event& t)
    // {
    //     if(x<t.x)return true;
        
    //     if(x==t.x)//两者x坐标重合，上上，下下，上下
    //     {
    //         if(h>t.h&&t.h>0)return true;
    //         else if(-h<-t.h&&h<0&&t.h<0)return true;
            
    //         if(h>0&&t.h<0)return true;

    //         //魔法，等价于h>t.h;哉乎者也！！！！！！！！！
    //     }

    //     return false;
    // }

    //释放魔法！！
    bool operator<(const Event& t) const
    {
        if (x != t.x) return x < t.x;
        return h > t.h; // 大道至简：完美处理上述三种重合情况
    }
};
int main()
{   //n个建筑，建筑轮廓上升或下降共2n个事件
    //multiset维护一个高度池，每个上升事件均放进去，
        //自动排序，最后一个元素，rbegin()即为当前最大高度
        //可重复
    //vector维护一个事件池，用于从左至右扫描遍历所有事件
    //从左往右，按照x坐标依次扫描所有事件，故需要先将2n个事件排序，
        //遇到上升事件，则加入高度池
            //（严格大于最大高度则cout）
        //遇到下降事件，用负号表示，则在当前的高度池中删除对应的上升事件
            //（如果最大高度改变了，则cout）
    
    //vector<PII>event;
    
    vector<Event>events;
    multiset<int> st={0};
    //multiset,别把l写漏了
    //multiset,元素可重复，有序，底层实现为红黑树，增删改查为O(logn)，支持lower_bound和upper_bound
   
    int n;
    cin>>n;
    while(n--)
    {
        int a,b,h;
        //共2n个事件
        cin>>a>>b>>h;//这行老是要忘哈哈哈哈哈
        Event evea={a,h},eveb={b,-h};
        events.push_back(evea);
        events.push_back(eveb);
    }
    sort(events.begin(),events.end());
    //事件池初始化完毕



    // //开始根据事件执行进行高度池的动态管理
    // for(auto event:events)
    // {
    //     if(event.h>0)
    //     {
    //         //pay attention,这个上升输出必须要先判断，若是最大的，先加入再判断即为自己和自己比，永不输出
    //         if(event.h>*st.rbegin())cout<<event.x<<" "<<event.h<<endl;           
           
    //         st.insert(event.h);
    //     //此处insert不需要管h对应的x（id），仅需表示扫描到当前位置时，前面所有的高度是什么情况即可
        
    //     //故高度池不需要维护一堆结构体事件，其实也可以，这样的话删除的时候可以指定某id的h，但是multiset又有排序规则，
    //     //好像理论上也行？
    //         //可行，但杀鸡用牛刀了嘿嘿嘿

    //         //if(event.h>*st.rbegin())cout<<event.x<<" "<<event.h<<endl;
    //     }
    //     else //下降事件
    //     {
    //         st.erase(st.find(-event.h));
    //         //很关键哎，下降事件的h为负数，需要添一个符号才是要删的上升事件
    //         if(*st.rbegin()<-event.h)
    //             cout<<event.x<<" "<<*st.rbegin()<<endl;
    //     }
    // }

    //代码更优雅：上升输出下降输出本质相同：天际线发生变化

    int prev_max = 0; // 记录前一个最大高度
    for (const auto& ev : events) //const auto&：在 for 循环中使用常量引用，避免不必要的拷贝
    {
        // 1. 无脑更新高度池
        if (ev.h > 0) st.insert(ev.h);
        else st.erase(st.find(-ev.h));

        // 2. 状态对比：只要当前最大高度和之前不一样，就说明产生了天际线拐点！
        int curr_max = *st.rbegin();
        if (curr_max != prev_max) 
        {
            cout << ev.x << " " << curr_max << "\n";
            prev_max = curr_max; // 更新状态
        }
    }





    return 0;
}