//
//  main.cpp
//  Demo10
//
//  Created by 陈学明 on 2022/5/8.
//

#include <iostream>


class Widget {
    
public:
    Widget& operator=(const Widget& rhs) {// 返回类型是个 “引用”，指向当前对象
        //...其他操作
        return *this;// 返回左侧对象
    }
    
};

int main(int argc, const char * argv[]) {
    // insert code here...
    std::cout << "Hello, World!\n";
    return 0;
}
