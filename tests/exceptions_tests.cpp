#include <pthread.h>
#include <cstdio>
#include <iostream>
#include <string>
#include <list>
#include <utility>
#include <memory>
#include "pthread/pthread.hpp"
#include "gtest/gtest.h"

TEST(exceptions, pthread_exception) {
    std::string expecting = "POSIX Thread exception test. ";
    expecting += strerror(-1);
    try {
        throw pthread::pthread_exception("POSIX Thread exception test.", -1);
    } catch (pthread::pthread_exception &ex) {
        EXPECT_STREQ(expecting.c_str(), ex.what());
        EXPECT_EQ(-1, ex.error_number());
    }
}

TEST(exceptions, thread_exception) {
    std::string expecting = "Thread exception test. ";
    expecting += strerror(-1);
    try {
        throw pthread::thread_exception("Thread exception test.", -1);
    } catch (pthread::pthread_exception &ex) {

        EXPECT_STREQ(expecting.c_str(), ex.what());
        EXPECT_EQ(-1, ex.error_number());
    }
}

TEST(exceptions, condition_variable_exception) {
    std::string expecting = "Condition variable exception test. ";
    expecting += strerror(-1);

    try {
        throw pthread::condition_variable_exception("Condition variable exception test.", -1);
    } catch (pthread::pthread_exception &ex) {
        EXPECT_STREQ(expecting.c_str(), ex.what());
        EXPECT_EQ(-1, ex.error_number());
    }

    try {
        throw pthread::condition_variable_exception();
    } catch (pthread::condition_variable_exception &ex) {
        EXPECT_STREQ("conditional_variable_exception", ex.what());
        EXPECT_EQ(0, ex.error_number());
    }
}

TEST(exceptions, read_write_lock_exception) {
    std::string expecting = "read_write_lock exception. ";
    expecting += strerror(-1);
    try {
        throw pthread::read_write_lock_exception("read_write_lock exception.", -1);
    } catch (pthread::pthread_exception &ex) {
        EXPECT_STREQ(expecting.c_str(), ex.what());
        EXPECT_EQ(-1, ex.error_number());
    }
}

TEST(exceptions, mutex_exception) {
    std::string expecting = "mutex exception test";
    try {
        throw pthread::mutex_exception("mutex exception test");
    } catch (pthread::pthread_exception &ex) {
        EXPECT_STREQ(expecting.c_str(), ex.what());
        EXPECT_EQ(0, ex.error_number());
    }
}

TEST(exceptions, timeout_exception) {
    std::string expecting = "Timeout exception test. ";
    expecting += strerror(ETIMEDOUT);
    try {
        throw pthread::timeout_exception("Timeout exception test.");
    } catch (pthread::pthread_exception &ex) {
        EXPECT_EQ(ETIMEDOUT, ex.error_number());
        EXPECT_STREQ(expecting.c_str(), ex.what());
    }
}

// pthread::util exception -----------------------------------------

TEST(exceptions, util_queue_exception) {
    std::string expecting = "Condition variable exception test. ";
    expecting += strerror(-1);
    try {
        throw pthread::util::queue_exception();
    } catch (std::exception &ex) {
        EXPECT_STREQ("queue_error occured.", ex.what());
    }
}

TEST(exceptions, util_queue_full_exception) {
    try {
        throw pthread::util::queue_full();
    } catch (pthread::util::queue_exception &ex) {
        EXPECT_STREQ("synchronized_queue full.", ex.what());
    }
}

TEST(exceptions, util_queue_access_timeout_exception) {
    try {
        throw pthread::util::queue_timeout();
    } catch (pthread::util::queue_exception &ex) {
        EXPECT_STREQ("synchronized_queue get/put timed out.", ex.what());
    }
}

void *starter_function(pthread::runnable *runner){
    std::cout << "function thread ID : " << pthread::this_thread::get_id() << ": " ;
    runner->run();
}

TEST(class_function, DISABLED_demo) {
    class function_class{
    public:

        function_class( pthread::runnable *runner){
            run(runner);
        }

        function_class( pthread::runnable &runner){
            run((void *)&runner);
        }
    private:
        int         _number;
        std::string _message;

        void run (void *runnable){
            std::cout << "thread ID : " << pthread::this_thread::get_id() << ": " ;
            reinterpret_cast<pthread::runnable *>(runnable)->run();
        }
    };

    class runner : public pthread::runnable{
    public:
        void run() noexcept override {
            std::cout << "runner: hello, world" << std::endl ;
        }
    };
    runner *run_ptr = new runner;
    runner run;
    function_class fc1(run_ptr);
    function_class fc2(run);

    pthread::thread thread{run};

    thread.join();
    delete run_ptr;
}
