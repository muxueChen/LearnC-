//
//  main.cpp
//  BasicType
//
//  Created by 陈学明 on 2021/12/4.
//

#include <iostream>

using namespace std;

typedef double wages;

typedef struct {
    string name;
    int age;
    double height;
    double weide;
    
} Student;


int main(int argc, const char * argv[]) {
    // insert code here...
    bool b = 42;
    cout << b << "\n";
    int i = b;
    cout << i << "\n";
    i = 3.14;
    cout << i << "\n";
    double pi = i;
    cout << pi << "\n";
    unsigned char c = -1;
    cout << "c:"<< int(c) << "\n";
    signed char c2 = 256;
    cout << int(c2) << "\n";
    int catch_22 = 0.0;
    wages item = 3.0;
    
    auto  a = item;
    //  位运算
    // << 左移
    // >> 右移
    // & 两者为1，结果为1
    // ｜ 两者只要有一个为1 ，结果为 1
    // ^ 两者不同结果为1，两者相同结果为0
//    std::cout << "Hello, World!\n";
    return 0;
}
