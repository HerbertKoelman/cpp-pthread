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
    pthread_exception( const char *message, const int pthread_errno = 0 );

    /**
     * @param message error message
     * @param pthread_errno a pthread function return code.
     */
    pthread_exception( const string message, const int pthread_errno = 0 );

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
    //std::string  _message;
    const char  *_message;
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
    timeout_exception(const string message);
  };

} // namespace pthread
#endif
