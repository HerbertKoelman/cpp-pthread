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
    if ( t.joinable() ) t.join();
}

TEST(abstract_thread, not_joinable) {

    test_thread t;
    t.start();
    pthread::this_thread::sleep_for(3 * 1000);

    EXPECT_TRUE( t.joinable() );
    t.join(); // once joined, the thread is no more a thread and can not be joined again.
    EXPECT_FALSE( t.joinable() );
}

TEST(abstract_thread_group, start_join) {
    try {
        pthread::thread_group threads(true);

        for (auto x = 10; x > 0; x--) {
            threads.add(new test_thread{});
        }

        threads.start();
        threads.join();

    } catch (pthread::pthread_exception &err) {
        std::cerr << "thread_group test case failed. " << err.what() << std::endl << std::flush;
        GTEST_FAIL();
    } catch (...) {
        std::cerr << "thread_group test case failed. Unexpected eexception catched." << std::endl << std::flush;
        GTEST_FAIL();
    }
}