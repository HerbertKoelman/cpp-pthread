/*
   $Id: condition_variable.C 28 2007-08-06 22:29:28Z hkoelman $
 */
#include <pthread.h>
#include <errno.h>
#include <time.h>
#include <sys/time.h>

#include <condition_variable.hpp>

namespace pthread {

  
  condition_variable::condition_variable () {
    pthread_cond_init ( &_condition, NULL );
  }

  condition_variable::~condition_variable () {
    if (pthread_cond_destroy(&_condition) != 0){
      throw pthread_exception("pthread condition variable destroy failed.");
    }
  }

  int condition_variable::wait() {
    int rc = 0;

    rc  = pthread_cond_wait (&_condition, &_mutex);

    return rc;
  }

  /*
     int condition_variable::wait () {
     int rc = 0 ;

     lock () ;
     rc  = pthread_cond_wait ( &cond, &mutex );

     return rc ;
     }
   */

  /* Default millis is 0 */
  int condition_variable::wait_for ( int millis ) {
    int rc = 0;

    timeval current;
    timespec timeout;
    
    gettimeofday ( &current, NULL );
    timeout.tv_sec = current.tv_sec + (millis * 0.001);

    rc = wait_for ( millis );

    rc  = pthread_cond_timedwait ( &_condition, &_mutex, &timeout );
    if ( rc == 0 || rc == ETIMEDOUT ) {
      rc = unlock ();
    }
    
    return rc;
  }

  /* Default millis is 0 */
  int condition_variable::wait_and_lock ( int millis ) {
    int rc = 0;
    timeval current;
    timespec timeout;

    gettimeofday ( &current, NULL );
    timeout.tv_sec = current.tv_sec + (millis * 0.001);

    rc = lock ();
    if ( rc == 0 ) {
      if ( millis == 0 ) {
        rc  = pthread_cond_wait ( &cond, &mutex );
      } else {
        rc  = pthread_cond_timedwait ( &cond, &mutex, &timeout );
      }
    }

    return rc;
  }

  int condition_variable::signal() {
    int rc = 0;

    rc = pthread_cond_signal ( &cond );

    return rc;
  }

  int condition_variable::broadcast () {

    int rc = 0;

    rc = pthread_cond_broadcast ( &cond );

    return rc;
  }
}
