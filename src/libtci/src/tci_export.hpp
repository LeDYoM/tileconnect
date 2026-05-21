#ifndef TCI_EXPORT_INCLUDE_HPP
#define TCI_EXPORT_INCLUDE_HPP

#ifdef _WIN32
    #ifdef tci_EXPORTS
        #define TCI_CLASS_API __declspec(dllexport)
    #elifdef tci_EXPORT_ALL
        #define TC_CLASS_API
    #else
        #define TCI_CLASS_API __declspec(dllimport)
    #endif
    #define TCI_HIDDEN
#else
    #define TCI_CLASS_API __attribute__((visibility("default")))
    #define TCI_HIDDEN __attribute__((visibility("hidden")))
#endif

#endif
