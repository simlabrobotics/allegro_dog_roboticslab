/*
 *\brief API for communication over CAN bus 
 *\detailed The API for communicating with the various motor controllers
 *          over the CAN bus interface on the SimLab 6DOF robot arm
 *
 *$Author: Sangyup Yi $
 *$Date: 2012/04/21 23:04:00 $
 *$Revision: 1.0 $
 */ 

#ifndef _CANDAPI_H
#define _CANDAPI_H

#include "canDefs.h"

CANAPI_BEGIN

#ifndef FALSE
#define FALSE (0)
#endif
#ifndef TRUE
#define TRUE (1)
#endif

/*=====================*/
/*       Defines       */
/*=====================*/
//constants
#define TX_QUEUE_SIZE       (32)
#define RX_QUEUE_SIZE       (32)
#define TX_TIMEOUT          (5)
#define RX_TIMEOUT          (5)
#define mbxID               (0)
#define BASE_ID             (0)
#define MAX_BUS             (4)

/******************/
/* CAN device API */
/******************/
int initCAN (int bus);		//initialize the CAN bus
int freeCAN (int bus);		//release the CAN bus
int canSendMsg(int bus, int id, char len, unsigned char *data, int blocking);
int canReadMsg(int bus, int *id, int *len, unsigned char *data, int blocking);

CANAPI_END

#endif
