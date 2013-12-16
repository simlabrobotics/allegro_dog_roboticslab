/*
**                         Copyright 1998 by KVASER AB            
**                   P.O Box 4076 SE-51104 KINNAHULT, SWEDEN
**             E-mail: staff@kvaser.se   WWW: http://www.kvaser.se
**
** This software is furnished under a license and may be used and copied
** only in accordance with the terms of such license.
**
*/
/*
** This file contains definitions which are retained for compatibility.
*/
#ifndef _OBSOLETE_H_
#define _OBSOLETE_H_

#define canCIRCUIT_ANY          -1          // Any circuit will do
#define canCARD_ANY             -1          // Any card will do
#define canCHANNEL_ANY          -1          // Any channel will do

// Retained for compatibility.
typedef void* (*canMemoryAllocator)  (size_t size);
typedef void  (*canMemoryDeallocator)(void* p);

typedef struct tagCanHWDescr {
    int                    circuitType;
    int                    cardType;
    int                    channel;
} canHWDescr;

typedef struct tagCanSWDescr {
    unsigned int           rxBufSize;
    unsigned int           txBufSize;
    canMemoryAllocator     alloc;
    canMemoryDeallocator   deAlloc;
} canSWDescr;

#ifdef __WIN32__
typedef struct {
    unsigned int size;
    unsigned int rxBufSize;
    unsigned int txBufSize;
} canSWDescriptorEx;
#endif

canStatus CANLIBAPI canLocateHardware(void);

int CANLIBAPI canOpen(const canHWDescr * const hwdescr,
                      const canSWDescr * const swdescr,
                      const unsigned int flags);

canStatus CANLIBAPI canGetCircuits(int * context,
                                   char * name,
                                   char * vendor,
                                   char * version,
                                   unsigned int * cardtype,
                                   unsigned int * circtype,
                                   unsigned int * channel);


//
// Some obsolete codes; they are not used nowadays.
//
#define canERR_BUFOFL           canERR_RESERVED_3   // "Buffer overflow"
#define canERR_REINIT           canERR_RESERVED_4   // "Reinitialization failed"
#define canERR_MSGLOST          canERR_RESERVED_5   // "Message lost by CAN-chip"
#define canERR_OVERRUN          canERR_RESERVED_6   // "Receive buffer overrun"
#define canERR_CARDCOMMFAILED   canERR_RESERVED_7   // "Can't communicate with the card"



// Circuit status flags, retained for compatibility
#define canCIRCSTAT_ERROR_PASSIVE 0x0001    // Error passive
#define canCIRCSTAT_BUS_OFF       0x0002    // Bus off
#define canCIRCSTAT_ERROR_WARNING 0x0004    // Error counter > 96

//
// Sticky status flags
//
#define canSTICKY_ACK_ERROR       0x80000801
#define canSTICKY_FORM_ERROR      0x80000002
#define canSTICKY_STUFF_ERROR     0x80000004
#define canSTICKY_CRC_ERROR       0x80000008
#define canSTICKY_BIT0_ERROR      0x80000010
#define canSTICKY_BIT1_ERROR      0x80000020
#define canSTICKY_HW_OVERRUN      0x80000040
#define canSTICKY_SW_OVERRUN      0x80000080

#define canSTAT_INACTIVE          canSTAT_RESERVED_1    // The circuit is not active (removed from the sw bus)


/*
#define canCIRCSTAT_TXERR       0x0001      // TX error has occurred
#define canCIRCSTAT_RXERR       0x0002      // Receive error has occurred
#define canCIRCSTAT_OVERRUN     0x0004      // Receiver overrun
#define canCIRCSTAT_WAKEUP      0x0008      // Wakeup has occurred
#define canCIRCSTAT_INT         0x0010      // ??
#define canCIRCSTAT_ACK         0x0020      // ACK error
#define canCIRCSTAT_BIT         0x0040      // Bit error
*/
/*
More apocryphs.
#define canCIRCSTAT_OVERRUN     0x0004      // Receiver overrun
#define canCIRCSTAT_WAKEUP      0x0008      // Wakeup has occurred
#define canCIRCSTAT_ACK         0x0010      // ACK error
#define canCIRCSTAT_BIT0        0x0020      // Bit error
#define canCIRCSTAT_BIT1        0x0040      // Bit error
#define canCIRCSTAT_BOFF        0x0080      // Chip went Bus Off
*/
/*
Even more.
#define canCIRCSTAT_BOFF_HW     0x10000L      // Chip is Bus Off
#define canCIRCSTAT_BOFF_SW     0x20000L      // We are Bus Off
*/



// Flags for canOpen
#define canWANT_ACTIONS         0x01
#define canWANT_OWN_BUFFERS     0x02
#define canWANT_ERROR_COUNTERS  0x04


// Circuit types.
// No circuit                   0
#define PCCAN_PHILIPS           1           // 82C200 on PCCAN 1.0
#define PCCAN_INTEL526          2           // Not supported.
#define PCCAN_INTEL527          3           // 82527 on PCCAN 1.0
#define CANCARD_NEC72005        4           // NEC72005 on CANCard  
#define CIRC_VIRTUAL            5
#define CIRC_SJA1000            6

// Card types.
#define canCARD_PCCAN         1             // PCCAN ver 1.x (KVASER)
#define canCARD_CANCARD       2             // CANCard (Softing)
#define canCARD_AC2           3             // CAN-AC2 (Softing)
#define canCARD_LAPCAN        4
#define canCARD_ISACAN        5
#define canCARD_PCCAN_OEM     6             // Special for OEM PCcan cards.


// The canFlgXXX are left for compatibility.
#define canFlgACCEPT            1
#define canFlgREJECT            2
#define canFlgCODE              3
#define canFlgMASK              4

// Flags for action routines
#define canDISCARD_MESSAGE      3
#define canRETAIN_MESSAGE       4

// 
// For busParams - sync on rising edge only or both rising and falling edge
// (Not supported)
#define canSLOW_MODE            0x01 // Sync on rising and falling edge

//
// CAN driver types; these constants are retained for compatibility.
//
#define canOFF                  0
#define canTRISTATE             1
#define canPULLUP               2
#define canPULLDOWN             3
#define canPUSHPULL             4        // This is the usual setting.
#define canINVPULLUP            5
#define canINVPULLDOWN          6
#define canINVPUSHPULL          7

// These were never implemented.
#define canIOCTL_LOCAL_ECHO_ON          3
#define canIOCTL_LOCAL_ECHO_OFF         4

// Never implemented.
#define canMSG_STATUS 0x0008


#ifdef __cplusplus
extern "C" {
#endif
canStatus CANLIBAPI canInstallAction(int handle, long id, int (*fn)());

canStatus CANLIBAPI canUninstallAction(int handle, long id);

canStatus CANLIBAPI canInstallOwnBuffer(int handle, long id, unsigned int len,
                                        void * buf);

canStatus CANLIBAPI canUninstallOwnBuffer(int handle, long id);
#ifdef __cplusplus
}
#endif



#endif
