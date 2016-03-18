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

#include "pthread/pthread_exception.hpp"
#include "pthread/mutex.hpp"

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
    
    /** wait for a condition to be signaled in a limited time frame.
     */
    cv_status wait_for (mutex &mtx, int millis );
    
    template<class Lambda> bool wait_for( mutex &mtx, int millis, Lambda lambda);
    
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
    pthread_cond_t _condition;
  };
  
  /** condition variable exception
   */
  class condition_variable_exception: public pthread_exception {
  public:
    condition_variable_exception( const string message, const int pthread_error = 0);
    virtual ~condition_variable_exception(){};
  };
  
  template<class Lambda> bool condition_variable::wait( mutex &mtx, Lambda lambda){
    while(!lambda()){
      wait(mtx);
    }
    return true;
  };

  template<class Lambda> bool condition_variable::wait_for( mutex &mtx, int millis, Lambda lambda){
    cv_status status = cv_status::no_timeout;
    
    while(!lambda() && status == cv_status::no_timeout){
      status = wait_for(mtx, millis);
    }
    
    return status == cv_status::no_timeout;
  };
  
} // namespace pthread
#endif