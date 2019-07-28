//
// Created by Herbert Koelman on 2019-07-28.
//

#include <pthread.h>
#include "pthread/pthread.hpp"
#include "gtest/gtest.h"

#include <cstdio>
#include <iostream>
#include <string>
#include <memory>
#include <ctime>

class test_runnable : public pthread::runnable {
public:

    void run() noexcept {
        std::cout << "runnable is running..." << std::endl ;
        pthread::this_thread::sleep_for(2 * 1000);
        std::cout << "Done" << std::endl;
    }
};

class test_thread : public pthread::abstract_thread {
public:

    void run() noexcept {
        std::cout << "abstract test thread is running..." << std::endl ;
        pthread::this_thread::sleep_for(2 * 1000);
        std::cout << "Done" << std::endl;
    }
};

TEST(thread, join) {

    test_runnable tr;
    pthread::thread t{tr}; // this starts running the thread

    EXPECT_TRUE(t.joinable());
    t.join();
}

TEST(thread, status) {

    test_runnable tr;
    pthread::thread t{tr}; // this starts running the thread

    EXPECT_EQ(t.status(), pthread::thread_status::a_thread);
}

TEST(thread, DISABLED_move_operator) {

    test_runnable tr;
    pthread::thread t1{tr}; // this starts running the thread
    pthread::thread t2 ;
    t2 = dynamic_cast<pthread::thread &&>(t1);
    EXPECT_EQ(t1.status(), pthread::thread_status::not_a_thread);
}

TEST(abstract_thread, join) {

    test_thread t;

    t.start();
    //pthread::this_thread::sleep_for(700);
    //EXPECT_TRUE(t.joinable());
    t.join();
}

TEST(thread_group, start_join){
    pthread::thread_group threads(true); // indicate that we want to join referenced threads when deallocating this instance.

    EXPECT_EQ(threads.size(), 0);

    for (auto x = 10 ; x > 0 ; x--){
        threads.add( new test_thread{});
    }

    EXPECT_EQ(threads.size(), 10);

    threads.start();
    threads.join();

    EXPECT_EQ(threads.size(), 10);
}