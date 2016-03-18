//
//  thread.cpp
//  cpp-pthread
//
//  Created by herbert koelman on 18/03/2016.
//  Copyright © 2016 urbix-software. All rights reserved.
//

#include "pthread/thread.hpp"

namespace pthread {
  
  extern "C" void * thread_startup (void *);
  
  /**
   This function is a helper function. It has normal C linkage, and is
   the base for newly created Thread objects. It runs the
   run method on the thread object passed to it (as a void *).
   */
  void *thread_startup(void *args) {
    
    thread *t = (thread *)args;
    t->run();
    if ( t->destroy_when_ended () ) delete t;
    
    return (NULL);
  }
  
  
  thread::thread ( bool destroy ): _destroy{destroy} {
    
    /* Initialize and set thread detached attribute */
    pthread_attr_init(&_attr);
    pthread_attr_setdetachstate(&_attr, PTHREAD_CREATE_JOINABLE);
  }
  
  thread::~thread () {
    pthread_attr_destroy(&_attr);
  }
  
  void thread::run () noexcept {
    
  }
  
  int thread::start () {
    
    int rc = 0;
    
    rc = pthread_create(&_thread, &_attr, thread_startup, (void *) this);
    
    return rc;
  }
  
  int thread::join () {
    int rc = 0;
    
    rc = pthread_join(_thread, (void **)&_status);
    
    return rc;
  }
  
  int thread::cancel () {
    int rc = 0;
    
    rc = pthread_cancel ( _thread );
    
    return rc;
  }

} // namespace pthread