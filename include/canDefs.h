/*
 *\brief Constants and enums for CAN communication
 *\detailed The constants and enums required for communicating with the AllegroDog via
 *          ESD CAN.
 *
 *$Author: Sangyup Yi $
 *$Date: 2013/08/27 21:23:00 $
 *$Revision: 1.0 $
 */ 

#ifndef _CANDEFS_H
#define _CANDEFS_H

#ifdef USING_NAMESPACE_CANAPI
#   define CANAPI_BEGIN namespace CANAPI {
#   define CANAPI_END };
#else
#   define CANAPI_BEGIN
#   define CANAPI_END
#endif

#define		STD		(bool)0
#define		EXT		(bool)1

///////////////////////////////////////////////
//  Define Motor Control(Driver) Board ID
#define ID_DEVICE_MAIN	    0x00
#define ID_DEVICE_01	    0x01
#define ID_DEVICE_02	    0x02
#define ID_DEVICE_03	    0x03
#define ID_DEVICE_04	    0x04
#define ID_DEVICE_05	    0x05
#define ID_DEVICE_06	    0x06
#define ID_DEVICE_07	    0x07
#define ID_DEVICE_08	    0x08
#define ID_DEVICE_09	    0x09
#define ID_DEVICE_10	    0x0a
#define ID_DEVICE_11	    0x0b
#define ID_DEVICE_12	    0x0c

#define ID_DEVICE_ALL	    0xff

////////////////////////////////////////////////
//  Define Member ID
#define ID_MEMBER_01   0x01
#define ID_MEMBER_02   0x02
#define ID_MEMBER_03   0x03
#define ID_MEMBER_04   0x04

#define ID_MEMBER_ALL  0xff

////////////////////////////////////////////////
// Define Group ID
#define ID_GROUP_01   0x01
#define ID_GROUP_02   0x02
#define ID_GROUP_03   0x03
#define ID_GROUP_04   0x04

#define ID_GROUP_ALL  0xff

////////////////////////////////////////////////
// Define CAN Command

////////////////////////////////////////////////
// Mode of Operation
#define CONTROL_MODE_POSITION           0x02
#define CONTROL_MODE_CURRENT            0x07
#define CONTROL_MODE_VELOCITY           0x03

#endif
