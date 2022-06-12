//
//  main.cpp
//  Demo2-Const
//
//  Created by 陈学明 on 2022/4/23.
//

#include <iostream>
#include <vector>


class Demo {
public:
    int nonConstFunction() {
        return num;
    }
private:
    const static int num = 10;
};

class TextBlook {
    
public:
    TextBlook(std::string _text) {
        text = _text;
    }
    const char & operator[](std::size_t position) const {
        return text[position];
    }
    
    char & operator[](std::size_t position) {
        return text[position];
    }
private:
    std::string text;
};

int main(int argc, const char * argv[]) {
    // insert code here...
//    char greeting[] = "Hello";// non-const pointer, non-const data
//    char *p = greeting;// non-const pointer, non-const data
//    const char *p1 = greeting;// non-const pointer, const data
//    char *const p2 = greeting;// const pointer, non-const data
//    const char *const p3 = greeting;// const pointer, const data
    
//    std::vector<int> vec;
//    const std::vector<int>::iterator iter =vec.begin(); // iter 的作用像个 T*const
//    *iter = 10;// 没问题，改变 const 所指物
    
//    ++iter;// 错误，iter 是 const
    
//    std::vector<int>::const_iterator cIter = vec.begin();
//    *cIter = 10;// 错误， *cIter 是const
//    ++cIter;// 没问题，改变 cIter
    TextBlook tb("Hello");
    std::cout<<tb[0];// 调用 TextBlook::operator[]
    tb[0] = 'x';// 正确
    
    const TextBlook ctb("World");
    std::cout<<ctb[0];// 调用 const TextBlook::operator[]
    ctb[0] = 'x';// 错误 写一个 const TextBlock
    return 0;
}
