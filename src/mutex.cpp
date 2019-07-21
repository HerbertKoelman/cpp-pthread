//
//  mutex.cpp
//  cpp-pthread
//
//  Created by herbert koelman on 18/03/2016.
//
//

#include "pthread/mutex.hpp"

namespace pthread {

    mutex::mutex() {
        auto rc = pthread_mutex_init(&_mutex, NULL);
        if (rc != 0) {
            throw mutex_exception("In constructor of mutex pthread_mutex_init(&mutex, NULL) failed. ", rc);
        }
    }

    mutex::~mutex() {
        pthread_mutex_destroy(&_mutex);
    }

    void mutex::lock() {
        int rc = -1;
        rc = pthread_mutex_lock(&_mutex);
        if (rc != 0) {
            throw mutex_exception("pthread_mutex_lock failed.", rc);
        }
    }

    bool mutex::try_lock() {
        bool status = false;

        auto rc = pthread_mutex_trylock(&_mutex);
        if (rc == 0) {
            status = true;
        } else {
            throw mutex_exception("pthread_mutex_trylock failed, already locked.", rc);
        }

        return status ;
    }

    void mutex::unlock() {
        auto rc = pthread_mutex_unlock(&_mutex);
        if (rc != 0) {
            throw mutex_exception("pthread_mutex_unlock failed.", rc);
        }
    }

}
