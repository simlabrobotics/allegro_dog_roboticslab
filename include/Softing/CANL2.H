/****************************************************************************/
/*                                                                          */
/*   Softing AG            Richard-Reitzner-Allee 6       85540 Haar        */
/*                                                                          */
/****************************************************************************/
/*                                                                          */
/*   Copyright (C) by Softing AG, 1997 - 2005   All rights reserved.        */
/*                                                                          */
/****************************************************************************/
/*                                                                          */
/*                               C A N L  2 . H                             */
/*                                                                          */
/****************************************************************************/
/*                                                                          */
/* MODULE_DESCRIPTION  CANL2.H                                              */
/* VERSION             4                                                    */
/* DATE                1.12.98                                              */
/*                     include file for CAN Layer 2 Library                 */
/*                     function prototypes and return codes                 */
/*                                                                          */
/****************************************************************************/
#ifndef _CANL2_H
#define _CANL2_H 
             
#if defined (WIN16) || defined (DOS)
#ifndef noUnicode
#define TCHAR char
#define _tcscpy strcpy
#define _tcsncpy strncpy
#define _stprintf sprintf
#define _tcslen strlen
#define _tcscat strcat
#define _tcscmp strcmp
#define _T(wert) wert
#define _tprintf printf
#define noUnicode
#endif
#endif

#define CAN_HANDLE unsigned long



#define  CANL2_SOCKET_AUTO   0xFFFF    /* Socket automatically searched     */
#define  CANL2_NO_IRQ        0x0       /* no interrupt requested            */
#define  CANL2_IRQ_AUTO      0xFFFF    /* interrupt automatically assigned  */
#define  CANL2_BASE_AUTO     0x0L      /* DPR-Base automatically assigned   */
#define  CANL2_SIZE_AUTO     0x0L      /* DPR-Size automatically assigned   */
#define  CANL2_AI_AUTO       0x0       /* chip does no matter               */
#define  CANL2_AI_DB8        0x01      /* chip is Databook                  */
#define  CANL2_READ_INIFILE  0xFFFE    /* DOS CAN-AC2-104: reads INI-File   */

/* ---- CAN chip-types -----------------------------------------------------*/
#define NEC005_CHIP            5
#define SJA1000_CHIP        1000


/* ---- Softing CAN Baugruppentypen ----------------------------------------*/

#define    D_CANCARD2        0x00000005L
#define    D_EDICCARDC       0x00000102L
#define    D_EDICCARD2       0x00000105L
#define    D_CANACPCI        0x00000007L
#define    D_CANACPCIDN      0x00000008L
#define    D_CANAC104        0x00000009L
#define    D_CANUSB          0x0000000AL
#define    D_CANECO104       0x0000000CL
#define    D_CANPROXPCIE     0x0000000DL
#define    D_CANPROX104      0x0000000EL
#define    D_CANFANUPC8      0x00008000L
#define    D_FG100CAN        0x10000000L


#define GET_FROM_SCIM -1



#ifdef WIN32
#pragma pack(push,1)                    /* pack alignment must be one        */
#else
#pragma pack(1)                        /* pack alignment must be one        */
#endif

#ifndef CANL2_EXPORTS
#define MAXLENCHNAME 80
typedef struct
{
  ULONG   u32Serial;   // Serialnumber
  ULONG   u32DeviceType; // device type
  ULONG   u32PhysCh; // physical channel number (1 or 2)
  ULONG   u32FwId; // id of the loaded firmware
  BOOLEAN bIsOpen; // TRUE if opened by a process
  TCHAR ChannelName[MAXLENCHNAME];        


} CHDSNAPSHOT, *PCHDSNAPSHOT;

#endif 



typedef struct canl2_ch_s {
    CAN_HANDLE     ulChannelHandle;
    unsigned char  sChannelName[80];
} CANL2_CH_STRUCT;

typedef struct  {
                  unsigned long  Ident;
                  int            DataLength;
                  int            RecOverrun_flag;
                  int            RCV_fifo_lost_msg;
                  unsigned char  RCV_data[8];
                  int            AckOverrun_flag;
                  int            XMT_ack_fifo_lost_acks;
                  int            XMT_rmt_fifo_lost_remotes;
                  int            Bus_state;
                  int            Error_state;
                  int            Can;
                  unsigned long  Time;
                } PARAM_STRUCT;

typedef struct 
{
  double fBaudrate;
  long s32Prescaler;
  long s32Tseg1;
  long s32Tseg2;
  long s32Sjw;
  long s32Sam;
  long s32AccCodeStd;
  long s32AccMaskStd;
  long s32AccCodeXtd;
  long s32AccMaskXtd;
  long s32OutputCtrl;
  BOOL    bEnableAck;
  BOOL    bEnableErrorframe;
  HANDLE  hEvent;
} L2CONFIG;


typedef struct
{
  unsigned char live_time; // number of 200ms ticks
  BOOL          is_led_on; // should the led be on or off for the live time?

} LED_SUBSTATE;

typedef struct
{
  unsigned short number_of_substates;

  LED_SUBSTATE substate[15];

} LED_FEATURE;


#ifdef WIN32
#pragma pack(pop)                    /* reset pack alignment to prev. value */
#else
#pragma pack()                       /* reset pack alignment to default     */
#endif

#ifndef _ERROR_BASES_
#define _ERROR_BASES_

#define INFORMATIONAL_BASE          (0x60000000)
#define WARNING_BASE                (0xA0000000)
#define ERROR_BASE                  (0xE0000000)

#endif

#define L2_ERROR_BASE              (0x000B0000)
#define VCARD_ERROR_BASE           (0x000A0000)


#define MK_VCARD_ERROR_CODE(VCARD_ERROR) ((VCARD_ERROR) ? (VCARD_ERROR_BASE | ERROR_BASE | VCARD_ERROR) : 0)


    /////////////////////////////////////////////////////////////////////////////
    //  VCARD ERROR CODES (Error codes from kernelmode driver)
    //
#ifndef CANL2_EXPORTS

#define VCARD_OK              0                        // success
#define VCARD_INTERNALERROR              0xE00A0001   // internal Error
#define VCARD_GENERALERROR               0xE00A0002   // general Error
#define VCARD_TIMEOUT                    0xE00A0003   // Timeout
#define VCARD_IOPENDING                  0xE00A0004   // pending driver call
#define VCARD_IOCANCELLED                0xE00A0005   // driver call was canceled
#define VCARD_ILLEGALCALL                0xE00A0006   // illegal driver call
#define VCARD_NOTSUPPORTED               0xE00A0007   // not implemented
#define VCARD_VERSIONERROR               0xE00A0008   // driver interface dll has the wrong version
#define VCARD_DRIVERVERSIONERROR         0xE00A0009   // wrong driver version
#define VCARD_DRIVERNOTFOUND             0xE00A000A   // driver not loaded / not installed, or device is not plugged
#define VCARD_NOTENOUGHMEMORY            0xE00A000B   // out of memory
#define VCARD_TOOMANYDEVICES             0xE00A000C   // driver can not handle as much devices
#define VCARD_UNKNOWNDEVICE              0xE00A000D   // unknown device
#define VCARD_DEVICEALREADYEXISTS        0xE00A000E   // device already exists
#define VCARD_DEVICEACCESSERROR          0xE00A000F   // access is not possible: device is already open
#define VCARD_RESOURCEALREADYREGISTERED  0xE00A0010   // ressorce is in use by an other device
#define VCARD_RESOURCECONFLICT           0xE00A0011   // ressorce conflict
#define VCARD_RESOURCEACCESSERROR        0xE00A0012   // ressorce can not be accessed
#define VCARD_PHYSMEMORYOVERRUN          0xE00A0013   // ungueltiger Zugriff auf physikalischen Speicher
#define VCARD_TOOMANYPORTS               0xE00A0014   // zu viele I/O Ports 20
#define VCARD_INTERRUPTERROR             0xE00A0015   // error while activating the interrupt
#define VCARD_UNKNOWNRESOURCE            0xE00A0016   // unknown ressorce
#define VCARD_CREATEDEVFAILED            0xE00A0017   // IoCreateDevice failed
#define VCARD_ATTACHTODEVSTACKFAILED     0xE00A0018   // IoAttachDeviceToDeviceStack failed
#define VCARD_SYMBOLICLINKFAILED         0xE00A0019   // failed to create a symbolic link
        //////////////////////////////////////////////////////////////////////
        ///    Errors which can occur while downloading the firmware
        //////////////////////////////////////////////////////////////////////
#define VCARD_NOCARDSERVICES             0xE00A001A   // can not access card services under Win '98
#define VCARD_CARDSERVICESVERSION        0xE00A001B   // wrong version of the card services under Win '98
#define VCARD_CARDSERVICESGETINFO        0xE00A001C   // error while accessing the card services under Win '98
#define VCARD_DEVICENOTFOUND             0xE00A001D   // device not found.
#define VCARD_NODPRAM                    0xE00A001E   // can not get a free address region for DPRAM from system
#define VCARD_CONTROLHWERROR             0xE00A001F   // Error while accessing hardware
#define VCARD_SBNCHECKSUM                0xE00A0020   // Checksum error in SBN format (dll binary may be corrupt)
#define VCARD_DPRAMACCESS                0xE00A0021   // can not access the DPRAM memory
#define VCARD_CARDREACTION               0xE00A0022   // Loader program for firmware download does no more react.
#define VCARD_NOSTARTADDRESS             0xE00A0023   // No startaddress defined in SBN (dll binary may be corrupt)
#define VCARD_NOINTERRUPT                0xE00A0024   // Interrupt does not work
        //////////////////////////////////////////////////////////////////////
        ///    Errors which can occur in the channel driver
        //////////////////////////////////////////////////////////////////////

#define VCARD_DRIVERNOTPRESENT           0xE00A0025   // Kernelmode driver is not loaded
#define VCARD_DEVICEISOPEN               0xE00A0026   // Device is already open
#define VCARD_DEVICELOCKINGERROR         0xE00A0027   // Device can not be locked
#define VCARD_OTHERFWISLOADED            0xE00A0028   // A other firmware is running on that device (CANalyzer/CANopen/DeviceNet firmware)
#define VCARD_CHANNELNOTOPEN             0xE00A0029   // channel can not be accessed, because it is not open.

        //////////////////////////////////////////////////////////////////////
        ///    status codes for CANusb Device Driver canusbw.sys
        //////////////////////////////////////////////////////////////////////

#define VCARD_PNPCALLERROR               0xE00A002A   // lower driver call in PnP process fails
#define VCARD_URBRETERROR                0xE00A002B   // URB returns USBD_ERROR code
#define VCARD_ERRORDEVICEDESC            0xE00A002C   // faulty device desc or read failed
#define VCARD_ERRORCONFIGDESC            0xE00A002D   // faulty config desc or read failed
#define VCARD_ERRORSELECTCONFIG          0xE00A002E   // unable to select configuration
#define VCARD_ERRORDECONFIG              0xE00A002F   // unable to deconfigure the device
#define VCARD_PIPEACCESSERROR            0xE00A0030   // unable to open usb pipe
#define VCARD_COMMUNICATIONBROKEN        0xE00A0031   // communication via usb pipe broken off

        //////////////////////////////////////////////////////////////////////
        ///    Errors which can occur in the canchd.dll
        //////////////////////////////////////////////////////////////////////
#define VCARD_FILENOTFOUND               0xE00A0032   // file not found
#define VCARD_ACCESSRIGHT                0xE00A0033

#define VCARD_OSERROR                    0xE00A0034   // error in OS call
#define VCARD_DEVICEIDMISMATCH           0xE00A0035   // wrong device id stored in registry
#define VCARD_MAXNUMOFCHANNELSOPEN       0xE00A0036   // the maximum number of channels are open
#define VCARD_INVALIDHANDLE              0xE00A0037   // a invalid handle is specified

#endif

/* ---- FUNCTION PROTOTYPES AND FUNCTION RETURN VALUES  --------------------*/
/* default return values -- valid for all functions (if no other value is 
   described)                                                               */
/* default return values -- valid for all functions                         */

#define  CANL2_OK                          0  /* function successful        */
#define  CANL2_ERR                        -1  /* function not successful    */
#define  CANL2_BOARD_NOT_INITIALIZED     -99  /* board not initialized:
                                                 INIL2_initialize_channel(.) 
                                                 was not yet called, or a 
                                                 INIL2_close_board was done */
#define CANL2_INVALID_CHANNEL_HANDLE     -1000 /* the channel handle is invalid */
#define CANL2_TOO_MANY_CHANNELS          -1002 /* no free channel available */
#define CANL2_VERSION_CONFLICT           -1003 /* wrong dll or driver version */
#define CANL2_FIRMWARE_DOWNLOAD          -1004 /* error while loading the firmware
                                                  may be a DPRAM access error. */

#define CANL2_CAN_NOT_LOAD_CANUSBDLL     -1005 /* the canusbm.dll can not be found/loaded */


#define INIL2_FW_FILE_NOT_FOUND          -2000 /* firmware file not found */
#define INIL2_ERROR_WHILE_FLASHING       -2001 /* flashing error */





/* ---- CANusb return values                                                */

#define FRW_ERROR_PIPEACCESS_ERROR       -602 /* unable to open usb pipe    */ 
#define FRW_ERROR_COMMUNICATIONBROKEN    -603 /* communication via usb pipe
                                                 broken                     */
#define FRW_ERR_NOVALID_LUPT_ENTRY       -604 /* no valid lookup table entry 
                                                 chosen                     */

/* ---- INIL2_initialize_board   -------------------------------------------*/

/* -- CANusb return values                                                  */
#define FRW_IF_ERR_FRWINIT_FAILED        -611 /* CANusb Framework 
                                                 initialization failed      */
       
#define CANL2_LIC_ERR						-101
#define CANL2_WRONG_PARAM					-102  // error in a parameter value
#define CANL2_FW_TIMEOUT					-104  // timout while communicating with the firmware
#define CANL2_HW_ERR						-108  // firmware runs on the wrong hardware
#define CANL2_DC_NO_DYN_OBJBUF				-109  // no object dyn. buffer 
#define CANL2_REQ_PENDING                   -110  // last request pending

#define CANL2_DATA_OVR						-111  // receive data overrun           
#define CANL2_REMOTE_OVR					-112  // rec. rem. frame overrun        
#define CANL2_NOT_ACTIVE					-113  // object not active              
#define CANL2_RXD_XMT_OVR                   -114  // transmit ack. overrun         
#define CANL2_OB_NOT_DEFINED				-115  // object not defined
#define CANL2_XMT_REQ_OVR                   -116  // xmt request fifo overrun
#define CANL2_NOT_IMPLEMENTED               -117  // not supported for this device

#define FRW_ERR_GDI_OLDSFW          -614 /* flash content out of date       */

#define ERROR_LINKING_FCT			-701 /* DLL is not compatible (canusbm.dll) */
#define ERROR_LINKING_DLL			-702 /* DLL not found (canusbm.dll)*/



PRAEDEF int MIDDEF INIL2_initialize_channel(CAN_HANDLE *pu32ChannelHandle, char *pChannelName);
#define INIL2_IB_ERR_VC_NOTENOUGHMEMORY  0xFE0B /* not enough memory       */

#ifdef WIN32
// +--- CANL2_set_interrupt_event ------------------------------------------
PRAEDEF int MIDDEF CANL2_set_interrupt_event(CAN_HANDLE Can, HANDLE hEvent);

// +--- CANL2_set_interrupt_event_name -------------------------------------
PRAEDEF int MIDDEF CANL2_set_interrupt_event_name(CAN_HANDLE Can, TCHAR *eventName);
#endif
// +--- CANL2_reset_chip         -------------------------------------------
PRAEDEF int MIDDEF CANL2_reset_chip(CAN_HANDLE Can);


// +--- CANL2_initialize_chip(CAN_HANDLE Can, 2) -------------------------------------------
// Error codes
//CANL2_II_REA_CONFLICT                   Parameter conflict: 

PRAEDEF int MIDDEF CANL2_initialize_chip (CAN_HANDLE Can, int presc,
                               int sjw,
                               int tseg1,
                               int tseg2,
                               int sam);




/* +--- CANL2_set_acceptance(CAN_HANDLE Can, 2)  ------------------------------------------- */
PRAEDEF int MIDDEF CANL2_set_acceptance (CAN_HANDLE Can, unsigned int AccCodeStd,
                              unsigned int AccMaskStd,
                              unsigned long AccCodeXtd,
                              unsigned long AccMaskXtd);

/* +--- CANL2_set_output_control(CAN_HANDLE Can, 2)  --------------------------------------- */
PRAEDEF int MIDDEF CANL2_set_output_control (CAN_HANDLE Can, int OutputControl);


// +--- CANL2_initialize_interface   ---------------------------------------
// Error codes
//CANL2_II_REA_CONFLICT                   Parameter conflict: 
                                     //   ReceiveEnableAll with dyn.
                                     //   obj. buffer or fifo mode   

PRAEDEF int MIDDEF CANL2_initialize_interface(CAN_HANDLE Can, int ReceiveFifoEnable,
                                   int ReceivePollAll,
                                   int ReceiveEnableAll,
                                   int ReceiveIntEnableAll,
                                   int AutoRemoteEnable,
                                   int TransmitReqFifoEnable,
                                   int TransmitPollAll,
                                   int TransmitAckEnableAll,
                                   int TransmitAckFifoEnable,
                                   int TransmitRmtFifoEnable);

// +--- CANL2_define_object(CAN_HANDLE Can, 2)          ------------------------------------
#define  CANL2_DO_PARAM_ERR               -1  // Parameter error            
#define  CANL2_DO_NO_DYNOBJ               -2  // dyn. obj. buf. not enabled 
#define  CANL2_DO_TOO_MANY_OBJECTS        -6  // too many objects defined   

PRAEDEF int MIDDEF CANL2_define_object (CAN_HANDLE Can, unsigned long Handle,
                             int FAR * ObjectNumber,
                             int Type,
                             int ReceiveIntEnable,
                             int AutoRemoteEnable,
                             int TransmitAckEnable);


// +--- CANL2_define_cyclic(CAN_HANDLE Can, 2)        --------------------------------------
#define  CANL2_DC_INVLD_OBJ_NR            -1  // invalid object number      

PRAEDEF int MIDDEF CANL2_define_cyclic (CAN_HANDLE Can, int ObjectNumber,
                             unsigned int Rate,
                             unsigned int Cycles);


// +--- CANL2_set_rcv_fifo_size      ---------------------------------------
//#define  CANL2_SRFS_PARAM_ERR               -1  // Parameter error

#define  CANL2_SRFS_NO_FIFO_MODE            -2  // no fifo mode enabled

PRAEDEF int MIDDEF CANL2_set_rcv_fifo_size(CAN_HANDLE Can,  int FifoSize);


// +--- CANL2_enable_fifo            ---------------------------------------

#define CANL2_WRONG_MODE                    -1001
PRAEDEF int MIDDEF CANL2_enable_fifo(CAN_HANDLE Can);

// +--- CANL2_optimize_rcv_speed     ---------------------------------------
// this function has no effect on the CANcard                               
#define  CANL2_ORS_PARAM_ERR              -1  // Parameter error            


// +--- CANL2_enable_dyn_obj_buf     ---------------------------------------

PRAEDEF int MIDDEF CANL2_enable_dyn_obj_buf(CAN_HANDLE Can);

// +--- CANL2_enable_fifo_transmit_ack  ------------------------------------

PRAEDEF int MIDDEF CANL2_enable_fifo_transmit_ack (CAN_HANDLE Can);


PRAEDEF int MIDDEF CANL2_initialize_fifo_mode(CAN_HANDLE Can, L2CONFIG *pUserCfg);


PRAEDEF int MIDDEF CANL2_get_all_CAN_channels(unsigned long u32ProvidedBufferSize,
		 unsigned long *pu32NeededBufferSize,
		 unsigned long *pu32NumOfChannels,
		 CHDSNAPSHOT *pBuffer);


// +--- CANL2_get_version            ---------------------------------------
// the parameter hw_version is not used with the CAN-AC2                    

PRAEDEF int MIDDEF CANL2_get_version(CAN_HANDLE Can, int FAR * sw_version, 
                                       int FAR * fw_version,
                                       int FAR * hw_version,
                                       int FAR * license,
                                       int FAR * can_chip_type);

// +--- CANL2_get_serial_number      ---------------------------------------
// this function has no effect on the CAN-AC2                               

PRAEDEF int MIDDEF CANL2_get_serial_number(CAN_HANDLE Can, unsigned long FAR * ser_number);

// +--- CANL2_start_chip             ---------------------------------------
// -- CANusb return values                                            
#define FRW_IOE_ERR_NOTENOUGHMEMORY     -612    // not enough memory  
#define FRW_IOE_ERR_INITFAILED          -613    // failed to initialize FRWOrder/Event   

PRAEDEF int MIDDEF CANL2_start_chip(CAN_HANDLE Can);

// +--- CANL2_send_remote_object(CAN_HANDLE Can, 2)  ---------------------------------------
#define  CANL2_SRO_PEND                   -1  // last request still pending     
#define  CANL2_SRO_TX_FIFO_FULL           -3  // transmit fifo full             

PRAEDEF int MIDDEF CANL2_send_remote_object (CAN_HANDLE Can, int ObjectNumber,
                                  int DataLength);


// +--- CANL2_supply_object_data(CAN_HANDLE Can, 2)  ---------------------------------------
#define  CANL2_SOD_REQ_OVR                -1  // request overrun                

PRAEDEF int MIDDEF CANL2_supply_object_data (CAN_HANDLE Can, int  ObjectNumber,
                                  int  DataLength,
                                  unsigned char FAR * pData);


// +--- CANL2_supply_rcv_object_data(CAN_HANDLE Can, 2)-------------------------------------

PRAEDEF int MIDDEF CANL2_supply_rcv_object_data (CAN_HANDLE Can, int  ObjectNumber,
                                      int  DataLength,
                                      unsigned char FAR * pData);


// +--- CANL2_send_object(CAN_HANDLE Can, 2)         ---------------------------------------

PRAEDEF int MIDDEF CANL2_send_object (CAN_HANDLE Can, int  ObjectNumber,
                           int  DataLength);

// +--- CANL2_write_object(CAN_HANDLE Can, 2)        ---------------------------------------

PRAEDEF int MIDDEF CANL2_write_object (CAN_HANDLE Can, int  ObjectNumber,
                            int  DataLength,
                            unsigned char FAR * pData);


 

// +--- CANL2_read_rcv_data(CAN_HANDLE Can, 2)       ---------------------------------------
#define CANL2_RRD_DATAFRAME					1  // data frame received            
#define CANL2_RRD_REMOTEFRAME				2  // remote frame received   


PRAEDEF int MIDDEF CANL2_read_rcv_data (CAN_HANDLE Can, int  ObjectNumber,
                             unsigned char FAR * pRCV_Data,
                             unsigned long FAR * Time);



// +--- CANL2_read_xmt_data(CAN_HANDLE Can, 2)       ---------------------------------------
#define  CANL2_RXD_NO_DATA                 0  // no message was transmitted     
#define  CANL2_RXD_DATAFRAME               1  // message was transmitted        
#define  CANL2_RXD_NO_DYN_OBJBUF          -3  // no dyn. obj-buffer enabled
                                              //   (only CAN-AC2/527)             

PRAEDEF int MIDDEF CANL2_read_xmt_data (CAN_HANDLE Can, int  ObjectNumber,
                             int FAR * pDataLength,
                             unsigned char FAR * pXMT_Data);



// +--- CANL2_read_ac                ---------------------------------------*/
#define  CANL2_RA_NO_DATA                 0  // no new data received        */    
#define  CANL2_RA_DATAFRAME               1  // std. data frame received    */    
#define  CANL2_RA_REMOTEFRAME             2  // std. remote frame received  */    
#define  CANL2_RA_TX_DATAFRAME            3  // transmission of std. data-
                                             //   frame is confirmed          */    
#define  CANL2_RA_TX_FIFO_OVR             4  // remote tx fifo overrun      */    
#define  CANL2_RA_CHG_BUS_STATE           5  // change of bus status        */    
#define  CANL2_RA_ERR_ADD                 7  // additional error causes     */    
#define  CANL2_RA_TX_REMOTEFRAME          8  // transmission of std. data-
                                             //   frame is confirmed          */    
#define  CANL2_RA_XTD_DATAFRAME           9  // xtd. data frame received    */    
#define  CANL2_RA_XTD_TX_DATAFRAME       10  // transmission of xtd. data-
                                             //   frame is confirmed          */    
#define  CANL2_RA_XTD_TX_REMOTEFRAME     11  // transmission of xtd. remote-
                                             //   frame is confirmed          */    
#define  CANL2_RA_XTD_REMOTEFRAME        12  // xtd. remote frame received 
                                             //   (only CANcard)              */    
#define  CANL2_RA_ERRORFRAME             15  // error frame detected  
                                             //   (only CANcard)              */    

PRAEDEF int MIDDEF CANL2_read_ac(CAN_HANDLE Can, PARAM_STRUCT* param);

// +--- CANL2_send_data              ---------------------------------------*/
#define  CANL2_SD_FIFO_FULL               -1  // transmit fifo full         */
#define  CANL2_SD_PARAM                   -11 // wrong parameter

PRAEDEF int MIDDEF CANL2_send_data (CAN_HANDLE Can, unsigned long Ident,
                         int Xtd,
                         int DataLength,
                         unsigned char* pData);



/* +--- CANL2_send_remote            ---------------------------------------*/
#define  CANL2_SR_FIFO_FULL               -1  /* transmit fifo full         */    


PRAEDEF int MIDDEF CANL2_send_remote (CAN_HANDLE Can, unsigned long Ident,
                           int Xtd,
                           int DataLength);

 

/* +--- CANL2_get_trigger(CAN_HANDLE Can, 2)            ---------------------------------------*/
/* these functions have no effect on the CANcard                               */
PRAEDEF int MIDDEF CANL2_get_trigger (CAN_HANDLE Can, int FAR * level);

PRAEDEF int MIDDEF CANL2_get_trigger2(CAN_HANDLE Can, int FAR * level);

/* +--- CANL2_set_trigger            ---------------------------------------*/
#define  CANL2_ST_FIFO_FULL               -1  /* transmit fifo full         */    

PRAEDEF int MIDDEF CANL2_set_trigger (CAN_HANDLE Can, int level);


/* +--- CANL2_reinitialize           ---------------------------------------*/
PRAEDEF int MIDDEF CANL2_reinitialize(CAN_HANDLE Can);

/* +--- CANL2_get_time               ---------------------------------------*/
PRAEDEF int MIDDEF CANL2_get_time(CAN_HANDLE Can, unsigned long FAR * time);

/* +--- CANL2_get_bus_state          ---------------------------------------*/
#define  CANL2_GBS_ERROR_ACTIVE            0  /* error active               */    
#define  CANL2_GBS_ERROR_PASSIVE           1  /* error passive              */    
#define  CANL2_GBS_ERROR_BUS_OFF           2  /* bus off                    */    

PRAEDEF int MIDDEF CANL2_get_bus_state(CAN_HANDLE Can);


/* +--- CANL2_reset_rcv_fifo         ---------------------------------------*/
PRAEDEF int MIDDEF CANL2_reset_rcv_fifo(CAN_HANDLE Can);

/* +--- CANL2_reset_xmt_fifo         ---------------------------------------*/
PRAEDEF int MIDDEF CANL2_reset_xmt_fifo(CAN_HANDLE Can);

/* +--- CANL2_reset_lost_msg_counter  --------------------------------------*/
PRAEDEF int MIDDEF CANL2_reset_lost_msg_counter(CAN_HANDLE Can);

/* +--- CANL2_read_rcv_fifo_level    ---------------------------------------*/
/* returns number of items in receive fifo   */
PRAEDEF int MIDDEF CANL2_read_rcv_fifo_level(CAN_HANDLE Can);

/* +--- CANL2_read_xmt_fifo_level    ---------------------------------------*/
/* returns number of items in transmit fifo  */
PRAEDEF int MIDDEF CANL2_read_xmt_fifo_level(CAN_HANDLE Can);

/* +--- CANL2_set_path               ---------------------------------------*/
#define  CANL2_SP_STRING_TOO_LONG        -1  /* path string too long        */    
PRAEDEF int MIDDEF CANL2_set_path(CAN_HANDLE Can, TCHAR FAR * path);

/* +--- INIL2_close_board            ---------------------------------------*/
PRAEDEF int MIDDEF INIL2_close_channel(CAN_HANDLE Can);
#define  INIL2_CB_ERR                     -1  /* error unlocking rerssources*/    

/* +--- CANL2_enable_error_frame_detection  --------------------------------*/
/* this function has no effect on the CAN-AC2                               */
PRAEDEF int MIDDEF CANL2_enable_error_frame_detection(CAN_HANDLE Can);

/* +--- CANL2_get_device_id  -----------------------------------------------*/
/* this function has no effect on the CAN-AC2                               */
PRAEDEF int MIDDEF CANL2_get_device_id(CAN_HANDLE Can, unsigned long *pulDeviceId);

/* +--- CANL2_init_signals/CANL2_read_signals/CANL2_write_signals            */
/* these functions are only available on the CAN-ACx-PCI. They are designed  */
/* for use with the CAN-ACx-PCI and piggy-back boards. These functions are   */
/* not in the standard users manual. For description read the piggy-back     */
/* manuals                                                                   */
PRAEDEF int MIDDEF CANL2_init_signals (CAN_HANDLE Can, unsigned long ulChannelDirectionMask,
                                         unsigned long ulChannelOutputDefaults);

PRAEDEF int MIDDEF CANL2_read_signals  (CAN_HANDLE Can, unsigned long *pulChannelRead);

PRAEDEF int MIDDEF CANL2_write_signals  (CAN_HANDLE Can, unsigned long pulChannelWrite, 
                                          unsigned long ulCareMask);


#endif  /*_CANL2_H */


