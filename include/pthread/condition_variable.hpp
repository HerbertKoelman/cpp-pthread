//
//  csfi_consumer.hpp
//  substation
//
//  Created by herbert koelman on 13/03/2016.
//  Copyright © 2016 urbix-software. All rights reserved.
//

#ifndef pthread_condition_variable_hpp
#define pthread_condition_variable_hpp

#include <pthread.h>
#include <string>
#include <time.h>
#include <sys/time.h>

#include "pthread/pthread_exception.hpp"
#include "pthread/mutex.hpp"
#include "pthread/lock_guard.hpp"

namespace pthread {
  
  enum class cv_status {
    no_timeout,
    timeout
  };
  
  /** pthread condition variable
   */
  class condition_variable { // : public pthread::mutex {
  public:
    
    /** wait for condition to be signaled
     */
    void wait ( mutex &mtx );
    template<class Lambda> bool wait( mutex &mtx, Lambda lambda);
    template<class Lambda> bool wait( lock_guard &lck, Lambda lambda);
    
    /** wait for a condition to be signaled in a limited time frame.
     */
    cv_status wait_for (mutex &mtx, int millis );
    
    template<class Lambda> bool wait_for( mutex &mtx, int millis, Lambda lambda);
    template<class Lambda> bool wait_for( lock_guard &lck, int millis, Lambda lambda);
    
    /** signal one waiting thread.
     *
     * The pthread_cond_signal() call unblocks at least one of the threads that are blocked
     * on the specified condition variable cond (if any threads are blocked on cond).
     */
    void notify_one () noexcept;
    
    /** signal all waiting threads
     * The pthread_cond_broadcast() call unblocks all threads currently blocked on the specified condition variable cond.
     */
    void notify_all () noexcept;
    
    // constructor/destructor ------------------------------------------------
    
    condition_variable ();
    virtual ~condition_variable();
    
  private:
    void milliseconds( int milliseconds);
    
    timespec timeout;
    pthread_cond_t _condition;
  };
  
  /** condition variable exception
   */
  class condition_variable_exception: public pthread_exception {
  public:
    condition_variable_exception( const string message, const int pthread_error = 0);
    virtual ~condition_variable_exception(){};
  };
  
  // template implementation ----------------------
  
  template<class Lambda> bool condition_variable::wait( mutex &mtx, Lambda lambda){
    bool stop_waiting = lambda();
    while(!stop_waiting){
      wait(mtx);
      stop_waiting = lambda(); // handling spurious wakeups
    }
    return true;
  };

  template<class Lambda> bool condition_variable::wait( lock_guard &lck, Lambda lambda){

    return wait( *(lck.mutex()), lambda);
  };
  
  template<class Lambda> bool condition_variable::wait_for( mutex &mtx, int millis, Lambda lambda){
    int rc = 0;
    cv_status status = cv_status::no_timeout;
    
    milliseconds(millis); // update timeout
    bool stop_waiting = lambda(); // returns ​false if the waiting should be continued.
    
    while(! stop_waiting && status == cv_status::no_timeout){
      
      rc  = pthread_cond_timedwait ( &_condition, &mtx._mutex, &timeout );
      
      switch (rc){
          
        case ETIMEDOUT:
          status = cv_status::timeout;
          break;
          
        case EINVAL:
          throw condition_variable_exception("The value specified by abstime is invalid.", rc);
          break;
          
        case EPERM:
          throw condition_variable_exception("The mutex was not owned by the current thread at the time of the call.", rc);
          break;
        default:
          status = cv_status::no_timeout ;
          break;
      }
      
      stop_waiting = !lambda();
    }
    
    
    return status == cv_status::no_timeout;
  };
  
  template<class Lambda> bool condition_variable::wait_for( lock_guard &lck, int millis, Lambda lambda){
    
    return wait_for(*(lck.mutex()),millis, lambda);
  };
  
} // namespace pthread
#endif