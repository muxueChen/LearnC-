//
//  ConstDemo.cpp
//  Demo1
//
//  Created by 陈学明 on 2022/4/23.
//

#include "ConstDemo.hpp"
#include <string>
/*
const char * const authorName = "Scott Meyers";
const std::string authorName("Scott Meyers");
 */

/// #defines to const,issu
class ConstDemo {
    
private:
    static const int NumTurns = 5;// 常量声明
    int scores[NumTurns];// 使用该常量
    
};
