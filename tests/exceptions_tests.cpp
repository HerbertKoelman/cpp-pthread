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
    try {
        throw pthread::pthread_exception("POSIX Thread exception test.", -1);
    } catch (pthread::pthread_exception &ex) {

        EXPECT_STREQ("POSIX Thread exception test.", ex.what());
        EXPECT_EQ(-1, ex.pthread_errno());
    }
}

TEST(exceptions, thread_exception) {
    try {
        throw pthread::thread_exception("Thread exception test.", 1);
    } catch (pthread::pthread_exception &ex) {

        EXPECT_STREQ("Thread exception test.", ex.what());
        EXPECT_EQ(1, ex.pthread_errno());
    }
}

TEST(exceptions, condition_variable_exception) {
    try {
        throw pthread::condition_variable_exception("Condition variable exception test");
    } catch (pthread::pthread_exception &ex) {
        EXPECT_STREQ("Condition variable exception test", ex.what());
        EXPECT_EQ(-1, ex.pthread_errno());
    }
}

TEST(exceptions, read_write_lock_exception) {
    try {
        throw pthread::read_write_lock_exception("read_write_lock exception");
    } catch (pthread::pthread_exception &ex) {
        EXPECT_STREQ("read_write_lock exception", ex.what());
        EXPECT_EQ(-1, ex.pthread_errno());
    }
}

TEST(exceptions, mutex_exception) {
    try {
        throw pthread::mutex_exception("mutex exception test");
    } catch (pthread::pthread_exception &ex) {
        EXPECT_STREQ("mutex exception test", ex.what());
        EXPECT_EQ(-1, ex.pthread_errno());
    }
}

TEST(exceptions, timeout_exception) {
    try {
        throw pthread::timeout_exception("Timeout exception test.");
    } catch (pthread::pthread_exception &ex) {
        EXPECT_EQ(60, ex.pthread_errno());
        EXPECT_STREQ("Timeout exception test.", ex.what());
    }
}

TEST(exceptions, util_queue_exception) {
    try {
        throw pthread::util::queue_exception();
    } catch (std::exception &ex) {
        EXPECT_STREQ("queue_error occured.", ex.what());
    }
}

TEST(exceptions, util_queue_full_exception) {
    try {
        throw pthread::util::queue_full();
    } catch (std::exception &ex) {
        EXPECT_STREQ("synchronized_queue full.", ex.what());
    }
}

TEST(exceptions, util_queue_access_timeout_exception) {
    try {
        throw pthread::util::queue_timeout();
    } catch (std::exception &ex) {
        EXPECT_STREQ("synchronized_queue get/put timed out.", ex.what());
    }
}