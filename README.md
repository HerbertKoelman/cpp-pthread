# what it does

IBM's compiler is not implement all the features of C++11 standard, especially it's laking the concurrency features that the standard brings.

This wrapper intends to bring these feature by wrapping the pthread library.  Of course, as it is a replacement of C++11 features, it is best to use the standard implementation if your compiler support it.

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

More [here](https://github.com/HerbertKoelman/cpp-pthread/wiki)