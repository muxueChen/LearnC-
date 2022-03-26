//
//  main.cpp
//  VirtualFunction
//
//  Created by 陈学明 on 2022/1/1.
//

#include <iostream>
#include <memory>

using namespace std;

class Base {
    
public:
    Base(int a):a(a) {}
    int a{0};
};

class D1: public Base {
public:
    D1(int a, int b):Base(a),b(b){}
    int b{0};

};

class D2 : public D1 {
public:
    D2(int a, int b, int c):D1(a, b), c(c) {}
    int c{0};
    ~D2(){}
};

int main(int argc, const char * argv[]) {
    D2 *obj = new D2(1, 2, 3);
    cout<<"obj->a:"<<obj->a<<"\n";
    cout<<"obj->b:"<<obj->b<<"\n";
    cout<<"obj->c:"<<obj->c<<"\n";
    delete obj;
    
    return 0;
}
// 虚函数、继承、动态绑定、动态类型、纯虚函数、析构函数、构造函数、函数派发方式、内存管理，这些必须搞清楚。
