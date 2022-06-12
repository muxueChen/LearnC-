//
//  main.cpp
//  Demo3Initialized
//
//  Created by 陈学明 on 2022/4/23.
//

#include <iostream>
#include <list>
#include "Test2.hpp"

class Point {
    int x, y;
    
public:
    
};
Point p;

class PhoneNumber {
    
public:
    
};

class ABEntry {
    std::string theName;
    std::string theAddress;
    std::list<PhoneNumber> thePhones;
    int numTimesConsulted;
    
public:
    ABEntry(const std::string &name, const std::string &address, const std::list<PhoneNumber> &phones);
};

ABEntry::ABEntry(const std::string &name, const std::string &address, const std::list<PhoneNumber> &phones):theName(name), theAddress(address), thePhones(phones),numTimesConsulted(0){}

int main(int argc, const char * argv[]) {
    // insert code here...
    int x = 0;// 手动进行初始化
    const char *text = "A c-style string";//对指针进行手动初始化
    double d;//
    std::cin>>d;// 以读取input stream 的方式完成初始化
    
    Directory tempDir();
    return 0;
}
