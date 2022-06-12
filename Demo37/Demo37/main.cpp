//
//  main.cpp
//  Demo37
//
//  Created by 陈学明 on 2022/6/12.
//

#include <iostream>

class Shape {
public:
    enum ShapeColor {Red, Green, Blue};
    void draw(ShapeColor color = Red) {
        doDraw(color);
    }
private:
    virtual void doDraw(ShapeColor color) {// 真正实现
        
    }
};

class Rectangle:Shape {
    
public:
    //...
private:
    void doDraw(ShapeColor color) override {// 不需要制定缺省参数
        //。。。
    }
};

int main(int argc, const char * argv[]) {
    // insert code here...
    std::cout << "Hello, World!\n";
    return 0;
}
