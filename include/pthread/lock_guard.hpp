//
//  lock_guard.hpp
//  lock_guard
//
//  Created by herbert koelman on 18/03/2016.
//
//

#ifndef pthread_lock_guard_H
#define pthread_lock_guard_H

// WARN pthread.h must be include as first hearder file of each source code file (see IBM's
// recommandation for more info p.285 chapter 8.3.1).
#include <pthread.h>

#include "pthread/mutex.hpp"

namespace pthread {

    /** \addtogroup concurrency
     *
     * @{
     */

    /**
     * This class was designed to encapsulate a mutex and automatically control the lock attribute.
     *
     * The lock_guard lock the associated mutex once we instanciate the class and the lock is automatically unlocked
     * once the object is destroyed. This allow us to correlate the lock with the scope of the object.
     *
     * > *WARN* this is a wrapper and therefore doesn't deal with allocating/deallocating the mutex it is guarding.
     *
     * @author herbert koelman
     * @tparam MutexType a kind of mutex
     */
    template<class MutexType>
    class lock_guard {

        friend class condition_variable;

    public:
        /**
         * The constructor is forced to only accept a mutex object or any object of a subclass.
         *
         * > *WARN* The mutex is locked up upon completion.
         *
         * @param mutex reference to a valid mutex type.
         * @see mutex#lock
         */
        explicit lock_guard(MutexType &mutex) { //: _mutex(&m) {
            _mutex = &mutex;
            _mutex->lock();
        }

        /** The destructor release the guarded mutex.
         *
         * @see mutex#unlock
         */
        ~lock_guard() {
            _mutex->unlock();
        }

        /** @return a const reference to the guarded mutex
         *
         * @deprecated condition_variable is now a friend class
         * @see condition_variable
         */
        MutexType *internal_mutex() {
            return _mutex;
        }

        /**Cannot be copied.
         */
        lock_guard(const lock_guard &) = delete;

        /**
         * Desabling the = operator.
         */
        void operator=(lock_guard &) = delete;

    private:
        MutexType *_mutex;
    };

    /** @} */
} // namespace pthread

#endif /* pthread_lock_guard_H */
