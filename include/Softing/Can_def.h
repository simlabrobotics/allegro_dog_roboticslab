/***************************************************************************/
/*                                                                         */
/*   Softing GmbH          Richard-Reitzner-Allee 6       85540 Haar       */
/*                                                                         */
/***************************************************************************/
/*                                                                         */
/*   Copyright (C) by Softing GmbH, 1997,1998,1999   All rights reserved.  */
/*                                                                         */
/***************************************************************************/
/*                                                                         */
/*                     C A N _ D E F . H                                   */
/*                                                                         */
/***************************************************************************/
/*                                                                         */
/* PROJECT:            CANLIB                                              */
/*                                                                         */
/* MODULE:             CAN_DEF.H                                           */
/*                                                                         */
/* AUTHOR:             Softing GmbH                                        */
/*                                                                         */
/* MODULE_DESCRIPTION  definitions for CAN-LAYER2 LIB                      */
/*                     CANcard and CAN-AC2                                 */
/*                                                                         */
/* USAGE               description of the macro definitions                */
/*                                                                         */
/*       Macro         Description                                         */                         
/*                                                                         */
/*       WIN16         define for building 16-bit windows (WIN3.11)        */
/*                     applications                                        */
/*                                                                         */
/*       WIN32         define for building 32-bit applications             */   
/*                     (WIN-NT 4.0 and WIN 95)                             */
/*                                                                         */
/*       DOS           define for MSDOS applications. DOS is               */
/*                     automatically defined, if neither WIN16             */
/*                     nor WIN32 is set.                                   */  
/*                                                                         */
/*       __cplusplus   define for C++ applications (normally defined       */
/*                     automatically by the compiler)                      */
/*                                                                         */
/***************************************************************************/
/*
*
*/

#ifndef  _CAN_DEF_H
#define  _CAN_DEF_H

/* remove comment lines for building a WIN3.11 application                 */
/*  #define WIN16 */
/* remove comment lines for building a WIN-NT4.0 or WIN95 application      */
/*  #define WIN32 */
/* remove comment lines for building a MS-DOS application                  */
/* #define DOS    */

/* compatibility with borland */
#ifdef __WIN32__
#ifndef WIN32
#define  WIN32
#endif
#endif

/* compatibility with borland */
#if defined (__MSDOS__) || defined (_DOS)
#ifndef DOS
#define DOS
#endif
#endif


#if !(defined (WIN16) || defined (WIN32))
/* default setting is DOS */
#ifndef DOS
#define DOS
#endif
#endif

/* check macro definitions */
#ifdef DOS 
#if defined(WIN16) || defined (WIN32)
#error only one macro definition (DOS or WIN16 or WIN32) allowed
#endif 
#endif

#ifdef WIN16 
#if defined(DOS) || defined (WIN32)
#error only one macro definition (DOS or WIN16 or WIN32) allowed
#endif 
#endif

#ifdef WIN32 
#if defined(WIN16) || defined (DOS)
#error only one macro definition (DOS or WIN16 or WIN32) allowed
#endif 
#endif

/* set the switch DOS, if a MS-DOS application is build  */
#ifdef  DOS 

#ifdef F_M /* far memory model */
#ifndef FAR
#define FAR far 
#endif
#else /* F_M */
#ifndef FAR
#define FAR  
#endif
#endif/* F_M */

#ifdef __cplusplus
#ifndef PRAEDEF 
#define PRAEDEF extern "C"
#endif
#ifndef PRAEDECL
#define PRAEDECL  extern "C"
#endif
#else /* cplusplus */
#ifndef PRAEDEF
#define PRAEDEF 
#endif
#ifndef PRAEDECL
#define PRAEDECL 
#endif
#endif


#ifndef PRAEDECL
#define PRAEDECL 
#endif

#ifndef MIDDECL
#define MIDDECL  
#endif

#ifndef MIDDEF
#define MIDDEF  
#endif
 
typedef int         BOOL;
#define FALSE           0
#define TRUE            1
typedef unsigned char       BYTE;
typedef unsigned short      WORD;
typedef unsigned long       DWORD;
typedef unsigned int        UINT;
#ifndef NULL
#define NULL            0
#endif

typedef char FAR*           LPSTR;
typedef const char FAR*     LPCSTR;
typedef BYTE FAR*           LPBYTE;
typedef int FAR*            LPINT;
typedef WORD FAR*           LPWORD;
typedef long FAR*           LPLONG;
typedef DWORD FAR*          LPDWORD;
typedef void FAR*           LPVOID;
#endif /* _DOS */



/* set the switch WIN32, if a Windows 95 or a Windows NT application is build  */
#ifdef WIN32


// export / import definition for the API functions
#ifdef SFCAN_INTERFACE_EXPORTS
#define SFCAN_INTERFACE __declspec(dllexport)
#else // SFCAN_INTERFACE_EXPORTS
#define SFCAN_INTERFACE __declspec(dllimport)
#endif // else SFCAN_INTERFACE_EXPORTS


#ifdef EMU

#ifdef __cplusplus
#define PRAEDEF  extern "C"
#define PRAEDECL extern "C"
#else
#define PRAEDEF
#define PRAEDECL
#endif

#define MIDDECL  pascal
#define MIDDEF   pascal

#else // EMU

#ifdef __cplusplus
#define PRAEDEF  extern "C" SFCAN_INTERFACE 
#define PRAEDECL extern "C" SFCAN_INTERFACE
#else
#define PRAEDEF  SFCAN_INTERFACE
#define PRAEDECL SFCAN_INTERFACE
#endif

#define MIDDECL  __stdcall
#define MIDDEF   __stdcall

#endif // EMU


#ifndef FAR
#define FAR
#endif

#ifndef PASCAL
#define PASCAL
#endif


#ifndef _export
#define _export
#endif



#endif /* WIN32 */

/* set the switch WIN16, if a Windows 3.11 application is build  */
#ifdef WIN16

#ifndef FAR
#define FAR  far
#endif

#ifdef __cplusplus
#define PRAEDEF extern "C"
#else
#define PRAEDEF
#endif
#define MIDDEF  __export FAR PASCAL

#ifdef __cplusplus
#define PRAEDECL extern "C"
#else
#define PRAEDECL
#endif
#define MIDDECL FAR PASCAL

#endif /* WIN16 */
/*##############################################*/

#endif /* _CAN_DEF_H */
                                                                       
