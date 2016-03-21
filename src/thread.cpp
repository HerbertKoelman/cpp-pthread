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
  
  extern "C" void * thread_startup (void *);
  
  void thread::run () noexcept {
  }
  
  void thread::sleep(const int millis){
    usleep(millis * 1000);
  }
  
  int thread::start () {
    
    int rc = 0;
    
    if ((rc = pthread_create(&_thread, &_attr, thread_startup, (void *) this)) != 0){
      throw thread_exception{"pthread_create failed.", rc };
    }
      
    return rc;
  }
  
  int thread::join () {
    int rc = 0;
    
    if ( (rc = pthread_join(_thread, (void **)&_status)) != 0){
      throw thread_exception{"pthread_join failed.", rc };
    }
    
    return rc;
  }
  
  int thread::cancel () {
    int rc = 0;
    
    rc = pthread_cancel ( _thread );

    if ( rc == 0 ){
      throw thread_exception{"pthread_ccancel failed.", rc };
    }

    return rc;
  }
  
  thread::thread ( bool destroy ): _destroy{destroy} {
    int rc = 0 ;
    /* Initialize and set thread detached attribute */
    if ( (rc = pthread_attr_init(&_attr)) != 0){
      throw thread_exception{"pthread_attr_init failed.", rc };
    }
    
    if ( (rc = pthread_attr_setdetachstate(&_attr, PTHREAD_CREATE_JOINABLE)) != 0 ){
      throw thread_exception{"pthread_attr_setdetachstate failed.", rc };
    }
  }
  
  thread::~thread () {
    pthread_attr_destroy(&_attr);
  }
  
  /**
   This function is a helper function. It has normal C linkage, and is
   the base for newly created Thread objects. It runs the
   run method on the thread object passed to it (as a void *).
   */
  void *thread_startup(void *args) {
    
    thread *t = (thread *)args;
    t->run();
    if ( t->destroy_when_ended () ) {
      delete t;
    }
    
    return (NULL);
  }
  
  // exception -------
  
  thread_exception::thread_exception(const string message, const int pthread_error): pthread_exception{message, pthread_error}{
  }
  
} // namespace pthread