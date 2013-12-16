

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
#include <malloc.h>
#include <assert.h>
//project headers
extern "C" {
#include "ntcan.h"
}
#include "canDefs.h"
#include "canAPI.h"



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
HANDLE              canDev[MAX_BUS];
#ifndef _WIN32
pthread_mutex_t     commMutex;
#endif


/*========================================*/
/*       Public functions (CAN API)       */
/*========================================*/
void allowMessage(int bus, int id, int mask){
    int i;
    DWORD retvalue;
	for(i=0;i<2048;i++){
		if((i & ~mask)==id){
			retvalue = canIdAdd(canDev[bus],i);
			if(retvalue != NTCAN_SUCCESS){
#ifndef _WIN32
			  syslog(LOG_ERR, "allowMessage(): canIdAdd() failed with error %d", retvalue);
#endif
			  printf("allowMessage(): canIdAdd() failed with error %ld\n", retvalue);
			}
		}
	}
}

int initCAN(int bus){
    DWORD retvalue;
#ifndef _WIN32
    pthread_mutex_init(&commMutex, NULL);
#endif
    
    retvalue = canOpen(bus, 0, TX_QUEUE_SIZE, RX_QUEUE_SIZE, TX_TIMEOUT, RX_TIMEOUT, &canDev[bus]);
    if(retvalue != NTCAN_SUCCESS){
#ifndef _WIN32
        syslog(LOG_ERR, "initCAN(): canOpen() failed with error %d", retvalue);
#endif
		printf("initCAN(): canOpen() failed with error %ld\n", retvalue);
        return(1);
    }
    
    retvalue = canSetBaudrate(canDev[bus], 0); // 1 = 1Mbps, 2 = 500kbps, 3 = 250kbps
    if(retvalue != 0)
    {
#ifndef _WIN32
        syslog(LOG_ERR, "initCAN(): canSetBaudrate() failed with error %d", retvalue);
#endif
		printf("initCAN(): canSetBaudrate() failed with error %ld\n", retvalue);
        return(1);
    }
    
    // Mask 3E0: 0000 0111 0000 0000
    //allowMessage(bus, 0x0000, 0x03E0); // Messages sent directly to host
    //allowMessage(bus, 0x0403, 0x03E0); // Group 3 messages
    //allowMessage(bus, 0x0406, 0x03E0); // Group 6 messages

	unsigned long Txid;
	if(bus==0)
	{
		/*Txid = ((unsigned long)(ID_REPLY_POSITION) <<8) | ((unsigned long)ID_DEVICE_MAIN <<4) | ((unsigned long)ID_DEVICE_01);
		allowMessage(bus, Txid, 0);
		Txid = ((unsigned long)(ID_REPLY_POSITION) <<8) | ((unsigned long)ID_DEVICE_MAIN <<4) | ((unsigned long)ID_DEVICE_02);
		allowMessage(bus, Txid, 0);
		Txid = ((unsigned long)(ID_REPLY_POSITION) <<8) | ((unsigned long)ID_DEVICE_MAIN <<4) | ((unsigned long)ID_DEVICE_03);
		allowMessage(bus, Txid, 0);
		Txid = ((unsigned long)(ID_REPLY_POSITION) <<8) | ((unsigned long)ID_DEVICE_MAIN <<4) | ((unsigned long)ID_DEVICE_04);
		allowMessage(bus, Txid, 0);
		Txid = ((unsigned long)(ID_REPLY_POSITION) <<8) | ((unsigned long)ID_DEVICE_MAIN <<4) | ((unsigned long)ID_DEVICE_05);
		allowMessage(bus, Txid, 0);
		Txid = ((unsigned long)(ID_REPLY_POSITION) <<8) | ((unsigned long)ID_DEVICE_MAIN <<4) | ((unsigned long)ID_DEVICE_06);
		allowMessage(bus, Txid, 0);*/
	}
	if(bus==1)
	{
		/*Txid = ((unsigned long)(ID_REPLY_POSITION) <<8) | ((unsigned long)ID_DEVICE_MAIN <<4) | ((unsigned long)ID_DEVICE_07);
		allowMessage(bus, Txid, 0);
		Txid = ((unsigned long)(ID_REPLY_POSITION) <<8) | ((unsigned long)ID_DEVICE_MAIN <<4) | ((unsigned long)ID_DEVICE_08);
		allowMessage(bus, Txid, 0);
		Txid = ((unsigned long)(ID_REPLY_POSITION) <<8) | ((unsigned long)ID_DEVICE_MAIN <<4) | ((unsigned long)ID_DEVICE_09);
		allowMessage(bus, Txid, 0);
		Txid = ((unsigned long)(ID_REPLY_POSITION) <<8) | ((unsigned long)ID_DEVICE_MAIN <<4) | ((unsigned long)ID_DEVICE_10);
		allowMessage(bus, Txid, 0);
		Txid = ((unsigned long)(ID_REPLY_POSITION) <<8) | ((unsigned long)ID_DEVICE_MAIN <<4) | ((unsigned long)ID_DEVICE_11);
		allowMessage(bus, Txid, 0);
		Txid = ((unsigned long)(ID_REPLY_POSITION) <<8) | ((unsigned long)ID_DEVICE_MAIN <<4) | ((unsigned long)ID_DEVICE_12);
		allowMessage(bus, Txid, 0);*/
	}


    return(0);
}

int freeCAN(int bus){
	DWORD retvalue;
    retvalue = canClose(canDev[bus]);
	if(retvalue != NTCAN_SUCCESS){
#ifndef _WIN32
        syslog(LOG_ERR, "freeCAN(): canClose() failed with error %d", retvalue);
#endif
		printf("freeCAN(): canClose() failed with error %ld\n", retvalue);
		return(1);
	}
	return(0);
}

int canReadMsg(int bus, int *id, int *len, unsigned char *data, int blocking){
    CMSG    msg;
    DWORD   retvalue;
    long    msgCt = 1;
    int     i;
    
    if(blocking){
        retvalue = canRead(canDev[bus], &msg, &msgCt, NULL);
    }else{
        retvalue = canTake(canDev[bus], &msg, &msgCt);
    }
    if(retvalue != NTCAN_SUCCESS){
#ifndef _WIN32
        syslog(LOG_ERR, "canReadMsg(): canRead/canTake error: 0x%08x\n", retvalue);
#endif
		printf("canReadMsg(): canRead/canTake error: 0x%08x\n", retvalue);
        if(retvalue == NTCAN_RX_TIMEOUT)
            return(1);
        else
            return(2);
    }
    if(msgCt == 1){
        *id = msg.id;
        *len = msg.len;
        for(i = 0; i < msg.len; i++)
            data[i] = msg.data[i];
            
        return(0);
    }
    
    return(1); // No message received, return err
}

int canSendMsg(int bus, int id, char len, unsigned char *data, int blocking){
    CMSG    msg;
    DWORD   retvalue;
    long    msgCt = 1;
    int     i;
    
    msg.id = id;
    msg.len = len & 0x0F;
    for(i = 0; i < len; i++)
        msg.data[i] = data[i];
    
    if(blocking){
        retvalue = canWrite(canDev[bus], &msg, &msgCt, NULL);
    }else{
        retvalue = canSend(canDev[bus], &msg, &msgCt);
    }
    
    if(retvalue != NTCAN_SUCCESS){
#ifndef _WIN32
        syslog(LOG_ERR, "canSendMsg(): canWrite/Send() failed with error 0x%08x\n", retvalue);
#endif
		printf("canSendMsg(): canWrite/Send() failed with error 0x%08x\n", retvalue);
        return(1);
    }
    return 0;
}


CANAPI_END
