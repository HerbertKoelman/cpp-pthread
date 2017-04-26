//! \file

#include <pthread.h>
#include "pthread/pthread.hpp"

#include <cstdio>
#include <iostream>
#include <string>
#include <memory>
#include <ctime>

class status {
  public:

    bool running(){

      if ( _stalled ){
        pthread::lock_guard<pthread::mutex> lck(_mutex);
        printf ("stalling thread %d ...\n", pthread::this_thread::get_id());
        _semaphore.wait(lck);
        printf("Running again %d\n", _stalled );
      }

      return _running;
    }

    void stop(){
      _running = false;
      std::cout << "stop workers" << std::endl;
     _semaphore.notify_all();
    }

    void stall(){
      std::cout << "stall workers" << std::endl;
      _stalled = true ;
     _semaphore.notify_all();
    }

    void run(){
      std::cout << "run workers" << std::endl;
      _stalled = false ;
     _semaphore.notify_all();
    }

    status(): _running(true), _stalled(false){
    }

  private:
    bool                        _running;
    bool                        _stalled;
    pthread::mutex              _mutex;
    pthread::condition_variable _semaphore;
};

class worker: public pthread::abstract_thread {
  public:

    void run() throw() {
      _id=pthread::this_thread::get_id();

      printf("worker %d started\n", _id);

      while ( _status->running()){
        fprintf(stderr, "worker %d is working...\n", _id );
        // pthread::this_thread::sleep_for(1);
      }

      printf("worker %d ended\n", _id);
    }

    worker(status *status):_id(0), _status(status){
    }

    pthread_t id() const {
      return _id;
    }

  private:
    status         *_status;
    pthread_t       _id;
};

int main(int argc, const char * argv[]) {
  
  auto test_status = EXIT_FAILURE;
  std::cout << "version: " << pthread::cpp_pthread_version() << std::endl;

  try {

    status state;
    pthread::thread_group threads;
    threads.add(new worker(&state));
    threads.add(new worker(&state));
    threads.add(new worker(&state));

    threads.start();

    bool stall = false;
    std::string entry;
    do{

      std::getline(std::cin, entry);
      if ( entry != "quit" ){
        if ( stall ) {
          state.run();
          stall = false;
        } else {
          state.stall();
          stall = true;
        }
      }

    } while ( entry != "quit" );

    state.stop();

    threads.join();

    test_status = EXIT_SUCCESS;
  }catch (std::exception &err ){
    std::cerr << __FILE__ << "(at:" << __LINE__ << ")" << err.what() << std::endl;
  }

  return test_status ;
}
