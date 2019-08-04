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
            std::cout << "test_runnable is running in a thread..." << std::flush;
            pthread::this_thread::sleep_for(1 * 1000);
            for (auto count = 1000; count > 0; count--) {
                counter += count;
            }
            std::cout << "Done" << std::endl << std::flush ;
        } catch (const std::exception &err) {
            std::cerr << "something went wrong while running test_runnable. " << err.what() << std::endl << std::flush;
        } catch ( ... ){
            std::cerr << "unexpected exception catched. " << std::endl << std::flush;
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

TEST(thread, DISABLED_cancel) {

    try {
        test_runnable tr;
        pthread::thread t{tr}; // this starts running the thread

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

TEST(thread, thread_constructor){

    pthread::thread t1 ;
    EXPECT_EQ(t1.status(), pthread::thread_status::not_a_thread);

    test_runnable runnable ;
    pthread::thread t2{runnable};
    EXPECT_EQ(t2.status(), pthread::thread_status::a_thread);
}

TEST(thread, DISABLED_move_operator) {

    test_runnable tr;
    pthread::thread t1{tr}; // this starts running the thread
    //pthread::thread t2 = t1;
    EXPECT_EQ(t1.status(), pthread::thread_status::not_a_thread);
}
