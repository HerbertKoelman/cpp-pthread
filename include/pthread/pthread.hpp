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

// WARN pthread.h must be include as first header file of each source code file (see IBM's
// recommendation for more info p.285 chapter 8.3.1).
#include <pthread.h>

#include "pthread/mutex.hpp"
#include "pthread/read_write_lock.hpp"
#include "pthread/lock_guard.hpp"
#include "pthread/condition_variable.hpp"
#include "pthread/thread.hpp"
#include "pthread/sync_queue.hpp"
#include "pthread/exceptions.hpp"

/** \namespace pthread
 *
 * C++11 pthread mock implementations
*/
namespace pthread {

    /**
     *  @example concurrency_tests.cpp
     *  @example thread_tests.cpp
     *  @example synchronized_queue_tests.cpp
     *  @example exceptions_tests.cpp
     *  @example abstract_thread_tests.cpp
     */

  /** @return library version */
  const char *cpp_pthread_version();

}
#endif /* pthread_pthread_hpp */
