#pragma once
#ifndef _RETXT_GLOBALUTILITIES_INCLUDED
#define _RETXT_GLOBALUTILITIES_INCLUDED

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * * macros (defined as positive integer literals or undefined, accourding to its meaning):
 *
 * * * on the target OS:
 *
 * _COMPILING_FOR_DOS					 target OS is any version of DOS
 * _COMPILING_FOR_MSDOS					 target OS is MS-DOS
 * _COMPILING_FOR_WIN16					 target OS is Windows 3.1 or older versions
 * _COMPILING_FOR_WINDOWS                target OS is Windows (95 and newer)
 * _COMPILING_FOR_MACH                   target OS is any one based on Mach kernel
 * _COMPILING_FOR_APPLE                  target OS is any of those belonging to Apple
 * _COMPILING_FOR_WATCHOS                target OS is watchOS (Apple Watch OS)
 * _COMPILING_FOR_IOS_SIMULATOR          target OS is iOS (simulator)
 * _COMPILING_FOR_IOS                    target OS is iOS (iPhone and iPad OS)
 * _COMPILING_FOR_OSX                    target OS is Mac OS X
 * _COMPILING_FOR_ANDROID                target OS is Android
 * _COMPILING_FOR_AIX                    target OS is AIX
 * _COMPILING_FOR_QNX                    target OS is QNX
 * _COMPILING_FOR_QNXNEUTRINO            target OS is QNX Neutrino
 * _COMPILING_FOR_QNX4                   target OS is QNX 4
 * _COMPILING_FOR_IBMOS400				 target OS is IBM OS/400
 * _COMPILING_FOR_DRAGONFLYBSD			 target OS is DragonFly BSD
 * _COMPILING_FOR_BSDOS					 target OS is BSD/OS
 * _COMPILING_FOR_NETBSD				 target OS is NetBSD
 * _COMPILING_FOR_OPENBSD				 target OS is OpenBSD
 * _COMPILINF_FOR_FREEBSD				 target OS is FreeBSD
 * _COMPILING_FOR_SYLLABLE				 target OS is Syllable
 * _COMPILING_FOR_ECOS					 target OS is eCos
 * _COMPILING_FOR_GNUHURD				 target OS is GNU (GNU/Hurd)
 * _COMPILING_FOR_LINUX                  target OS is any one based on Linux kernel
 * _COMPILING_FOR_GNULINUX               target OS is GNU/Linux
 * _COMPILING_FOR_OTHER_UNIX_SYSTEMS     target OS is any BSD system not covered by other macros
 * _COMPILING_FOR_OTHER_UNIX_SYSTEMS     target OS is any UNIX/UNIX-like system not covered by other macros
 * _COMPILING_FOR_UNIX                   target OS is any UNIX/UNIX-like system
 * _COMPILING_FOR_BSD                    target OS is any BSD system
 * _COMPILING_FOR_POSIX_COMPLIANTS       target OS is any POSIX-compliant system
 * _COMPILING_FOR_UNKNOWN_OS             target OS is unknown (not any of the ones above)
 *
 * * * on C/C++ features and full versions support:
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
 * * aliases from stdint.h/cstdint:
 *
 *   (signed types)       (unsigned types)
 *
 *   Integer type with the maximum width supported:
 *
 *   intmax                uintmax
 *
 *   Integer types with a width of exactly 8, 16, 32, or 64 bits.
 *
 *   int8                  uint8
 *   int16                 uint16
 *   int32                 uint32
 *   int64                 uint64
 *
 *   Integer type with a minimum of 8, 16, 32, or 64 bits.
 *   No other integer type exists with lesser size and at least
 *   the specified width.
 *
 *   intleast8             uintleast8
 *   intleast16            uintleast16
 *   intleast32            uintleast32
 *   intleast64            uintleast64
 *
 *   Integer type with a minimum of 8, 16, 32, or 64 bits.
 *   At least as fast as any other integer type with at least
 *   the specified width.
 *
 *   intfast8              uintfast8
 *   intfast16             uintfast16
 *   intfast32             uintfast32
 *   intfast64             uintfast64
 *
 *   Integer type capable of holding a value converted from/to
 *   a void pointer without changing its value.
 *
 *   intptr                uintptr
 *
 *   => The above typedefs may not be defined in some library
 *      implementations and *will* not be defined if no types with
 *      such characteristics exist.
 * 
 * 
 * * types comparison
 * 
 * isTheSameType<type1, type2>::value    true if type1 and type2 are the same type
 * isTheSameType(type1, type2)           same as "isTheSameType<type1, type2>::value"
 * isBooleanType(type)                   true if type is the same as "bool"
 * isCharacterType(type)                 true if type is a character type (char, wchar_t...)
 * isSignedIntegerType(type)             true if type is a signed integer type (int, long int...)
 * isUnsignedIntegerType(type)           true if type is an unsigned integer type (size_t, unsigned int...)
 * isIntegerType(type)                   true if type is an integer type (size_t, int...) 
 * isIntegralType(type)                  true if type is an integral type (size_t, char, int, bool...)
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
#else
	#if defined(__FUNCTION__)
		#define currentFunctionName (__PROPER_STR(__FUNCTION__))
	#endif
	#if defined(__funcsig__)
		#define currentFunctionSignature (__PROPER_STR(__funcsig__))
	#else
		#define currentFunctionSignature currentFunctionName
	#endif
#endif

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * target OS detection                                               *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 */

// DOS
#if defined(__DOS__) || defined(MSDOS) || defined(_MSDOS) || defined(__MSDOS__)
	#define _COMPILING_FOR_DOS
	#if defined(MSDOS) || defined(_MSDOS) || defined(__MSDOS__)
		#define _COMPILING_FOR_MSDOS
		#define targetOSName (__PROPER_STR("MS-DOS"))
	#else
		#define targetOSName (__PROPER_STR("DOS"))
	#endif
	
// Windows CE
#elif defined(_WIN32_WCE)
	#define _COMPILING_FOR_WINDOWSCE
	#define targetOSName (__PROPER_STR("Windows CE"))
	
// Windows 3.1 and older
#elif defined(_WINDOWS) || defined(__WINDOWS__) || defined(__WINDOWS_386__)
	#define _COMPILING_FOR_WIN16
	#if defined(__WINDOWS_386__)
		#define 
		#define targetOSName (__PROPER_STR("Windows (Win16 32-bit)"))
	#else
		#define targetOSName (__PROPER_STR("Windows (Win16)"))
	#endif

// Windows
#elif defined(_WIN32) || defined(_WIN64) || defined(__NT__) || defined(Q_OS_WIN32)
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
#elif defined(__ANDROID__) || defined(ANDROID) || defined(__ANDROID_API__) || defined(Q_OS_ANDROID)
    #define _COMPILING_FOR_ANDROID
    #define targetOSName (__PROPER_STR("Android"))

// AIX
#elif defined(_AIX) || defined(__TOS_AIX__) || defined(Q_OS_AIX)
    #define _COMPILING_FOR_AIX
    #define targetOSName (__PROPER_STR("AIX"))
    
// QNX
#elif defined(__QNX__) || defined(__QNXNTO__)
	#define _COMPILING_FOR_QNX
	#if defined(__QNXNTO__)
		#define _COMPILING_FOR_QNXNEUTRINO
		#define targetOSName (__PROPER_STR("QNX Neutrino"))
	#else
		#define _COMPILING_FOR_QNX4
		#define targetOSName (__PROPER_STR("QNX 4"))
	#endif
	
// MINIX
#elif defined(__minix)
	#define _COMPILING_FOR_MINIX
    #define targetOSName (__PROPER_STR("MINIX"))
	
// LynxOS
#elif defined(__Lynx__)
	#define _COMPILING_FOR_LYNXOS
    #define targetOSName (__PROPER_STR("LynxOS"))
    
// eCos
#elif defined(__ECOS)
	#define _COMPILING_FOR_ECOS
	#define targetOSName (__PROPER_STR("eCos"))

// Syllable
#elif defined(__SYLLABLE__)
	#define _COMPILING_FOR_SYLLABLE
	#define targetOSName (__PROPER_STR("Syllable"))
	
// MorphOS
#elif defined(__MORPHOS__)
	#define _COMPILING_FOR_MORPHOS
	#define targetOSName (__PROPER_STR("MorphOS"))
	
// BeOS
#elif defined(__BEOS__)
	#define _COMPILING_FOR_BEOS
	#define targetOSName (__PROPER_STR("BeOS"))
	
// AmigaOS
#elif defined(AMIGA) || defined(__amigaos__)
	#define _COMPILING_FOR_AMIGAOS
	#define targetOSName (__PROPER_STR("AmigaOS"))
	
// Ultrix
#elif defined(ultrix) || defined(__ultrix) || defined(__ultrix__) || (defined(unix) && defined(vax))
	#define _COMPILING_FOR_ULTRIX
	#define targetOSName (__PROPER_STR("Ultrix"))
	
// IBM OS/400
#elif defined(__OS400__)
	#define _COMPILING_FOR_IBMOS400
	#define targetOSName (__PROPER_STR("IBM OS/400"))
	
// UNICOS
#elif defined(_UNICOS)
	#define _COMPILING_FOR_UNICOS
	#define targetOSName (__PROPER_STR("UNICOS"))
	
// HP-UX
#elif defined(_hpux) || defined(hpux) || defined(__hpux)
	#define _COMPILING_FOR_HPUX
	#define targetOSName (__PROPER_STR("HP-UX"))
	
// OS/2
#elif defined(OS2) || defined(_OS2)|| defined(__OS2__)|| defined(__TOS_OS2__)
	#define _COMPILING_FOR_OS2
	#define targetOSName (__PROPER_STR("OS/2"))
	
// GNU/kFreeBSD
#elif defined(__FreeBSD_kernel__) && defined(__GLIBC__)
	#define _COMPILING_FOR_GNUKFREEBSD
	#define targetOSName (__PROPER_STR("GNU/kFreeBSD"))
	
// Dragonfly BSD
#elif defined(__DragonFly__)
	#define _COMPILING_FOR_DRAGONFLYBSD
	#define targetOSName (__PROPER_STR("DragonFly BSD"))
	
// BSD/OS
#elif defined(__bsdi__)
	#define _COMPILING_FOR_BSDOS
	#define targetOSName (__PROPER_STR("BSD/OS"))

// NetBSD
#elif defined(__NetBSD__)
	#define _COMPILING_FOR_NETBSD
	#define targetOSName (__PROPER_STR("NetBSD"))

// OpenBSD
#elif defined(__OpenBSD__)
	#define _COMPILING_FOR_OPENBSD
	#define targetOSName (__PROPER_STR("OpenBSD"))

// FreeBSD
#elif defined(__FreeBSD__) || defined(__FreeBSD_kernel__)
	#define _COMPILING_FOR_FREEBSD
	#define targetOSName (__PROPER_STR("FreeBSD"))

// GNU/Linux
#elif defined(__linux) || defined(__linux__) || defined(__LINUX__) || defined(linux) || defined(Q_OS_LINUX)
    #define _COMPILING_FOR_LINUX
    #if defined(__gnu_linux__)
        #define _COMPILING_FOR_GNULINUX
        #define targetOSName (__PROPER_STR("GNU/Linux"))
    #else
        #define targetOSName (__PROPER_STR("some Linux-based OS"))
    #endif

// GNU (GNU/Hurd)
#elif defined(__gnu_hurd__) || defined(__GNU__)
	#define _COMPILING_FOR_GNUHURD
	#define targetOSName (__PROPER_STR("GNU/Hurd"))
	
// uncovered BSD systems	
#elif defined(BSD)
    #define _COMPILING_FOR_OTHER_BSD_SYSTEMS
    #define targetOSName (__PROPER_STR("some BSD system"))

// uncovered UNIX systems
#elif defined(__unix) || defined(__unix__) || defined(__UNIX__) || defined(unix) || defined(Q_OS_UNIX)
    #define _COMPILING_FOR_OTHER_UNIX_SYSTEMS
    #define targetOSName (__PROPER_STR("some UNIX system"))
#endif

// BSD systems (all)
#if defined(BSD) || defined(__FreeBSD__) || defined(__NetBSD__) || defined(__OpenBSD__) || defined(__bsdi__) || defined(__DragonFly__)
    #define _COMPILING_FOR_BSD
#endif

// UNIX systems (all)
#if defined(__unix) || defined(__unix__) || defined(__UNIX__) || defined(unix) || defined(Q_OS_UNIX)
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
 * compiler identification                                           *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 */

#define compilerNameAndVersion (__PROPER_STR("?"))
#define compilerLongName (__PROPER_STR("?"))
#define compilerShortName (__PROPER_STR("?"))

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

#define isBooleanType(generic)         (isTheSameType(generic, bool))

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

#define isIntegerType(generic)         (isSignedIntegerType(generic) || \
                                        isUnsignedIntegerType(generic))
                                        
#define isIntegralType(generic)        (isSignedIntegerType(generic) || \
                                        isUnsignedIntegerType(generic) || \
                                        isCharacterType(generic) || \
                                        isBooleanType(generic))
                                        
#define isFloatingPointType(generic)   (isTheSameType(generic, float) || \
                                        isTheSameType(generic, double) || \
                                        isTheSameType(generic, long double))

#endif // _RETXT_GLOBALUTILITIES_INCLUDED

