### What it does

[![Build Status](https://travis-ci.com/HerbertKoelman/cpp-pthread.svg?branch=master)](https://travis-ci.com/HerbertKoelman/cpp-pthread) [![Quality Gate Status](https://sonarcloud.io/api/project_badges/measure?project=HerbertKoelman_cpp-pthread&metric=alert_status)](https://sonarcloud.io/dashboard?id=HerbertKoelman_cpp-pthread) [![codecov](https://codecov.io/gh/HerbertKoelman/cpp-pthread/branch/master/graph/badge.svg)](https://codecov.io/gh/HerbertKoelman/cpp-pthread)

Some C/C++ compilers are not implementing all of C++11 and above standard, it's often lacking the concurrency features that the standard brings. These compilers will at some point be updated. I was therefore looking for a way to reduce the effort of switching from a specific implementation to the C++11 standard one.

This project is the resulting code.

> _WARN_ of course, this library is a replacement of C++11 features, it is best to use the standard implementation if your compiler support it.

To use this library:

    mkdir build && cd build
    cmake ..
    make install

Install moves files into your system's default location for headers and libraries (often /usr/local/include and /usr/local/lib). Use this command line argument to change install target directory:

    cmake -DCMAKE_INSTALL_PREFIX=/your/destination/

Doxygen [documentation](http://herbertkoelman.github.io/cpp-pthread/doc/html/) can be generated with this command. I hope this help make things easier to use and understand.

    make doxygen

> Doxygen can be downloaded [here](http://www.stack.nl/~dimitri/doxygen/index.html).

The `make` target `package` will produce au tar.gz that can be distributed.

### How to use it

Once compiled and installed in a location that suites you, use your compiler options to reference the headers and the library directory. In almost all cases you can:
* include `#include "pthread/phtread.hpp"` in your code to replace of the standard includes.
* replace `std` namespace  with `pthread` ( std::condition_variable becomes pthread::condition_variable, etc)

Once your compiler is upgraded you simply include the standard `#include <thread>` and replace the namespace `pthread` for `std`. 

Sample code can be found in the `tests` directory.

If you use CMake, the test are built and run like this (in your `build` directory):

   make all test
   
### Coverage

The `cmake/CoverageConfig.cmake` package create a make target **coverage**. When used, it will produce a coverage report using `lcov`.

```
$ make coverage
[100%] Coverage report (/shared/home/herbert/c++/cpp-pthread/cmake/LCOV)
Handling directory [./CMakeFiles/cpp-pthread-static.dir/src]
Initializing [coverage.info] file
Handling directory [./googletest-build/googlemock/gtest/CMakeFiles/gtest.dir/src]
Adding tracefile [/tmp/tmp.GQvVYkluCo.coverage] to [coverage.info]
Handling directory [./googletest-build/googlemock/gtest/CMakeFiles/gtest_main.dir/src]
Adding tracefile [/tmp/tmp.4xDeqRUZJO.coverage] to [coverage.info]
Handling directory [./tests/CMakeFiles/abstract_thread_tests.dir]
Adding tracefile [/tmp/tmp.UqJ7PH9H1e.coverage] to [coverage.info]
Handling directory [./tests/CMakeFiles/concurrency_tests.dir]
Adding tracefile [/tmp/tmp.a24dLMFBkh.coverage] to [coverage.info]
Handling directory [./tests/CMakeFiles/exception_tests.dir]
Adding tracefile [/tmp/tmp.GsxpxMbh9W.coverage] to [coverage.info]
Handling directory [./tests/CMakeFiles/synchronized_queue_tests.dir]
Adding tracefile [/tmp/tmp.NxAsqKaPSp.coverage] to [coverage.info]
Handling directory [./tests/CMakeFiles/thread_tests.dir]
Adding tracefile [/tmp/tmp.8J60C5yNeg.coverage] to [coverage.info]
Reading tracefile coverage.info
                                          |Lines       |Functions  |Branches
Filename                                  |Rate     Num|Rate    Num|Rate     Num
================================================================================
[/shared/home/herbert/c++/cpp-pthread/]
include/pthread/condition_variable.hpp    |64.7%     68| 100%     5|    -      0
include/pthread/lock_guard.hpp            | 100%     28| 100%     6|    -      0
include/pthread/sync_queue.hpp            |79.3%     29|77.8%     9|    -      0
include/pthread/thread.hpp                |83.3%     12|83.3%     6|    -      0
src/condition_variable.cpp                |68.8%     64|80.0%    10|    -      0
src/exceptions.cpp                        |91.5%     47|88.2%    17|    -      0
src/mutex.cpp                             |81.8%     33|83.3%     6|    -      0
src/pthread.cpp                           | 100%      2| 100%     1|    -      0
src/read_write_lock.cpp                   |69.6%     46|72.7%    11|    -      0
src/thread.cpp                            |79.0%    162|92.6%    27|    -      0
tests/abstract_thread_tests.cpp           |98.0%     99| 100%    13|    -      0
tests/concurrency_tests.cpp               |93.5%    123| 100%    12|    -      0
tests/exceptions_tests.cpp                |97.8%    181|95.2%    21|    -      0
tests/synchronized_queue_tests.cpp        |94.5%    109|94.4%    18|    -      0
tests/thread_tests.cpp                    |88.1%    201|95.2%    21|    -      0
================================================================================
                                    Total:|87.2%   1204|91.3%   183|    -      0
```

> **WARN** you have to run the test before a report can be displayed (`make all test`).

### Useful links

#### Memory management on AIX

Memory management on AIX is quite sophisticated, memory management can be fine tuned very precisely. Consider using these compiler/linker options when using pthreads:
* -bmaxdata:0xN0000000 this option activates the large memory model, N is a number in the range of [1-8].
* -bmaxmem=-1 this option tells the compiler to use as much memory it needs to optimize your code.

Thread stack size:
* 32bits programs allocate 96KB per thread on the program's heap.
* 64bits programs allocate 192KB per thread on the program's heap.

On many Linux implementations and on Mac OS X the stack size is defaulted to 8MB. You may consider setting this as a default.

More detailed information can be found in this [RedBook](http://www.redbooks.ibm.com/redbooks/pdfs/sg245674.pdf) (chapter 8).

#### project links

* [project's home](https://github.com/HerbertKoelman/cpp-pthread)
* [project's doxygen](http://herbertkoelman.github.io/cpp-pthread/doc/html/)

#### other things

* POSIX Threads [documentation](http://pubs.opengroup.org/onlinepubs/007908799/xsh/threads.html)
* [std::thread](http://en.cppreference.com/w/cpp/thread/thread) implementation we try to mimic
* [std::lock_guard](http://en.cppreference.com/w/cpp/thread/lock_guard/lock_guard)  implementation we try to mimic 
* [std::mutex](http://en.cppreference.com/w/cpp/thread/mutex) implementation we try to mimic
* [std::condition_variable](http://en.cppreference.com/w/cpp/thread/condition_variable) implementation we try to mimic

### Conan

This project can produce Conan Artifacts using these commands:

    $ conan create .. rcs/testing
    ...
    $ conan upload cpp-pthread
    ...

### misc

* author herbert koelman (herbert.koelman@me.com)
* github [cpp-pthread](https://github.com/HerbertKoelman/cpp-pthread)

### Diagrams

![overview](diagrams/threads-classes.png)
