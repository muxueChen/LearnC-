//
//  main.cpp
//  Demo36
//
//  Created by 陈学明 on 2022/6/12.
//

#include <iostream>

class B {
    
public:
    void mf();
};

class D: public B {
public:
    void mf();
};


int main(int argc, const char * argv[]) {
    
    // 虽然我们队 B、D 和 mf一无所知，但面对一个类型为D的对象x
    D x;
    B *pB = &x;
    pB->mf();
    
    D *pD = &x;
    pD->mf();
    return 0;
}
