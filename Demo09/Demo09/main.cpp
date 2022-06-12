//
//  main.cpp
//  Demo09
//
//  Created by 陈学明 on 2022/5/7.
//

#include <iostream>

class Transaction {

public:
    Transaction(const std::string &logInfo);
    // 版本1、2
//    virtual void logTransation() const;
    
    // 版本3
    void logTransation(const std::string &logInfo) const;
// 版本2
//    void init() {
//        logTransation();
//    }
};
// 版本1、2
//void Transaction::logTransation() const {
//    std::cout<<"Transaction";
//}

// 版本3
void Transaction::logTransation(const std::string &logInfo) const {
    std::cout<<logInfo;
}

// 版本1
//Transaction::Transaction() {
//    logTransation();
//}

// 版本2
//Transaction::Transaction(const std::string &logInfo) {
//    init();
//}

// 版本3
Transaction::Transaction(const std::string &logInfo) {
    logTransation(logInfo);
}


class BuyTransation:public Transaction {
    
    static std::string createLogString() {
        return "BuyTransation_Log";
    }
    
public:
    // 版本3
    BuyTransation():Transaction(createLogString()) {
        
    }
    
    // 版本1、2
    virtual void logTransation() const {
        std::cout<<"BuyTransation";
    }
    
    
};

class SellTransation:public Transaction {
    static std::string createLogString() {
        return "SellTransation_Log";
    }
public:
    // 版本3
    SellTransation():Transaction(createLogString()) {
        
    }
    
    virtual void logTransation() const {
        std::cout<<"SellTransation";
    }
};

int main(int argc, const char * argv[]) {
    BuyTransation b;
    return 0;
}
