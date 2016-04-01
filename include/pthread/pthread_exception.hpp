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

namespace pthread {
  
  using namespace std ;
  
  /** general purpose pthread exception.
   */
  class pthread_exception: public exception {
    
  public:
    
#ifdef __IBMCPP__
    pthread_exception( const string message, const int pthread_number = 0 ): _message(message), _pthread_errno(pthread_number){};
#else
    pthread_exception( const string message, const int pthread_number = 0 ): _message{message}, _pthread_errno{pthread_number}{};
#endif
    
    virtual ~pthread_exception(){};
    
    virtual const char *what() const __NOEXCEPT__ __OVERRIDE__ { return _message.c_str();};
    
    virtual int pthread_errno(){ return _pthread_errno ;};
    
    virtual const char *pthread_errmsg(){ return strerror(_pthread_errno );};
    
  private:
    string  _message;
    int     _pthread_errno;
    
  };
  
  /** pthread operation timed out.
   */
  class timeout_exception: public pthread_exception{
  public:
    timeout_exception(const string message): pthread_exception(message, ETIMEDOUT){};
  };

} // namespace pthread
#endif
