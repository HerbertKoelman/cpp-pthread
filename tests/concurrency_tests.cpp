//! \file

#include <pthread.h>
#include "pthread/pthread.hpp"
#include "gtest/gtest.h"

#include <cstdio>
#include <iostream>
#include <string>
#include <memory>
#include <ctime>

class concurrency_test_runnable : public pthread::abstract_thread {
public:

    void run() noexcept override {
        pthread::lock_guard<pthread::mutex> lock{*_mutex};
        pthread::this_thread::sleep_for(2 * 1000);
    }

    concurrency_test_runnable(pthread::mutex *mutex) : _something{0}, _mutex{mutex} {
        // intentional
    }

    void set_something(const int something){
        pthread::lock_guard<pthread::write_lock> lock(_properties_accessor_lock);
        _something = something;
    }

    const int something(){
        pthread::lock_guard<pthread::read_lock> lock(_properties_accessor_lock);
        return _something;
    }

private:

    int _something;
    pthread::mutex *_mutex;
    pthread::read_write_lock _properties_accessor_lock;
};

TEST(concurrency, mutex) {

    bool success = false;

    pthread::mutex mutex;
    concurrency_test_runnable mtr{&mutex};
    mtr.start();

    pthread::this_thread::sleep_for(500); // just to let time to startup the thread.

    EXPECT_FALSE(mutex.try_lock()); // return false, because the test thread has already locked the mutex
    EXPECT_NO_THROW(pthread::lock_guard<pthread::mutex> lock(mutex));
    EXPECT_TRUE(mutex.try_lock()); // the test thread has ended and the lock can be aquired again

    mtr.join();
}

TEST(concurrency, read_write_lock) {
    bool success = false;

    try {
        pthread::mutex mutex;

        concurrency_test_runnable thread{&mutex};
        thread.start();

        thread.set_something(1000);
        EXPECT_EQ(1000, thread.something());

        pthread::read_write_lock _rwlock;

        {
            pthread::lock_guard<pthread::read_lock> lock(_rwlock);

        }

        {
            pthread::lock_guard<pthread::write_lock> lock(_rwlock);
        }

        thread.join();

        success = true;

    } catch (std::exception &err) {
        std::cerr << __FILE__ << "(at:" << __LINE__ << ")" << err.what() << std::endl;
    }

    EXPECT_TRUE(success);
}

TEST(concurrency, try_read_write_lock) {
    bool success = false;

    try {

        pthread::read_write_lock rwlock;

        rwlock.try_lock();
        rwlock.unlock();

        success = true;

    } catch (const std::exception &err) {
        std::cerr << err.what() << std::endl << std::flush;
    }

    EXPECT_TRUE(success);
}

TEST(concurrency, condition_variable_wait_for) {
    pthread::condition_variable condition;
    pthread::mutex mutex;
    bool stop_waiting = true;

    /* wait 1s for condition to be signaled. When returning from the wait_for method call, the mutex is locked.
     *
     * Therefore we expect, try_lock to throw an exception to signal that the mutex is already locked.
     */
    EXPECT_EQ(pthread::cv_status::timedout, condition.wait_for(mutex, 1 * 1000));
    EXPECT_FALSE(mutex.try_lock());
    mutex.unlock(); // free to lock

    EXPECT_NO_THROW(condition.notify_all());
    EXPECT_NO_THROW(condition.notify_one());

    {
        pthread::lock_guard<pthread::mutex> lock{mutex};
        EXPECT_EQ(pthread::cv_status::timedout, condition.wait_for(lock, 1 * 1000));
    }

    {
        pthread::lock_guard<pthread::mutex> lock{mutex};
        EXPECT_EQ(true, condition.wait_for(lock, 1 * 1000, [stop_waiting] {
                                               std::cout << "running lambda, stop_waiting : " << stop_waiting << std::endl;
                                               return stop_waiting;
                                           }
        )
        );
    }


    {
        pthread::lock_guard<pthread::mutex> lock{mutex};
        EXPECT_EQ(false,
                condition.wait_for(lock, 1 * 1000, [stop_waiting] {
                                                std::cout << "running lambda, stop_waiting : " << stop_waiting << std::endl;
                                                return !stop_waiting;
                                            })
        );

    }
}

/* NOSONAR for later use
   class test_thread: public pthread::abstract_thread{
    public:
        void run() noexcept override{
            pthread::this_thread::sleep_for(5*1000);
            std::cout << "thread is waiting for mutex" << std::endl;
            pthread::lock_guard<pthread::mutex> lock(*_mutex);
            std::cout << "thread got mutex" << std::endl;
        }

        test_thread( pthread::mutex *mutex): _mutex(mutex){

        }
    private:
        pthread::mutex *_mutex;
    };

    pthread::mutex mutex;

    test_thread thread{&mutex};
    thread.start();
    {
        pthread::lock_guard<pthread::mutex> lock(mutex);
        pthread::this_thread::sleep_for(10*1000);
    }

    thread.join();

 */