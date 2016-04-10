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

#include "pthread/config.h"

#include "pthread/pthread_exception.hpp"
#include "pthread/mutex.hpp"
#include "pthread/lock_guard.hpp"

namespace pthread {
  
  /** condition variable current wait status. */
  enum cv_status {
    no_timeout, /*!< unblocked before a timeout occured */
    timedout    /*!< condition timedout */
  };
  
  /** pthread condition variable
   *
   *The condition_variable class is a synchronization primitive that can be used to block a thread, or multiple threads
   * at the same time, until another thread both modifies a shared variable (the condition), and notifies the 
   * condition_variable.
   *
   * The thread that intends to modify the variable has to
   * - acquire a std::mutex (typically via std::lock_guard)
   * - perform the modification while the lock is held
   * - execute notify_one or notify_all on the std::condition_variable (the lock does not need to be held for notification)
   *
   * Even if the shared variable is atomic, it must be modified under the mutex in order to correctly publish the modification
   * to the waiting thread.
   *
   * Upon successful return, the mutex shall have been locked and shall be owned by the calling thread.
   *
   * @author herbert koelman
   */
  class condition_variable {
  public:
    
    /** wait for condition to be signaled
     *
     * This method atomically release mutex and cause the calling thread to block; atomically here means "atomically with respect to
     * access by another thread to the mutex and then the condition variable". Call notify_one or notify_all to signal the condition.
     *
     * Upon successful return, the mutex has been locked and is owned by the calling thread.
     *
     * @param mtx ralated mutex, which must be locked by the current thread.
     */
    void wait ( mutex &mtx );
    
    /** @see wait
     */
    void wait ( lock_guard<pthread::mutex> lck);
    
    /** wait for condition to be signaled
     *
     * This method atomically release mutex and cause the calling thread to block; atomically here means "atomically with respect to
     * access by another thread to the mutex and then the condition variable". Call notify_one or notify_all to signal the condition.
     *
     * Upon successful return, the mutex has been locked and is owned by the calling thread.
     *
     * The lambda (closure) is run to check if the condition was met. Lambda should false if the waiting should be continued.
     * The signature of the predicate function should be equivalent to the following: bool pred();
     *
     * @param mtx ralated mutex, which must be locked by the current thread.
     * @param lambda run to check if condition was met.
     * @return true if lmabda returned true.
     */
    template<class Lambda> bool wait( mutex &mtx, Lambda lambda);
    
    /** wait for condition to be signaled
     *
     * This method atomically release mutex and cause the calling thread to block; atomically here means "atomically with respect to
     * access by another thread to the mutex and then the condition variable". Call notify_one or notify_all to signal the condition.
     *
     * Upon successful return, the mutex has been locked and is owned by the calling thread.
     *
     * The lambda (closure) is run to check if the condition was met. Lambda should false if the waiting should be continued.
     * The signature of the predicate function should be equivalent to the following: bool pred();
     *
     * @param lck ralated mutex lock_guard, which must be locked by the current thread.
     * @param lambda run to check if condition was met.
     * @return true if lmabda returned true.
     */
    template<class Lambda> bool wait( lock_guard<pthread::mutex> &lck, Lambda lambda);
    
    /** wait for condition to be signaled within given time frame
     *
     * This method atomically release mutex and cause the calling thread to block; atomically here means "atomically with respect to
     * access by another thread to the mutex and then the condition variable". Call notify_one or notify_all to signal the condition.
     *
     * Upon successful return, the mutex has been locked and is owned by the calling thread.
     *
     * If this method is called with millis < 0 then the timeout time is not recalculated. This make it possible to handle spurious 
     * unblocking of condition variable without the need of a lambda expression. The call sequence is then: while(! check_condition() && wait_for(lck, 200) == no_tiemout );
     *
     * @param mtx ralated mutex, which must be locked by the current thread.
     * @param millis milliseconds to wait for this instance to signaled.
     * @return cv_status (either timeout or no_timeout)
     */
    cv_status wait_for (mutex &mtx, int millis );

    /** @see #wait_for (mutex &, int)
     */
    cv_status wait_for (lock_guard<pthread::mutex> &lck, int millis );
    
    /** wait for condition to be signaled within a given time frame
     *
     * This method atomically release mutex and cause the calling thread to block; atomically here means "atomically with respect to
     * access by another thread to the mutex and then the condition variable". Call notify_one or notify_all to signal the condition.
     *
     * Upon successful return, the mutex has been locked and is owned by the calling thread.
     *
     * The lambda (closure) is run to check if the condition was met. Lambda should false if the waiting should be continued.
     * The signature of the predicate function should be equivalent to the following: bool lambda();
     *
     * @param mtx ralated mutex, which must be locked by the current thread.
     * @param millis milli seconds to wait for condition to be signaled.
     * @param lambda run to check if condition was met.
     * @return true if lmabda returned true.
     */
    template<class Lambda> bool wait_for( mutex &mtx, int millis, Lambda lambda);
    
    /** wait for condition to be signaled within a given time frame
     *
     * This method atomically release mutex and cause the calling thread to block; atomically here means "atomically with respect to
     * access by another thread to the mutex and then the condition variable". Call notify_one or notify_all to signal the condition.
     *
     * Upon successful return, the mutex has been locked and is owned by the calling thread.
     *
     * The lambda (closure) is run to check if the condition was met. Lambda should false if the waiting should be continued.
     * The signature of the predicate function should be equivalent to the following: bool lambda();
     *
     * @param lck ralated mutex lock_guard, which must be locked by the current thread.
     * @param millis milli seconds to wait for condition to be signaled.
     * @param lambda run to check if condition was met.
     * @return true if lmabda returned true.
     */
    template<class Lambda> bool wait_for( lock_guard<pthread::mutex> &lck, int millis, Lambda lambda);
    
    /** signal one waiting thread.
     *
     * The pthread_cond_signal() call unblocks at least one of the threads that are blocked
     * on the specified condition variable cond (if any threads are blocked on cond).
     */
    void notify_one () __NOEXCEPT__;
    
    /** signal all waiting threads
     * The pthread_cond_broadcast() call unblocks all threads currently blocked on the specified condition variable cond.
     */
    void notify_all () __NOEXCEPT__;
    
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

    /** thrown when mutex actions fail
     *
     * @param message short description
     * @param pthread_errno error returned by the pthread function
     */
    condition_variable_exception( const string message, const int pthread_errno = 0);
    virtual ~condition_variable_exception(){};
  };
  
  // template implementation ----------------------
  
  template<class Lambda> bool condition_variable::wait( mutex &mtx, Lambda lambda){
    
    bool stop_waiting = lambda();
    
    while(!stop_waiting){
      wait(mtx);
      stop_waiting = lambda(); // handling spurious wakeups
    }
    
    return stop_waiting;
  };

  template<class Lambda> bool condition_variable::wait( lock_guard<pthread::mutex> &lck, Lambda lambda){

    return wait( *(lck.mutex()), lambda);
  };
  
  template<class Lambda> bool condition_variable::wait_for( mutex &mtx, int millis, Lambda lambda){
    int rc = 0;
    cv_status status = no_timeout;
    
    milliseconds(millis); // update timeout
    bool stop_waiting = lambda(); // returns ​false if the waiting should be continued.
    
    while(! stop_waiting && status == no_timeout){
      
      rc  = pthread_cond_timedwait ( &_condition, &mtx._mutex, &timeout );
      
      switch (rc){
          
        case ETIMEDOUT:
          status = timedout;
          break;
          
        case EINVAL:
          throw condition_variable_exception("The value specified by abstime is invalid.", rc);
          break;
          
        case EPERM:
          throw condition_variable_exception("The mutex was not owned by the current thread at the time of the call.", rc);
          break;
        default:
          status = no_timeout ;
          break;
      }
      
      stop_waiting = lambda();
    }
    
    
    return stop_waiting ; //status == cv_status::no_timeout;
  };
  
  template<class Lambda> bool condition_variable::wait_for( lock_guard<pthread::mutex> &lck, int millis, Lambda lambda){
    
    return wait_for(*(lck.mutex()),millis, lambda);
  };
  
} // namespace pthread
#endif
