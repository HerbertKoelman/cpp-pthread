//
//  Created by herbert koelman on 13/03/2016.
//  Copyright © 2016 urbix-software. All rights reserved.
//

#ifndef pthread_condition_variable_hpp
#define pthread_condition_variable_hpp

// must be include as first header file of each source code file (see IBM's
// recommandation for more info p.285 �8.3.1).
#include <pthread.h>
#include <string>
#include <ctime>
#include <sys/time.h>

#include "pthread/exceptions.hpp"
#include "pthread/mutex.hpp"
#include "pthread/lock_guard.hpp"

namespace pthread {

  /** \addtogroup concurrency Concurrency
   *
   * Set of classes to handle concurrent access to shared ressources.
   *
   * @author herbert koelman (herbert.koelman@me.com)
   * @{
   */

  /** condition variable current wait status. */
  enum cv_status {
    no_timeout, /*!< unblocked before a timeout occured */
    timedout    /*!< condition timedout */
  };

  /** Condition variable.
   *
   * The condition_variable class is a synchronization primitive that can be used to block a thread, or multiple threads
   * at the same time, until another thread both modifies a shared variable (the condition), and notifies the
   * condition_variable.
   *
   * The thread that intends to modify the variable has to
   * - acquire a pthread::mutex (typically via pthread::lock_guard)
   * - perform the modification while the lock is held
   * - execute notify_one or notify_all on the pthread::condition_variable (the lock does not need to be held for notification)
   *
   * Even if the shared variable is atomic, it must be modified under the mutex in order to correctly publish the modification
   * to the waiting thread.
   *
   * Upon successful return, the mutex shall have been locked and shall be owned by the calling thread.
   *
   * @author herbert koelman (herbert.koelman@me.com)
   */
  class condition_variable {
  public:

    /** Wait for condition to be signaled.
     *
     * This method atomically release mutex and cause the calling thread to block; atomically here means "atomically with respect to
     * access by another thread to the mutex and then the condition variable". Call notify_one or notify_all to signal the condition.
     *
     * Upon successful return, the mutex has been locked and is owned by the calling thread.
     *
     * @param mtx ralated mutex, which must be locked by the current thread.
     * @see notify_one
     * @see notify_all
     */
    void wait ( mutex &mtx );

    /** @see wai
     */
    void wait ( lock_guard<pthread::mutex> lck);

    /** Wait for condition to be signaled.
     *
     * This method atomically release mutex and cause the calling thread to block; atomically here means "atomically with respect to
     * access by another thread to the mutex and then the condition variable". Call notify_one or notify_all to signal the condition.
     *
     * Upon successful return, the mutex has been locked and is owned by the calling thread.
     *
     * The lambda (closure) is run to check if the condition was met. Lambda should return false if the waiting should be continued.
     * The signature of the predicate function should be equivalent to the following: bool pred();
     *
     * @param mtx ralated mutex, which must be locked by the current thread.
     * @param lambda run to check if condition was met.
     * @return true if lmabda returned true.
     * @see notify_one
     * @see notify_all
     */
    template<class Lambda> bool wait( mutex &mtx, Lambda lambda);

    /** Wait for condition to be signaled.
     *
     * This method atomically release mutex and cause the calling thread to block; atomically here means "atomically with respect to
     * access by another thread to the mutex and then the condition variable". Call notify_one or notify_all to signal the condition.
     *
     * Upon successful return, the mutex has been locked and is owned by the calling thread.
     *
     * The lambda (closure) is run to check if the condition was met. Lambda should return false if the waiting should be continued.
     * The signature of the predicate function should be equivalent to the following: bool pred();
     *
     * @param lck ralated mutex lock_guard, which must be locked by the current thread.
     * @param lambda run to check if condition was met.
     * @return true if lmabda returned true.
     * @see notify_one
     * @see notify_all
     */
    template<class Lambda> bool wait( lock_guard<pthread::mutex> &lck, Lambda lambda);

    /** Wait for condition to be signaled within given time frame.
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
     * @throw condition_variable_exception is thrown either if timeout calculation failed or mutex ownership was wrong.
     * @see notify_one
     * @see notify_all
     */
    cv_status wait_for (mutex &mtx, int millis );

    /** @see #wait_for (mutex &, int)
     */
    cv_status wait_for (lock_guard<pthread::mutex> &lck, int millis );

    /** Wait for condition to be signaled within a given time frame.
     *
     * This method atomically release mutex and cause the calling thread to block; atomically here means "atomically with respect to
     * access by another thread to the mutex and then the condition variable". Call notify_one or notify_all to signal the condition.
     *
     * Upon successful return, the mutex has been locked and is owned by the calling thread.
     *
     * The lambda (closure) is run to check if the condition was met. Lambda should return false if the waiting should be continued.
     * The signature of the predicate function should be equivalent to the following: bool lambda();
     *
     * @param mtx ralated mutex, which must be locked by the current thread.
     * @param millis milli seconds to wait for condition to be signaled.
     * @param lambda run to check if condition was met.
     * @return true if lmabda returned true.
     * @see notify_one
     * @see notify_all
     */
    template<class Lambda> bool wait_for( mutex &mtx, int millis, Lambda lambda);

    /** Wait for condition to be signaled within a given time frame.
     *
     * This method atomically release mutex and cause the calling thread to block; atomically here means "atomically with respect to
     * access by another thread to the mutex and then the condition variable". Call notify_one or notify_all to signal the condition.
     *
     * Upon successful return, the mutex has been locked and is owned by the calling thread.
     *
     * The lambda (closure) is run to check if the condition was met. Lambda should return false if the waiting should be continued.
     * The signature of the predicate function should be equivalent to the following: bool lambda();
     *
     * @param lck ralated mutex lock_guard, which must be locked by the current thread.
     * @param millis milli seconds to wait for condition to be signaled.
     * @param lambda run to check if condition was met.
     * @return true if lmabda returned true.
     * @see notify_one
     * @see notify_all
     */
    template<class Lambda> bool wait_for( lock_guard<pthread::mutex> &lck, int millis, Lambda lambda);

    /** Signal one waiting thread.
     *
     * The call unblocks at least one of the threads that are blocked on the specified condition variable cond (if any threads are blocked on cond).
     */
#if __cplusplus < 201103L
    void notify_one () throw() ;
#else
    void notify_one () noexcept;
#endif

    /** Signal all waiting threads.
     *
     * The call unblocks all threads currently blocked on the specified condition variable cond.
     */
#if __cplusplus < 201103L
    void notify_all () throw() ;
#else
    void notify_all () noexcept ;
#endif

    /**
     * not copy-assignable
     */
     void operator=( const condition_variable &) = delete;

    // constructor/destructor ------------------------------------------------

    /** construct a new condition_variable (pthread_cond_init).
     */
    condition_variable ();

    /** destroy a condition_variable (pthread_cond_destroy)
     *
     */
    virtual ~condition_variable();

  private:

    void milliseconds( int milliseconds);

    timespec timeout;
    pthread_cond_t _condition; //!< NOSONAR this union is declared in the POSIX Threading library. It cannot be changed (ignoring rule MISRA C++:2008, 9-5-1 - Unions shall not be used.)
  };

  /** @} */

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

    //return wait( *(lck.mutex()), lambda);
    return wait( *(lck._mutex), lambda);
  };

  template<class Lambda> bool condition_variable::wait_for( mutex &mtx, int millis, Lambda lambda){
    int rc = 0;
    cv_status status = no_timeout;

    milliseconds(millis); // update timeout
    bool stop_waiting = lambda(); // returns ​false if the waiting should be continued.

    while((! stop_waiting) && (status == no_timeout)){

      rc  = pthread_cond_timedwait ( &_condition, &mtx._mutex, &timeout );

      switch (rc){

        case ETIMEDOUT:
          status = timedout;
          break;

        case EINVAL:
          throw condition_variable_exception("The value specified by abstime is invalid.", rc); // NOSONAR we use throw instead of return.
          break;

        case EPERM:
          throw condition_variable_exception("The mutex was not owned by the current thread at the time of the call.", rc); // NOSONAR we use throw instead of return.
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

    // return wait_for(*(lck.mutex()),millis, lambda);
    return wait_for(*(lck._mutex),millis, lambda);
  };


} // namespace pthread


#endif
