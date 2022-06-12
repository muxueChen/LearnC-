//
//  main.cpp
//  Demo1
//
//  Created by 陈学明 on 2022/4/23.
//

#include <iostream>

// 问题二
#define CALL_WITH_MAX(a, b) f((a)>(b)?(a):(b))
void f(int a) {
    
}

template <typename T>
inline void callWithMax(const T& a, const T& b) {
    f( a > b ? a : b);
}

int main(int argc, const char * argv[]) {
    // insert code here...
    int a =5, b =0;
    CALL_WITH_MAX(++a, b);
    CALL_WITH_MAX(++a, b+10);
    std::cout << "Hello, World!\n";
    return 0;
}
