//
//  Test1.hpp
//  Demo3Initialized
//
//  Created by 陈学明 on 2022/4/23.
//

#ifndef Test1_hpp
#define Test1_hpp

#include <stdio.h>


class FileSystem {// 来自程序库1
    //。。。
public:
    //。。。
    int numDisks() const;
};

FileSystem & tfs() {
    static FileSystem fs;
    return fs;
}
extern FileSystem tfs;// 准备给其他对象使用的对象，tfs 代表 “the file system”
#endif /* Test1_hpp */
