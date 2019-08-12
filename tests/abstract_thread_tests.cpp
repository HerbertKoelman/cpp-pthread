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
#include <chrono>

class test_thread : public pthread::abstract_thread {
public:

    void run() noexcept {

        try {
            long counter = 0;
            std::cout << std::flush << "Test thread is running..." << std::flush;
            pthread::this_thread::sleep_for(25 * 100);
            for (auto count = 1000; count > 0; count--) {
                counter += count;
            }
            std::cout << "Done" << std::endl << std::flush;
        } catch (std::exception &err) {
            std::cerr << "something went wrong while running test_thread. " << err.what() << std::endl << std::flush;
        }
    }
};

TEST(abstract_thread, constructor) {

    test_thread t;
    t.start();
    t.join();
}

TEST(abstract_thread, joinable) {

    test_thread t;
    t.start();
    if (t.joinable()) t.join();
}

TEST(abstract_thread, not_joinable) {

    test_thread t;
    t.start();
    pthread::this_thread::sleep_for(3 * 1000);

    EXPECT_TRUE(t.joinable());
    t.join(); // once joined, the thread is no more a thread and can not be joined again.
    EXPECT_FALSE(t.joinable());
}


TEST(absttract_thread, self_join) {

    class test_join_thread : public pthread::abstract_thread {
    public:

        void run() noexcept {
            EXPECT_THROW(join(), pthread::thread_exception); // this should not work, self joining
        }
    };

    test_join_thread T1;
    T1.start();
    pthread::this_thread::sleep_for(500);
    EXPECT_NO_THROW(T1.join());
}

TEST(abstract_thread_group, start_auto_join) {
    pthread::thread_group threads{true};

    for (auto x = 10; x > 0; x--) {
        threads.add(new test_thread{});
    }

    EXPECT_EQ(threads.size(), 10);

    threads.start();
}

TEST(abstract_thread_group, start_join) {
    pthread::thread_group threads;

    for (auto x = 10; x > 0; x--) {
        threads.add(new test_thread{});
    }

    EXPECT_EQ(threads.size(), 10);

    threads.start();
    threads.join();
}