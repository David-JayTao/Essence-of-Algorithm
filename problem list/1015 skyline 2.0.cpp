#include <iostream>
#include <set>
#include <vector>
#include <algorithm>

using namespace std;

struct Event {
    int x;
    int h;
    bool operator<(const Event& t) const {
        if (x != t.x) return x < t.x; // x不同，按x坐标从小到大扫描
        return h > t.h;               // x相同，魔法降临：巧妙处理左左、右右、左右的重叠逻辑
    }
};
int main() {
    // 优化输入输出流性能（竞赛标配）
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    if (!(cin >> n)) return 0;

    vector<Event> events;
    // 预分配空间，避免vector频繁扩容（2n个事件）
    events.reserve(2 * n); 

    while (n--) {
        int a, b, h;
        cin >> a >> b >> h;
        // 使用emplace_back直接在末尾构造，代码更短，效率更高
        events.emplace_back(Event{a, h});
        events.emplace_back(Event{b, -h});
    }
    // 事件排序
    sort(events.begin(), events.end());

    multiset<int> st = {0}; // 高度池，0作为保底地面
    int prev_max = 0;       // 记录上一步的最高点

    // 状态驱动的高度管理
    for (const auto& event : events) {
        // 1. 更新高度池
        if (event.h > 0) {
            st.insert(event.h);
        } else {
            st.erase(st.find(-event.h)); 
        }

        // 2. 检查天际线是否发生变化
        int curr_max = *st.rbegin();
        if (curr_max != prev_max) {
            cout << event.x << " " << curr_max << "\n"; // 使用\n代替endl
            prev_max = curr_max;
        }
    }
    return 0;
}