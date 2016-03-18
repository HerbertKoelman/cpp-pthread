/*
 $Id: condition_variable.C 28 2007-08-06 22:29:28Z hkoelman $
 */

#include <time.h>
#include <sys/time.h>

#include "pthread/condition_variable.hpp"

namespace pthread {
  
  void condition_variable::wait(mutex &mtx) {
    pthread_cond_wait ( &_condition, &mtx._mutex);
  }
  
  /* Default millis is 0 */
  cv_status condition_variable::wait_for ( mutex &mtx, int millis ) {
    int rc = 0;
    cv_status status = cv_status::no_timeout;
    
    timeval current;
    timespec timeout;
    
    gettimeofday ( &current, NULL );
    timeout.tv_sec = current.tv_sec + (millis * 0.001);
    
    rc  = pthread_cond_timedwait ( &_condition, &mtx._mutex, &timeout );
    
    switch (rc){
        
      case ETIMEDOUT:
        status = cv_status::timeout;
        break;
        
      default:
        status = cv_status::no_timeout ;
        break;
    }
    
    return status;
  }
  
  void condition_variable::notify_one() noexcept {
    pthread_cond_signal ( &_condition );
  }
  
  void condition_variable::notify_all () noexcept{
    pthread_cond_broadcast ( &_condition );
    
  }
  
  // constuctors & destructors --------------
  
  condition_variable::condition_variable () {
    int rc = pthread_cond_init ( &_condition, NULL );
    if ( rc != 0 ){
      throw condition_variable_exception("pthread_cond_init failed.", rc);
    }
  }
  
  condition_variable::~condition_variable () {
    if (pthread_cond_destroy(&_condition) != 0){
      throw pthread_exception("pthread condition variable destroy failed.", 0);
    }
  }
  
  // excpetions -------
  
  condition_variable_exception::condition_variable_exception( const string message, const int pthread_error): pthread_exception{message, pthread_error}{
  };
  
  
} // namespace pthread
