//
//  main.cpp
//  Demo20
//
//  Created by 陈学明 on 2022/5/8.
//

#include <iostream>

class Person {
    std::string name;
    std::string address;
public:
    Person();
    virtual ~Person() {}

};


class Student:public Person {
    std::string schoolName;
    std::string schoolAddress;
public:
    Student();
    ~Student(){}
};


// 对象分割问题

class Window {

public:
    std::string name() const;
    virtual void dispaly() const;
};


class WindowWithScrollBars:public Window {
    
public:
    virtual void display() const;
};

//void printNameDisplay(Window w) {
//    std::cout<<w.name();
//    w.dispaly();
//}

void printNameDisplay(const Window &w) {
    std::cout<<w.name();
    w.dispaly();
}


WindowWithScrollBars wwsb;
printNameDisplay(wwsb);


// 版本1
//bool validateStudent(Student s);

// 版本2
bool validateStudent(const Student &s);

Student plato;
bool platoIsOK = validateStudent(plato);

int main(int argc, const char * argv[]) {
    // insert code here...
    std::cout << "Hello, World!\n";
    return 0;
}
