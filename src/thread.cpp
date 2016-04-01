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
    
    pthread_t get_id(){
      return pthread_self();
    }
  }
  
  int thread::join () {
    int rc = 0;
    
    if ( _thread == this_thread::get_id()){
      throw pthread_exception("join failed, join yourself would endup in deadlock.");
    }
    if ( _status == thread_status::not_a_thread ){
      throw pthread_exception("join failed, this is not a thread.");
    }
    
    if ( (rc = pthread_join(_thread, (void **)&_status)) != 0){
      throw thread_exception("pthread_join failed.", rc );
    }
    
    return rc;
  }

  int thread::cancel () {
    int rc = 0;
    
    if ( _status == thread_status::not_a_thread ){
      throw pthread_exception("cancel failed, this is not a thread.");
    }
      
    if((rc = pthread_cancel ( _thread )) != 0){
      throw thread_exception("pthread_cancel failed.", rc );
    } else {
      _status = thread_status::not_a_thread;
    }

    return rc;
  }
  
#ifdef __IBMCPP__
  thread::thread(): _status(thread_status::not_a_thread), _thread(0){
#else
  thread::thread(): _status{thread_status::not_a_thread}, _thread{nullptr}{
#endif
    
  }
  
#ifdef __IBMCPP__
  thread::thread (const runnable &work): thread(){
#else
  thread::thread (const runnable &work): thread{}{
#endif
    int rc = 0 ;
    pthread_attr_t attr(0);
    
    /* Initialize and set thread detached attribute */
    if ( (rc = pthread_attr_init(&attr)) != 0){
      throw thread_exception("pthread_attr_init failed.", rc );
    }
    
    if ( (rc = pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE)) != 0 ){
      throw thread_exception("pthread_attr_setdetachstate failed.", rc );
    }
    
    if ((rc = pthread_create(&_thread, &attr, thread_startup_runnable, (void *) &work)) != 0){
      throw thread_exception("pthread_create failed.", rc );
    } else {
      _status = thread_status::a_thread;
      pthread_attr_destroy(&attr);
    }

  }
  
  /* move constructor */
  thread::thread(thread&& other){
   
    swap(other);
  }
  
  /* move operator */
  thread& thread::operator=(thread&& other){
  
    swap(other);
    
    return *this;
  }
  
  void thread::swap(thread& other){
    std::swap(_thread, other._thread);
    std::swap(_status, other._status);
  }
  
  thread::~thread () {
//    if ( _status == thread_status::a_thread ){
//      pthread_attr_destroy(&_attr);
//    }
  }
  
  /**
   This function is a helper function. It has normal C linkage, and is
   the base for newly created Thread objects. It runs the
   run method on the thread object passed to it (as a void *).
   */
  void *thread_startup_runnable(void *runner) {
    
    static_cast<runnable *>(runner)->run();
    
    return (NULL);
  }
  
  // exception -------
  
#ifdef __IBMCPP__
  thread_exception::thread_exception(const string message, const int pthread_error): pthread_exception(message, pthread_error){
#else
  thread_exception::thread_exception(const string message, const int pthread_error): pthread_exception{message, pthread_error}{
#endif
  }
  
} // namespace pthread
