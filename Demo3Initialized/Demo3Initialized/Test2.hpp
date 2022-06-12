//
//  Test2.hpp
//  Demo3Initialized
//
//  Created by 陈学明 on 2022/4/23.
//

#ifndef Test2_hpp
#define Test2_hpp

#include <stdio.h>
#include "Test1.hpp"

class Directory {// 程序库2
    
public:
    Directory() {
        int disks = tfs().numDisks();
        // 。。。
    }
};

Directory &tempDir() {
    static Directory td;
    return td;
}

#endif /* Test2_hpp */
