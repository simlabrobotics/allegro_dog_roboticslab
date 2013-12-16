/* Kai, Copyright 2004-2011 SimLab Co., Ltd. All rights reserved.
 *
 * This library is commercial and cannot be redistributed, and/or modified
 * WITHOUT ANY ALLOWANCE OR PERMISSION OF SimLab Co., LTD.
 */

/** 
@file kaiDefinition.h
@brief Definitions of strings, constatns, macros, and enumerations used by Kai library.
@author SimLab
@date 2010/12/10
*/
#ifndef __KAIDEFINITION_H__
#define __KAIDEFINITION_H__


#ifndef WIN32
#include <sys/types.h>
#endif
#if defined(WIN32) || defined(WINCE)
#	if defined(KAI_EXPORTS)
#		define KAIEXPORT __declspec(dllexport)
#	elif defined(KAI_IMPORTS)
#		define KAIEXPORT __declspec(dllimport)
#	else
#		define KAIEXPORT
#	endif
#else
#	define KAIEXPORT
#endif

#ifdef __cplusplus
#	define KAI_EXTERN_C_BEGIN	extern "C" {
#	define KAI_EXTERN_C_END	}
#else
#	define KAI_EXTERN_C_BEGIN
#	define KAI_EXTERN_C_END
#endif



const int kaiTCP = 0;				///< TCP channel.
const int kaiUDP = 1;				///< UDP channel.
const int kaiINVALID_SOCKET = -1;	///< Kai invalid socket handle.
const int kaiFAIL = -1;				///< Failure.
const int kaiSUCCESS = 1;			///< Success.

#ifndef WIN32
const int SOCKET_ERROR = -1;
const int INVALID_SOCKET = -1;
#endif

#ifndef WIN32
typedef int SOCKET;
#endif

const unsigned long kaiBLOCKING_SOCKET = 0;		///< Socket which supports synchronized(blocked) data send and receive.
const unsigned long kaiNON_BLOCKING_SOCKET = 1; ///< Socket which supports non-blocked data send and receive.

typedef unsigned char	kaiCommand;	///< Kai message command data type.
typedef unsigned short	kaiMsgID;	///< Kai message id data type.
typedef unsigned short	kaiMsgSize;	///< Kai message size data type.
typedef unsigned char	kaiFlag;	///< Kai message flag data type.

#pragma pack(push, 1)
/**
 * Kai message header.
 */
typedef struct  
{	
	kaiMsgID id;		///< Message id.
	kaiMsgSize size;	///< Message size. It includes only the size of data buffer. It doesn't include the size of kaiHeader.
	short reserved;		///< (Reserved).
	kaiFlag flag;		///< Flag.
	char checksum;		///< Checksum.
} kaiHeader;
#pragma pack(pop)

const kaiMsgSize Size_kaiHEADER = sizeof(kaiHeader);	///< Data structure size of kaiHeader.


const size_t Size_kaiMsgID = sizeof(kaiMsgID);			///< Size of Kai message id data type.
const size_t Size_kaiMsgSize = sizeof(kaiMsgSize);		///< Size of Kai message size data type.
const size_t Size_kaiFlag = sizeof(kaiFlag);			///< Size of Kai message flag data type.

const kaiMsgSize kaiMAX_MSG_SIZE = 1024*64 - 1;			///< Maximum size of kaiMsg.
const kaiMsgSize kaiMAX_DATA_SIZE = kaiMAX_MSG_SIZE - Size_kaiHEADER; ///< Maximum size of kaiMsg data buffer.
const unsigned kaiBUFFER_SIZE = 1024*1024;				///< Size of receive/send buffer of kaiSocket.


const long kaiNO_EVENT		= 0x00;	///< Define no network event handler.
const long kaiON_MESSAGE	= 0x01;	///< Define a message handler.
const long kaiON_ACCEPT		= 0x02; ///< Define a accept handler.
const long kaiON_CLOSE		= 0x04; ///< Define a socket close handler.
const long kaiON_SEND		= 0x08;	///< Define a message send event handler.
const long kaiON_RECV		= 0x10;	///< Define a message receive event handler.


#endif
