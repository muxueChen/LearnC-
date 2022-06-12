//
//  main.cpp
//  Demo4DeclareVirtual
//
//  Created by 陈学明 on 2022/4/23.
//

#include <iostream>

class TimeKeeper {
    
public:
    TimeKeeper();
    virtual ~TimeKeeper();
};


class AtomicClock: public TimeKeeper {};// 原子钟
class WaterClock: public TimeKeeper {};// 水钟
class WristClock: public TimeKeeper {};// 腕表

TimeKeeper *getTimeKeeper();


class Point {
public:
    Point(int _x, int _y):x(_x), y(_y){};
private:
    int x, y;
};

int main(int argc, const char * argv[]) {
    // insert code here...
    std::cout << "Hello, World!\n";
    return 0;
}
