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

#include "canDefs.h"
#include "canAPI.h"
#include "canlib.h"

typedef union _float
{
   float value;
   unsigned char arr[4];
}msg_type_float;


CANAPI_BEGIN


#define CH_COUNT			(int)2 // number of CAN channels

static int hCAN[CH_COUNT] = {-1, -1}; // CAN channel handles



/*========================================*/
/*       Public functions (CAN API)       */
/*========================================*/
int initCAN(int bus)
{
	assert(bus >= 0 && bus < CH_COUNT);

	canStatus ret;
	
	printf("<< CAN: Initialize Library...\n");
	canInitializeLibrary();
	printf("\t- Done\n");
	Sleep(200);

	printf("<< CAN: Open Channel...\n");
	hCAN[bus] = canOpenChannel(bus, canOPEN_EXCLUSIVE);
	if (hCAN[bus] < 0) return -1;
	printf("\t- Ch.%2d (OK)\n", bus);
	printf("\t- Done\n");
	Sleep(200);

	printf("<< CAN: Set Bus Parameter...\n");
	ret = canSetBusParams(hCAN[bus], BAUD_1M, 0, 0, 0, 0, 0);
	if (ret < 0) return -2;
	printf("\t- Done\n");
	Sleep(200);

	printf("<< CAN: Bus On...\n");
	ret = canBusOn(hCAN[bus]);
	if (ret < 0) return -3;
	printf("\t- Done\n");
	Sleep(200);

	return 0;
}

int resetCAN(int bus)
{
	assert(bus >= 0 && bus < CH_COUNT);

	canStatus ret;

	printf("<< CAN: Reset Bus...\n");
	ret = canResetBus(hCAN[bus]);
	if (ret < 0) return ret;
	printf("\t- Done\n");
	Sleep(200);

	return 0;
}

int freeCAN(int bus)
{
	assert(bus >= 0 && bus < CH_COUNT);

	canStatus ret;

	printf("<< CAN: Close...\n");
	ret = canClose(hCAN[bus]);
	if (ret < 0) return ret;
	hCAN[bus] = 0;
	printf("\t- Done\n");
	Sleep(200);
	return 0;
}

int canReadMsg(int bus, int *id, int *len, unsigned char *data, int blocking)
{
    long Rxid;
	unsigned char rdata[8];
	unsigned int dlc;
	unsigned int flag;
	unsigned long time;
	canStatus ret;

	memset(rdata, NULL, sizeof(rdata));
	ret = canRead(hCAN[bus], &Rxid, rdata, &dlc, &flag, &time);
	if (ret != canOK) return (int)ret;

    *id = Rxid;
    *len = dlc;
    for(int nd=0; nd<(int)dlc; nd++) data[nd] = rdata[nd];
	return 0;
}

int canSendMsg(int bus, int id, char len, unsigned char *data, int blocking)
{
    canStatus ret;
    ret = canWrite(hCAN[bus], id, data, len, STD);
    if (canOK != ret)
    {
        printf("CAN write error\n");
        return (int)ret;
    }
    return 0;
}


CANAPI_END
