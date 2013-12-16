/*
**                        Copyright 1997 by KVASER AB
**            P.O Box 4076 S-51104 KINNAHULT, SWEDEN Tel. +46 320 15287
**
** This software is furnished under a license and may be used and copied
** only in accordance with the terms of such license.
**
*/
/*
** This file removes the differences between BCC and MSC with regards
** to some predefined macros. The other differences remain. Sigh.
*/

#ifndef _PREDEF_H_
#define _PREDEF_H_

// 1) Win16: Borland defines _Windows
//    MSC defines _WINDOWS
//    We want _WINDOWS (but also _Windows to be sure)
#if defined(_Windows) && !defined(_WINDOWS)
#   define _WINDOWS 1
#endif

#if defined(_WINDOWS) && !defined(_Windows)
#   define _Windows 1
#endif


// 2) Win32: Borland defines _Windows and __WIN32__ and _WIN32
//    MSC defines _WIN32
//    We want __WIN32__ and _WINDOWS and _Windows and _WIN32
#if defined(_WIN32) || defined(__WIN32__)
#   if !defined(__WIN32__)
#       define __WIN32__ 1
#   endif
#   if !defined(_WINDOWS)
#       define _WINDOWS 1
#   endif
#   if !defined(_Windows)
#       define _Windows 1
#   endif
#endif


// 3) DLL: Borland defines __DLL__
//    MSC defines _DLL
//    We want __DLL__
#if defined (_DLL) && !defined(__DLL__)
#   define __DLL__ 1
#endif


#endif

