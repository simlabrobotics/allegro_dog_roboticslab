/* RoboticsLab, Copyright 2008-2011 SimLab Co., Ltd. All rights reserved.
 *
 * This library is commercial and cannot be redistributed, and/or modified
 * WITHOUT ANY ALLOWANCE OR PERMISSION OF SimLab Co., LTD.
 */

/**
 * @file rTermDef.h
 * @brief defines rTerm constants.
 */
#ifndef __rTermDEF_H__
#define __rTermDEF_H__

#ifndef WIN32
#include <sys/types.h>
#endif
#if defined(WIN32) || defined(WINCE)
#	if defined(RTERM_EXPORTS)
#		define RTERMEXPORT __declspec(dllexport)
#	elif defined(RTERM_IMPORTS)
#		define RTERMEXPORT __declspec(dllimport)
#	else
#		define RTERMEXPORT
#	endif
#else
#	define RTERMEXPORT
#endif

#ifdef __cplusplus
#	define RTERM_EXTERN_C_BEGIN	extern "C" {
#	define RTERM_EXTERN_C_END	}
#else
#	define RTERM_EXTERN_C_BEGIN
#	define RTERM_EXTERN_C_END
#endif

#ifdef NOT_USING_NAMESPACE_RTERM
#   define RTERMAPI
#   define RTERMAPIDEF
#   define RTERMAPI_BEGIN
#   define RTERMAPI_END
#else
#   define RTERMAPI
#   define RTERMAPIDEF
#   define RTERMAPI_BEGIN namespace rTerm {
#   define RTERMAPI_END }
#endif


/*! \def RP_RETCODE_SUCCESS
 */
#define RP_RETCODE_SUCCESS 0    ///< Return value when requesting a query returns success.
/*! \def RP_RETCODE_FAIL
 */
#define RP_RETCODE_FAIL    1    ///< Return value when requesting a query encountered a problem.


#endif // __rTermDEF_H__
