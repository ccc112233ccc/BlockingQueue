# BlockingQueue


## Description

一个阻塞队列，支持生产者消费者模型，支持阻塞等待队列空。
队列的实现基于std::mutex和std::condition_variable，使用了C++11的特性。
队列底层使用了std::deque作为存储容器，支持动态扩容。

## Usage

```cpp
#include "BlockingQueue.h"

int main() {
    ccutil::BlockingQueue<int> queue;
    queue.push(1);
    int val = queue.pop();
    return 0;
}
```