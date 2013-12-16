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
#endif
#include <windows.h>
#include <malloc.h>
#include <assert.h>
//project headers
extern "C" {
#include "nican.h"
}
#include "canDefs.h"
#include "canAPI.h"


CANAPI_BEGIN

#define CH_COUNT			(int)2 // number of CAN channels
NCTYPE_OBJH canDev[CH_COUNT] = {0, 0};

#define canMSG_MASK             0x00ff      // Used to mask the non-info bits
#define canMSG_RTR              0x0001      // Message is a remote request
#define canMSG_STD              0x0002      // Message has a standard ID
#define canMSG_EXT              0x0004      // Message has an extended ID
#define canMSG_WAKEUP           0x0008      // Message to be sent / was received in wakeup mode
#define canMSG_NERR             0x0010      // NERR was active during the message
#define canMSG_ERROR_FRAME      0x0020      // Message is an error frame
#define canMSG_TXACK            0x0040      // Message is a TX ACK (msg is really sent)
#define canMSG_TXRQ             0x0080      // Message is a TX REQUEST (msg is transfered to the chip)

//#define _DUMP_RXFRAME (1)


/* This function converts the absolute time obtained from ncReadMult into a
   string. */
void AbsTimeToString(NCTYPE_ABS_TIME *time, char *TimeString)
{

   SYSTEMTIME	stime;
   FILETIME		localftime;

   FileTimeToLocalFileTime((FILETIME *)(time), &localftime);
   FileTimeToSystemTime(&localftime, &stime);
	  sprintf(TimeString, "%02d:%02d:%02d.%03d",
            stime.wHour, stime.wMinute, stime.wSecond, 
            stime.wMilliseconds);
}

/* Print a description of an NI-CAN error/warning. */
void PrintStat(int bus, NCTYPE_STATUS Status, char *source) 
{
	char StatusString[1024];
     
	if (Status != 0) 
	{
		ncStatusToString(Status, sizeof(StatusString), StatusString);
		printf("\n%s\nSource = %s\n", StatusString, source);

		// On error, close object handle.
		printf("<< CAN: Close\n");
		ncCloseObject(canDev[bus]);
		canDev[bus] = 0;
		//exit(1);
	}
}

/* Print read frame */
void PrintRxFrame(NCTYPE_CAN_STRUCT& RxFrame)
{
	char output[15];
	char CharBuff[50];
	AbsTimeToString(&RxFrame.Timestamp, &output[0]);
	printf("%s     ", output);
	sprintf (&CharBuff[0], "%8.8X", RxFrame.ArbitrationId);
	printf("%s     ", CharBuff);
	sprintf (&CharBuff[0], "%s","CAN Data Frame");
	printf("%s     ", CharBuff); 
	sprintf (&CharBuff[0], "%1d", RxFrame.DataLength);
	printf("%s     ", CharBuff); 
	for (int j=0; j<RxFrame.DataLength; j++)
	{
		sprintf (CharBuff, " %02X", RxFrame.Data[j]);
		printf("%s", CharBuff); 
	}
	printf("\n");
}


/*========================================*/
/*       Public functions (CAN API)       */
/*========================================*/
int initCAN(int bus)
{
	NCTYPE_ATTRID		AttrIdList[8];
	NCTYPE_UINT32		AttrValueList[8];
	//NCTYPE_UINT32		Baudrate = 125000;  // BAUD_125K
	NCTYPE_UINT32		Baudrate = NC_BAUD_1000K;
	char				Interface[15];
	NCTYPE_STATUS		Status = 0;
	NCTYPE_OBJH			TxHandle;
	
	sprintf_s(Interface, "CAN%d", bus);
	
	// Configure the CAN Network Interface Object	
	AttrIdList[0] =     NC_ATTR_BAUD_RATE;   
	AttrValueList[0] =  Baudrate;
	AttrIdList[1] =     NC_ATTR_START_ON_OPEN;
	AttrValueList[1] =  NC_TRUE;
	AttrIdList[2] =     NC_ATTR_READ_Q_LEN;
	AttrValueList[2] =  100;
	AttrIdList[3] =     NC_ATTR_WRITE_Q_LEN;
	AttrValueList[3] =  10;	
	AttrIdList[4] =     NC_ATTR_CAN_COMP_STD;
	AttrValueList[4] =  0;//0xCFFFFFFF;
	AttrIdList[5] =     NC_ATTR_CAN_MASK_STD;
	AttrValueList[5] =  NC_CAN_MASK_STD_DONTCARE;
	AttrIdList[6] =     NC_ATTR_CAN_COMP_XTD;
	AttrValueList[6] =  0;//0xCFFFFFFF;
	AttrIdList[7] =     NC_ATTR_CAN_MASK_XTD;
	AttrValueList[7] =  NC_CAN_MASK_XTD_DONTCARE;
	
	printf("<< CAN: Config\n");
	Status = ncConfig(Interface, 2, AttrIdList, AttrValueList);
	if (Status < 0) 
	{
		PrintStat(bus, Status, "ncConfig");
		return Status;
	}
	printf("   - Done\n");
    
	// open the CAN Network Interface Object
	printf("<< CAN: Open Channel\n");
	Status = ncOpenObject (Interface, &TxHandle);
	if (Status < 0) 
	{
		PrintStat(bus, Status, "ncOpenObject");
		return Status;
	}
	canDev[bus] = TxHandle;
	printf("   - Done\n");
	return 0;
}

int freeCAN(int bus)
{
	if (!canDev[bus])
		return -1;

	NCTYPE_STATUS		Status = 0;

	printf("<< CAN: Close\n");
	Status = ncCloseObject(canDev[bus]);    
	if (Status < 0)
	{
		PrintStat(bus, Status, "ncCloseObject");
		return Status;
	}
	canDev[bus] = 0;
	printf("   - Done\n");
	return 0;
}

int canReadMsg(int bus, int *id, int *len, unsigned char *data, int blocking)
{
	if (!canDev[bus])
		return -1;

	NCTYPE_STATUS Status = 0;
	NCTYPE_STATE  State = 0;
	NCTYPE_CAN_STRUCT RxFrame;

	if (blocking)
	{
		Status = ncWaitForState(canDev[bus], NC_ST_READ_AVAIL, NC_DURATION_INFINITE, &State);
		if (Status < 0 || !(NC_ST_READ_AVAIL & State))
			return -1; // failed
	}

	memset(&RxFrame, 0, sizeof(NCTYPE_CAN_FRAME));
	//RxFrame.FrameType = NC_FRMTYPE_DATA;
	RxFrame.DataLength = *len;
	Status = ncRead(canDev[bus], sizeof(NCTYPE_CAN_STRUCT), &RxFrame);
	if (Status < 0 || NC_FRMTYPE_DATA != RxFrame.FrameType)
	{
		PrintStat(bus, Status, "ncRead");
		return Status;
	}
	else if (Status == CanWarnOldData)
	{
		(*len) = 0;
		return Status;
	}

	(*id) = RxFrame.ArbitrationId;
	//(*time) = (FILETIME)(RxFrame.Timestamp);
	(*len) = RxFrame.DataLength;
	for (int i=0; i<RxFrame.DataLength; i++)
		((NCTYPE_UINT8_P)data)[i] = RxFrame.Data[i];
#ifdef _DUMP_RXFRAME
	PrintRxFrame(RxFrame);
#endif
	return Status;
}

int canSendMsg(int bus, int id, char len, unsigned char *data, int blocking)
{
	if (!canDev[bus])
		return -1;

	NCTYPE_STATUS Status = 0;
	NCTYPE_STATE  State = 0;
	NCTYPE_CAN_FRAME TxFrame;
	TxFrame.IsRemote = NC_FRMTYPE_DATA;
	TxFrame.ArbitrationId = id;
	TxFrame.DataLength = len;
	for (int i=0; i<len; i++)
		TxFrame.Data[i] = ((NCTYPE_UINT8_P)data)[i];
	Status = ncWrite(canDev[bus], sizeof(NCTYPE_CAN_FRAME), &TxFrame);
	if (Status < 0)
	{
		PrintStat(bus, Status, "ncWrite");
		return Status;
	}
	if (blocking)
	{
		Status = ncWaitForState(canDev[bus], (NC_ST_WRITE_SUCCESS | NC_ST_WARNING | NC_ST_ERROR), NC_DURATION_INFINITE, &State);
		if (Status < 0 || !(NC_ST_WRITE_SUCCESS & State))
			return -1; // failed
	}

	return Status;
}


CANAPI_END
