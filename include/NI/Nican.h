/*
NICAN.H

This is the C/C++ header file for all NI-CAN Application Programming
Interfaces (APIs), including:
* NI-CAN Frame API
* NI-CAN Channel API

This header file is not intended for use as a programming reference.
For complete information on the data types and functions 
declared in this file, refer to the NI-CAN manual.
*/
#ifdef internal
/*
NOTE to NI developers: When adding new defines, take care to ensure
that constants do not overlap between the three APIs.
*/
#endif
#ifdef internal
/**********************************************************************
**                C o m p i l e r   s w i t c h e s                  **
***********************************************************************
** BASIC            Used for automatic translation of the headerfile **
**                  to a visual basic module.                        **
** NO_FRAME_API     Excludes the Frame API definitions from the      **
**                  header file.                                     **
** NO_CHANNEL_API   Excludes the Channel API definitions from the    **
**                  header file.                                     **
** NO_DNET_API      Excludes the DNET API definitions from the       **
**                  header file.                                     **
**********************************************************************/
#endif
/**********************************************************************/

   /* This constant prevents redefinition errors when you include
   nican.h multiple times.  There is no need to define it in your code.  */
#ifndef __NC_NOINC_nican
#define __NC_NOINC_nican

#ifndef BASIC
/***********************************************************************
                     C O M P I L E R   D E F I N E S
***********************************************************************/

   /* Needed for C++ to C (DLL) calls  */
#ifdef __cplusplus
   extern "C" {
#endif

   /* These constants can be used to selectively exclude certain classes
   of constants.  There is normally no need to define them in your code.  */
#ifndef __NC_NOINC_compiler

   /* Used for function prototypes */
#ifdef _NCDEF_NOT_DLL_
   #define  _NCFUNC_

   #define NCT_FUNC
#else
   #define  _NCFUNC_                   __stdcall

   #ifdef MAPPER_EXPORTS
   #  define NCT_FUNC   _declspec(dllexport) __stdcall
   #else
   #  define NCT_FUNC   __stdcall
   #endif
#endif


   /* This is required when using ncReadMult from C++.
   By default the compiler returns sizeof(NCTYPE_CAN_FRAME_TIMED)
   as 24 due to alignment on 4-byte boundaries (it pads 2 bytes at the end).  
   Since the ncReadMult function returns an array of frames back-to-back, 
   we must ensure that sizeof(NCTYPE_CAN_FRAME_TIMED) is returned as 22, 
   the actual size of the structure.  Aligning structure members on 2-byte 
   boundaries accomplishes this.  */
#pragma  pack(2)
#ifdef _CVI_
#pragma  EnableLibraryRuntimeChecking
#endif /* _CVI_ */

#endif /* __NC_NOINC_compiler */
#endif /* BASIC */

#ifndef NO_FRAME_API
/*****************************************************************************/
/******************** N I - C A N   F R A M E    A P I ***********************/
/*****************************************************************************/

/***********************************************************************
                            D A T A   T Y P E S
***********************************************************************/

#ifndef __NC_NOINC_type

typedef  char                       NCTYPE_INT8;
typedef  short                      NCTYPE_INT16;
typedef  long                       NCTYPE_INT32;
typedef  unsigned char              NCTYPE_UINT8;
typedef  unsigned short             NCTYPE_UINT16;
typedef  unsigned long              NCTYPE_UINT32;
typedef  float                      NCTYPE_REAL;
typedef  double                     NCTYPE_LREAL;

   /* This two-part declaration is required for compilers which do not
   provide support for native 64-bit integers.  */
typedef  struct {
   NCTYPE_UINT32                       LowPart;
   NCTYPE_UINT32                       HighPart;
} NCTYPE_UINT64;

//#BASIC Exclude
   /* Boolean value is encoded as an unsigned 8-bit integer in which
   bit 0 is used to indicate true or false, and the upper 7 bits
   are always zero.  */
//#BASIC End Exclude
typedef  unsigned char              NCTYPE_BOOL;

//#BASIC Exclude
   /* ASCII string is encoded like all C language strings: as an array
   of characters with zero (the NULL character) used to indicate
   end-of-string.  This is known as an ASCIIZ string. */
//#BASIC End Exclude
typedef  char *                     NCTYPE_STRING;

#ifdef BASIC
typedef  char *                     NCTYPE_STR;
#endif /* BASIC */

typedef  NCTYPE_INT32               NCTYPE_STATUS;
typedef  NCTYPE_UINT32              NCTYPE_OBJH;
typedef  NCTYPE_UINT32              NCTYPE_VERSION;
typedef  NCTYPE_UINT32              NCTYPE_DURATION;
typedef  NCTYPE_UINT32              NCTYPE_ATTRID;
typedef  NCTYPE_UINT32              NCTYPE_OPCODE;
typedef  NCTYPE_UINT32              NCTYPE_PROTOCOL;
typedef  NCTYPE_UINT32              NCTYPE_BAUD_RATE;
typedef  NCTYPE_UINT32              NCTYPE_STATE;

//#BASIC Exclude
   /* Pointer to a variable of any type */
//#BASIC End Exclude
typedef  void *                     NCTYPE_ANY_P;

//#BASIC Exclude
   /* Pointers (used for function prototypes) */
//#BASIC End Exclude
typedef  NCTYPE_INT8 *              NCTYPE_INT8_P;
typedef  NCTYPE_INT16 *             NCTYPE_INT16_P;
typedef  NCTYPE_INT32 *             NCTYPE_INT32_P;
typedef  NCTYPE_UINT8 *             NCTYPE_UINT8_P;
typedef  NCTYPE_UINT16 *            NCTYPE_UINT16_P;
typedef  NCTYPE_UINT32 *            NCTYPE_UINT32_P;
typedef  NCTYPE_REAL *              NCTYPE_REAL_P;
typedef  NCTYPE_LREAL *             NCTYPE_LREAL_P;
typedef  NCTYPE_UINT64 *            NCTYPE_UINT64_P;
typedef  NCTYPE_BOOL *              NCTYPE_BOOL_P;
typedef  NCTYPE_STATUS *            NCTYPE_STATUS_P;
typedef  NCTYPE_OBJH *              NCTYPE_OBJH_P;
typedef  NCTYPE_VERSION *           NCTYPE_VERSION_P;
typedef  NCTYPE_DURATION *          NCTYPE_DURATION_P;
typedef  NCTYPE_ATTRID *            NCTYPE_ATTRID_P;
typedef  NCTYPE_OPCODE *            NCTYPE_OPCODE_P;
typedef  NCTYPE_PROTOCOL *          NCTYPE_PROTOCOL_P;
typedef  NCTYPE_BAUD_RATE *         NCTYPE_BAUD_RATE_P;
typedef  NCTYPE_STATE *             NCTYPE_STATE_P;


#ifndef __NC_NOINC_createnotif

   /* This is the prototype for callback function passed to
   ncCreateNotification (cannot be used to declare your callback). */
//#BASIC Include DNet_API
#ifdef BASIC
typedef long NCTYPE_NOTIFY_CALLBACK;
#else
typedef  NCTYPE_STATE (_NCFUNC_ * NCTYPE_NOTIFY_CALLBACK) (
                                    NCTYPE_OBJH       ObjHandle,
                                    NCTYPE_STATE      CurrentState,
                                    NCTYPE_STATUS     Status,
                                    NCTYPE_ANY_P      RefData);
#endif
//#BASIC End Include
#endif /* __NC_NOINC_createnotif */

typedef  NCTYPE_UINT32              NCTYPE_COMM_TYPE;
typedef  NCTYPE_UINT32              NCTYPE_RTSI_MODE;
typedef  NCTYPE_UINT32              NCTYPE_RTSI_SIG_BEHAV;

//#BASIC Exclude
   /* This type can be typecasted to/from the Microsoft 
   Win32 type FILETIME (see MSVC 4.x WINBASE.H).  When 
   your host computer powers up, NC_ATTR_ABS_TIME is loaded
   with the system time of the host computer (value obtained
   using Win32 GetSystemTimeAsFileTime).  Thus, the timestamps
   obtained using ncRead can be converted to local time zone
   format using Win32 FileTimeToLocalFileTime, then can be
   converted to SYSTEMTIME (with year, month, day, and so on)
   using Win32 FileTimeToSystemTime.  */
//#BASIC End Exclude
typedef  NCTYPE_UINT64              NCTYPE_ABS_TIME;

//#BASIC Exclude
   /* CAN arbitration ID (used for both standard and extended IDs).
   The bit NC_FL_CAN_ARBID_XTD (0x20000000) indicates extended ID.  */
//#BASIC End Exclude
typedef  NCTYPE_UINT32              NCTYPE_CAN_ARBID;

   /* Type for ncWrite of CAN Network Interface Object */
typedef  struct {
   NCTYPE_CAN_ARBID                    ArbitrationId;
   NCTYPE_BOOL                         IsRemote;
   NCTYPE_UINT8                        DataLength;
   NCTYPE_UINT8                        Data[8];
} NCTYPE_CAN_FRAME;

   /* Type for ncRead of CAN Network Interface Object */
typedef  struct {
   NCTYPE_ABS_TIME                     Timestamp;
   NCTYPE_CAN_ARBID                    ArbitrationId;
   NCTYPE_BOOL                         IsRemote;
   NCTYPE_UINT8                        DataLength;
   NCTYPE_UINT8                        Data[8];
} NCTYPE_CAN_FRAME_TIMED;

   /* Type for ncRead of CAN Network Interface Object (using FrameType instead of IsRemote).
   Type for ncWrite of CAN Network Interface Object when timed transmission is enabled. */
typedef  struct {
   NCTYPE_ABS_TIME                     Timestamp;
   NCTYPE_CAN_ARBID                    ArbitrationId;
   NCTYPE_UINT8                        FrameType;
   NCTYPE_UINT8                        DataLength;
   NCTYPE_UINT8                        Data[8];
} NCTYPE_CAN_STRUCT;

   /* Type for ncWrite of CAN Object */
typedef  struct {
   NCTYPE_UINT8                        Data[8];
} NCTYPE_CAN_DATA;

   /* Type for ncRead of CAN Object */
typedef  struct {
   NCTYPE_ABS_TIME                     Timestamp;
   NCTYPE_UINT8                        Data[8];
} NCTYPE_CAN_DATA_TIMED;

//#BASIC Exclude
   /* Pointers (used for function prototypes) */
//#BASIC Exclude Exclude
typedef  NCTYPE_COMM_TYPE *         NCTYPE_COMM_TYPE_P;
typedef  NCTYPE_ABS_TIME *          NCTYPE_ABS_TIME_P;
typedef  NCTYPE_CAN_ARBID *         NCTYPE_CAN_ARBID_P;
typedef  NCTYPE_CAN_FRAME *         NCTYPE_CAN_FRAME_P;
typedef  NCTYPE_CAN_FRAME_TIMED *   NCTYPE_CAN_FRAME_TIMED_P;
typedef  NCTYPE_CAN_STRUCT *        NCTYPE_CAN_STRUCT_P;
typedef  NCTYPE_CAN_DATA *          NCTYPE_CAN_DATA_P;
typedef  NCTYPE_CAN_DATA_TIMED *    NCTYPE_CAN_DATA_TIMED_P;
typedef  NCTYPE_RTSI_MODE *         NCTYPE_RTSI_MODE_P;
typedef  NCTYPE_RTSI_SIG_BEHAV *    NCTYPE_RTSI_SIG_BEHAV_P;

//#BASIC Exclude
   /* Included for backward compatibility with older versions of NI-CAN */
//#BASIC End Exclude
typedef  NCTYPE_UINT32              NCTYPE_BKD_TYPE;
typedef  NCTYPE_UINT32              NCTYPE_BKD_WHEN;
typedef  NCTYPE_BKD_TYPE *          NCTYPE_BKD_TYPE_P;
typedef  NCTYPE_BKD_WHEN *          NCTYPE_BKD_WHEN_P;

#endif /* __NC_NOINC_type */

/***********************************************************************
                              S T A T U S
***********************************************************************/

#ifndef __NC_NOINC_status
   /* NCTYPE_STATUS

   NI-CAN and NI-DNET use the standard NI status format.
   This status format does not use bit-fields, but rather simple
   codes in the lower byte, and a common base for the upper bits.  
   This standard NI status format ensures that all NI-CAN errors are located
   in a specific range of codes.  Since this range does not overlap with
   errors reported from other NI products, NI-CAN is supported within
   environments such as LabVIEW and MeasurementStudio.

   If your application currently uses the NI-CAN legacy error codes, 
   you must change to the standard NI error codes.  For instructions on updating
   your code, refer to KnowledgeBase article # 2BBD8JHR on www.ni.com.

   If you shipped an executable to your customers that uses the legacy
   NI-CAN error codes, and you must upgrade those customers to the
   newest version of NI-CAN, contact National Instruments Technical 
   Support to obtain instructions for re-enabling the legacy status format.
   */
#define  NICAN_WARNING_BASE         (NCTYPE_STATUS)(0x3FF62000)
#define  NICAN_ERROR_BASE           (NCTYPE_STATUS)(0xBFF62000)

   /* Success values (you can simply use zero as well)  */
#define  CanSuccess                 0
#define  DnetSuccess                0

   /* Numbers 0x001 to 0x0FF are used for status codes defined prior to 
   NI-CAN v1.5.  These codes can be mapped to/from the legacy status format.  */
#define  CanErrFunctionTimeout      (NICAN_ERROR_BASE | 0x001)    // was NC_ERR_TIMEOUT
#define  CanErrWatchdogTimeout      (NICAN_ERROR_BASE | 0x021)    // was NC_ERR_TIMEOUT
#define  DnetErrConnectionTimeout   (NICAN_ERROR_BASE | 0x041)    // was NC_ERR_TIMEOUT
#define  DnetWarnConnectionTimeout  (NICAN_WARNING_BASE | 0x041)
#define  CanErrScheduleTimeout      (NICAN_ERROR_BASE | 0x0A1)    // was NC_ERR_TIMEOUT
#define  CanErrDriver               (NICAN_ERROR_BASE | 0x002)    // was NC_ERR_DRIVER
#define  CanWarnDriver              (NICAN_WARNING_BASE | 0x002)    // was NC_ERR_DRIVER
#define  CanErrBadNameSyntax        (NICAN_ERROR_BASE | 0x003)    // was NC_ERR_BAD_NAME
#define  CanErrBadIntfName          (NICAN_ERROR_BASE | 0x023)    // was NC_ERR_BAD_NAME
#define  CanErrBadCanObjName        (NICAN_ERROR_BASE | 0x043)    // was NC_ERR_BAD_NAME
#define  CanErrBadParam             (NICAN_ERROR_BASE | 0x004)    // was NC_ERR_BAD_PARAM
#define  CanErrBadHandle            (NICAN_ERROR_BASE | 0x024)    // was NC_ERR_BAD_PARAM
#define  CanErrBadAttributeValue    (NICAN_ERROR_BASE | 0x005)    // was NC_ERR_BAD_ATTR_VALUE
#define  CanErrAlreadyOpen          (NICAN_ERROR_BASE | 0x006)    // was NC_ERR_ALREADY_OPEN
#define  CanWarnAlreadyOpen         (NICAN_WARNING_BASE | 0x006)  // was NC_ERR_ALREADY_OPEN
#define  DnetErrOpenIntfMode        (NICAN_ERROR_BASE | 0x026)    // was NC_ERR_ALREADY_OPEN
#define  DnetErrOpenConnType        (NICAN_ERROR_BASE | 0x046)    // was NC_ERR_ALREADY_OPEN
#define  CanErrNotStopped           (NICAN_ERROR_BASE | 0x007)    // was NC_ERR_NOT_STOPPED
#define  CanErrOverflowWrite        (NICAN_ERROR_BASE | 0x008)    // was NC_ERR_OVERFLOW
#define  CanErrOverflowCard         (NICAN_ERROR_BASE | 0x028)    // was NC_ERR_OVERFLOW
#define  CanErrOverflowChip         (NICAN_ERROR_BASE | 0x048)    // was NC_ERR_OVERFLOW
#define  CanErrOverflowRxQueue      (NICAN_ERROR_BASE | 0x068)    // was NC_ERR_OVERFLOW
#define  CanWarnOldData             (NICAN_WARNING_BASE | 0x009)  // was NC_ERR_OLD_DATA
#define  CanErrNotSupported         (NICAN_ERROR_BASE | 0x00A)    // was NC_ERR_NOT_SUPPORTED
#define  CanWarnComm                (NICAN_WARNING_BASE | 0x00B)  // was NC_ERR_CAN_COMM
#define  CanErrComm                 (NICAN_ERROR_BASE | 0x00B)    // was NC_ERR_CAN_COMM
#define  CanWarnCommStuff           (NICAN_WARNING_BASE | 0x02B)  // was NC_ERR_CAN_COMM
#define  CanErrCommStuff            (NICAN_ERROR_BASE | 0x02B)    // was NC_ERR_CAN_COMM
#define  CanWarnCommFormat          (NICAN_WARNING_BASE | 0x04B)  // was NC_ERR_CAN_COMM
#define  CanErrCommFormat           (NICAN_ERROR_BASE | 0x04B)    // was NC_ERR_CAN_COMM
#define  CanWarnCommNoAck           (NICAN_WARNING_BASE | 0x06B)  // was NC_ERR_CAN_COMM
#define  CanErrCommNoAck            (NICAN_ERROR_BASE | 0x06B)    // was NC_ERR_CAN_COMM
#define  CanWarnCommTx1Rx0          (NICAN_WARNING_BASE | 0x08B)  // was NC_ERR_CAN_COMM
#define  CanErrCommTx1Rx0           (NICAN_ERROR_BASE | 0x08B)    // was NC_ERR_CAN_COMM
#define  CanWarnCommTx0Rx1          (NICAN_WARNING_BASE | 0x0AB)  // was NC_ERR_CAN_COMM
#define  CanErrCommTx0Rx1           (NICAN_ERROR_BASE | 0x0AB)    // was NC_ERR_CAN_COMM
#define  CanWarnCommBadCRC          (NICAN_WARNING_BASE | 0x0CB)  // was NC_ERR_CAN_COMM
#define  CanErrCommBadCRC           (NICAN_ERROR_BASE | 0x0CB)    // was NC_ERR_CAN_COMM
#define  CanWarnCommUnknown         (NICAN_WARNING_BASE | 0x0EB)  // was NC_ERR_CAN_COMM
#define  CanErrCommUnknown          (NICAN_ERROR_BASE | 0x0EB)    // was NC_ERR_CAN_COMM
#define  CanWarnTransceiver         (NICAN_WARNING_BASE | 0x00C)  // was NC_ERR_CAN_XCVR
#define  CanWarnRsrcLimitQueues     (NICAN_WARNING_BASE | 0x02D)  // was NC_ERR_RSRC_LIMITS
#define  CanErrRsrcLimitQueues      (NICAN_ERROR_BASE | 0x02D)    // was NC_ERR_RSRC_LIMITS
#define  DnetErrRsrcLimitIO         (NICAN_ERROR_BASE | 0x04D)    // was NC_ERR_RSRC_LIMITS
#define  DnetErrRsrcLimitWriteSrvc  (NICAN_ERROR_BASE | 0x06D)    // was NC_ERR_RSRC_LIMITS
#define  DnetErrRsrcLimitReadSrvc   (NICAN_ERROR_BASE | 0x08D)    // was NC_ERR_RSRC_LIMITS
#define  DnetErrRsrcLimitRespPending (NICAN_ERROR_BASE | 0x0AD)   // was NC_ERR_RSRC_LIMITS
#define  DnetWarnRsrcLimitRespPending (NICAN_WARNING_BASE | 0x0AD)
#define  CanErrRsrcLimitRtsi        (NICAN_ERROR_BASE | 0x0CD)    // was NC_ERR_RSRC_LIMITS
#define  DnetErrNoReadAvail         (NICAN_ERROR_BASE | 0x00E)    // was NC_ERR_READ_NOT_AVAIL
#define  DnetErrBadMacId            (NICAN_ERROR_BASE | 0x00F)    // was NC_ERR_BAD_NET_ID
#define  DnetErrDevInitOther        (NICAN_ERROR_BASE | 0x010)    // was NC_ERR_DEVICE_INIT
#define  DnetErrDevInitIoConn       (NICAN_ERROR_BASE | 0x030)    // was NC_ERR_DEVICE_INIT
#define  DnetErrDevInitInputLen     (NICAN_ERROR_BASE | 0x050)    // was NC_ERR_DEVICE_INIT
#define  DnetErrDevInitOutputLen    (NICAN_ERROR_BASE | 0x070)    // was NC_ERR_DEVICE_INIT
#define  DnetErrDevInitEPR          (NICAN_ERROR_BASE | 0x090)    // was NC_ERR_DEVICE_INIT
#define  DnetErrDevInitVendor       (NICAN_ERROR_BASE | 0x0B0)    // was NC_ERR_DEVICE_INIT
#define  DnetErrDevInitDevType      (NICAN_ERROR_BASE | 0x0D0)    // was NC_ERR_DEVICE_INIT
#define  DnetErrDevInitProdCode     (NICAN_ERROR_BASE | 0x0F0)    // was NC_ERR_DEVICE_INIT
#define  DnetErrDeviceMissing       (NICAN_ERROR_BASE | 0x011)    // was NC_ERR_DEVICE_MISSING
#define  DnetWarnDeviceMissing      (NICAN_WARNING_BASE | 0x011)  // was NC_ERR_DEVICE_MISSING
#define  DnetErrFragmentation       (NICAN_ERROR_BASE | 0x012)    // was NC_ERR_FRAGMENTATION
#define  DnetErrIntfNotOpen         (NICAN_ERROR_BASE | 0x033)    // was NC_ERR_NO_CONFIG
#define  DnetErrErrorResponse       (NICAN_ERROR_BASE | 0x014)    // was NC_ERR_DNET_ERR_RESP
#define  CanWarnNotificationPending (NICAN_WARNING_BASE | 0x015)  // was NC_ERR_NOTIF_PENDING
#define  CanErrConfigOnly           (NICAN_ERROR_BASE | 0x017)    // was NC_ERR_CONFIG_ONLY
#define  CanErrPowerOnSelfTest      (NICAN_ERROR_BASE | 0x018)    // PowerOn self test Failure

#define  ni985xErrBluebonnet	     	(NICAN_ERROR_BASE | 0x0AA)    // Special error for 985x 1.3 to prevent usage on 907x targets

#define  LinErrCommBit              (NICAN_ERROR_BASE | 0x1A0)    // Detected incorrect bit value
#define  LinErrCommFraming          (NICAN_ERROR_BASE | 0x1A1)    // Detected incorrect stop bit of the frame
#define  LinErrCommResponseTimout   (NICAN_ERROR_BASE | 0x1A2)    // Detected a timeout for a field of the LIN frame.
#define  LinErrCommWakeup           (NICAN_ERROR_BASE | 0x1A3)    // Detected unexpected behavior when attempting to wake, or be awakened by, the LIN.  
#define  LinErrCommForm             (NICAN_ERROR_BASE | 0x1A4)    // Detected that the form of a LIN frame was incorrect. 
#define  LinErrCommBusNoPowered     (NICAN_ERROR_BASE | 0x1A5)    // Did not detect power on the LIN. 

//The percent difference between the passed in baud rate and the actual baud rate was greater than or equal to 0.5%.  LIN 2.0 specifies a clock tolerance of less than 0.5% for a master and less than 1.5% for a slave.
#define  LinWarnBaudRateOutOfTolerance (NICAN_WARNING_BASE | 0x1A6)    

   // Numbers 0x100 to 0x1FF are used for the NI-CAN Frame API, the NI-DNET API, and LIN.
   // Numbers 0x1A0 to 0x1DF are used for the NI-CAN Frame API for LIN .
   // Numbers 0x200 to 0x2FF are used for the NI-CAN Channel API.
   // Numbers 0x300 to 0x3FF are reserved for future use.
#define  CanErrMaxObjects           (NICAN_ERROR_BASE | 0x100)
#define  CanErrMaxChipSlots         (NICAN_ERROR_BASE | 0x101)
#define  CanErrBadDuration          (NICAN_ERROR_BASE | 0x102)
#define  CanErrFirmwareNoResponse   (NICAN_ERROR_BASE | 0x103)
#define  CanErrBadIdOrOpcode        (NICAN_ERROR_BASE | 0x104)
#define  CanWarnBadSizeOrLength     (NICAN_WARNING_BASE | 0x105)
#define  CanErrBadSizeOrLength      (NICAN_ERROR_BASE | 0x105)
#define  CanErrNotifAlreadyInUse    (NICAN_ERROR_BASE | 0x107)
#define  CanErrOneProtocolPerCard   (NICAN_ERROR_BASE | 0x108)
#define  CanWarnPeriodsTooFast      (NICAN_WARNING_BASE | 0x109)
#define  CanErrDllNotFound          (NICAN_ERROR_BASE | 0x10A)
#define  CanErrFunctionNotFound     (NICAN_ERROR_BASE | 0x10B)
#define  CanErrLangIntfRsrcUnavail  (NICAN_ERROR_BASE | 0x10C)
#define  CanErrRequiresNewHwSeries  (NICAN_ERROR_BASE | 0x10D)
#define  CanErrHardwareNotSupported CanErrRequiresNewHwSeries
#define  CanErrSeriesOneOnly        (NICAN_ERROR_BASE | 0x10E)  //depreciated error code
#define  CanErrSetAbsTime           (NICAN_ERROR_BASE | 0x10F)
#define  CanErrBothApiSameIntf      (NICAN_ERROR_BASE | 0x110)
#define  CanErrWaitOverlapsSameObj  (NICAN_ERROR_BASE | 0x111)
#define  CanErrNotStarted           (NICAN_ERROR_BASE | 0x112)                   
#define  CanErrConnectTwice         (NICAN_ERROR_BASE | 0x113)
#define  CanErrConnectUnsupported   (NICAN_ERROR_BASE | 0x114)
#define  CanErrStartTrigBeforeFunc  (NICAN_ERROR_BASE | 0x115)
#define  CanErrStringSizeTooLarge   (NICAN_ERROR_BASE | 0x116)
#define  CanErrQueueReqdForReadMult (NICAN_ERROR_BASE | 0x117)
#define  CanErrHardwareInitFailed   (NICAN_ERROR_BASE | 0x118)
#define  CanErrOldDataLost          (NICAN_ERROR_BASE | 0x119)
#define  CanErrOverflowChannel      (NICAN_ERROR_BASE | 0x11A)
#define  CanErrUnsupportedModeMix   (NICAN_ERROR_BASE | 0x11C)   
#define  CanErrNoNetIntfConfig      (NICAN_ERROR_BASE | 0x11D)   
#define  CanErrBadTransceiverMode   (NICAN_ERROR_BASE | 0x11E)   
#define  CanErrWrongTransceiverAttr (NICAN_ERROR_BASE | 0x11F) 
#define  CanErrRequiresXS           (NICAN_ERROR_BASE | 0x120) 
#define  CanErrDisconnected         (NICAN_ERROR_BASE | 0x121) 
#define  CanErrNoTxForListenOnly    (NICAN_ERROR_BASE | 0x122)
#define  CanErrSetOnly              (NICAN_ERROR_BASE | 0x123) 
#define  CanErrBadBaudRate          (NICAN_ERROR_BASE | 0x124) 
#define  CanErrOverflowFrame        (NICAN_ERROR_BASE | 0x125)
#define  CanWarnRTSITooFast         (NICAN_WARNING_BASE | 0x126)
#define  CanErrNoTimebase           (NICAN_ERROR_BASE | 0x127)
#define  CanErrTimerRunning         (NICAN_ERROR_BASE | 0x128)
#define  DnetErrUnsupportedHardware (NICAN_ERROR_BASE | 0x129)
#define  CanErrInvalidLogfile       (NICAN_ERROR_BASE | 0x12A)
#define  CanErrMaxPeriodicObjects   (NICAN_ERROR_BASE | 0x130)
#define  CanErrUnknownHardwareAttribute (NICAN_ERROR_BASE | 0x131)
#define  CanErrDelayFrameNotSupported (NICAN_ERROR_BASE | 0x132)
#define  CanErrVirtualBusTimingOnly (NICAN_ERROR_BASE | 0x133)

#define  CanErrVirtualNotSupported  (NICAN_ERROR_BASE | 0x135)
#define  CanErrWriteMultLimit       (NICAN_ERROR_BASE | 0x136)   // WriteMult does not allow write more that 512 frames at a time
#define  CanErrObsoletedHardware    (NICAN_ERROR_BASE | 0x137)
#define  CanErrVirtualBusTimingMismatch   (NICAN_ERROR_BASE | 0x138)
#define  CanErrVirtualBusOnly       (NICAN_ERROR_BASE | 0x139)
#define  CanErrConversionTimeRollback   (NICAN_ERROR_BASE | 0x13A)
#define  CanErrInterFrameDelayExceeded (NICAN_ERROR_BASE | 0x140)
#define  CanErrLogConflict             (NICAN_ERROR_BASE | 0x141)
#define  CanErrBootLoaderUpdated (NICAN_ERROR_BASE | 0x142)// Error, bootloader not compatible with firmware.

   /* Included for backward compatibility with older versions of NI-CAN */
#define  CanWarnLowSpeedXcvr        CanWarnTransceiver   // applies to HS as well as LS
#define  CanErrOverflowRead         CanErrOverflowCard   // overflow in card memory now lower level

#endif /* __NC_NOINC_status */

/***********************************************************************
                          A T T R I B U T E   I D S
***********************************************************************/

#ifndef __NC_NOINC_attrid
   /* Attributes of the NI driver (NCTYPE_ATTRID values)
      For every attribute ID, its full name, datatype,
      permissions, and applicable objects are listed in the comment.
   */

         /* Current State, NCTYPE_STATE, Get, CAN Interface/Object */
#define  NC_ATTR_STATE              0x80000009
         /* Status, NCTYPE_STATUS, Get, CAN Interface/Object */
#define  NC_ATTR_STATUS             0x8000000A
         /* Baud Rate, Set, CAN Interface
            Note that in addition to standard baud rates like 125000,
            this attribute also allows you to program non-standard
            or otherwise uncommon baud rates.  If bit 31 (0x80000000)
            is set, the low 16 bits of this attribute are programmed
            directly into the bit timing registers of the CAN 
            communications controller.  The low byte is programmed as
            BTR0 of the Intel 82527 chip (8MHz clock), and the high byte 
            as BTR1, resulting in the following bit map:
               15  14  13  12  11  10   9   8   7   6   5   4   3   2   1   0
               sam (tseg2 - 1) (  tseg1 - 1   ) (sjw-1) (     presc - 1     )
            For example, baud rate 0x80001C03 programs the CAN communications
            controller for 125000 baud (same baud rate 125000 decimal).
            For more information, refer to the reference manual for
            any CAN communications controller chip.  */
#define  NC_ATTR_BAUD_RATE          0x80000007
         /* Start On Open, NCTYPE_BOOL, Set, CAN Interface */
#define  NC_ATTR_START_ON_OPEN      0x80000006	      
         /* Absolute Time, NCTYPE_ABS_TIME, Get, CAN Interface */
#define  NC_ATTR_ABS_TIME           0x80000008
         /* Period, NCTYPE_DURATION, Set, CAN Object */
#define  NC_ATTR_PERIOD             0x8000000F
         /* Timestamping, NCTYPE_BOOL, Set, CAN Interface */
#define  NC_ATTR_TIMESTAMPING       0x80000010
         /* Read Pending, NCTYPE_UINT32, Get, CAN Interface/Object */
#define  NC_ATTR_READ_PENDING       0x80000011
         /* Write Pending, NCTYPE_UINT32, Get, CAN Interface/Object */
#define  NC_ATTR_WRITE_PENDING      0x80000012
         /* Read Queue Length, NCTYPE_UINT32, Set, CAN Interface/Object */
#define  NC_ATTR_READ_Q_LEN         0x80000013
         /* Write Queue Length, NCTYPE_UINT32, Set, CAN Interface/Object */
#define  NC_ATTR_WRITE_Q_LEN        0x80000014
         /* Receive Changes Only, NCTYPE_BOOL, Set, CAN Object */
#define  NC_ATTR_RX_CHANGES_ONLY    0x80000015
         /* Communication Type, NCTYPE_COMM_TYPE, Set, CAN Object */
#define  NC_ATTR_COMM_TYPE          0x80000016
         /* RTSI Mode, NCTYPE_RTSI_MODE, Set, CAN Interface/Object */
#define  NC_ATTR_RTSI_MODE          0x80000017
         /* RTSI Signal, NCTYPE_UINT8, Set, CAN Interface/Object */
#define  NC_ATTR_RTSI_SIGNAL        0x80000018
         /* RTSI Signal Behavior, NCTYPE_RTSI_SIG_BEHAV, 
         Set, CAN Interface/Object */
#define  NC_ATTR_RTSI_SIG_BEHAV     0x80000019
        /* RTSI Frame for CAN Object, NCTYPE_UINT32, Get/Set, CAN Object */
#define  NC_ATTR_RTSI_FRAME         0x80000020
        /* RTSI Skip Count, NCTYPE_UINT32, Set, CAN Interface/Object */
#define  NC_ATTR_RTSI_SKIP          0x80000021

         /* Standard Comparator, NCTYPE_CAN_ARBID, Set, CAN Interface */
#define  NC_ATTR_COMP_STD           0x80010001
         /* Standard Mask (11 bits), NCTYPE_UINT32, Set, CAN Interface */
#define  NC_ATTR_MASK_STD           0x80010002
         /* Extended Comparator (29 bits), NCTYPE_CAN_ARBID, Set, CAN Interface */
#define  NC_ATTR_COMP_XTD           0x80010003
         /* Extended Mask (29 bits), NCTYPE_UINT32, Set, CAN Interface */
#define  NC_ATTR_MASK_XTD           0x80010004
         /* Transmit By Response, NCTYPE_BOOL, Set, CAN Object */
#define  NC_ATTR_TX_RESPONSE        0x80010006
         /* Data Frame Length, NCTYPE_UINT32, Set, CAN Object */
#define  NC_ATTR_DATA_LEN           0x80010007
         /* Log Comm Errors, NCTYPE_BOOL, Get/Set,
         CAN Interface (Low-speed boards only) */
#define  NC_ATTR_LOG_COMM_ERRS      0x8001000A
		   /* Notify Multiple Length, NCTYPE_UINT32, Get/Set, CAN Interface/Object */
#define  NC_ATTR_NOTIFY_MULT_LEN    0x8001000B
         /* Receive Queue Length, NCTYPE_UINT32, Set, CAN Interface */
#define  NC_ATTR_RX_Q_LEN           0x8001000C
          /* Is bus timing enabled on virtual hardware,
            NC_FALSE: Used in case of frame-channel conversion
            NC_TRUE: NI-CAN will simluate the bus timing on TX, set timestamp 
            of the frame when it is received,default behavior,NCTYPE_UINT32,Get/Set,
            CAN Interface   */  
#define  NC_ATTR_VIRTUAL_BUS_TIMING  0xA0000031
         /* Transmit mode,NCTYPE_UINT32,Get/Set,
             0 (immediate) [default], 1 (timestamped)
            CAN Interface */
#define  NC_ATTR_TRANSMIT_MODE      0x80020029
         /* Log start trigger in the read queue,NCTYPE_UINT32,Get/Set,
            CAN Interface*/
#define  NC_ATTR_LOG_START_TRIGGER     0x80020031
         /* Timestamp format in absolute or relative mode, NCTYPE_UINT32, Get/Set,
            CAN Interface .*/
#define  NC_ATTR_TIMESTAMP_FORMAT   0x80020032
         /* Rate of the incoming clock pulses.Rate can either be
            10(M Series DAQ) or 20(E series DAQ) Mhz,NCTYPE_UINT32,Get/Set,
            CAN Interface */
#define  NC_ATTR_MASTER_TIMEBASE_RATE  0x80020033
         /* Number of frames that can be written without overflow,NCTYPE_UINT32, Get,CAN Interface*/
#define  NC_ATTR_WRITE_ENTRIES_FREE        0x80020034
         /* Timeline recovery,NCTYPE_UINT32,Get/Set,
             Valid only in timestamped transmit mode (NC_ATTR_TRANSMIT_MODE = 1)
             0 (false) [default], 1 (true)
             CAN Interface .*/       
#define  NC_ATTR_TIMELINE_RECOVERY  0x80020035
         /* Log Bus Errors, NCTYPE_UINT32, Get/Set, CAN Interface/Object  
             0 (false) [default], 1 (true)*/
#define  NC_ATTR_LOG_BUS_ERROR  0x80020037
         /* Log Transceiver Faults, NCTYPE_UINT32, Get/Set, CAN Interface/Object 
             Log NERR into NI read queue
             0 (false) [default], 1 (true)*/
#define  NC_ATTR_LOG_TRANSCEIVER_FAULT  0x80020038 
         /* Termination Select, NCTYPE_UINT32, Get/Set, CAN Interface/Object 
             CAN HS - Not Selectable
             CAN LS - 0 (1k ohm) [default], 1 (5k ohm)
             LIN - 0 (disabled) [default], 1 (enabled) */
#define  NC_ATTR_TERMINATION  0x80020041
         /*** LIN Attributes **/
         /* LIN - Sleep, NCTYPE_UINT32, Get/Set,
             0 (False) [default], 1 (true) */
#define  NC_ATTR_LIN_SLEEP  0x80020042
         /* LIN - Check Sum Type, NCTYPE_UINT32, Get/Set,
             0 (classic) [default], 1 (enhanced) */
#define  NC_ATTR_LIN_CHECKSUM_TYPE  0x80020043
         /* LIN - Response Timeout, NCTYPE_UINT32, Get/Set,
             0 [default], 1 - 65535 (in 50 us increments for LIN specific frame response timing) */
#define  NC_ATTR_LIN_RESPONSE_TIMEOUT  0x80020044
         /* LIN - Enable DLC Check, NCTYPE_UINT32, Get/Set, 
             0 (false) [default], 1 (true) */
#define  NC_ATTR_LIN_ENABLE_DLC_CHECK  0x80020045 
         /* LIN - Log Wakeup, NCTYPE_UINT32, Get/Set,
             0 (false) [default], 1 (true) */
#define  NC_ATTR_LIN_LOG_WAKEUP  0x80020046
      
   /* Attributes specific to Series 2 hardware (not supported on Series 1). */

         /* Enable Listen Only on SJA1000, NCTYPE_UINT32, Get/Set 
             0 (false) [default], 1 (enable)*/
#define  NC_ATTR_LISTEN_ONLY        0x80010010
         /* Returns Receive Error Counter from SJA1000, NCTYPE_UINT32, Get, CAN Interface/Object */
#define  NC_ATTR_RX_ERROR_COUNTER   0x80010011
         /* Returns Send Error Counter from SJA1000, NCTYPE_UINT32, Get, CAN Interface/Object */
#define  NC_ATTR_TX_ERROR_COUNTER   0x80010012
         /* Series 2 Comparator, NCTYPE_UINT32, Get/Set, CAN Interface/Object
             11 bits or 29 bits depending on NC_ATTR_SERIES2_FILTER_MODE
             0 [default] */
#define  NC_ATTR_SERIES2_COMP       0x80010013
         /* Series 2 Mask, NCTYPE_UINT32, Get/Set, CAN Interface/Object
             11 bits or 29 bits depending on NC_ATTR_SERIES2_FILTER_MODE
             0xFFFFFFFF [default] */
#define  NC_ATTR_SERIES2_MASK       0x80010014
         /* Series 2 Filter Mode, NCTYPE_UINT32, Get/Set, CAN Interface/Object 
             NC_FILTER_SINGLE_STANDARD [default], NC_FILTER_SINGLE_EXTENDED,
             NC_FILTER_DUAL_EXTENDED, NC_FILTER_SINGLE_STANDARD*/
#define  NC_ATTR_SERIES2_FILTER_MODE 0x80010015
         /* Self Reception, NCTYPE_UINT32, Get/Set, CAN Interface/Object
             Echo transmitted frames in read queue
             0 (false) [default], 1 (true) */
#define  NC_ATTR_SELF_RECEPTION     0x80010016
         /* Single Shot Transmit, NCTYPE_UINT32, Get/Set, CAN Interface/Object
             Single Shot = No retry on error transmissions
             0 (false) [default]. 1 (true) */
#define  NC_ATTR_SINGLE_SHOT_TX     0x80010017
#define  NC_ATTR_BEHAV_FINAL_OUT    0x80010018
         /* Transceiver Mode, NCTYPE_UINT32, Get/Set, CAN Interface/Object 
             NC_TRANSCEIVER_MODE_NORMAL [default], NC_TRANSCEIVER_MODE_SLEEP,
             NC_TRANSCEIVER_MODE_SW_HIGHSPEED, NC_TRANSCEIVER_MODE_SW_WAKEUP*/
#define  NC_ATTR_TRANSCEIVER_MODE   0x80010019
         /* Transceiver External Out, NCTYPE_UINT32, Bitmask, Get/Set, CAN Interface 
             on XS cards and external transceivers, it sets MODE0 and MODE1 pins on CAN port,
             and sleep of CAN controller chip
             NC_TRANSCEIVER_OUT_MODE0 | NC_TRANSCEIVER_OUT_MODE1 [default]
             NC_TRANSCEIVER_OUT_MODE0, NC_TRANSCEIVER_OUT_MODE1, NC_TRANSCEIVER_OUT_SLEEP*/ 
#define  NC_ATTR_TRANSCEIVER_EXTERNAL_OUT 0x8001001A
         /* Transceiver External In, NCTYPE_UINT32, Get, CAN Interface 
             on XS cards, reads STATUS pin on CAN port*/
#define  NC_ATTR_TRANSCEIVER_EXTERNAL_IN 0x8001001B
         /* Error Code Capture and Arbitration Lost Capture, NCTYPE_UINT32, Get, CAN Interface/Object
             Returns Error Code Capture and Arbitration Lost Capture registers */
#define  NC_ATTR_SERIES2_ERR_ARB_CAPTURE 0x8001001C
         /* Transceiver Type, NCTYPE_UINT32, Get/Set, CAN Interface 
             NC_TRANSCEIVER_TYPE_DISC (disconnect), NC_TRANSCEIVER_TYPE_EXT (external),
             NC_TRANSCEIVER_TYPE_HS (high speed), NC_TRANSCEIVER_TYPE_LS (low speed),
             NC_TRANSCEIVER_TYPE_SW (single wire)*/
#define  NC_ATTR_TRANSCEIVER_TYPE   0x80020007  
   


   /* Informational attributes (hardware and version info).  Get, CAN Interface only 
   These attribute IDs can be used with ncGetHardwareInfo and ncGetAttribute.  */
#define  NC_ATTR_NUM_CARDS          0x80020002     // Number of Cards present in system.
#define  NC_ATTR_HW_SERIAL_NUM      0x80020003     // Serial Number of card
#define  NC_ATTR_HW_FORMFACTOR      0x80020004     // Formfactor of card - NC_HW_FORMFACTOR_???
#define  NC_ATTR_HW_SERIES          0x80020005     // Series of Card - NC_HW_SERIES_???
#define  NC_ATTR_NUM_PORTS          0x80020006     // Number of Ports present on card
#define  NC_ATTR_HW_TRANSCEIVER     NC_ATTR_TRANSCEIVER_TYPE // NC_HW_TRANSCEIVER_???
#define  NC_ATTR_INTERFACE_NUM      0x80020008     // 0 for CAN0, 1 for CAN1, etc...
#define  NC_ATTR_VERSION_MAJOR      0x80020009     // U32 major version (X in X.Y.Z)
#define  NC_ATTR_VERSION_MINOR      0x8002000A     // U32 minor version (Y in X.Y.Z)
#define  NC_ATTR_VERSION_UPDATE     0x8002000B     // U32 minor version (Z in X.Y.Z)
#define  NC_ATTR_VERSION_PHASE      0x8002000C     // U32 phase (1=alpha, 2=beta, 3=release)
#define  NC_ATTR_VERSION_BUILD      0x8002000D     // U32 build (primarily useful for beta)
#define  NC_ATTR_VERSION_COMMENT    0x8002000E     // String comment on version (max 80 chars)

   /* Included for backward compatibility with older versions of NI-CAN */
      /* NCTYPE_ATTRID values, CAN Interface/Object */
#define  NC_ATTR_PROTOCOL           0x80000001
#define  NC_ATTR_PROTOCOL_VERSION   0x80000002
#define  NC_ATTR_SOFTWARE_VERSION   0x80000003
#define  NC_ATTR_BKD_READ_SIZE      0x8000000B
#define  NC_ATTR_BKD_WRITE_SIZE     0x8000000C
#define  NC_ATTR_BKD_TYPE           0x8000000D
#define  NC_ATTR_BKD_WHEN_USED      0x8000000E
#define  NC_ATTR_BKD_PERIOD         0x8000000F
#define  NC_ATTR_BKD_CHANGES_ONLY   0x80000015
#define  NC_ATTR_SERIAL_NUMBER      0x800000A0
#define  NC_ATTR_CAN_BIT_TIMINGS    0x80010005
#define  NC_ATTR_BKD_CAN_RESPONSE   0x80010006
#define  NC_ATTR_CAN_DATA_LENGTH    0x80010007
#define  NC_ATTR_CAN_COMP_STD       0x80010001
#define  NC_ATTR_CAN_MASK_STD       0x80010002
#define  NC_ATTR_CAN_COMP_XTD       0x80010003
#define  NC_ATTR_CAN_MASK_XTD       0x80010004
#define  NC_ATTR_CAN_TX_RESPONSE    0x80010006
#define  NC_ATTR_NOTIFY_MULT_SIZE   0x8001000B
#define  NC_ATTR_RESET_ON_START     0x80010008
#define  NC_ATTR_NET_SYNC_COUNT     0x8001000D
#define  NC_ATTR_IS_NET_SYNC        0x8001000E
#define  NC_ATTR_START_TRIG_BEHAVIOR 0x80010023      
      /* NCTYPE_BKD_TYPE values */
#define  NC_BKD_TYPE_PEER2PEER      0x00000001
#define  NC_BKD_TYPE_REQUEST        0x00000002
#define  NC_BKD_TYPE_RESPONSE       0x00000003
      /* NCTYPE_BKD_WHEN values */
#define  NC_BKD_WHEN_PERIODIC       0x00000001
#define  NC_BKD_WHEN_UNSOLICITED    0x00000002
      /* Special values for background read/write data 
      sizes (NC_ATTR_BKD_READ_SIZE and NC_ATTR_BKD_WRITE_SIZE). */
#define  NC_BKD_CAN_ZERO_SIZE       0x00008000

#endif /* __NC_NOINC_attrid */

/***********************************************************************
                    O T H E R   C O N S T A N T S
***********************************************************************/

#ifndef __NC_NOINC_other
   /* NCTYPE_BOOL (true/false values) */
#define  NC_TRUE                    1
#define  NC_FALSE                   0

   /* NCTYPE_DURATION (values in one millisecond ticks) */
#define  NC_DURATION_NONE           0              /* zero duration */
#define  NC_DURATION_INFINITE       0xFFFFFFFF     /* infinite duration */
#define  NC_DURATION_1MS            1              /* one millisecond */
#define  NC_DURATION_10MS           10
#define  NC_DURATION_100MS          100
#define  NC_DURATION_1SEC           1000           /* one second */
#define  NC_DURATION_10SEC          10000
#define  NC_DURATION_100SEC         100000
#define  NC_DURATION_1MIN           60000          /* one minute */

   /* NCTYPE_PROTOCOL (values for supported protocols) */
#define  NC_PROTOCOL_CAN            1              /* Controller Area Net */
#define  NC_PROTOCOL_DNET           2              /* DeviceNet */
#define  NC_PROTOCOL_LIN            3              /* LIN */


   /* NCTYPE_STATE (bit masks for states). 
   Refer to other NC_ST values below for backward compatibility. */
//#BASIC Include DNet_API
//#BASIC Enum NC_STATE_ENUM
      /* Any object */
#define  NC_ST_READ_AVAIL           0x00000001
      /* Any object */
#define  NC_ST_WRITE_SUCCESS        0x00000002

#ifndef __NC_NOINC_other
         /* Expl Msg object only */
   #define  NC_ST_ESTABLISHED          0x00000008
#endif
   /* Included for backward compatibility with older versions of NI-CAN */
      /* NCTYPE_STATE (bit masks for states): Prior to NI-CAN 2.0,
      the Stopped state emabled detection of the Bus Off condition, 
      which stopped communication independent of NI-CAN functions such as ncAction(Stop). 
      Since the Bus Off condition is an error, and errors are detected automatically
      in v2.0 and later, this state is now obsolete. For compatibility, it may be
      returned as the DetectedState of ncWaitForState, but this bit should be
      ignored by new NI-CAN applications. */
#define  NC_ST_STOPPED              0x00000004
      /* NCTYPE_STATE (bit masks for states): Prior to NI-CAN 2.0,
      the Error state emabled detection of background errors (such as the Bus Off condition). 
      For v2.0 and later, the ncWaitForState function returns automatically when any
      error occurs, so this state is now obsolete. For compatibility, it may be
      returned as the DetectedState of ncWaitForState, but this bit should be
      ignored by new NI-CAN applications. */
#define  NC_ST_ERROR                0x00000010
      /* NCTYPE_STATE (bit masks for states): Prior to NI-CAN 2.0,
      the Warning state emabled detection of background warnings (such as Error Passive). 
      For v2.0 and later, the ncWaitForState function will not abort when a warning occurs, 
      but it will return the warning, so this state is now obsolete. For compatibility, 
      it may be returned as the DetectedState of ncWaitForState, but this bit should be
      ignored by new NI-CAN applications. */
#define  NC_ST_WARNING              0x00000020
//#BASIC End Enum

//#BASIC End Include

      /* Any object */
#define  NC_ST_READ_MULT            0x00000008
      /* State to detect when a CAN port has been woken up remotely.*/
#define  NC_ST_REMOTE_WAKEUP        0x00000040

       /* NI only */
#define  NC_ST_WRITE_MULT           0x00000080

//#BASIC Include DNet_API
   /* NCTYPE_OPCODE values */
//#BASIC Enum NC_OPCODE_ENUM
      /* Interface object */
#define  NC_OP_START                0x80000001
      /* Interface object */
#define  NC_OP_STOP                 0x80000002
      /* Interface object */
#define  NC_OP_RESET                0x80000003

#ifndef __NC_NOINC_other
      /* Interface object only */
#define  NC_OP_ACTIVE               0x80000004
      /* Interface object only */
#define  NC_OP_IDLE                 0x80000005
#endif
//#BASIC End Enum
//#BASIC End Include
      /* Interface object, Param is used */
#define  NC_OP_RTSI_OUT             0x80000004

   /* NCTYPE_BAUD_RATE (values for baud rates) */
#define  NC_BAUD_10K                10000
#define  NC_BAUD_100K               100000
#define  NC_BAUD_125K               125000
#define  NC_BAUD_250K               250000
#define  NC_BAUD_500K               500000
#define  NC_BAUD_1000K              1000000

   /* NCTYPE_COMM_TYPE values */
#define  NC_CAN_COMM_RX_UNSOL          0x00000000  /* rx unsolicited data */
#define  NC_CAN_COMM_TX_BY_CALL        0x00000001  /* tx data by call */
#define  NC_CAN_COMM_RX_PERIODIC       0x00000002  /* rx periodic using remote */
#define  NC_CAN_COMM_TX_PERIODIC       0x00000003  /* tx data periodically */
#define  NC_CAN_COMM_RX_BY_CALL        0x00000004  /* rx by call using remote */
#define  NC_CAN_COMM_TX_RESP_ONLY      0x00000005  /* tx by response only */
#define  NC_CAN_COMM_TX_WAVEFORM       0x00000006  /* tx periodic "waveform" */

   /* NCTYPE_RTSI_MODE values */
#define  NC_RTSI_NONE               0           /* no RTSI usage */
#define  NC_RTSI_TX_ON_IN           1           /* transmit on input pulse */
#define  NC_RTSI_TIME_ON_IN         2           /* timestamp on in pulse */
#define  NC_RTSI_OUT_ON_RX          3           /* output on receive */
#define  NC_RTSI_OUT_ON_TX          4           /* output on transmit cmpl */
#define  NC_RTSI_OUT_ACTION_ONLY    5           /* output by ncAction only */

   /* NCTYPE_RTSI_SIG_BEHAV values */
#define  NC_RTSISIG_PULSE           0           /* pulsed input / output */
#define  NC_RTSISIG_TOGGLE          1           /* toggled output */


   /* NC_ATTR_START_TRIG_BEHAVIOUR values */
#define  NC_START_TRIG_NONE            0
#define  NC_RESET_TIMESTAMP_ON_START   1
#define  NC_LOG_START_TRIG             2



   /* NCTYPE_CAN_ARBID (bit masks) */
         /* When frame type is data (NC_FRMTYPE_DATA) or remote (NC_FRMTYPE_REMOTE),
         this bit in ArbitrationId is interpreted as follows:
         If this bit is clear, the ArbitrationId is standard (11-bit).
         If this bit is set, the ArbitrationId is extended (29-bit).  */
#define  NC_FL_CAN_ARBID_XTD        0x20000000  

   /* NCTYPE_CAN_ARBID (special values) */
         /* Special value used to disable comparators. */      
#define  NC_CAN_ARBID_NONE          0xCFFFFFFF

   /* Values for the FrameType (IsRemote) field of CAN frames.  */
#define  NC_FRMTYPE_DATA            0
#define  NC_FRMTYPE_REMOTE          0x01
   /* NI only */
#define  NC_FRMTYPE_COMM_ERR        0x02     // Communication warning/error (NC_ATTR_LOG_COMM_ERRS)
#define  NC_FRMTYPE_RTSI            0x03     // RTSI pulse (NC_ATTR_RTSI_MODE=NC_RTSI_TIME_ON_IN)

  /* Status for Driver NetIntf (and Driver CanObjs):
            ArbID=0
            DataLength=0
            Timestamp=<time of start trigger>
  */
#define  NC_FRMTYPE_TRIG_START      0x04
#define  NC_FRMTYPE_DELAY           0x05    // Adds a delay between 2 timestamped frames.

  /* CAN Frame to indicate Bus Error. Format: Byte 0:CommState,Byte1:Tx Err Ctr,
     Byte2: Rx Err Ctr, Byte 3: ECC register. Byte 4-7: Don't care.*/
#define  NC_FRMTYPE_BUS_ERR         0x06 
  /* Frame to indicate status of Nerr. Byte 0: 1 = NERR , 0 = No Fault.*/
#define  NC_FRMTYPE_TRANSCEIVER_ERR 0x07

  //Response frame for LIN communication
#define  NC_FRMTYPE_LIN_RESPONSE_ENTRY   0x10
  //Header frame for LIN communication
#define  NC_FRMTYPE_LIN_HEADER     0x11
  //Full frame for LIN communication
#define  NC_FRMTYPE_LIN_FULL       0x12
  //Wakeup frame for LIN communication
#define  NC_FRMTYPE_LIN_WAKEUP_RECEIVED     0x13
  //Sleep frame for LIN communication
#define  NC_FRMTYPE_LIN_BUS_INACTIVE      0x14
  //Bus error frame for LIN communication
#define  NC_FRMTYPE_LIN_BUS_ERR    0x15

   /* Special values for CAN mask attributes (NC_ATTR_MASK_STD/XTD) */
#define  NC_MASK_STD_MUSTMATCH      0x000007FF
#define  NC_MASK_XTD_MUSTMATCH      0x1FFFFFFF
#define  NC_MASK_STD_DONTCARE       0x00000000     // recommended for Series 2
#define  NC_MASK_XTD_DONTCARE       0x00000000     // recommended for Series 2
#define  NC_SERIES2_MASK_MUSTMATCH   0x00000000 
#define  NC_SERIES2_MASK_DONTCARE    0xFFFFFFFF 

   // Values for NC_ATTR_HW_SERIES attribute
#define  NC_HW_SERIES_1                0        // Intel 82527 CAN chip, legacy RTSI
#define  NC_HW_SERIES_2                1        // Phillips SJA1000 CAN chip, updated RTSI
#define  NC_HW_SERIES_847X             2        // Low-cost USB without sync
#define  NC_HW_SERIES_847X_SYNC        3        // Low-cost USB with sync


   // Values for SourceTerminal of ncConnectTerminals.
#define  NC_SRC_TERM_RTSI0                0
#define  NC_SRC_TERM_RTSI1                1
#define  NC_SRC_TERM_RTSI2                2
#define  NC_SRC_TERM_RTSI3                3
#define  NC_SRC_TERM_RTSI4                4
#define  NC_SRC_TERM_RTSI5                5  
#define  NC_SRC_TERM_RTSI6                6
#define  NC_SRC_TERM_RTSI_CLOCK           7
#define  NC_SRC_TERM_PXI_STAR             8
#define  NC_SRC_TERM_INTF_RECEIVE_EVENT   9
#define  NC_SRC_TERM_INTF_TRANSCEIVER_EVENT 10
#define  NC_SRC_TERM_PXI_CLK10            11
#define  NC_SRC_TERM_20MHZ_TIMEBASE       12
#define  NC_SRC_TERM_10HZ_RESYNC_CLOCK    13
#define  NC_SRC_TERM_START_TRIGGER        14

   // Values for DestinationTerminal of ncConnectTerminals.
#define  NC_DEST_TERM_RTSI0               0
#define  NC_DEST_TERM_RTSI1               1
#define  NC_DEST_TERM_RTSI2               2
#define  NC_DEST_TERM_RTSI3               3
#define  NC_DEST_TERM_RTSI4               4
#define  NC_DEST_TERM_RTSI5               5
#define  NC_DEST_TERM_RTSI6               6
#define  NC_DEST_TERM_RTSI_CLOCK          7
#define  NC_DEST_TERM_MASTER_TIMEBASE     8
#define  NC_DEST_TERM_10HZ_RESYNC_CLOCK   9
#define  NC_DEST_TERM_START_TRIGGER       10

   // Values for NC_ATTR_HW_FORMFACTOR attribute
#define  NC_HW_FORMFACTOR_PCI          0
#define  NC_HW_FORMFACTOR_PXI          1
#define  NC_HW_FORMFACTOR_PCMCIA       2
#define  NC_HW_FORMFACTOR_AT           3
#define  NC_HW_FORMFACTOR_USB          4

   // Values for NC_ATTR_TRANSCEIVER_TYPE attribute
#define  NC_TRANSCEIVER_TYPE_HS        0  // High-Speed
#define  NC_TRANSCEIVER_TYPE_LS        1  // Low-Speed / Fault-Tolerant
#define  NC_TRANSCEIVER_TYPE_SW        2  // Single-Wire
#define  NC_TRANSCEIVER_TYPE_EXT       3  // External connection
#define  NC_TRANSCEIVER_TYPE_DISC      4  // Disconnected
#define  NC_TRANSCEIVER_TYPE_LIN       5  // LIN
#define  NC_TRANSCEIVER_TYPE_UNKNOWN   0xFF  // Unknown (Get for Series 1 PCMCIA HS dongle)

   // Values for legacy NC_ATTR_HW_TRANSCEIVER attribute
#define  NC_HW_TRANSCEIVER_HS          NC_TRANSCEIVER_TYPE_HS
#define  NC_HW_TRANSCEIVER_LS          NC_TRANSCEIVER_TYPE_LS
#define  NC_HW_TRANSCEIVER_SW          NC_TRANSCEIVER_TYPE_SW
#define  NC_HW_TRANSCEIVER_EXT         NC_TRANSCEIVER_TYPE_EXT
#define  NC_HW_TRANSCEIVER_DISC        NC_TRANSCEIVER_TYPE_DISC
 
   // Values for NC_ATTR_TRANSCEIVER_MODE attribute.
#define  NC_TRANSCEIVER_MODE_NORMAL         0
#define  NC_TRANSCEIVER_MODE_SLEEP          1
#define  NC_TRANSCEIVER_MODE_SW_WAKEUP      2  // Single-Wire Wakeup
#define  NC_TRANSCEIVER_MODE_SW_HIGHSPEED   3  // Single-Wire High Speed

   // Values for NC_ATTR_BEHAV_FINAL_OUT attribute (CAN Objs of type NC_CAN_COMM_TX_PERIODIC)
#define  NC_OUT_BEHAV_REPEAT_FINAL     0
#define  NC_OUT_BEHAV_CEASE_TRANSMIT   1

   // Values for NC_ATTR_SERIES2_FILTER_MODE
#define  NC_FILTER_SINGLE_STANDARD     0
#define  NC_FILTER_SINGLE_EXTENDED     1
#define  NC_FILTER_DUAL_STANDARD       2
#define  NC_FILTER_DUAL_EXTENDED       3

      // Values for SourceTerminal of ncConnectTerminals.
#define  NC_SRC_TERM_10HZ_RESYNC_EVENT    NC_SRC_TERM_10HZ_RESYNC_CLOCK
#define  NC_SRC_TERM_START_TRIG_EVENT     NC_SRC_TERM_START_TRIGGER
      // Values for DestinationTerminal of ncConnectTerminals.
#define  NC_DEST_TERM_10HZ_RESYNC         NC_DEST_TERM_10HZ_RESYNC_CLOCK
#define  NC_DEST_TERM_START_TRIG          NC_DEST_TERM_START_TRIGGER
      /* NCTYPE_VERSION (NC_ATTR_SOFTWARE_VERSION); ncGetHardwareInfo preferable */
#define  NC_MK_VER_MAJOR            0xFF000000
#define  NC_MK_VER_MINOR            0x00FF0000
#define  NC_MK_VER_SUBMINOR         0x0000FF00
#define  NC_MK_VER_BETA             0x000000FF
      /* ArbitrationId; use IsRemote or FrameType to determine RTSI frame. */
#define  NC_FL_CAN_ARBID_INFO       0x40000000
#define  NC_ARBID_INFO_RTSI_INPUT   0x00000001
      /* NC_ATTR_STD_MASK and NC_ATTR_XTD_MASK */
#define  NC_CAN_MASK_STD_MUSTMATCH  NC_MASK_STD_MUSTMATCH
#define  NC_CAN_MASK_XTD_MUSTMATCH  NC_MASK_XTD_MUSTMATCH
#define  NC_CAN_MASK_STD_DONTCARE   NC_MASK_STD_DONTCARE
#define  NC_CAN_MASK_XTD_DONTCARE   NC_MASK_XTD_DONTCARE

         /* Values for NC_ATTR_TRANSMIT_MODE(Immediate or timestamped).*/
#define  NC_TX_MODE_IMMEDIATE            0
#define  NC_TX_MODE_TIMESTAMPED          1

         /* Values for NC_ATTR_TIMESTAMP_FORMAT.*/
#define  NC_TIME_FORMAT_ABSOLUTE           0
#define  NC_TIME_FORMAT_RELATIVE           1
         /* Values for NC_ATTR_MASTER_TIMEBASE_RATE.Rate can either be
         10(M Series DAQ) or 20(E series DAQ) Mhz.
         This attribute is applicable only to PCI/PXI.*/
#define  NC_TIMEBASE_RATE_10        10
#define  NC_TIMEBASE_RATE_20        20

#endif /* __NC_NOINC_other */

/***********************************************************************
                F U N C T I O N   P R O T O T Y P E S
***********************************************************************/

#ifndef __NC_NOINC_func
/* Naming conventions for sizes:
   Sizeof?        Indicates size of buffer passed in, and has no relation to
                  the number of bytes sent/received on the network (C only).
   ?Length        Indicates number of bytes to send on network.
   Actual?Length  Indicates number of bytes received from network.
*/

extern NCTYPE_STATUS _NCFUNC_ ncAction(
                           NCTYPE_OBJH          ObjHandle,
                           NCTYPE_OPCODE        Opcode,
                           NCTYPE_UINT32        Param);

//#BASIC Include DNet_API
extern NCTYPE_STATUS _NCFUNC_ ncCloseObject(
                           NCTYPE_OBJH          ObjHandle);

//#BASIC End Include
extern NCTYPE_STATUS _NCFUNC_ ncConfig(
                           NCTYPE_STRING        ObjName,
                           NCTYPE_UINT32        NumAttrs,
                           NCTYPE_ATTRID_P      AttrIdList,
                           NCTYPE_UINT32_P      AttrValueList);

extern NCTYPE_STATUS _NCFUNC_ ncConnectTerminals(
                           NCTYPE_OBJH          ObjHandle,
                           NCTYPE_UINT32        SourceTerminal,
                           NCTYPE_UINT32        DestinationTerminal,
                           NCTYPE_UINT32        Modifiers);

extern NCTYPE_STATUS _NCFUNC_ ncDisconnectTerminals(
                           NCTYPE_OBJH          ObjHandle,
                           NCTYPE_UINT32        SourceTerminal,
                           NCTYPE_UINT32        DestinationTerminal,
                           NCTYPE_UINT32        Modifiers);

#ifndef __NC_NOINC_createnotif

//#BASIC Include DNet_API
extern NCTYPE_STATUS _NCFUNC_ ncCreateNotification(
                           NCTYPE_OBJH          ObjHandle,
                           NCTYPE_STATE         DesiredState,
                           NCTYPE_DURATION      Timeout,
                           NCTYPE_ANY_P         RefData,
                           NCTYPE_NOTIFY_CALLBACK Callback);

//#BASIC End Include
#endif /* __NC_NOINC_createnotif */

extern NCTYPE_STATUS _NCFUNC_ ncGetAttribute(
                           NCTYPE_OBJH          ObjHandle,
                           NCTYPE_ATTRID        AttrId,
                           NCTYPE_UINT32        SizeofAttr,
                           NCTYPE_ANY_P         Attr);

extern NCTYPE_STATUS _NCFUNC_ ncGetHardwareInfo(
                           NCTYPE_UINT32        CardIndex,
                           NCTYPE_UINT32        PortIndex,
                           NCTYPE_ATTRID        AttrId,
                           NCTYPE_UINT32        AttrSize,
                           NCTYPE_ANY_P         Attr);

extern NCTYPE_STATUS _NCFUNC_ ncOpenObject(
                           NCTYPE_STRING        ObjName,
                           NCTYPE_OBJH_P        ObjHandle);

extern NCTYPE_STATUS _NCFUNC_ ncRead(
                           NCTYPE_OBJH          ObjHandle,
                           NCTYPE_UINT32        SizeofData,
                           NCTYPE_ANY_P         Data);

extern NCTYPE_STATUS _NCFUNC_ ncReadMult(
                           NCTYPE_OBJH          ObjHandle,
                           NCTYPE_UINT32        SizeofData,
                           NCTYPE_ANY_P         Data,
                           NCTYPE_UINT32_P      ActualDataSize);

extern NCTYPE_STATUS _NCFUNC_ ncReset(
                           NCTYPE_STRING        IntfName,
                           NCTYPE_UINT32        Param);

extern NCTYPE_STATUS _NCFUNC_ ncSetAttribute(
                           NCTYPE_OBJH          ObjHandle,
                           NCTYPE_ATTRID        AttrId,
                           NCTYPE_UINT32        SizeofAttr,
                           NCTYPE_ANY_P         Attr);

#ifndef BASIC
extern void          _NCFUNC_ ncStatusToString(
                           NCTYPE_STATUS        Status,
                           NCTYPE_UINT32        SizeofString,
                           NCTYPE_STRING        ErrorString);

#else /* BASIC */
extern void          _NCFUNC_ ncStatusToString(
                           NCTYPE_STATUS        Status,
                           NCTYPE_UINT32        SizeofString,
                           NCTYPE_STR           ErrorString);

#endif /* BASIC */
//#BASIC Include DNet_API
extern NCTYPE_STATUS _NCFUNC_ ncWaitForState(
                           NCTYPE_OBJH          ObjHandle,
                           NCTYPE_STATE         DesiredState,
                           NCTYPE_DURATION      Timeout,
                           NCTYPE_STATE_P       CurrentState);

//#BASIC End Include
                                             
extern NCTYPE_STATUS _NCFUNC_ ncWrite(
                           NCTYPE_OBJH          ObjHandle,
                           NCTYPE_UINT32        SizeofData,
                           NCTYPE_ANY_P         Data);

#ifndef BASIC
extern NCTYPE_STATUS _NCFUNC_ ncWriteMult(
                           NCTYPE_OBJH          ObjHandle,
                           NCTYPE_UINT32        SizeofData,
                           NCTYPE_CAN_STRUCT_P   FrameArray);
#else
extern NCTYPE_STATUS _NCFUNC_ ncWriteMult(
                           NCTYPE_OBJH          ObjHandle,
                           NCTYPE_UINT32        SizeofData,
                           NCTYPE_ANY_P         FrameArray);
#endif                           

#endif /* __NC_NOINC_func */

#endif // NO_FRAME_API
#ifndef NO_CHANNEL_API
/*****************************************************************************/
/****************** N I - C A N   C H A N N E L    A P I *********************/
/*****************************************************************************/

/***********************************************************************
                            D A T A   T Y P E S
***********************************************************************/

//#BASIC Exclude
   // BASIC data types
//#BASIC End Exclude
#ifndef _NIDAQ_Header_        // NI-DAQ's header defines basic types same as below.

#ifndef _NI_i8_DEFINED_
#define _NI_i8_DEFINED_
typedef signed char     i8;
#endif
#ifndef _NI_i16_DEFINED_
#define _NI_i16_DEFINED_
typedef signed short    i16;
#endif
#ifndef _NI_i32_DEFINED_
#define _NI_i32_DEFINED_
typedef signed long     i32;
#endif
#ifndef _NI_u8_DEFINED_
#define _NI_u8_DEFINED_
typedef unsigned char   u8;	
#endif
#ifndef _NI_u16_DEFINED_
#define _NI_u16_DEFINED_
typedef unsigned short  u16;
#endif
#ifndef _NI_u32_DEFINED_
#define _NI_u32_DEFINED_
typedef unsigned long   u32;
#endif
#ifndef _NI_f32_DEFINED_
#define _NI_f32_DEFINED_
typedef float           f32;
#endif
#ifndef _NI_f64_DEFINED_
#define _NI_f64_DEFINED_
typedef double          f64;
#endif

#endif // _NIDAQ_Header_

//#BASIC Exclude
   // Strings
//#BASIC End Exclude
#ifndef _NI_str_DEFINED_
#define _NI_str_DEFINED_
typedef char *          str;
#endif
#ifndef _NI_cstr_DEFINED_
#define _NI_cstr_DEFINED_
typedef const char *    cstr;
#endif

typedef u32 nctTypeTaskRef;   /* Task Reference handle */

typedef i32 nctTypeStatus;    /* Return value */

   // Timestamp returned from Read functions
#ifndef BASIC
typedef struct { u32 LowPart; u32 HighPart;} nctTypeTimestamp;
#else
typedef struct { u32 dwLowDateTime; u32 dwHighDateTime;} FileTime;
#endif 

#define NCT_MAX_UNIT_LEN   64

   // Message configuration for nctCreateMessage
typedef struct
{
   i32 MsgArbitrationID;   // Arbitration ID of the message
   i32 MsgDataBytes;       // Number of bytes in the message
   i32 Extended;           // 0=standard ID, 1=extended ID
} nctTypeMessageConfig;

   // Channel configuration for nctCreateMessage
typedef struct
{
   i32 StartBit;           // Start bit of the channel in the message
   i32 NumBits;            // Number of bits used for this channel
   i32 DataType;           // Data format: Signed, Unsigned or Float
   i32 ByteOrder;          // Byte ordered in Intel or Motorola type
   f64 ScalingFactor;      // a in y=ax+b
   f64 ScalingOffset;      // b in y=ax+b
   f64 MaxValue;           // max. allowed value on the channel
   f64 MinValue;           // min. allowed value on the channel
   f64 DefaultValue;       // default value returned as long as no message arrives
   char Unit[NCT_MAX_UNIT_LEN];// Unit string
} nctTypeChannelConfig;

   // Mode channel configuration for nctCreateMessageEx
typedef struct
{
   u32 ModeValue;          // Value for the mode channel
   i32 StartBit;           // Start bit of the channel in the message
   i32 NumBits;            // Number of bits used for this channel
   i32 ByteOrder;          // Byte ordered in Intel or Motorola type
   u32 DefaultValue;       // Default value for mode channel when a message is written
} nctTypeModeChanConfig;

   // Channel configuration for nctCreateMessageEx
typedef struct
{
   i32 StartBit;           // Start bit of the channel in the message
   i32 NumBits;            // Number of bits used for this channel
   i32 DataType;           // Data format: Signed, Unsigned or Float
   i32 ByteOrder;          // Byte ordered in Intel or Motorola type
   f64 ScalingFactor;      // a in y=ax+b
   f64 ScalingOffset;      // b in y=ax+b
   f64 MaxValue;           // max. allowed value on the channel
   f64 MinValue;           // min. allowed value on the channel
   f64 DefaultValue;       // default value returned as long as no message arrives
   char Unit[NCT_MAX_UNIT_LEN];  // Unit string
   i32 NumModeChannels;    // 0: normal channel, 1: mode dependent channel
   nctTypeModeChanConfig ModeChannel;  // Mode channel description
} nctTypeChannelConfigMDM;

#define nctChannelConfigId_MDM 2

/***********************************************************************
                              S T A T U S
***********************************************************************/

#define nctSuccess                        0

   // Numbers 0x200 to 0x2FF are used for the NI-CAN Channel API (see NICANTSK.H).
#define nctErrMaxTasks                    (NICAN_ERROR_BASE | 0x200)
#define nctErrUndefinedChannel            (NICAN_ERROR_BASE | 0x201)
#define nctErrAmbiguousChannel            (NICAN_ERROR_BASE | 0x202)
#define nctErrConflictingMessage          (NICAN_ERROR_BASE | 0x203)
#define nctErrStringSizeTooSmall          (NICAN_ERROR_BASE | 0x204)
#define nctErrOpenFile                    (NICAN_ERROR_BASE | 0x205)
#define nctWarnNoWaveformMode             (NICAN_WARNING_BASE | 0x206)
#define nctErrNullPointer                 (NICAN_ERROR_BASE | 0x207)
#define nctErrOnlyOneMsgTimestamped       (NICAN_ERROR_BASE | 0x208)
#define nctErrModeMismatch                (NICAN_ERROR_BASE | 0x209)
#define nctErrReadTimeStampedTimeOut      (NICAN_ERROR_BASE | 0x20A)
#define nctErrAmbiguousIntf               (NICAN_ERROR_BASE | 0x20B)
#define nctErrNoReceiver                  (NICAN_ERROR_BASE | 0x20C)
#define nctWarnTaskAlreadyStarted         (NICAN_WARNING_BASE | 0x20D)
#define nctErrStartTriggerTimeout         (NICAN_ERROR_BASE | 0x20E)
#define nctErrUndefinedMessage            (NICAN_ERROR_BASE | 0x20F)
#define nctErrNumberOfBytes               (NICAN_ERROR_BASE | 0x210)
#define nctErrZeroSamples                 (NICAN_ERROR_BASE | 0x211)
#define nctErrNoValueNotSpecified         (NICAN_ERROR_BASE | 0x212)
#define nctErrInvalidChannel              (NICAN_ERROR_BASE | 0x213)
#define nctErrOverlappingChannels         (NICAN_ERROR_BASE | 0x214)
#define nctWarnOverlappedChannel          (NICAN_WARNING_BASE | 0x214)
#define nctWarnMessagesRenamed            (NICAN_WARNING_BASE | 0x215)
#define nctWarnChannelsSkipped            (NICAN_WARNING_BASE | 0x216)
#define nctErrTooLargeInteger             (NICAN_ERROR_BASE | 0x217)

   /* The following status codes are inherited from the Frame API.
   If you detect a code not listed here, consult the Frame API error listing.  */
#define nctErrFunctionTimeout             CanErrFunctionTimeout
#define nctErrScheduleTimeout             CanErrScheduleTimeout
#define nctErrDriver                      CanErrDriver
#define nctErrBadIntf                     CanErrBadIntfName
#define nctErrBadParam                    CanErrBadParam
#define nctErrBadHandle                   CanErrBadHandle
#define nctErrBadPropertyValue            CanErrBadAttributeValue
#define nctErrOverflowWrite               CanErrOverflowWrite
#define nctErrOverflowChip                CanErrOverflowChip
#define nctErrOverflowRxQueue             CanErrOverflowRxQueue
#define nctWarnOldData                    CanWarnOldData
#define nctErrNotSupported                CanErrNotSupported
#define nctWarnComm                       CanWarnComm
#define nctErrComm                        CanErrComm
#define nctWarnCommStuff                  CanWarnCommStuff
#define nctErrCommStuff                   CanErrCommStuff
#define nctWarnCommFormat                 CanWarnCommFormat
#define nctErrCommFormat                  CanErrCommFormat
#define nctWarnCommNoAck                  CanWarnCommNoAck
#define nctErrCommNoAck                   CanErrCommNoAck  
#define nctWarnCommTx1Rx0                 CanWarnCommTx1Rx0
#define nctErrCommTx1Rx0                  CanErrCommTx1Rx0  
#define nctWarnCommTx0Rx1                 CanWarnCommTx0Rx1
#define nctErrCommTx0Rx1                  CanErrCommTx0Rx1
#define nctWarnCommBadCRC                 CanWarnCommBadCRC
#define nctErrCommBadCRC                  CanErrCommBadCRC
#define nctWarnTransceiver                CanWarnTransceiver 
#define nctWarnRsrcLimitQueues            CanWarnRsrcLimitQueues 
#define nctErrRsrcLimitQueues             CanErrRsrcLimitQueues
#define nctErrRsrcLimitRtsi               CanErrRsrcLimitRtsi   
#define nctErrMaxMessages                 CanErrMaxObjects
#define nctErrMaxChipSlots                CanErrMaxChipSlots  
#define nctErrBadSampleRate               CanErrBadDuration 
#define nctErrFirmwareNoResponse          CanErrFirmwareNoResponse 
#define nctErrBadIdOrOpcode               CanErrBadIdOrOpcode
#define nctWarnBadSizeOrLength            CanWarnBadSizeOrLength
#define nctErrBadSizeOrLength             CanErrBadSizeOrLength
#define nctWarnScheduleTooFast            CanWarnPeriodsTooFast
#define nctErrDllNotFound                 CanErrDllNotFound
#define nctErrFunctionNotFound            CanErrFunctionNotFound 
#define nctErrLangIntfRsrcUnavail         CanErrLangIntfRsrcUnavail 
#define nctErrRequiresNewHwSeries         CanErrRequiresNewHwSeries   
#define nctErrSeriesOneOnly               CanErrSeriesOneOnly   
#define nctErrBothApiSameIntf             CanErrBothApiSameIntf
#define nctErrTaskNotStarted              CanErrNotStarted
#define nctErrConnectTwice                CanErrConnectTwice
#define nctErrConnectUnsupported          CanErrConnectUnsupported
#define nctErrStartTrigBeforeFunc         CanErrStartTrigBeforeFunc
#define nctErrStringSizeTooLarge          CanErrStringSizeTooLarge
#define nctErrHardwareInitFailed          CanErrHardwareInitFailed
#define nctErrOldDataLost                 CanErrOldDataLost
#define nctErrOverflowChannel             CanErrOverflowChannel
#define nctErrUnsupportedModeMix          CanErrUnsupportedModeMix   
#define nctErrBadTransceiverMode          CanErrBadTransceiverMode    
#define nctErrWrongTransceiverProp        CanErrWrongTransceiverAttr 
#define nctErrRequiresXS                  CanErrRequiresXS 
#define nctErrDisconnected                CanErrDisconnected 
#define nctErrNoTxForListenOnly           CanErrNoTxForListenOnly
#define nctErrBadBaudRate                 CanErrBadBaudRate
#define nctErrOverflowFrame               CanErrOverflowFrame

   /* Included for backward compatibility with older versions of NI-CAN */
#define nctWarnLowSpeedXcvr               nctWarnTransceiver // applies to HS as well as LS
#define nctErrOverflowDriver              nctErrOverflowChannel

/***********************************************************************
                          P R O P E R T Y   I D S
***********************************************************************/

   // PropertyId of nctGetProperty and nctSetProperty
//#BASIC Enum NCTTYPE_PPROPERTY_ID
#define nctPropChanStartBit         100001
#define nctPropChanNumBits          100002
#define nctPropChanDataType         100003
#define nctPropChanByteOrder        100004
#define nctPropChanScalFactor       100005
#define nctPropChanScalOffset       100006
#define nctPropChanMaxValue         100007
#define nctPropChanMinValue         100008
#define nctPropChanDefaultValue     100009
#define nctPropChanUnitString       100031
#define nctPropChanIsModeDependent  100033
#define nctPropChanModeValue        100034

#define nctPropMsgArbitrationId     100010
#define nctPropMsgIsExtended        100011
#define nctPropMsgByteLength        100012
#define nctPropMsgDistribution      100020
#define nctPropMsgName              100032

#define nctPropSamplesPending       100013
#define nctPropNumChannels          100014
#define nctPropTimeout              100015
#define nctPropInterface            100016
#define nctPropSampleRate           100017
#define nctPropMode                 100018
#define nctPropNoValue              100019

   // The following properties are inherited from the Frame API
#define nctPropBehavAfterFinalOut   NC_ATTR_BEHAV_FINAL_OUT

#define nctPropIntfBaudRate         NC_ATTR_BAUD_RATE
#define nctPropIntfListenOnly       NC_ATTR_LISTEN_ONLY
#define nctPropIntfRxErrorCounter   NC_ATTR_RX_ERROR_COUNTER
#define nctPropIntfTxErrorCounter   NC_ATTR_TX_ERROR_COUNTER
#define nctPropIntfSeries2Comp      NC_ATTR_SERIES2_COMP
#define nctPropIntfSeries2Mask      NC_ATTR_SERIES2_MASK
#define nctPropIntfSeries2FilterMode NC_ATTR_SERIES2_FILTER_MODE
#define nctPropIntfSingleShotTx     NC_ATTR_SINGLE_SHOT_TX
#define nctPropIntfSelfReception    NC_ATTR_SELF_RECEPTION
#define nctPropIntfTransceiverMode  NC_ATTR_TRANSCEIVER_MODE
#define nctPropIntfTransceiverExternalOut NC_ATTR_TRANSCEIVER_EXTERNAL_OUT
#define nctPropIntfTransceiverExternalIn NC_ATTR_TRANSCEIVER_EXTERNAL_IN
#define nctPropIntfSeries2ErrArbCapture NC_ATTR_SERIES2_ERR_ARB_CAPTURE
#define nctPropIntfTransceiverType  NC_ATTR_TRANSCEIVER_TYPE
#define nctPropIntfVirtualBusTiming NC_ATTR_VIRTUAL_BUS_TIMING

#define nctPropHwSerialNum          NC_ATTR_HW_SERIAL_NUM
#define nctPropHwFormFactor         NC_ATTR_HW_FORMFACTOR
#define nctPropHwSeries             NC_ATTR_HW_SERIES
#define nctPropHwTransceiver        NC_ATTR_TRANSCEIVER_TYPE

#define nctPropHwMasterTimebaseRate	NC_ATTR_MASTER_TIMEBASE_RATE 
#define nctPropHwTimestampFormat	   NC_ATTR_TIMESTAMP_FORMAT   


#define nctPropVersionMajor         NC_ATTR_VERSION_MAJOR
#define nctPropVersionMinor         NC_ATTR_VERSION_MINOR
#define nctPropVersionUpdate        NC_ATTR_VERSION_UPDATE
#define nctPropVersionPhase         NC_ATTR_VERSION_PHASE
#define nctPropVersionBuild         NC_ATTR_VERSION_BUILD
#define nctPropVersionComment       NC_ATTR_VERSION_COMMENT
//#BASIC End Enum

/***********************************************************************
                    O T H E R   C O N S T A N T S
***********************************************************************/

   // Mode parameter of nctIntialize, and nctPropMode property
//#BASIC Enum NCTTYPE_TASK_MODE
#define nctModeInput                      0
#define nctModeOutput                     1
#define nctModeTimestampedInput           2
#define nctModeOutputRecent               3
//#BASIC End Enum

   // DataType of nctCreateMessage, and nctPropChanDataType property
//#BASIC Enum NCTTYPE_CHANNEL_DATATYPE
#define nctDataSigned                     0
#define nctDataUnsigned  	               1
#define nctDataFloat                      2
//#BASIC End Enum

   // ByteOrder of nctCreateMessage, and nctPropChanByteOrder property
//#BASIC Enum NCTTYPE_BYTE_ORDER
#define nctOrderIntel                     0
#define nctOrderMotorola                  1
//#BASIC End Enum

   // Values for nctPropHwSeries property
#define  nctHwSeries1                     NC_HW_SERIES_1 // Intel 82527, legacy RTSI
#define  nctHwSeries2                     NC_HW_SERIES_2 // Phillips SJA1000, updated RTSI

   /* Values for nctPropHwMasterTimebaseRate.*/
#define  nctHwTimebaseRate10			      NC_TIMEBASE_RATE_10
#define  nctHwTimebaseRate20			      NC_TIMEBASE_RATE_20

   /* Values for nctPropHwTimestampFormat .*/
#define  nctHwTimeFormatAbsolute		      NC_TIME_FORMAT_ABSOLUTE
#define  nctHwTimeFormatRelative		      NC_TIME_FORMAT_RELATIVE


   // SourceTerminal of ncConnectTerminals.
//#BASIC Enum NCTTYPE_SOURCE_TERMINAL
#define  nctSrcTermRTSI0                  0
#define  nctSrcTermRTSI1                  1
#define  nctSrcTermRTSI2                  2
#define  nctSrcTermRTSI3                  3
#define  nctSrcTermRTSI4                  4
#define  nctSrcTermRTSI5                  5  
#define  nctSrcTermRTSI6                  6
#define  nctSrcTermRTSI_Clock             7
#define  nctSrcTermPXI_Star               8
#define  nctSrcTermIntfReceiveEvent       9
#define  nctSrcTermIntfTransceiverEvent   10
#define  nctSrcTermPXI_Clk10              11
#define  nctSrcTerm20MHzTimebase          12
#define  nctSrcTerm10HzResyncClock        13
#define  nctSrcTermStartTrigger           14
//#BASIC End Enum

   // DestinationTerminal of ncConnectTerminals.
   //#BASIC Enum NCTTYPE_DESTINATION_TERMINAL
#define  nctDestTermRTSI0                 0
#define  nctDestTermRTSI1                 1
#define  nctDestTermRTSI2                 2
#define  nctDestTermRTSI3                 3
#define  nctDestTermRTSI4                 4
#define  nctDestTermRTSI5                 5  
#define  nctDestTermRTSI6                 6
#define  nctDestTermRTSI_Clock            7
#define  nctDestTermMasterTimebase        8
#define  nctDestTerm10HzResyncClock       9
#define  nctDestTermStartTrigger          10
//#BASIC End Enum

   // Values for nctPropHwFormFactor property
//#BASIC Enum NCTTYPE_PROPHWFORMFACTOR
#define  nctHwFormFactorPCI               NC_HW_FORMFACTOR_PCI
#define  nctHwFormFactorPXI               NC_HW_FORMFACTOR_PXI
#define  nctHwFormFactorPCMCIA            NC_HW_FORMFACTOR_PCMCIA
#define  nctHwFormFactorAT                NC_HW_FORMFACTOR_AT
//#BASIC End Enum

   // Values for nctPropIntfTransceiverType property
#define  nctTransceiverTypeHS             NC_TRANSCEIVER_TYPE_HS
#define  nctTransceiverTypeLS             NC_TRANSCEIVER_TYPE_LS
#define  nctTransceiverTypeSW             NC_TRANSCEIVER_TYPE_SW
#define  nctTransceiverTypeExternal       NC_TRANSCEIVER_TYPE_EXT
#define  nctTransceiverTypeDisconnect     NC_TRANSCEIVER_TYPE_DISC

   // Values for nctPropHwTransceiver property
//#BASIC Enum NCTTTYPE_PROPHWTRANSCEIVER
#define  nctHwTransceiverHS               nctTransceiverTypeHS
#define  nctHwTransceiverLS               nctTransceiverTypeLS
#define  nctHwTransceiverSW               nctTransceiverTypeSW
#define  nctHwTransceiverExternal         nctTransceiverTypeExternal
#define  nctHwTransceiverDisconnect       nctTransceiverTypeDisconnect
//#BASIC End Enum

   // Values for nctPropIntfTransceiverMode property
#define  nctTransceiverModeNormal         NC_TRANSCEIVER_MODE_NORMAL
#define  nctTransceiverModeSleep          NC_TRANSCEIVER_MODE_SLEEP
#define  nctTransceiverModeSWWakeup       NC_TRANSCEIVER_MODE_SW_WAKEUP
#define  nctTransceiverModeSWHighSpeed    NC_TRANSCEIVER_MODE_SW_HIGHSPEED

   // Values for nctPropIntfSeries2FilterMode
#define  nctFilterSingleStandard          NC_FILTER_SINGLE_STANDARD
#define  nctFilterSingleExtended          NC_FILTER_SINGLE_EXTENDED
#define  nctFilterDualStandard            NC_FILTER_DUAL_STANDARD
#define  nctFilterDualExtended            NC_FILTER_DUAL_EXTENDED

   // Values for nctPropBehavAfterFinalOut property
//#BASIC Enum NCTTYPE_PROPBEHAVAFTERFINALOUT
#define  nctOutBehavRepeatFinalSample     NC_OUT_BEHAV_REPEAT_FINAL
#define  nctOutBehavCeaseTransmit         NC_OUT_BEHAV_CEASE_TRANSMIT
//#BASIC End Enum

   // Values for nctPropMultiFrameDistr property
#define  nctDistrUniform                  0
#define  nctDistrBurst                    1

   // Mode parameter of nctGetNames
//#BASIC Enum NCTTYPE_GETNAMES_MODE
#define  nctGetNamesModeChannels          0
#define  nctGetNamesModeMessages          1
//#BASIC End Enum

   // Included for backward compatibility with older versions of NI-CAN.
//#BASIC Exclude
#define  nctSrcTermRTSI_CLOCK             nctSrcTermRTSI_Clock
#define  nctSrcTermPXI_STAR               nctSrcTermPXI_Star
#define  nctSrcTermPXI_CLK10              nctSrcTermPXI_Clk10
#define  nctSrcTermStartTrigEvent         nctSrcTermStartTrigger
#define  nctDestTermRTSI_CLOCK            nctDestTermRTSI_Clock
//#BASIC End Exclude
#define  nctSrcTerm10HzResyncEvent        nctSrcTerm10HzResyncClock
#define  nctDestTerm10HzResync            nctDestTerm10HzResyncClock
#define  nctDestTermStartTrig             nctDestTermStartTrigger

   

#define  NC_MAX_WRITE_MULT                512  

/***********************************************************************
                F U N C T I O N   P R O T O T Y P E S
***********************************************************************/

#ifndef __NC_NOINC_func

nctTypeStatus NCT_FUNC nctClear (
                           nctTypeTaskRef TaskRef);

nctTypeStatus NCT_FUNC nctConnectTerminals (
                           nctTypeTaskRef TaskRef,
                           u32 SourceTerminal,
                           u32 DestinationTerminal,
                           u32 Modifiers);

nctTypeStatus NCT_FUNC nctCreateMessage (
                           nctTypeMessageConfig *MessageConfig,
                           u32 NumberOfChannels,
                           nctTypeChannelConfig ChannelConfigList[],
                           i32 Interface,
                           i32 Mode,
                           f64 SampleRate,
                           nctTypeTaskRef *TaskRef);

nctTypeStatus NCT_FUNC nctCreateMessageEx (
                           u32 ConfigId,
                           void *MessageConfig,
                           u32 NumberOfChannels,
                           void *ChannelConfigList,
                           i32 Interface,
                           i32 Mode,
                           f64 SampleRate,
                           nctTypeTaskRef *TaskRef);

nctTypeStatus NCT_FUNC nctDisconnectTerminals (
                           nctTypeTaskRef TaskRef,
                           u32 SourceTerminal,
                           u32 DestinationTerminal,
                           u32 Modifiers);

nctTypeStatus NCT_FUNC nctGetNames (
                           cstr FilePath,
                           u32 Mode,
                           cstr MessageName,
                           u32 SizeofChannelList,
                           str ChannelList);

nctTypeStatus NCT_FUNC nctGetNamesLength (
                           cstr FilePath,
                           u32 Mode,
                           cstr MessageName,
                           u32 *SizeOfNames);

nctTypeStatus NCT_FUNC nctGetProperty (
                           nctTypeTaskRef TaskRef,
                           cstr ChannelName,
                           u32 PropertyId,
                           u32 SizeofValue,
                           void *Value);

nctTypeStatus NCT_FUNC nctInitialize  (
                           cstr ChannelList,
                           i32 Interface,
                           i32 Mode,
                           f64 SampleRate,
                           nctTypeTaskRef *TaskRef);

nctTypeStatus NCT_FUNC nctInitStart  (
                           cstr ChannelList,
                           i32 Interface,
                           i32 Mode,
                           f64 SampleRate,
                           nctTypeTaskRef *TaskRef);

#ifndef BASIC
nctTypeStatus NCT_FUNC nctRead (
                           nctTypeTaskRef TaskRef,
                           u32 NumberOfSamplesToRead,
                           nctTypeTimestamp *StartTime,
                           nctTypeTimestamp *DeltaTime,
                           f64 *SampleArray,
                           u32 *NumberOfSamplesReturned);

nctTypeStatus NCT_FUNC nctReadTimestamped (
                           nctTypeTaskRef TaskRef,
                           u32 NumberOfSamplesToRead,
                           nctTypeTimestamp *TimestampArray,
                           f64 *SampleArray,
                           u32 *NumberOfSamplesReturned);
#else
nctTypeStatus NCT_FUNC nctRead (
                           nctTypeTaskRef TaskRef,
                           u32 NumberOfSamplesToRead,
                           FileTime *StartTime,
                           FileTime *DeltaTime,
                           f64 *SampleArray,
                           u32 *NumberOfSamplesReturned);

nctTypeStatus NCT_FUNC nctReadTimestamped (
                           nctTypeTaskRef TaskRef,
                           u32 NumberOfSamplesToRead,
                           FileTime *TimestampArray,
                           f64 *SampleArray,
                           u32 *NumberOfSamplesReturned);
#endif
nctTypeStatus NCT_FUNC nctStart (
                           nctTypeTaskRef TaskRef);


nctTypeStatus NCT_FUNC nctStop (
                           nctTypeTaskRef TaskRef);

nctTypeStatus NCT_FUNC nctSetProperty (
                           nctTypeTaskRef TaskRef,
                           cstr ChannelName,
                           u32 PropertyId,
                           u32 SizeofValue,
                           void *Value);

nctTypeStatus NCT_FUNC nctWrite (
                           nctTypeTaskRef TaskRef,
                           u32 NumberOfSamplesToWrite,
                           f64 * SampleArray);

#endif /* __NC_NOINC_func */
#endif /* NO_CHANNEL_API */
#ifndef NO_DNET_API
/*****************************************************************************/
/************************* N I - D N E T   A P I *****************************/
/*****************************************************************************/
/*
    Note that the NI-DNET API is not yet supported from within NI-CAN.
*/
/***********************************************************************
                            D A T A   T Y P E S
***********************************************************************/

#ifndef __NC_NOINC_type

typedef  NCTYPE_UINT32              NCTYPE_POLL_MODE;
typedef  NCTYPE_UINT32              NCTYPE_CONN_TYPE;
typedef  NCTYPE_UINT8               NCTYPE_ITEMTYPE;            

   /* Maximum value for ServDataLength of 
   ncReadDnetExplMsg and ncWriteDnetExplMsg.
   NI-DNET VER 1.1: Increased from 100. */
#define  NC_MAX_SERV_DATA           240   

   /* Type for Data parameter of ncRead of the Dnet Expl Msg Object.
   This type is used internally by NI-DNET, and is not required for
   use of ncReadDnetExplMsg.  */
typedef struct {
   NCTYPE_UINT16           ServiceCode;
   NCTYPE_UINT16           ServDataLength;
   NCTYPE_UINT8            ServData[NC_MAX_SERV_DATA];
} NCTYPE_DNET_EXPL_MSG_READ;

   /* Type for Data parameter of ncWrite of the Dnet Expl Msg Object.
   This type is used internally by NI-DNET, and is not required for
   use of ncWriteDnetExplMsg.  */
typedef struct {
   NCTYPE_UINT16           ServiceCode;
   NCTYPE_UINT16           ClassId;
   NCTYPE_UINT16           InstanceId;
   NCTYPE_UINT16           ServDataLength;
   NCTYPE_UINT8            ServData[NC_MAX_SERV_DATA];
} NCTYPE_DNET_EXPL_MSG_WRITE;

#endif /* __NC_NOINC_type */

/***********************************************************************
                              S T A T U S
***********************************************************************/

/* For DeviceNet status codes, refer to the preceding NI-CAN status. */

/***********************************************************************
                          A T T R I B U T E   I D S
***********************************************************************/

#ifndef __NC_NOINC_attrid

         /* NC_ATTR_BAUD_RATE: Interface Baud Rate, NCTYPE_BAUD_RATE,
         Get, Dnet Interface
            This attribute returns either 125000, 250000, or 500000. */

         /* NC_ATTR_SERIAL_NUMBER: Serial Number, NCTYPE_UINT32, 
         Get, Dnet Interface
         This attribute returns the serial number of the National
         Instruments DeviceNet hardware.  It is the same serial 
         number used for the Duplicate MAC ID Check messages.  */

         /* MAC ID, NCTYPE_UINT8, Get, 
         Dnet Interface & Dnet Expl Msg & Dnet I/O */
#define  NC_ATTR_MAC_ID             0x80000080
         /* Vendor ID, NCTYPE_UINT16, 
               Set, Dnet Expl Msg & Dnet I/O,
               Get, Dnet Interface */
#define  NC_ATTR_VENDOR_ID          0x80000082
         /* Product Code, NCTYPE_UINT16, 
               Set, Dnet Expl Msg & Dnet I/O,
               Get, Dnet Interface */
#define  NC_ATTR_PRODUCT_CODE       0x80000083
         /* Device Type, NCTYPE_UINT16, 
               Set, Dnet Expl Msg & Dnet I/O,
               Get, Dnet Interface */
#define  NC_ATTR_DEVICE_TYPE        0x80000084

      /* Dnet I/O attributes */
         /* Input Length, NCTYPE_UINT32, Get, Dnet I/O */
#define  NC_ATTR_IN_LEN             0x80000091
         /* Output Length, NCTYPE_UINT32, Get, Dnet I/O */
#define  NC_ATTR_OUT_LEN            0x80000092
         /* Exp Packet Rate, NCTYPE_DURATION, Get, Dnet I/O */
#define  NC_ATTR_EXP_PACKET_RATE    0x80000095
         /* COS/Cyclic Ack Suppress, NCTYPE_BOOL, Set, Dnet I/O */
#define  NC_ATTR_ACK_SUPPRESS       0x8000009A
         /* COS Inhibit Timer, NCTYPE_DURATION, Set, Dnet I/O */
#define  NC_ATTR_INHIBIT_TIMER      0x80000097
         /* COS/Cyclic Ack Timer, NCTYPE_DURATION, Set, Dnet I/O
         Given its obscure nature, this attribute is not documented 
         in the Prog Ref Manual, or provided in LabVIEW/CVI enums.
         For a COS/Cyclic I/O Object (ConnectionType = COS or Cyclic), 
         this sets the value for the Ack Timer in the DeviceNet Ack 
         Handler Object of both the NI-DNET and remote device side of 
         the I/O connection.  The Ack Timer is the number of milliseconds 
         to wait for an acknowledgement before timing out the
         I/O connection (or producing a retry).  The default value
         for this attribute is 16ms, as listed in the DeviceNet
         specification.  */
#define  NC_ATTR_ACK_TIMER          0x80000096
         /* Keep Expl Msging, NCTYPE_BOOL, Set, Dnet I/O 
         Given its obscure nature, this attribute is not documented 
         in the Prog Ref Manual, or provided in LabVIEW/CVI enums.
         This attribute controls whether to keep an explicit
         messaging connection open when not in use.  The default
         value for this attribute is NC_TRUE, which means that 
         if needed (no Deferred Delete) NI-DNET will "ping" a remote 
         device every few seconds in order to keep its expl msg 
         connection open.  If you are only using I/O connections for 
         a device, and wish to stop all expl msg traffic, set this 
         attribute to NC_FALSE.  */
#define  NC_ATTR_KEEP_EXPL_MSG      0x80000099

      /* Dnet Interface attributes */
         /* Poll Mode, NCTYPE_POLL_MODE, Get, Dnet Interface */
#define  NC_ATTR_POLL_MODE          0x8000009B
			/* Expl Msg Inhibit Timer, NCTYPE_UINT8, Set, Dnet Interface
         Given its obscure nature, this attribute is not documented 
         in the Prog Ref Manual, or provided in LabVIEW/CVI enums.  
         It sets the time between successive transmits of explicit 
         message requests by the NI-DNET interface (for all objects of 
         that interface).  Its default is zero, which provides the 
         fastest turnaround time for expl msgs.  If you are writing 
         a "who" or similar utility, and you do not want your explicit 
         messaging to corrupt ongoing I/O messaging (such as for another 
         master), you can set this attribute to 10-100ms prior to 
         starting communication.  */
#define  NC_ATTR_EM_INHIBIT_TIMER   0x8000008F

#endif /* __NC_NOINC_attrid */

/***********************************************************************
                    O T H E R   C O N S T A N T S
***********************************************************************/

#ifndef __NC_NOINC_other

	/* Values for NCTYPE_POLL_MODE
   Used for PollMode of ncOpenDnetIntf */
//#BASIC Enum NC_POLL_MODE_ENUM
#define  NC_POLL_AUTO               0x00000000  /* Automatic (default) */
#define  NC_POLL_SCAN               0x00000001  /* Scanned */
#define  NC_POLL_INDIV              0x00000002  /* Individual */
#ifdef BASIC
   #define  NC_POLL_INDIVIDUAL         0x00000002  /* Individual */
#endif
//#BASIC End Enum

	/* Values for NCTYPE_CONN_TYPE
   Used for ConnectionType of ncOpenDnetIO */
//#BASIC Enum NC_CONN_TYPE_ENUM
#define  NC_CONN_POLL               0x00000000
#define  NC_CONN_STROBE             0x00000001
#define  NC_CONN_COS                0x00000002
#define  NC_CONN_CYCLIC             0x00000003
//#BASIC End Enum

#endif /* __NC_NOINC_other */

/***********************************************************************
                F U N C T I O N   P R O T O T Y P E S
***********************************************************************/

#ifndef __NC_NOINC_func

extern NCTYPE_STATUS _NCFUNC_ ncGetDnetAttribute(
                           NCTYPE_OBJH             ObjHandle,
                           NCTYPE_UINT16           ClassId,
                           NCTYPE_UINT16           InstanceId,
                           NCTYPE_UINT8            AttributeId,
                           NCTYPE_DURATION         Timeout,
                           NCTYPE_UINT16           SizeofAttrData,
                           NCTYPE_ANY_P            AttrData,
                           NCTYPE_UINT16_P         ActualAttrDataLength,
                           NCTYPE_UINT16_P         DeviceError);

extern NCTYPE_STATUS _NCFUNC_ ncGetDriverAttr(
                           NCTYPE_OBJH             ObjHandle,
                           NCTYPE_ATTRID           AttrId,
                           NCTYPE_UINT32           SizeofAttr,
                           NCTYPE_ANY_P            Attr);

extern NCTYPE_STATUS _NCFUNC_ ncOpenDnetExplMsg(
                           NCTYPE_STRING           IntfName,
                           NCTYPE_UINT32           DeviceMacId,
                           NCTYPE_OBJH_P           ObjHandle);

extern NCTYPE_STATUS _NCFUNC_ ncOpenDnetIntf(
                           NCTYPE_STRING           IntfName,
                           NCTYPE_UINT32           IntfMacId,
                           NCTYPE_UINT32           BaudRate,
                           NCTYPE_POLL_MODE        PollMode,
                           NCTYPE_OBJH_P           ObjHandle);

extern NCTYPE_STATUS _NCFUNC_ ncOpenDnetIO(
                           NCTYPE_STRING           IntfName,
                           NCTYPE_UINT32           DeviceMacId,
                           NCTYPE_CONN_TYPE        ConnectionType,
                           NCTYPE_UINT32           InputLength,
                           NCTYPE_UINT32           OutputLength,
                           NCTYPE_UINT32           ExpPacketRate,
                           NCTYPE_OBJH_P           ObjHandle);

extern NCTYPE_STATUS _NCFUNC_ ncOperateDnetIntf(
                           NCTYPE_OBJH             ObjHandle,
                           NCTYPE_OPCODE           Opcode,
                           NCTYPE_UINT32           Param);

extern NCTYPE_STATUS _NCFUNC_ ncReadDnetExplMsg(
                           NCTYPE_OBJH             ObjHandle,
                           NCTYPE_UINT8_P          ServiceCode,
                           NCTYPE_UINT16           SizeofServData,
                           NCTYPE_ANY_P            ServData,
                           NCTYPE_UINT16_P         ActualServDataLength);

extern NCTYPE_STATUS _NCFUNC_ ncReadDnetIO(
                           NCTYPE_OBJH             ObjHandle,
                           NCTYPE_UINT32           SizeofData,
                           NCTYPE_ANY_P            Data);

extern NCTYPE_STATUS _NCFUNC_ ncSetDnetAttribute(
                           NCTYPE_OBJH             ObjHandle,
                           NCTYPE_UINT16           ClassId,
                           NCTYPE_UINT16           InstanceId,
                           NCTYPE_UINT8            AttributeId,
                           NCTYPE_DURATION         Timeout,
                           NCTYPE_UINT16           AttrDataLength,
                           NCTYPE_ANY_P            AttrData,
                           NCTYPE_UINT16_P         DeviceError);

extern NCTYPE_STATUS _NCFUNC_ ncSetDriverAttr(
                           NCTYPE_OBJH             ObjHandle,
                           NCTYPE_ATTRID           AttrId,
                           NCTYPE_UINT32           SizeofAttr,
                           NCTYPE_ANY_P            Attr);

extern NCTYPE_STATUS _NCFUNC_ ncWriteDnetExplMsg(
                           NCTYPE_OBJH             ObjHandle,
                           NCTYPE_UINT8            ServiceCode,
                           NCTYPE_UINT16           ClassId,
                           NCTYPE_UINT16           InstanceId,
                           NCTYPE_UINT16           ServDataLength,
                           NCTYPE_ANY_P            ServData);

extern NCTYPE_STATUS _NCFUNC_ ncWriteDnetIO(
                           NCTYPE_OBJH             ObjHandle,
                           NCTYPE_UINT32           SizeofData,
                           NCTYPE_ANY_P            Data);

#endif /* __NC_NOINC_func */
#endif /* NO_DNET_API */
/**********************************************************************/

#pragma  pack()

#ifdef __cplusplus
   /* See top of header file.  */
   }

/**********************************************************************/
#endif // __cplusplus
#endif /* __NC_NOINC_nican */


/* EOF */

