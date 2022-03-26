//
//  main.cpp
//  Inherit
//
//  Created by 陈学明 on 2022/1/9.
//

#include <iostream>
#include <string>

using namespace std;
// 杯子
class Glass {
    
public:
    // 容量
    float capacity;
    
    Glass(float capacity):capacity(capacity) {
        cout<<"Glass()"<<endl;
    }
    ~Glass() {
        cout<<"~Glass"<<endl;
    }
};

// 纸杯
class DixieGlass:public Glass {
    
public:
    float recession;
    DixieGlass(float recession, float capacity):Glass(capacity), recession(recession) {
        cout<<"DixieGlass()"<<endl;
    }
    ~DixieGlass(){
        cout<<"~DixieGlass()"<<endl;
    }
};

// 动物
class Animal {
    
    
public:
    float weight;
    void run() {
        
    }
    virtual ~Animal() {
        cout<<this.type()<<endl;
    }
};

class Dog {
public:
};


// 1. 当初始化子类时会先初始化父类；
// 2. 当销毁一个对象时，会调用当前对象静态类型的析构函数，在调用当前对象静态类型父类型的析构函数
int main(int argc, const char * argv[]) {
    DixieGlass *dg =new DixieGlass(1.0, 2.00);
    delete dg;
    
    Dog *dog = new Dog();
    delete dog;
    return 0;
}
