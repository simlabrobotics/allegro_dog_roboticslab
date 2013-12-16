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
#include "Softing/Can_def.h"
#include "Softing/CANL2.h"
}
#include "canDefs.h"
#include "canAPI.h"


typedef union _float
{
   float value;
   unsigned char arr[4];
}msg_type_float;


CANAPI_BEGIN

#define CH_COUNT			(int)2 // number of CAN channels

static CAN_HANDLE hCAN[CH_COUNT] = {-1, -1}; // CAN channel handles


/*========================================*/
/*       Public functions (CAN API)       */
/*========================================*/
int initCAN(int bus)
{
	assert(bus >= 0 && bus < CH_COUNT);

	int ret = 0;
	
	////////////////////////////////////////////////////////////////////////
	// Init Channel
	char ch_name[256];
	sprintf_s(ch_name, 256, "CAN-ACx-PCI_%d", (bus+1));
	printf("Open CAN channel: %s...\n", ch_name);

	ret = INIL2_initialize_channel(&hCAN[bus], ch_name);
	if (ret)
	{
		printf("\tError: CAN open\n");
		return ret;
	}

	///////////////////////////////////////////////////////////////////////
	// Reset Chip
//	ret = CANL2_reset_chip(hCAN[bus]);
//	if (ret)
//	{
//		printf("\tError: CAN reset chip\n");
//		INIL2_close_channel(hCAN[bus]);
//		hCAN[bus] = 0;
//		return ret;
//	}

	///////////////////////////////////////////////////////////////////////
	// Init Chip
//	ret = CANL2_initialize_chip(hCAN[bus], 1, 1, 4, 3, 0);
//	if (ret)
//	{
//		printf("\tError: CAN set baud rate\n");
//		INIL2_close_channel(hCAN[bus]);
//		hCAN[bus] = 0;
//		return ret;
//	}
	
	///////////////////////////////////////////////////////////////////////
	// Set Acceptance
//	ret = CANL2_set_acceptance(hCAN[bus], m_id, 0x7ff, m_id, 0x1fffffff);
//	if (ret)
//	{
//		printf("\tError: CAN set acceptance\n");
//		INIL2_close_channel(hCAN[bus]);
//		hCAN[bus] = 0;
//		return ret;
//	}
	
	///////////////////////////////////////////////////////////////////////
	// Set Out Control
//	ret = CANL2_set_output_control(hCAN[bus], -1);

	///////////////////////////////////////////////////////////////////////
	// Enable FIFO
	L2CONFIG L2Config;
	L2Config.fBaudrate = 1000.0;
	L2Config.bEnableAck = 0;
	L2Config.bEnableErrorframe = 0;
	L2Config.s32AccCodeStd = 0;
	L2Config.s32AccMaskStd = 0;
	L2Config.s32AccCodeXtd = 0;
	L2Config.s32AccMaskXtd = 0;
	L2Config.s32OutputCtrl = GET_FROM_SCIM;
	L2Config.s32Prescaler = 1;
	L2Config.s32Sam = 0;
	L2Config.s32Sjw = 1;
	L2Config.s32Tseg1 = 4;
	L2Config.s32Tseg2 = 3;
	L2Config.hEvent = (void*)-1;
	ret = CANL2_initialize_fifo_mode(hCAN[bus], &L2Config);
	if (ret)
	{
		printf("\tError: CAN set fifo mode\n");
		INIL2_close_channel(hCAN[bus]);
		hCAN[bus] = 0;
		return ret;
	}

	return 0;
}

int freeCAN(int bus)
{
	INIL2_close_channel(hCAN[bus]);
	hCAN[bus] = 0;
	return 0;
}

int canReadMsg(int bus, int *id, int *len, unsigned char *data, int blocking)
{
    int ret;
	PARAM_STRUCT param;
    int i;
	
	ret = CANL2_read_ac(hCAN[bus], &param);

	switch (ret)
	{
	case CANL2_RA_XTD_DATAFRAME :
        *id = param.Ident;
        *len = param.DataLength;
        for(i = 0; i < *len; i++)
            data[i] = param.RCV_data[i];
		break;

	case CANL2_RA_DATAFRAME:
        *id = param.Ident;
        *len = param.DataLength;
        for(i = 0; i < *len; i++)
            data[i] = param.RCV_data[i];
		break;

	case CANL2_RA_NO_DATA:
		return -1;

	default:
		return -1;
	}

    return 0;
}

int canSendMsg(int bus, int id, char len, unsigned char *data, int blocking)
{
    int ret = CANL2_send_data(hCAN[bus], id, STD, len, (unsigned char*)data);
	if (ret)
	{
		printf("CAN write error\n");
		return ret;
	}

	return 0;
}


CANAPI_END
