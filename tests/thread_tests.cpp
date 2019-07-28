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
        try {
            long counter = 0;
            //std::cout << "runnable is running..." << std::endl;
            //pthread::this_thread::sleep_for(1 * 1000);
            for (auto count = 1000; count > 0; count--) {
                counter += count;
            }
            //std::cout << "Done" << std::endl;
        } catch (std::exception &err) {
            std::cerr << "something went wrong while running test_runnable. " << err.what() << std::endl;
        }
    }

};

class test_thread : public pthread::abstract_thread {
public:

    void run() noexcept {
        try {
            long counter = 0;
            // std::cout << "abstract test thread is running..." << std::endl;
            //pthread::this_thread::sleep_for(1 * 1000);
            for (auto count = 1000; count > 0; count--) {
                counter += count;
            }
            //std::cout << "Done" << std::endl;
        } catch (std::exception &err) {
            std::cerr << "something went wrong while running test_thread. " << err.what() << std::endl;
        }
    }
};

TEST(thread, join) {

    try {
        test_runnable tr;
        pthread::thread t{tr}; // this starts running the thread

        EXPECT_TRUE(t.joinable());
        t.join();
    } catch (std::exception &err) {
        std::cerr << "something went wrong when canceling a thread. " << err.what() << std::endl;
        FAIL();
    }

    SUCCEED();
}

TEST(thread, status) {

    test_runnable tr;
    pthread::thread t{tr}; // this starts running the thread

    EXPECT_EQ(t.status(), pthread::thread_status::a_thread);
}

TEST(thread, cancel) {

    try {
        test_runnable tr;
        pthread::thread t{tr}; // this starts running the thread

        //pthread::this_thread::sleep_for(1000); // let the thread start up

        //std::cout << "Canceling..." << std::endl << std::flush ;

        t.cancel();
    } catch (pthread::thread_exception &err) {
        std::cerr << "something went wrong when canceling a thread. " << std::flush ;
        std::cerr << err.what() << std::endl << std::flush;
        FAIL();
    } catch ( ... ){
        std::cerr << "unexpected exception...." << std::endl << std::flush;
        FAIL();
    }

    SUCCEED();
}

TEST(thread, DISABLED_move_operator) {

    test_runnable tr;
    pthread::thread t1{tr}; // this starts running the thread
    pthread::thread t2;
    t2 = dynamic_cast<pthread::thread &&>(t1);
    EXPECT_EQ(t1.status(), pthread::thread_status::not_a_thread);
}

TEST(abstract_thread, DISABLED_join) {

    test_thread t;

    t.start();
    t.join();
}

TEST(thread_group, DISABLED_start_join) {
    try {
        pthread::thread_group threads(true);

        for (auto x = 10; x > 0; x--) {
            threads.add(new test_thread{});
        }

        threads.start();
        threads.join();

    } catch (pthread::pthread_exception &err) {
        std::cerr << "thread_group test case failed. " << err.what() << std::endl;
        GTEST_FAIL();
    } catch (...) {
        std::cerr << "thread_group test case failed. Unexpected eexception catched." << std::endl;
        GTEST_FAIL();
    }
}