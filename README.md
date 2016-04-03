# What it does

IBM's compiler is not implementing all the features of C++11 standard, especially it's lacking the concurrency features that the standard brings. This will at some point be fixed and was therfore looking at a way reduce the effort to switch from a specific implementation to the C++11 standard one. This projetc is the resulting code.


This wrapper intends to bring these feature by implementing C++11 interface and using the  pthread library.  Of course, as it is a replacement of C++11 features, it is best to use the standard implementation if your compiler support it. This can be done rather easely by using the standard namespace `std` instead of this library's specific one `pthread`.


To use this library:
```
configure
make
make install
```

Install moves files into your system's default localtion of headers and libraries (often /usr/local/include and /usr/local/lib). Use this command to change target directory:
```
configure --prefix=/usr/local
```

[Documentation](http://herbertkoelman.github.io/cpp-pthread/doc/html/) can be generated with this command:
```
make doxygen
...
```

This command creates a `doc` directory wich will contain the generated documentation.

> **warning** generating documentation requires that doxygen is installed.

# How to use it

Once compiled and installed in a location that suites you, use your compiler options to reference the headers and the libraries directory. In almoast all casses you can:
* include `#include "pthread/phtread.hpp"` in your code.
* comment the c++11 standard includes in your code
* declare that you're now using the namespace pthread (`using namespace pthread ;`)

It should now compile use this very simple (but often good enough) implementation.

# Usefull links

* [documentation](http://pubs.opengroup.org/onlinepubs/007908799/xsh/threads.html) of the underlying POSIX threading library 
* [project's home](https://github.com/HerbertKoelman/cpp-pthread)
* [project's doxygen](http://herbertkoelman.github.io/cpp-pthread/doc/html/)
* [std::thread](http://en.cppreference.com/w/cpp/thread/thread) implementation we try to mimic
* [std::lock_guard](http://en.cppreference.com/w/cpp/thread/lock_guard/lock_guard)  implementation we try to mimic 
* [std::mutex](http://en.cppreference.com/w/cpp/thread/mutex) implementation we try to mimic
* [std::condition_variable](http://en.cppreference.com/w/cpp/thread/condition_variable) implementation we try to mimic

More [here](https://github.com/HerbertKoelman/cpp-pthread/wiki)

# misc

* author herbert koelman
* github [cpp-pthread](https://github.com/HerbertKoelman/cpp-pthread)
