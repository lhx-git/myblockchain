
//
// Created by lhx on 2/8/22.
//

#include "common/concurrent_queue/blocking_concurrent_queue.hpp"
#include "glog/logging.h"

//class A {
//public:
//    A() : b(3) {
//        DLOG(INFO) << "调用了a的默认构造函数\n";
//    }
//    ~A() {
//        DLOG(INFO) << "调用了a的析构函数函数\n";
//    }
//    int b;
//};
//
//class B {
//public:
//    A a;
//    //A *a; 如果a是非指针类型的话, B对象在分配内存时,会调用A的默认构造,为a对象分配内存.
//            //然后B对象析构时,会调用A的析构函数
//    std::vector<int> vector;
//};



int main() {
//    //一个类对象,使用new关键字为其分配内存, 然后其内部对象是否需要使用new关键字为其分配内存,使用new与不使用new的区别是什么
//    B *b = new B();
//    //此时B中的a对象仍为分配内存空间, 但是vector已经分配了内存空间
//    DLOG(INFO) << sizeof(b) << std::endl;
//    for (int i = 0; i < 10000; i++) {
//        b->vector.push_back(i);
//    }
//    DLOG(INFO) << sizeof(b) << std::endl;
//    delete b;


//    moodycamel::BlockingConcurrentQueue<std::string*> q;
//    std::thread consumer1([&]() {
//        for (int i = 0; i < 200; i++) {
//            std::string* item = nullptr;
//            //q.wait_dequeue(item);
//            bool res = q.wait_dequeue_timed(item, std::chrono::milliseconds(1000));
//            if (res) {
//                DLOG(INFO) << std::this_thread::get_id() << "consume item : " << *item << std::endl;
//                delete item;
//            } else {
//                DLOG(INFO) << "nothing to deque" << std::endl;
//            }
//
//        }
//    });
//
//    std::thread consumer2([&]() {
//        for (int i = 0; i < 200; i++) {
//            std::string* item = nullptr;
//            //q.wait_dequeue(item);
//            bool res = q.wait_dequeue_timed(item, std::chrono::milliseconds(1000));
//            if (res) {
//                DLOG(INFO) << std::this_thread::get_id() << "consume item : " << *item << std::endl;
//                delete item;
//            } else {
//                DLOG(INFO) << "nothing to deque" << std::endl;
//            }
//        }
//    });

    std::string a = "123456";
    const char* c = a.c_str();//返回一个null中止的内部数据
    a.data(); //返回内部数据
    size_t pos;

    if ((pos = a.find("12", 0, 6)) != std::string::npos) {
        DLOG(INFO) << pos << std::endl; // 0
    }


    std::string sub = a.substr(2, 3); //345
    DLOG(INFO) << sub << std::endl;

    a.replace(1, 3, "nidaye"); //1nidaye56
    DLOG(INFO) << a << std::endl;

    a.insert(1, "213"); // 1213nidaye56
    DLOG(INFO) << a << std::endl;

    a.erase(1, 3); //1nidaye56
    DLOG(INFO) << a << std::endl;

    a += "211";
    //a.append("211"); //1nidaye56211
    DLOG(INFO) << a << std::endl;

    a.resize(20);
    DLOG(INFO) << a << std::endl; //1nidaye562110000

    a.resize(3);
    DLOG(INFO) << a << std::endl; //1ni

    a.clear();
    DLOG(INFO) << a << std::endl;  //



//    std::thread producer([&]() {
//        std::this_thread::sleep_for(std::chrono::milliseconds(100));
//        for (int i = 0; i < 200; i++) {
//            std::string* item = new std::string(std::to_string(i));
//            q.try_enqueue(item);
//        }
//    });
//
//    std::thread producer2([&]() {
//        std::this_thread::sleep_for(std::chrono::milliseconds(100));
//        for (int i = 200; i < 400; i++) {
//            std::string* item = new std::string(std::to_string(i));
//            q.try_enqueue(item);
//        }
//    });
//
//    consumer1.join();
//    consumer2.join();
//    producer.join(); //让主线程等待 producer线程执行完成在继续执行
//    producer2.join();
    return 0;
}