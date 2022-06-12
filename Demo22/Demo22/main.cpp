//
//  main.cpp
//  Demo22
//
//  Created by 陈学明 on 2022/6/5.
//

#include <iostream>

class AccessLevels {
public:
    int getReadOnly() const {return readOnly;}
    void setReadWrite(int value) {readWrite = value;}
    int getReadWirte() const {return  readWrite;}
    void setWriteOnly(int value) {writeOnly = value;}
private:
    int noAccess;//外部不允许访问
    int readOnly;// 外部只读权限
    int readWrite;// 允许外部读写权限
    int writeOnly;// 外部只写权限
};

class Person {
public:
    void setAge(int value) { age = value;}
    int getAge() const {return age;};
private:
    int age;
};

int main(int argc, const char * argv[]) {
    // insert code here...
    std::cout << "Hello, World!\n";
    return 0;
}
