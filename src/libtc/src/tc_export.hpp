#ifndef TC_EXPORT_INCLUDE_HPP
#define TC_EXPORT_INCLUDE_HPP

#ifdef _WIN32
    #ifdef tc_EXPORTS
        #define TC_CLASS_API __declspec(dllexport)
    #elifdef tc_EXPORT_ALL
        #define TC_CLASS_API
    #else
        #define TC_CLASS_API __declspec(dllimport)
    #endif
    #define TC_HIDDEN
#else
    #define TC_CLASS_API __attribute__((visibility("default")))
    #define TC_HIDDEN __attribute__((visibility("hidden")))
#endif

#endif
