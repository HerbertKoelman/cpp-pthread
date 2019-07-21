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

// must be include as first hearder file of each source code file (see IBM's
// recommandation for more info p.285 �8.3.1).
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

  /** @return library version */
  const char *cpp_pthread_version();

}
#endif /* pthread_pthread_hpp */
