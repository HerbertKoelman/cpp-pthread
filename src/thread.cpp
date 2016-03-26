//
//  thread.cpp
//  cpp-pthread
//
//  Created by herbert koelman on 18/03/2016.
//  Copyright © 2016 urbix-software. All rights reserved.
//

#include "pthread/thread.hpp"
#include <unistd.h>

namespace pthread {

  namespace this_thread {
    void sleep(const int millis){
      usleep(millis * 1000);
    }
  }
  
  int thread::join () {
    int rc = 0;
    
    if ( _status == thread_status::not_a_thread ){
      throw pthread_exception{"cancel failed, this is not a thread."};
    }
    
    if ( (rc = pthread_join(_thread, (void **)&_status)) != 0){
      throw thread_exception{"pthread_join failed.", rc };
    }
    
    return rc;
  }

  int thread::cancel () {
    int rc = 0;
    
    if ( _status == thread_status::not_a_thread ){
      throw pthread_exception{"cancel failed, this is not a thread."};
    }
      
    if((rc = pthread_cancel ( _thread )) != 0){
      throw thread_exception{"pthread_ccancel failed.", rc };
    } else {
      _status = thread_status::not_a_thread;
    }

    return rc;
  }
  
  thread::thread(): _status{thread_status::not_a_thread} {
    
  }
  
  thread::thread (const runnable &work): thread{}{
    int rc = 0 ;
    
    /* Initialize and set thread detached attribute */
    if ( (rc = pthread_attr_init(&_attr)) != 0){
      throw thread_exception{"pthread_attr_init failed.", rc };
    }
    
    if ( (rc = pthread_attr_setdetachstate(&_attr, PTHREAD_CREATE_JOINABLE)) != 0 ){
      throw thread_exception{"pthread_attr_setdetachstate failed.", rc };
    }
    
    if ((rc = pthread_create(&_thread, &_attr, thread_startup_runnable, (void *) &work)) != 0){
      throw thread_exception{"pthread_create failed.", rc };
    } else {
      _status = thread_status::a_thread;
    }

  }
  
  thread::thread(thread&& other){
   
    move(std::move(other));
  }
  
  thread& thread::operator=(thread&& other){
  
    move(std::move(other));
    return *this;
  }
  
  void thread::move(thread&& other){
    lock_guard<mutex> lck(_mtx);
    
    if ( other._status == thread_status::not_a_thread ){
      throw thread_exception{"not a thread, move constructor failed."};
    }
    
    _thread = std::move(other._thread);
    _attr   = std::move(other._attr);
    _status = thread_status::a_thread;
    other._status = thread_status::not_a_thread;
  }
  
  thread::~thread () {
    if ( _status == thread_status::a_thread ){
      pthread_attr_destroy(&_attr);
    }
  }
  
  /**
   This function is a helper function. It has normal C linkage, and is
   the base for newly created Thread objects. It runs the
   run method on the thread object passed to it (as a void *).
   */
  extern "C" void *thread_startup_runnable(void *runner) {
    
    static_cast<runnable *>(runner)->run();
    
    return (NULL);
  }
  
  extern "C" void *thread_statup_function(void *function){
   
   return NULL;
  }
  
  // exception -------
  
  thread_exception::thread_exception(const string message, const int pthread_error): pthread_exception{message, pthread_error}{
  }
  
} // namespace pthread