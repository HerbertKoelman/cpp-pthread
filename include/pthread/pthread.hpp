//
//  cpp_pthread.hpp
//  cpp_pthread
//
//  Created by herbert koelman on 18/03/2016.
//
//

/**
 * \author herbert koelman
 * \date 18/03/2016
 */

#ifndef pthread_pthread_hpp
#define pthread_pthread_hpp

#include <errno.h>
#include <exception>
#include <string>

#include "pthread/config.h"

#include "pthread/mutex.hpp"
#include "pthread/lock_guard.hpp"
#include "pthread/condition_variable.hpp"
#include "pthread/thread.hpp"

/** \namespace pthread 
 *
 * C++11 pthread mock implementations
*/
namespace pthread {

  extern "C" const char *cpp_pthread_version();
  
}
#endif /* pthread_pthread_hpp */
