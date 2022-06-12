//
//  main.cpp
//  Demo28
//
//  Created by 陈学明 on 2022/6/12.
//

#include <iostream>

class Point {
public:
    Point(int _x, int _y):x(_x), y(_y) {}
    
    int x{0};
    int y{0};
};

struct RectData {
    Point ulhc;// 左上角
    Point lrhc;// 右下角
};

class Rectangle {

public:
    const Point &upperLeft() const {
        return pData->lrhc;
    }
    const Point &lowerRight() const {return pData->ulhc;}
private:
    std::shared_ptr<RectData> pData;
};

class GUIObject {};
const Rectangle boundingBox(const GUIObject & obj);




int main(int argc, const char * argv[]) {
    // insert code here...
    std::cout << "Hello, World!\n";
    Rectangle rect;
    const Point &p = rect.upperLeft();
    
    GUIObject *pgo;//
    const Point *pUpperLeft = &(boundingBox(*pgo).upperLeft());
//    p.x = 5;
    return 0;
}
