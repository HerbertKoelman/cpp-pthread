//! \file

#include <pthread.h>
#include "pthread/pthread.hpp"

#include <cstdio>
#include <iostream>
#include <string>
#include <atomic>
#include <memory>
#include <ctime>

#define MESSAGES_TO_PRODUCE 500000 // messages produced
#define CONSUMER_PROCESSING_DURATION 20 // millis

#define CONSUMERS 10 // number of consumer threads
#define PRODUCERS 1  // number of producer threads
#define QUEUE_MAX_SIZE 40 // max size of the queue.

class message {
  public:
    message (const std::string &buffer, unsigned long number):_message(buffer), _number(number){
      time(&_timestamp);
      //printf ("message allocated: %s (timestamp: %ld, number: %ld)\n", _message.c_str(), _timestamp, _number);
    };

    virtual ~message(){
      //printf ("message de-allocated: %s (creation timestamp: %ld, number: %ld)\n", _message.c_str(), _timestamp, _number);
    };

    const std::string content() const {
      return _message.c_str();
    };

    void set_content(const std::string &message){
      _message = message ;
    };
  
    const time_t timestamp() const {
      return _timestamp;
    };

  private:
    std::string _message;
    time_t      _timestamp;
  unsigned long _number;
};

typedef std::shared_ptr<message> message_ptr;
typedef pthread::util::sync_queue<message_ptr> sync_message_queue;

class status {
  public:

    status(sync_message_queue &queue): _queue(queue){
      printf ("status allocated\n");
    };

    virtual ~status(){
      printf ("status de-allocated\n");
    };

    bool running() const { return _running; };
  
    void stop() {
      pthread::lock_guard<pthread::mutex> lock(_mutex);
      _running = false ;
      printf("class status changed running to %d (%s, %d)\n", _running, __FILE__, __LINE__);
    };

  protected:
    sync_message_queue &_queue;

  private:
    static bool           _running;
    static pthread::mutex _mutex;
};

class producer : public status, public pthread::abstract_thread {
  public:

    producer(sync_message_queue &queue): status(queue){
    };

    void run() noexcept {
      printf ("start producing %d messages\n", MESSAGES_TO_PRODUCE);
      for( auto x = MESSAGES_TO_PRODUCE; (x > 0) && running() ; x-- ){
        message_ptr pmessage(new message("producer creation...", x));
        
        _queue.put (pmessage);
        
        //pthread::this_thread::sleep_for(1*1000);
      }
      printf("send stop producing message\n");
      message_ptr pmessage(new message("stop", -1));
      _queue.put (pmessage);
    };
};

class consumer : public status, public pthread::abstract_thread {
  public:

    consumer(sync_message_queue &queue): status(queue){
    };

    void run() noexcept {
      printf ("starting consumer\n");
      message_ptr pmessage ; // (new message("hello"));
      while( running() ){

        try{
          _queue.get(pmessage, 1200);
          
          if ( pmessage->content().find("stop") != std::string::npos ){
            stop();
          }
         
          printf("queue's current content is %zu (thrd: %d );\n", _queue.size(), pthread::this_thread::get_id());
          pthread::this_thread::sleep_for(CONSUMER_PROCESSING_DURATION);
          
          //printf("consumer received message: %s (creation timestamp: %ld)\n", pmessage->content().c_str(), pmessage->timestamp());
          
          pmessage->set_content("this message content should be displayed when de-allocating message.");
          //printf("consumer modification: %s (creation timestamp: %ld)\n", pmessage->content().c_str(), pmessage->timestamp());
          
        }catch (pthread::util::queue_timeout &err){
          printf("queue get timed out (%s, at %d)\n", __FILE__, __LINE__);
        }
      }
      printf("stopping consumer\n");
    };
};

bool           status::_running = true;
pthread::mutex status::_mutex ;

int main(int argc, const char * argv[]) {
  
  std::cout << "version: " << pthread::cpp_pthread_version() << std::endl;

  try {

    sync_message_queue queue(QUEUE_MAX_SIZE);
    status status(queue);

    pthread::thread_group group;

    for ( auto x = CONSUMERS ; x > 0 ; x--){
      group.add(new consumer(queue));
    }
    
    for ( auto x = PRODUCERS ; x > 0 ; x-- ){
      group.add(new producer(queue));
    }

    group.start();

    group.join();
    printf("threads joined main programm (%s, %d)\n", __FILE__, __LINE__);

  }catch (std::exception &err ){
    std::cerr << __FILE__ << "(at:" << __LINE__ << ")" << err.what() << std::endl;
  }
}
