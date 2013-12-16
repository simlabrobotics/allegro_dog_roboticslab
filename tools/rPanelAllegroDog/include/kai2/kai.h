/* Kai, Copyright 2004-2011 SimLab Co., Ltd. All rights reserved.
 *
 * This library is commercial and cannot be redistributed, and/or modified
 * WITHOUT ANY ALLOWANCE OR PERMISSION OF SimLab Co., LTD.
 */

/**
 * @mainpage Kai
 *
 * @section INTRODUCTION
 * Kai is a C++ class library to develop a network module.
 * 
 * @section CREATEINFO
 * - Author : SimLab Co., Ltd.
 * - Date : 2004/4/13
 *
 * @section MODIFYINFO
 * - 2010/12/10 : version 2.1, First documented.
 *
 * @remarks
 * This library is commercial and cannot be redistributed, and/or modified
 * WITHOUT ANY ALLOWANCE OR PERMISSION OF SimLab Co., LTD.
 */

/** 
@file kai.h
@brief main include file to use Kai library.
@author SimLab
@date 2010/12/10
*/
#ifndef __KAI_H__
#define __KAI_H__

#ifdef WIN32
#include <WinSock2.h>
#else
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/time.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <netinet/ip.h>
#endif

#include "kaiDefinition.h"
#include "kaiMsg.h"
#include "kaiSocket.h"
#include "kaiServer.h"



KAI_EXTERN_C_BEGIN

/** 
 * initialize network
 */
KAIEXPORT int kaiInitialize();

/**
 * disable network.
 */ 
KAIEXPORT void kaiShutdown();


KAI_EXTERN_C_END


#endif
