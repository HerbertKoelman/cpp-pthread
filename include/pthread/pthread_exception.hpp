//
//  mutex.hpp
//  cpp-pthread
//
//  Created by herbert koelman on 18/03/2016.
//
//

#ifndef pthread_pthread_exception_hpp
#define pthread_pthread_exception_hpp

#include "pthread/config.h"

#include <errno.h>
#include <cstring>

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
    pthread_exception( const string message, const int pthread_errno = 0 ): _message(message), _pthread_errno(pthread_errno){};

    virtual ~pthread_exception(){};
    
    /** @return exception's error message. */
#if __cplusplus < 201103L
    virtual const char *what() const throw() { return _message.c_str();};
#else
    virtual const char *what() const noexcept override{ return _message.c_str();};
#endif    

    /** @return pthread error code that was at the orgin of the error */
    virtual int pthread_errno(){ return _pthread_errno ;};
    
    /** @return related pthread_errno error message using strerror */
    virtual const char *pthread_errmsg(){ return strerror(_pthread_errno );};
    
  private:
    string  _message;
    int     _pthread_errno;
    
  };
  
  /** pthread operation timed out.
   */
  class timeout_exception: public pthread_exception{
  public:
    /** thrown when a time out occurs.
     *
     * @param message timeout condition
     */
    timeout_exception(const string message): pthread_exception(message, ETIMEDOUT){};
  };

} // namespace pthread
#endif
