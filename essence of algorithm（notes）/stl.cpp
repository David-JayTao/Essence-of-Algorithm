#include<iostream>
using namespace std;

#include<vector>
#include<string>
#include<queue>
#include<stack>
#include<deque>
#include<set>
#include<map>
#include<bitset>//整数直接转为二进制
int main()
{
vector<int> v(10,8);    
//vector,动态数组，倍增
    v.size();
    v.empty();
    v.clear();
    v.push_back(1);
    v.pop_back();
    v.back();
    v.front();
    //[]随机访问，复杂度O(1)
//按照字典序比较：逐个比较元素，直到遇到不同的元素为止，返回比较结果

    //迭代器本质为指针，故需要解引用
    //v.end()==v[v.size()];最后一个数的下一个位置，不能访问    
    for(vector<int>::iterator it=v.begin();it!=v.end();it++)
        cout<<*it<<" ";
    for(auto it=v.begin();it!=v.end();it++)
        cout<<*it<<" ";   
      
    for(auto item:v)
        cout<<item<<" ";
    
    cout<<endl;

pair<int,string> p1(1,"2a"),p2(1,"3a");
//本质是已定义的结构体，且自带比较，可以简化代码

//pair，一个东西有两个不同的属性，并按照某一种属性排序
//pair<int ,pair<int,string>> p1(1,{2,"lyx"});
//字典序比较，先比较第一个元素，若相同则比较第二个元素
    
    p1={10,"lyx"};
    p1=make_pair(12,"lyx");

string s;    
//string
    s.size();s.length();
    s.empty();
    s.clear();

    s="LYXIloveYou";
    cout<<s.substr(3,5)<<endl;//从下标3开始，长度为5的子串
    cout<<s.substr(3)<<endl;//从下标3开始，直到字符串末尾的子串

    printf("%s\n",s);
    //未定义行为，s为一个类，
    //%s 占位符非常“原始”，它只认识指向以 \0 结尾的字符数组的指针（也就是 char*）。
    printf("%s\n",s.c_str());//c_str()将string转换为C风格字符串，返回一个const char*指针

queue<int> q;
//queue，队列
    q.size();
    q.empty();
    //队列没有q.clear();，直接新建就行，q=queue<int>();
    q.push(1);
    q.pop();
    q.front();//注意不是top，语义不一样；
    //front前面
    //top顶部（stack，heap）
    q.back();

priority_queue<int> pq;
//priority_queue,堆，默认是大根堆
    pq.top();//这个是顶部
    pq.push(1);
    pq.pop();
    //想要小根堆
        //法一：黑科技
            //插入元素时，加负号，取出元素时再去掉负号
        //法二：定义法
            priority_queue<int,vector<int>,greater<int>>heap;

stack<int> st;  
//stack,栈，后进先出
    st.size();
    st.empty();
    //栈没有st.clear();，直接新建就行，st=stack<int>();
    st.push(1);
    st.pop();//
    st.top();//这个不是front

deque<int> deq;
//deque,加强版的vector双端队列，既可以在头部插入和删除，也可以在尾部插入和删除
//比较慢；可支持随机寻址[]  
    deq.size();
    deq.empty();
    deq.clear();
    deq.push_back(1); deq.pop_back();
    deq.push_front(2);deq.pop_front();
    deq.front();//类比queue
    deq.back();
    //[]


//核心差异：

//set,map,multiset,multimap,
//有序，增删改查为O(logn),基于自平衡二叉树（红黑树）

//unordered_set,unordered_map,unordered_multiset,unordered_multimap，
//无序，增删改查为O(1)（平均），底层实现为“哈希表”！（映射），  不支持lower_bound和upper_bound，不支持迭代器的++，--


//是弱平衡，不是AVL（严格平衡）

set<int> s1={1,7,3,4,5};
    //set,multiset,元素不重复，元素可重复
        s1.size();
        s1.empty();
        s1.clear();
        s1.begin();s1.end();//++ --；返回前驱和后继，O（logn）
        //set(元素不重复),multiset（元素可重复）
        s1.insert(1);
        s1.erase(1);
            //针对multiset才有区别
            //erase(元素值x)删除所有x，O(k+logn)
            //erase(迭代器)删除这个迭代器
        s1.find(1);//返回一个迭代器，指向元素1的位置，若不存在则返回s.end()
        s1.count(1);
        //返回元素1的个数，set中只能有一个元素，返回0或1；
        //multiset中可以有多个元素，返回0、1或更多

        //********set最核心的两个操作**********//
        s1.lower_bound(3);//返回大于等于x的最小的数的迭代器
        s1.upper_bound(3);//返回大于x的最小的数的迭代器
        //不存在返回s1.end()

map<int,string> m;
    //map,key不重复，一对一，映射，value可重复，支持[]
    //multimap,key可重复，一对多，value可重复，但让不支持[]，只能通过迭代器访问
multimap<int,string> mm; 
    m.insert({1,"lyx"});//插入一个键值对，一个pair<int,string>
    //类似数组访问
    m[1]="lyx";//O[logn]插入一个键值对，key为1，value为"lyx"，如果key已存在，则更新value
    m.erase(1);//删除key为1的键值对
    m.find(1);//返回一个迭代器，指向key为1的位置，若不存在则返回m.end()
    m.count(1);//返回key为1的键值对的个数，map
    mm.lower_bound(3);//返回大于等于x的最小的数的迭代器
    mm.upper_bound(3);//返回大于x的最小的数的迭代器

//unordered_set,unordered_map,unordered_multiset,unordered_multimap
    //和上面类似，但增删改查均变为O(1)（平均），底层实现为哈希表，
    //元素无序，不支持lower_bound和upper_bound，不支持迭代器的++，--




//bitset,压位，空间仅为1/8
//位集，适合存储大量的0和1，节省空间，常用于布尔数组
    bitset<10000> b; //定义一个大小为10000的位集
    b.set(3,0); //将下标为3的位置设置为0
    b.set(3); //将下标为3的位置设置为1
    b.reset(3); //将下标为3的位置设置为0
    b.flip(3); //将下标为3的位置取反
    b.test(3); //测试下标为3的位置是否为1，返回true或false
    b.count(); //返回位集中值为1的个数
    b.size(); //返回位集的大小，即10000
    b.any(); //测试位集中是否至少有一个值为1，返回true或false
    b.none(); //测试位集中是否所有值都为0，返回true或false
    b.to_string(); //将位集转换为字符串，返回一个由0和1组成的字符串，长度为位集的大小
    b.to_ulong(); //将位集转换为无符号长整数，返回一个unsigned long类型的数值，位集的大小不能超过unsigned long的位数，否则会抛出overflow_error异常
    b.to_ullong(); //将位集转换为无符号长长整数，返回一个unsigned long long类型的数值，位集的大小不能超过unsigned long long的位数，否则会抛出overflow_error异常
    b.reset(); //将位集的所有位置设置为0
    b.set(); //将位集的所有位置设置为1

    b.flip(); //将位集的所有位置取反//相当于~
    
    b[3]; //访问下标为3的位置，返回一个bool类型的值，表示该位置是否为1
    
    return 0;
}