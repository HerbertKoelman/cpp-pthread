// Implement missing C++11 features
//
// More here http://www.ibm.com/support/knowledgecenter/SSGH3R_13.1.2/com.ibm.xlcpp131.aix.doc/proguide/rvaluereferences.html
//

//#ifdef __IBMCPP__

namespace ibm {
    template <typename T> struct remove_reference { 
        typedef T type; 
    };
    template <typename T> struct remove_reference<T&> { 
        typedef T type; 
    };
    template <typename T> struct remove_reference<T&&> { 
        typedef T type;  
    };

    namespace Impl {
        template <typename T> struct NotAnLvalueReference { 
            enum { value = 1 };
        };
        template <typename T> struct NotAnLvalueReference<T&> { 
            enum { value = 0 }; 
        };
    }

    template <typename T> inline 
    T &&forward(typename remove_reference<T>::type &t) {
        return static_cast<T &&>(t);
    }
 
    template <typename T> inline 
    T &&forward(typename remove_reference<T>::type &&t) {
        static_assert(Impl::NotAnLvalueReference<T>::value, 
                      "T cannot be an lvalue reference type when "
                      "calling this overload.");
        return static_cast<T &&>(t);
    }
 
    template <typename T> inline 
    typename remove_reference<T>::type &&move(T &&t) {
        return static_cast<typename remove_reference<T>::type &&>(t);
    }
}
//#endif
