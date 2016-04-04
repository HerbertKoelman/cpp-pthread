//
//  cpp_pthread.hpp
//  cpp_pthread
//
//  Created by herbert koelman on 18/03/2016.
//
//

#ifndef pthread_pthread_hpp
#define pthread_pthread_hpp

#include "pthread/pthread.hpp"
#include "pthread/config.h"


namespace pthread {

extern "C" const char *cpp_pthread_version(){
#ifndef CPP_PTHREAD_VERSION
  return "missing CPP_PTHREAD_VERSION define. Re-run configure" ;
#else
  return CPP_PTHREAD_VERSION ;
#endif
}

}
#endif /* pthread_pthread_hpp */
