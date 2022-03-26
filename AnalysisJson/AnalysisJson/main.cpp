//
//  main.cpp
//  AnalysisJson
//
//  Created by 陈学明 on 2022/3/22.
//

#include <iostream>
#include "leptjson.h"

// 从零开始的 JSON 库教程
int main(int argc, const char * argv[]) {
    // insert code here...
    std::cout << "Hello, World!\n";
    lept_value value;
    const char json[] = "";
    int ret = lept_parse(&value, json);
    return 0;
}
