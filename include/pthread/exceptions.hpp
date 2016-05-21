//
//  exceptions.hpp
//  cpp-pthread
//
//  Created by herbert koelman on 18/03/2016.
//
//

#ifndef PTHREAD_EXCEPTIONS_HPP
#define PTHREAD_EXCEPTIONS_HPP

#include <pthread.h>

#include <errno.h>
#include <string>    // std::string
#include <cstring>
#include <exception> // std::exception

namespace pthread {
  
  using namespace std ;
  
  /** general purpose pthread exception.
   */
  class pthread_exception: public exception {
    
  public:
    
    /**
     * @param message error message
     * @param pthread_errno a pthread function return code.
     */
//    pthread_exception( const char *message, const int pthread_errno = 0 );

    /**
     * @param message error message
     * @param pthread_errno a pthread function return code.
     */
    pthread_exception( const string &message, const int pthread_errno = 0 );

    virtual ~pthread_exception();
    
    /** @return exception's error message. */
#if __cplusplus < 201103L
    virtual const char *what() const throw() ;
#else
    virtual const char *what() const noexcept override ;
#endif    

    /** @return pthread error code that was at the orgin of the error */
    virtual int pthread_errno() ;
    
    /** @return related pthread_errno error message using strerror */
    virtual const char *pthread_errmsg() ;
    
  private:
    std::string  _message;
    //const char  *_message;
    int          _pthread_errno;
    
  };
  
  /** pthread operation timed out.
   */
  class timeout_exception: public pthread_exception{
  public:
    /** thrown when a time out occurs.
     *
     * @param message timeout condition
     */
    timeout_exception(const string &message);
  };

  /** throw to indicate that something went wrong with a mutex.
   */
  class mutex_exception: public pthread_exception {
  public:

    /** thrown when mutex actions fail
     *
     * @param message short description
     * @param pthread_errno error returned by the pthread function
     */
    mutex_exception( const std::string &message, const int pthread_errno = 0) ;
    
  };

  /** Condition variable exception
   */
  class condition_variable_exception: public pthread_exception {
  public:

    /** thrown when mutex actions fail
     *
     * @param message short description
     * @param pthread_errno error returned by the pthread function
     */
    condition_variable_exception( const string &message, const int pthread_errno = 0);
    virtual ~condition_variable_exception(){};
  };
  
  /** thrown to indicate that something went wrong with a thread */
  class thread_exception: public pthread_exception {
  public:
    /**
     * @param message short error description.
     * @param pthread_error value return by a function in the pthread library.
     */
    thread_exception(const string &message, const int pthread_error = 0);
  };

} // namespace pthread
#endif
