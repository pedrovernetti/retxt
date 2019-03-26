#ifndef _GLOBALUTILITIES_INCLUDED
#define _GLOBALUTILITIES_INCLUDED

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * * macros (defined as positive integer literals or undefined, accourding to its meaning):
 *
 * * * on the target OS:
 *
 * _COMPILING_FOR_WINDOWS                target OS is any version of Windows
 * _COMPILING_FOR_MACH                   target OS is any one based on Mach kernel
 * _COMPILING_FOR_APPLE                  target OS is any of those belonging to Apple
 * _COMPILING_FOR_WATCHOS                target OS is watchOS (Apple Watch OS)
 * _COMPILING_FOR_IOS_SIMULATOR          target OS is iOS (simulator)
 * _COMPILING_FOR_IOS                    target OS is iOS (iPhone and iPad OS)
 * _COMPILING_FOR_OSX                    target OS is Mac OS X
 * _COMPILING_FOR_ANDROID                target OS is Android
 * _COMPILING_FOR_AIX                    target OS is AIX
 * _COMPILING_FOR_LINUX                  target OS is any one based on Linux kernel
 * _COMPILING_FOR_GNULINUX               target OS is GNU/Linux
 * _COMPILING_FOR_OTHER_UNIX_SYSTEMS     target OS is any UNIX system not covered by other macros
 * _COMPILING_FOR_UNIX                   target OS is any UNIX system
 * _COMPILING_FOR_POSIX_COMPLIANTS       target OS is any POSIX-compliant system
 * _COMPILING_FOR_UNKNOWN_OS             target OS is unknown (not any of the ones above)
 *
 * * * on C/C++ features and full versions support:
 *
 *   * (specific feature support macros are defined as an integer representing the supported version of
 *   * the feature; e.g.: if v0.9 lambda expressions are supported, _SUPPORTS_LAMBDA_EXPRESSIONS is
 *   * defined as 09, otherwise if v1.0 is supported, it is defined as 10, and so on)
 *
 * _NONCPP                               code won't be compiled as C++ (non-C++ code only)
 * _NO_C99_SUPPORT                       no reliable support for C99 (non-C++ code only)
 *
 * * strings (std::string("...") - if _NONCPP is undefined, string literal otherwise):
 *
 * compilationDate                       date when the compilation occurred
 * compilationTime                       time when the compilation occurred
 * compilationDateAndTime                date/time when the compilation occurred (C++ code only)
 * thisFileName                          name/path (what the compiler takes) of the file it's within
 * thisFileModificationDateAndTime       last modification date/time of the file it's within
 * currentFunctionName                   name of the function it's within
 * targetOSName                          name of the OS which is being targeted
 * compilerNameAndVersion                name and version of the used compiler
 *
 *
 * * values (size_t):
 *
 * currentLine                           number of the line it's placed at
 * previousLine                          number of the line before the one it's placed at
 *
 * * values (respective integral type):
 *
 * maximumShortValue                     maximum value of the 'short int' type
 * minimumShortValue                     minimum value of the 'short int' type
 * maximumIntegerValue                   maximum value of the 'int' type
 * minimumIntegerValue                   minimum value of the 'int' type
 * maximumLongValue                      maximum value of the 'long int' type
 * minimumLongValue                      minimum value of the 'long int' type
 *
 * * booleans:
 *
 * longIntIsInt                          true if 'long int' and 'int' actually have equal sizes
 *
 * * functions:
 *
 * maximumSignedValueOf(type)            returns the maximum value of 'type' (for signed integral types only)
 * minimumSignedValueOf(type)            returns the minimum value of 'type' (for signed integral types only)
 * maximumUnsignedValueOf(type)          returns the maximum value of 'type' (for unsigned integral types only)
 * minimumUnsignedValueOf(type)          returns 0
 *
 * 
 * * aliases:
 *
 * byte                                  'unsigned char'
 *
 * * renamed typedefs from stdint.h/cstdint:
 *
 *  (signed types)      (unsigned types)
 *
 *    Integer type with the maximum width supported:
 *
 *    intmax               uintmax
 *
 *    Integer types with a width of exactly 8, 16, 32, or 64 bits.
 *
 *    int8                 uint8
 *    int16                uint16
 *    int32                uint32
 *    int64                uint64
 *
 *    Integer type with a minimum of 8, 16, 32, or 64 bits.
 *    No other integer type exists with lesser size and at least
 *  the specified width.
 *
 *    intleast8            uintleast8
 *    intleast16           uintleast16
 *    intleast32           uintleast32
 *    intleast64           uintleast64
 *
 *    Integer type with a minimum of 8, 16, 32, or 64 bits.
 *    At least as fast as any other integer type with at least
 *  the specified width.
 *
 *    intfast8             uintfast8
 *    intfast16            uintfast16
 *    intfast32            uintfast32
 *    intfast64            uintfast64
 *
 *    Integer type capable of holding a value converted from/to
 *  a void pointer without changing its value.
 *
 *    intptr                uintptr
 *
 *    =>    The above typedefs may not be defined in some library
 *        implementations and *will* not be defined if no types with
 *         such characteristics exist.
 * 
 * 
 * * types comparison
 * 
 * isTheSameType<type1, type2>::value     true if type1 and type2 are the same type
 * isTheSameType(type1, type2)             same as "isTheSameType<type1, type2>::value"
 * isBooleanType(type)                     true if type is the same as "bool"
 * isCharacterType(type)                 true if type is a character type (char, wchar_t...)
 * isSignedIntegerType(type)             true if type is a signed integer type (int, long int...)
 * isUnsignedIntegerType(type)             true if type is an unsigned integer type (size_t, unsigned int...)
 * isIntegerType(type)                     true if type is an integer type (size_t, int...) 
 * isIntegralType(type)                     true if type is an integral type (size_t, char, int, bool...)
 * isFloatingPointType(type)             true if type is a floating point type (float, double...)
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 */

#if defined(__cplusplus)
    #include <string>
    #define __PROPER_STR(a) std::string(a)
#else
    #pragma message ( "C++ features may be unsupported" )
    #if __STDC_VERSION__ < 199901L
        #define _NO_C99_SUPPORT (1)
        #pragma message ( "C99 not fully supported" )
    #endif
    #define _NONCPP (1)
    #define __PROPER_STR(a) (a)
#endif

#if __STDC_VERSION__ >= 199901L
#elif defined(__FUNCTION__)
    #define currentFunctionName __PROPER_STR(__FUNCTION__)
#endif
// #define currentFunctionSignature __PROPER_STR(__funcsig__)

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * target OS detection                                               *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 */

// Windows
#if defined(_WIN32) || defined(_WIN64) || defined(Q_OS_WIN32)
    #define _COMPILING_FOR_WINDOWS
    #define targetOSName (__PROPER_STR("Windows"))
    #if defined(_WIN64) || defined(Q_OS_WIN64)
        #define _COMPILING_FOR_64BITS_ARCHITECTURE
    #endif

// Mach-based systems
#elif defined(__MACH__)
    #define _COMPILING_FOR_MACH
    #if defined(__APPLE__)
        #define _COMPILING_FOR_APPLE
        #include "TargetConditionals.h"
        #if TARGET_IPHONE_SIMULATOR
            #define _COMPILING_FOR_IOS_SIMULATOR
            #define targetOSName (__PROPER_STR("iOS (simulator)"))
        #elif TARGET_OS_WATCH
            #define _COMPILING_FOR_WATCHOS
            #define targetOSName (__PROPER_STR("watchOS"))
        #elif TARGET_OS_IPHONE
            #define _COMPILING_FOR_IOS
            #define targetOSName (__PROPER_STR("iOS"))
        #else
            #define _COMPILING_FOR_OSX
            #define targetOSName (__PROPER_STR("OS X"))
        #endif
    #else
        #define targetOSName (__PROPER_STR("some Mach-based OS"))
    #endif

// Android
#elif defined(__ANDROID__) || defined(ANDROID) || defined(Q_OS_ANDROID)
    #define _COMPILING_FOR_ANDROID
    #define targetOSName (__PROPER_STR("Android"))

// AIX
#elif defined(_AIX) || defined(Q_OS_AIX)
    #define _COMPILING_FOR_AIX
    #define targetOSName (__PROPER_STR("AIX"))

// GNU/Linux
#elif defined(__linux) || defined(__linux__) || defined(linux) || defined(Q_OS_LINUX)
    #define _COMPILING_FOR_LINUX
    #if defined(__gnu_linux__)
        #define _COMPILING_FOR_GNULINUX
        #define targetOSName (__PROPER_STR("GNU/Linux"))
    #else
        #define targetOSName (__PROPER_STR("some Linux-based OS"))
    #endif

// UNIX systems
#elif defined(__unix) || defined(__unix__) || defined(unix) || defined(Q_OS_UNIX)
    #define _COMPILING_FOR_OTHER_UNIX_SYSTEMS
    #define targetOSName (__PROPER_STR("some UNIX system"))
#endif
#if defined(__unix) || defined(__unix__) || defined(unix) || defined(Q_OS_UNIX)
    #define _COMPILING_FOR_UNIX
#endif

// POSIX-compliant systems
#if defined(__posix)
    #define _COMPILING_FOR_POSIX_COMPLIANTS
    #ifndef targetOSName
        #define targetOSName (__PROPER_STR("some POSIX-compliant OS"))
    #endif
#endif

// Unknown OS
#ifndef targetOSName
    #define _COMPILING_FOR_UNKNOWN_OS
    #pragma message ( "unknown target OS" )
    #define targetOSName (__PROPER_STR("unknown OS"))
#endif

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * compiler identification / features support                        *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 */

#define compilerNameAndVersion (__PROPER_STR("?"))
#define compilerLongName (__PROPER_STR("?"))
#define compilerShortName (__PROPER_STR("?"))

//

// C++11 full support detection
#if _SUPPORTS_FULL_CPP11_LANG && _SUPPORTS_FULL_CPP11_LIB
#define _FULL_CPP11_SUPPORT
#endif

// C++14 full support detection
#if _SUPPORTS_FULL_CPP14_LANG && _SUPPORTS_FULL_CPP14_LIB
#define _FULL_CPP14_SUPPORT
#endif

// C++17 full support detection
#if _SUPPORTS_FULL_CPP17_LANG && _SUPPORTS_FULL_CPP17_LIB
#define _FULL_CPP17_SUPPORT
#endif

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * utility strings and values                                        *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 */

// utility strings
#define compilationDate (__PROPER_STR(__DATE__))
#define compilationTime (__PROPER_STR(__TIME__))
#ifndef _NONCPP
    #define compilationDateAndTime (__PROPER_STR(__PROPER_STR(__DATE__) + " " + __TIME__))
#endif
#define thisFileName (__PROPER_STR(__FILE__))
#define thisFileModificationDateAndTime (__PROPER_STR(__TIMESTAMP__))

// utility values
#define currentLine ((size_t)(__LINE__))
#define previousLine ((size_t)((__LINE__ > 0) ? (__LINE__ - 1) : 0))
#define maximumShortValue ((short int)(((short unsigned int)(-1)) / 2))
#define minimumShortValue ((short int)((((short unsigned int)(-1)) / 2) + 1))
#define maximumIntegerValue ((int)(((unsigned int)(-1)) / 2))
#define minimumIntegerValue ((int)((((unsigned int)(-1)) / 2) + 1))
#define maximumLongValue ((long int)(((long unsigned int)(-1)) / 2))
#define minimumLongValue ((long int)((((long unsigned int)(-1)) / 2) + 1))

// utility booleans
#define longIntIsInt (maximumValueOf(long int) == maximumValueOf(int))

// utility functions
#if defined(_COMPILING_FOR_64BITS_ARCHITECTURE) && (!defined(_NO_C99_SUPPORT))
    #define maximumSignedValueOf(typeName) ((long long int)((typeName)(((unsigned typeName)(-1)) / 2)))
    #define minimumSignedValueOf(typeName) ((long long int)((typeName)((((unsigned typeName)(-1)) / 2) + 1)))
    #define maximumUnsignedValueOf(typeName) ( \
            (0 < ((typeName)((unsigned long long int)((typeName)(-1))))) ? \
                ((unsigned long long int)((typeName)(-1))) : 0 \
        )
#else
    #define maximumSignedValueOf(typeName) ((long int)((typeName)(((unsigned typeName)(-1)) / 2)))
    #define minimumSignedValueOf(typeName) ((long int)((typeName)((((unsigned typeName)(-1)) / 2) + 1)))
    #define maximumUnsignedValueOf(typeName) ( \
            (0 < ((typeName)((unsigned long int)((typeName)(-1))))) ? \
                ((unsigned long int)((typeName)(-1))) : 0 \
        )
#endif
#define minimumUnsignedValueOf(typeName) ((typeName)(0))

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * type aliases                                                      *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 */
 
#ifdef _NONCPP
    #include <stdint.h>
    typedef unsigned char byte;
#else
    #include <cstdint>
    using byte = unsigned char;
#endif

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * types comparison                                                  *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 */
 
template<typename generic1, typename generic2>
struct isTheSameType 
{
    enum : bool { value = false };
};

template <typename generic>
struct isTheSameType<generic, generic>
{
    enum : bool { value = true };
};

#define isTheSameType(generic1, generic2) isTheSameType<generic1,generic2>::value

#define isBooleanType(generic)           (isTheSameType(generic, bool))

#define isCharacterType(generic)       (isTheSameType(generic, char) || \
                                        isTheSameType(generic, unsigned char) || \
                                        isTheSameType(generic, wchar_t) || \
                                        isTheSameType(generic, unsigned wchar_t))
                                    
#define isSignedIntegerType(generic)   (isTheSameType(generic, short int) || \
                                        isTheSameType(generic, int) || \
                                        isTheSameType(generic, long int)|| \
                                        isTheSameType(generic, long long int))
                                    
#define isUnsignedIntegerType(generic) (isTheSameType(generic, unsigned short int) || \
                                        isTheSameType(generic, unsigned int) || \
                                        isTheSameType(generic, unsigned long int) || \
                                        isTheSameType(generic, unsigned long long int))

#define isIntegerType(generic)               (isSignedIntegerType(generic) || \
                                        isUnsignedIntegerType(generic))
                                        
#define isIntegralType(generic)           (isSignedIntegerType(generic) || \
                                        isUnsignedIntegerType(generic) || \
                                        isCharacterType(generic) || \
                                        isBooleanType(generic))
                                        
#define isFloatingPointType(generic)   (isTheSameType(generic, float) || \
                                        isTheSameType(generic, double) || \
                                        isTheSameType(generic, long double))

#endif // _GLOBALUTILITIES_INCLUDED

