

/*======================*/
/*       Includes       */
/*======================*/
//system headers
#include <stdio.h>
#include <errno.h>
#ifndef _WIN32
#include <inttypes.h>
#include <pthread.h>
#include <syslog.h>
#include <unistd.h>
#else
#include <windows.h>
#endif
#include <malloc.h>
#include <assert.h>
//project headers
extern "C" {
#include "Peak/PCANBasic.h"
}
#include "canDefs.h"
#include "canAPI.h"

typedef union _float
{
   float value;
   unsigned char arr[4];
}msg_type_float;

CANAPI_BEGIN

/*=====================*/
/*       Defines       */
/*=====================*/
//macros
#define isAlpha(c) ( ((c >= 'A') && (c <= 'Z')) ? 1 : 0 )
#define isSpace(c) ( (c == ' ') ? 1 : 0 )
#define isDigit(c) ( ((c >= '0') && (c <= '9')) ? 1 : 0 )
#define ADDR2NODE(x) ((((x) >> 5) & 0x001F) - BASE_ID)
#define NODE2ADDR(x) (((mbxID + BASE_ID) << 5) | ((x) + BASE_ID))
#define GROUPID(n)   (((mbxID + BASE_ID) << 5) | (0x0400 + (n)))
#define BROADCAST    (GROUPID(0))
#define Border(Value,Min,Max)  (Value<Min)?Min:((Value>Max)?Max:Value)
//typedefs & structs
typedef unsigned long DWORD;

/*=========================================*/
/*       Global file-scope variables       */
/*=========================================*/
TPCANHandle canDev[MAX_BUS] = {
	PCAN_PCIBUS1,
	PCAN_PCIBUS2,
	PCAN_PCIBUS3,
	PCAN_PCIBUS4
};
//TPCANHandle canDev[MAX_BUS] = {
//	PCAN_USBBUS1,
//	PCAN_USBBUS2,
//	PCAN_USBBUS3,
//	PCAN_USBBUS4,
	/*PCAN_USBBUS5,
	PCAN_USBBUS6,
	PCAN_USBBUS7,
	PCAN_USBBUS8*/
//};


/*========================================*/
/*       Public functions (CAN API)       */
/*========================================*/
int initCAN(int bus){
	TPCANStatus Status = PCAN_ERROR_OK;
	char strMsg[256];
	TPCANBaudrate Baudrate = PCAN_BAUD_1M;
	TPCANType HwType = 0;
	DWORD IOPort = 0;
	WORD Interrupt = 0;
	//int param;

	Status = CAN_Initialize(canDev[bus], Baudrate, HwType, IOPort, Interrupt);
	if (Status != PCAN_ERROR_OK)
	{
		CAN_GetErrorText(Status, 0, strMsg);
		printf("initCAN(): CAN_Initialize() failed with error %ld\n", Status);
		printf("%s\n", strMsg);
		return Status;
	}

 //   if(bus==0)
	//{
 //       Status = CAN_FilterMessages(
	//	    canDev[bus],
 //           ((unsigned long)(ID_REPLY_POSITION) <<8) | ((unsigned long)ID_DEVICE_MAIN <<4) | ((unsigned long)ID_DEVICE_01),
 //           ((unsigned long)(ID_REPLY_POSITION) <<8) | ((unsigned long)ID_DEVICE_MAIN <<4) | ((unsigned long)ID_DEVICE_06),
	//	    PCAN_MESSAGE_STANDARD);
 //   }
 //   else
 //   {
 //       Status = CAN_FilterMessages(
	//	    canDev[bus],
 //           ((unsigned long)(ID_REPLY_POSITION) <<8) | ((unsigned long)ID_DEVICE_MAIN <<4) | ((unsigned long)ID_DEVICE_07),
 //           ((unsigned long)(ID_REPLY_POSITION) <<8) | ((unsigned long)ID_DEVICE_MAIN <<4) | ((unsigned long)ID_DEVICE_12),
	//	    PCAN_MESSAGE_STANDARD);
 //   }
	//if (Status != PCAN_ERROR_OK)
	//{
	//	CAN_GetErrorText(Status, 0, strMsg);
	//	printf("initCAN(): CAN_FilterMessages() failed with error %ld\n", Status);
	//	printf("%s\n", strMsg);
	//	//return Status;
	//}

	//param = PCAN_PARAMETER_ON;
	//Status = CAN_SetValue(canDev[bus], PCAN_BUSOFF_AUTORESET, &param, sizeof(param));
	//if (Status != PCAN_ERROR_OK)
	//{
	//	CAN_GetErrorText(Status, 0, strMsg);
	//	printf("initCAN(): CAN_SetValue(PCAN_BUSOFF_AUTORESET, PCAN_PARAMETER_ON) failed with error %ld\n", Status);
	//	printf("%s\n", strMsg);
	//	//return Status;
	//}

	Status = CAN_Reset(canDev[bus]);
	if (Status != PCAN_ERROR_OK)
	{
		CAN_GetErrorText(Status, 0, strMsg);
		printf("initCAN(): CAN_Reset() failed with error %ld\n", Status);
		printf("%s\n", strMsg);
		//return Status;
	}

	return 0; // PCAN_ERROR_OK
}

int freeCAN(int bus){
	TPCANStatus Status = PCAN_ERROR_OK;
	char strMsg[256];

	Status = CAN_Uninitialize(canDev[bus]);
	if (Status != PCAN_ERROR_OK)
	{
		CAN_GetErrorText(Status, 0, strMsg);
		printf("freeCAN(): CAN_Uninitialize() failed with error %ld\n", Status);
		printf("%s\n", strMsg);
		return Status;
	}

	return 0; // PCAN_ERROR_OK
}

int canReadMsg(int bus, int *id, int *len, unsigned char *data, int blocking){
	TPCANMsg CANMsg;
	TPCANTimestamp CANTimeStamp;
	TPCANStatus Status = PCAN_ERROR_OK;
	char strMsg[256];
	int i;

	// We execute the "Read" function of the PCANBasic                
	//
	Status = CAN_Read(canDev[bus], &CANMsg, &CANTimeStamp);
	
	if (Status != PCAN_ERROR_OK)
	{
		if (Status != PCAN_ERROR_QRCVEMPTY)
		{
			CAN_GetErrorText(Status, 0, strMsg);
			printf("canReadMsg(): CAN_Read() failed with error %ld\n", Status);
			printf("%s\n", strMsg);
		}
		return Status;
	}
	else if (PCAN_MESSAGE_STATUS == CANMsg.MSGTYPE)
	{
		return PCAN_ERROR_QRCVEMPTY;
	}

	(*id) = CANMsg.ID;
	(*len) = CANMsg.LEN;
	for(i = 0; i < CANMsg.LEN; i++)
		data[i] = CANMsg.DATA[i];

	return 0;
}

int canSendMsg(int bus, int id, char len, unsigned char *data, int blocking){
	TPCANMsg CANMsg;
	TPCANStatus Status = PCAN_ERROR_OK;
	char strMsg[256];
	int i;

	CANMsg.ID = id;
	CANMsg.LEN = len & 0x0F;
	for(i = 0; i < len; i++)
        CANMsg.DATA[i] = data[i];
	CANMsg.MSGTYPE = PCAN_MESSAGE_STANDARD;

	Status = CAN_Write(canDev[bus], &CANMsg);
	if (Status != PCAN_ERROR_OK)
	{
		CAN_GetErrorText(Status, 0, strMsg);
		printf("canSendMsg(): CAN_Write() failed with error %ld\n", Status);
		printf("%s\n", strMsg);
		return Status;
	}

	return 0;
}


CANAPI_END
