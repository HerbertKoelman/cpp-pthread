//
//  thread.hpp
//  cpp-pthread
//
//  Created by herbert koelman on 18/03/2016.
//  Copyright © 2016 urbix-software. All rights reserved.
//

#ifndef pthread_thread_hpp
#define pthread_thread_hpp

#include <pthread.h>
#include <string>
#include <functional>

#include "pthread/pthread_exception.hpp"
#include "pthread/mutex.hpp"

namespace pthread {
  
  extern "C" void *thread_startup_runnable (void *);
  extern "C" void *thread_statup_function(void *arg);
  
  enum class thread_status{
    not_a_thread,
    a_thread
  };
  
  namespace this_thread{
    /** let the current thread sleep for the given milli seconds.
     *
     * @param milis time to wait.
     */
    void sleep(const int millis);
  };
  
  /**
   * Interface of a runnable class.
   *
   * Yiou can write code to be run through a Thread by implementing this interface.
   */
  class runnable {
  private:
    
  public:
    /**
     * This method must be overriden
     */
    virtual void run () noexcept = 0 ;
  };
  
  /**
   * Handles POSIX (Portable Operating System Interface) threads.
   *
   * <code>
   *     class reader_thread: public runnable {
   *     public:
   *       void run() {...}
   *     };
   *
   *     reade_thread rt;
   *     thread t{rt};
   *     t.join();
   * </code>
   */
  class thread {
  public:
    
    /**
     * create a new thread status is no_a_thread
     */
    thread();
    
    /** create a new thread
     *
     * The new thread is made runnable, and will start executing the run routine, with.
     *
     * @param runner a class that implements the runnable interface.
     */
    thread( const runnable &runner );
    
    template<class Function, class... Args> thread(Function&& f, Args&&... args);
    
    /** move contructor
     *
     * Used when pushing instances into containers (i.e. list vector). Once moved this not a thread anymore. Status is thread_status::not_a_thread
     */
    thread( thread&& other);
    
    /** copy constructor makes no sense.
     */
    thread(const thread &) = delete ;
    
    /**
     * cleanup thread ressources.
     */
    virtual ~thread();
    
    /**
     * The join method blocks the calling thread until the thread terminates. The target thread's 
     * termination status is returned in the status parameter.
     *
     * If the target thread is already terminated, the method returns immediately.
     *
     * This method does not itself cause a thread to be terminated.
     */
    int join();
    
    /**
     * The cancel method requests the cancellation of the thread. The action depends on the
     * cancelability of the target thread:
     *
     * o If its cancelability is disabled, the cancellation request is set pending.
     * o If its cancelability is deferred, the cancellation request is set pending till the thread reaches a cancellation point.
     * o If its cancelability is asynchronous, the cancellation request is acted upon immediately; in some cases, it may result in unexpected behaviour.
     *
     * The cancellation of a thread terminates it safely, using the same termination
     * procedure as the pthread_exit subroutine.
     */
    int cancel();
    
    /** @return the status of the thread.
     */
    inline thread_status status() { return _status ;};
    
    /** copying doesn't make sense
     */
    thread& operator=(const thread&) = delete ;
    
    /** move a thread to another thread.
     *
     * The moved thread is not a thread anymore (thread_status::not_a_thread).
     */
    thread& operator=(thread&& other);
    
  private:
    void move ( thread&& other );
    mutex          _mtx;
    pthread_t      _thread;
    pthread_attr_t _attr;
    
    thread_status  _status;
  };
  
  // exception & errors --------
  
  class thread_exception: public pthread_exception {
  public:
    thread_exception(const string message, const int pthread_error = 0);
  };
  
  // template implementations ------
  
  template<class Function, class... Args>
  thread::thread(Function&& func, Args&&... args){
    int rc = 0 ;

    auto work = std::bind(func, std::forward<Args>(args)...);
   
//    action();
    
    /* Initialize and set thread detached attribute */
    if ( (rc = pthread_attr_init(&_attr)) != 0){
      throw thread_exception{"pthread_attr_init failed.", rc };
    }
    
    if ( (rc = pthread_attr_setdetachstate(&_attr, PTHREAD_CREATE_JOINABLE)) != 0 ){
      throw thread_exception{"pthread_attr_setdetachstate failed.", rc };
    }
    
    
    if ((rc = pthread_create(&_thread, &_attr, thread_statup_function, (void *) []{work();} )) != 0){
    if ((rc = pthread_create(&_thread, &_attr, [&work](void *)->void *{ work(); }, (void *) nullptr)) != 0){
      throw thread_exception{"pthread_create failed.", rc };
    } else {
      _status = thread_status::a_thread;
    }

    
//    std::function<Function> function{Func};
//    auto f = std::bind(std::forward<Function>(func), std::forward<Args>(args)...);
    
  }
  
 
} // pthread
#endif /* thread_hpp */
