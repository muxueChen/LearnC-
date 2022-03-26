//
//  main.cpp
//  DataProces
//
//  Created by 陈学明 on 2022/3/5.
//

#include <iostream>
#include <sstream>

using namespace std;

int main(int argc, const char * argv[]) {
    // insert code here...
    std::cout << "Hello, World!\n";
    bool b = 42;// 为真
    int i = b;// 值为1
    i = 3.14;// i=3
    double pi = i;// pi=3.0
    unsigned char c = -1;// 假设char占8bit，c的值为255；
    printf("c:%d\n", c);
    signed char c2 = 256;// 假设char占8bit，c的值是0；
    printf("c2:%d\n", c2);
    unsigned char ucode1 = 0x4f60;
    unsigned char ucode2 = 0x597d;
    unsigned char ucode3 = 0x4e16;
    unsigned char ucode4 = 0x754c;
//    stringstream stream;
//    stream<<ucode1;//向流中传值
//    stream<<ucode2;//向流中传值
//    stream<<ucode3;//向流中传值
//    stream<<ucode4;//向流中传值
    string result = "";
    result += to_string(ucode1);
    result += to_string(ucode2);
    result += to_string(ucode3);
    result += to_string(ucode4);
    printf ("%lc\n",ucode1);
    printf ("%lc\n",ucode2);
    printf ("%lc\n",ucode3);
    printf ("%lc\n",ucode4);
    
//    std::cout << "result:"<<result;
    return 0;
}
