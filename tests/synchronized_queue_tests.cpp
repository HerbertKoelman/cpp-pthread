//! \file

#include <pthread.h>
#include "pthread/pthread.hpp"
#include "gtest/gtest.h"

#include <cstdio>
#include <iostream>
#include <string>
#include <memory>
#include <ctime>
#include <csignal>

#define MESSAGES_TO_PRODUCE 5000 // messages produced
#define CONSUMER_PROCESSING_DURATION 20 // millis

#define CONSUMERS 10 // number of consumer threads
#define PRODUCERS 1  // number of producer threads
#define QUEUE_MAX_SIZE 40 // max size of the queue.

class message;

#if __IBMCPP_TR1__
typedef std::tr1::shared_ptr<message> message_ptr;
#else
typedef std::shared_ptr<message> message_ptr;
#endif

typedef pthread::util::sync_queue<message_ptr> sync_message_queue;

sync_message_queue *queue = NULL;

class message {
public:
    message(const std::string &buffer, unsigned long number) : _message(buffer), _number(number) {
        time(&_timestamp);
        //printf ("message allocated: %s (timestamp: %ld, number: %ld)\n", _message.c_str(), _timestamp, _number);
    };

    virtual ~message() {
        //printf ("message de-allocated: %s (creation timestamp: %ld, number: %ld)\n", _message.c_str(), _timestamp, _number);
    };

    const std::string content() const {
        return _message.c_str();
    };

    void set_content(const std::string &message) {
        _message = message;
    };

    const time_t timestamp() const {
        return _timestamp;
    };

private:
    std::string _message;
    time_t _timestamp;
    unsigned long _number;
};

class status {
public:

    status(sync_message_queue &queue) : _queue(queue) {
        printf("status allocated\n");
    };

    virtual ~status() {
        printf("status de-allocated\n");
    };

    bool running() const { return _running; };

    void stop() {
        pthread::lock_guard<pthread::mutex> lock(_mutex);
        _running = false;
        printf("class status changed running to %d (%s, %d)\n", _running, __FILE__, __LINE__);
    };

protected:
    sync_message_queue &_queue;

private:
    static bool _running;
    static pthread::mutex _mutex;
};

class producer : public status, public pthread::abstract_thread {
public:

    producer(sync_message_queue &queue) : status(queue) {
    };

#if __cplusplus < 201103L
    void run() throw() {
#else

    void run() noexcept {
#endif
        printf("start producing %d messages\n", MESSAGES_TO_PRODUCE);
        for (auto x = MESSAGES_TO_PRODUCE; (x > 0) && running(); x--) {
            message_ptr pmessage(new message("producer creation...", x));

            _queue.put(pmessage);

            //pthread::this_thread::sleep_for(1*1000);
        }
        std::cout << "send stop message" << std::endl;
        message_ptr pmessage(new message("stop", -1));
        _queue.put(pmessage);
    };
};

class consumer : public status, public pthread::abstract_thread {
public:

    consumer(sync_message_queue &queue) : status(queue) {
    };

#if __cplusplus < 201103L
    void run() throw() {
#else

    void run() noexcept {
#endif

        printf("starting consumer\n");
        message_ptr pmessage; // (new message("hello"));
        printf("queue max size is %zu\n", _queue.max_size());
        while (running()) {

            try {
                _queue.get(pmessage, 1200);
                count();

                if (pmessage->content().find("stop") != std::string::npos) {
                    stop();
                }

                if ((100 % counter()) == 0) {
                    printf("queue's current content is %zu (thrd: %o );\n", _queue.size(), pthread::this_thread::get_id());
                }

                pthread::this_thread::sleep_for(CONSUMER_PROCESSING_DURATION);

                //printf("consumer received message: %s (creation timestamp: %ld)\n", pmessage->content().c_str(), pmessage->timestamp());

                pmessage->set_content("this message content should be displayed when de-allocating message.");
                //printf("consumer modification: %s (creation timestamp: %ld)\n", pmessage->content().c_str(), pmessage->timestamp());

            } catch (pthread::util::queue_timeout &err) {
                printf("queue get timed out (%s, at %d)\n", __FILE__, __LINE__);
            }
        }

        printf("stopping consumer\n");
    };

    static int counter() {
        pthread::lock_guard<pthread::read_lock> lock(_rwlock);
        return _counter;
    }

private:

    void count() {
        pthread::lock_guard<pthread::write_lock> lock(_rwlock);
        _counter++;
    }

    static int _counter;
    static pthread::read_write_lock _rwlock;

};

int            consumer::_counter = 0;
pthread::read_write_lock consumer::_rwlock;
bool           status::_running = true;
pthread::mutex status::_mutex;

// int synchronized_queue() {
TEST(synchronized_queue, producer_consumer) {

    auto pstatus = EXIT_FAILURE;

    try {
        std::cout << "Version: " << pthread::cpp_pthread_version() << std::endl;
        queue = new sync_message_queue(QUEUE_MAX_SIZE);

        status status(*queue);

        pthread::thread_group group;

        for (auto x = CONSUMERS; x > 0; x--) {
            group.add(new consumer(*queue));
        }

        for (auto x = PRODUCERS; x > 0; x--) {
            group.add(new producer(*queue));
        }

        group.start();
        group.join();

        auto consumed_messages = consumer::counter();

        // The + 1 is because we send one more message: a stop message
        EXPECT_EQ(MESSAGES_TO_PRODUCE + 1, consumed_messages);

        std::cout << "threads joined main program, consumed " << consumed_messages << " messages" << std::endl;

        pstatus = EXIT_SUCCESS;
    } catch (std::exception &err) {
        std::cerr << __FILE__ << "(at:" << __LINE__ << ")" << err.what() << std::endl;
    }

    EXPECT_EQ(pstatus, EXIT_SUCCESS);
}