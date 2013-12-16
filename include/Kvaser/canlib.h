/*
**                   Copyright 1994-2005 by KVASER AB, SWEDEN      
**                        WWW: http://www.kvaser.com
**
** This software is furnished under a license and may be used and copied
** only in accordance with the terms of such license.
**
** Description:
**   Definitions for the CANLIB API.
**   Do not use this file in a 16-bit environment.
**   
**   MATLAB users: if you define WIN32_LEAN_AND_MEAN before including
**   this file, you will see a lot less warnings.
** ---------------------------------------------------------------------------
*/
#ifndef _CANLIB_H_
#define _CANLIB_H_

#include <stdlib.h>
#include <windows.h>
#include "canstat.h"
#include "predef.h"
#include "canevt.h"


typedef int canHandle;

#define canINVALID_HANDLE      (-1)

#define WM__CANLIB              (WM_USER+16354)


// Flags for canOpenChannel
// 0x01, 0x02, 0x04 are obsolete and reserved.
// The canWANT_xxx names are also obsolete, use canOPEN_xxx instead for new developments.
#define canWANT_EXCLUSIVE           0x0008
#define canWANT_EXTENDED            0x0010
#define canWANT_VIRTUAL             0x0020
#define canOPEN_EXCLUSIVE           canWANT_EXCLUSIVE
#define canOPEN_REQUIRE_EXTENDED    canWANT_EXTENDED
#define canOPEN_ACCEPT_VIRTUAL      canWANT_VIRTUAL
#define canOPEN_OVERRIDE_EXCLUSIVE  0x0040
#define canOPEN_REQUIRE_INIT_ACCESS 0x0080
#define canOPEN_NO_INIT_ACCESS      0x0100
#define canOPEN_ACCEPT_LARGE_DLC    0x0200  // DLC can be greater than 8

// Flags for canAccept
#define canFILTER_ACCEPT        1
#define canFILTER_REJECT        2
#define canFILTER_SET_CODE_STD  3
#define canFILTER_SET_MASK_STD  4
#define canFILTER_SET_CODE_EXT  5
#define canFILTER_SET_MASK_EXT  6

#define canFILTER_NULL_MASK     0L


//
// CAN driver types - not all are supported on all cards.
//
#define canDRIVER_NORMAL           4
#define canDRIVER_SILENT           1
#define canDRIVER_SELFRECEPTION    8
#define canDRIVER_OFF              0
// 2,3,5,6,7 are reserved values for compatibility reasons.

/*
** Common bus speeds. Used in canSetBusParams.
** The values are translated in canlib, canTranslateBaud().
*/
#define canBITRATE_1M        (-1)
#define canBITRATE_500K      (-2)
#define canBITRATE_250K      (-3)
#define canBITRATE_125K      (-4)
#define canBITRATE_100K      (-5)
#define canBITRATE_62K       (-6)
#define canBITRATE_50K       (-7)
#define canBITRATE_83K       (-8)
#define canBITRATE_10K       (-9)

// The BAUD_xxx names are retained for compability.
#define BAUD_1M        (-1)
#define BAUD_500K      (-2)
#define BAUD_250K      (-3)
#define BAUD_125K      (-4)
#define BAUD_100K      (-5)
#define BAUD_62K       (-6)
#define BAUD_50K       (-7)
#define BAUD_83K       (-8)




/*
** IOCTL types
*/
#define canIOCTL_PREFER_EXT             1
#define canIOCTL_PREFER_STD             2
// 3,4 reserved.
#define canIOCTL_CLEAR_ERROR_COUNTERS   5
#define canIOCTL_SET_TIMER_SCALE        6
#define canIOCTL_SET_TXACK              7


#define CANID_METAMSG  (-1L)        // Like msgs containing bus status changes
#define CANID_WILDCARD (-2L)        // We don't care or don't know

//
// Define CANLIBAPI unless it's done already.
// (canlib.c provides its own definitions of CANLIBAPI, DLLIMPORT
// and DLLEXPORT before including this file.)
//
#ifndef CANLIBAPI
#   if defined(__BORLANDC__)
#      define CANLIBAPI __stdcall
#      define DLLIMPORT 
#      define DLLEXPORT _export
#   elif defined(_MSC_VER) || defined(__MWERKS__) || defined(__GNUC__)
#      define CANLIBAPI __stdcall
#      define DLLIMPORT __declspec(dllimport)
#      define DLLEXPORT __declspec(dllexport)
#   endif
#endif


#ifdef __cplusplus
extern "C" {
#endif

void CANLIBAPI canInitializeLibrary(void);

canStatus CANLIBAPI canClose(const int handle);

canStatus CANLIBAPI canBusOn(const int handle);

canStatus CANLIBAPI canBusOff(const int handle);

canStatus CANLIBAPI canSetBusParams(const int handle,
                           long freq,
                           unsigned int tseg1,
                           unsigned int tseg2,
                           unsigned int sjw,
                           unsigned int noSamp,
                           unsigned int syncmode);

canStatus CANLIBAPI canGetBusParams(const int handle,
                              long * freq,
                              unsigned int * tseg1,
                              unsigned int * tseg2,
                              unsigned int * sjw,
                              unsigned int * noSamp,
                              unsigned int * syncmode);

canStatus CANLIBAPI canSetBusOutputControl(const int handle,
                                     const unsigned int drivertype);

canStatus CANLIBAPI canGetBusOutputControl(const int handle,
                                     unsigned int * drivertype);

canStatus CANLIBAPI canAccept(const int handle,
                        const long envelope,
                        const unsigned int flag);

canStatus CANLIBAPI canReadStatus(const int handle,
                            unsigned long * const flags);

canStatus CANLIBAPI canReadErrorCounters(int handle,
                               unsigned int * txErr,
                               unsigned int * rxErr,
                               unsigned int * ovErr);

canStatus CANLIBAPI canWrite(int handle, long id, void * msg,
                       unsigned int dlc, unsigned int flag);

canStatus CANLIBAPI canWriteSync(int handle, unsigned long timeout);

canStatus CANLIBAPI canRead(int handle,
                      long * id,
                      void * msg,
                      unsigned int * dlc,
                      unsigned int * flag,
                      unsigned long * time);

canStatus CANLIBAPI canReadWait(int handle,
                          long * id,
                          void * msg,
                          unsigned int * dlc,
                          unsigned int * flag,
                          unsigned long * time,
                          unsigned long timeout);

canStatus CANLIBAPI canReadSpecific(int handle, long id, void * msg,
                              unsigned int * dlc, unsigned int * flag,
                              unsigned long * time);

canStatus CANLIBAPI canReadSync(int handle, unsigned long timeout);

canStatus CANLIBAPI canReadSyncSpecific(int handle, long id, unsigned long timeout);

canStatus CANLIBAPI canReadSpecificSkip(int hnd,
                                  long id,
                                  void * msg,
                                  unsigned int * dlc,
                                  unsigned int * flag,
                                  unsigned long * time);

canStatus CANLIBAPI canSetNotify(int handle, HWND aHWnd, unsigned int aNotifyFlags);

canStatus CANLIBAPI canTranslateBaud(long * const freq,
                               unsigned int * const tseg1,
                               unsigned int * const tseg2,
                               unsigned int * const sjw,
                               unsigned int * const nosamp,
                               unsigned int * const syncMode);

canStatus CANLIBAPI canGetErrorText(canStatus err, char * buf, unsigned int bufsiz);

unsigned short CANLIBAPI canGetVersion(void);

canStatus CANLIBAPI canIoCtl(int handle, unsigned int func,
                       void * buf, unsigned int buflen);

unsigned long CANLIBAPI canReadTimer(int hnd);

int CANLIBAPI canOpenChannel(int channel, int flags);

canStatus CANLIBAPI canGetNumberOfChannels(int * channelCount);

canStatus CANLIBAPI canGetChannelData(int channel, int item, void *buffer, size_t bufsize);

#define canCHANNELDATA_CHANNEL_CAP              1
#define canCHANNELDATA_TRANS_CAP                2
#define canCHANNELDATA_CHANNEL_FLAGS            3   // available, etc
#define canCHANNELDATA_CARD_TYPE                4   // canHWTYPE_xxx
#define canCHANNELDATA_CARD_NUMBER              5   // Number in machine, 0,1,...
#define canCHANNELDATA_CHAN_NO_ON_CARD          6
#define canCHANNELDATA_CARD_SERIAL_NO           7
#define canCHANNELDATA_TRANS_SERIAL_NO          8
#define canCHANNELDATA_CARD_FIRMWARE_REV        9
#define canCHANNELDATA_CARD_HARDWARE_REV        10
#define canCHANNELDATA_CARD_UPC_NO              11
#define canCHANNELDATA_TRANS_UPC_NO             12
#define canCHANNELDATA_CHANNEL_NAME             13
#define canCHANNELDATA_DLL_FILE_VERSION         14
#define canCHANNELDATA_DLL_PRODUCT_VERSION      15
#define canCHANNELDATA_DLL_FILETYPE             16
#define canCHANNELDATA_TRANS_TYPE               17
#define canCHANNELDATA_DEVICE_PHYSICAL_POSITION 18
#define canCHANNELDATA_UI_NUMBER                19
#define canCHANNELDATA_TIMESYNC_ENABLED         20
#define canCHANNELDATA_DRIVER_FILE_VERSION      21
#define canCHANNELDATA_DRIVER_PRODUCT_VERSION   22
#define canCHANNELDATA_MFGNAME_UNICODE          23
#define canCHANNELDATA_MFGNAME_ASCII            24
#define canCHANNELDATA_DEVDESCR_UNICODE         25
#define canCHANNELDATA_DEVDESCR_ASCII           26
#define canCHANNELDATA_DRIVER_NAME              27


// channelFlags in canChannelData
#define canCHANNEL_IS_EXCLUSIVE         0x0001
#define canCHANNEL_IS_OPEN              0x0002


// Hardware types.
#define canHWTYPE_NONE         0        // Unknown
#define canHWTYPE_VIRTUAL      1        // Virtual channel.
#define canHWTYPE_LAPCAN       2        // LAPcan Family
#define canHWTYPE_CANPARI      3        // CANpari
#define canHWTYPE_PCCAN        8        // PCcan Family
#define canHWTYPE_PCICAN       9        // PCIcan Family
#define canHWTYPE_USBCAN      11        // USBcan Family and relatives
#define canHWTYPE_PCICAN_II   40        // PCIcan II family
#define canHWTYPE_USBCAN_II   42        // USBcan II, Memorator et al
#define canHWTYPE_SIMULATED   44        // SImulated CAN bus for Creator
#define canHWTYPE_ACQUISITOR  46        // Acquisitor et al
#define canHWTYPE_LEAF        48        // Kvaser Leaf Family
#define canHWTYPE_PC104_PLUS     50     // PC104+
#define canHWTYPE_PCICANX_II     52     // PCIcanx II
#define canHWTYPE_MEMORATOR_II   54     // Memorator Professional
#define canHWTYPE_MEMORATOR_PRO  54     // Memorator Professional
#define canHWTYPE_USBCAN_PRO  56        // USBcan Professional

// Channel capabilities.
#define canCHANNEL_CAP_EXTENDED_CAN         0x00000001L
#define canCHANNEL_CAP_BUS_STATISTICS       0x00000002L
#define canCHANNEL_CAP_ERROR_COUNTERS       0x00000004L
#define canCHANNEL_CAP_CAN_DIAGNOSTICS      0x00000008L
#define canCHANNEL_CAP_GENERATE_ERROR       0x00000010L
#define canCHANNEL_CAP_GENERATE_OVERLOAD    0x00000020L
#define canCHANNEL_CAP_TXREQUEST            0x00000040L
#define canCHANNEL_CAP_TXACKNOWLEDGE        0x00000080L
#define canCHANNEL_CAP_VIRTUAL              0x00010000L
#define canCHANNEL_CAP_SIMULATED            0x00020000L

// Driver (transceiver) capabilities
#define canDRIVER_CAP_HIGHSPEED             0x00000001L

#define canIOCTL_GET_RX_BUFFER_LEVEL            8
#define canIOCTL_GET_TX_BUFFER_LEVEL            9
#define canIOCTL_FLUSH_RX_BUFFER                10
#define canIOCTL_FLUSH_TX_BUFFER                11
#define canIOCTL_GET_TIMER_SCALE                12
#define canIOCTL_SET_TXRQ                       13
#define canIOCTL_GET_EVENTHANDLE                14
#define canIOCTL_SET_BYPASS_MODE                15
#define canIOCTL_SET_WAKEUP                     16
#define canIOCTL_GET_DRIVERHANDLE               17
#define canIOCTL_MAP_RXQUEUE                    18
#define canIOCTL_GET_WAKEUP                     19
#define canIOCTL_SET_REPORT_ACCESS_ERRORS       20
#define canIOCTL_GET_REPORT_ACCESS_ERRORS       21
#define canIOCTL_CONNECT_TO_VIRTUAL_BUS         22
#define canIOCTL_DISCONNECT_FROM_VIRTUAL_BUS    23
#define canIOCTL_SET_USER_IOPORT                24
#define canIOCTL_GET_USER_IOPORT                25
#define canIOCTL_SET_BUFFER_WRAPAROUND_MODE     26
#define canIOCTL_SET_RX_QUEUE_SIZE              27
#define canIOCTL_SET_USB_THROTTLE               28
#define canIOCTL_GET_USB_THROTTLE               29

// For canIOCTL_xxx_USER_IOPORT
typedef struct {
  unsigned int portNo;
  unsigned int portValue;
} canUserIoPortData;

canStatus CANLIBAPI canWaitForEvent(int hnd, DWORD timeout);

canStatus CANLIBAPI canSetBusParamsC200(int hnd, BYTE btr0, BYTE btr1);

canStatus CANLIBAPI canSetDriverMode(int hnd, int lineMode, int resNet);
canStatus CANLIBAPI canGetDriverMode(int hnd, int *lineMode, int *resNet);

// Item codes for canGetVersionEx()
#define canVERSION_CANLIB32_VERSION     0
#define canVERSION_CANLIB32_PRODVER     1
#define canVERSION_CANLIB32_PRODVER32   2
#define canVERSION_CANLIB32_BETA        3

unsigned int CANLIBAPI canGetVersionEx(unsigned int itemCode);

canStatus CANLIBAPI canParamGetCount (void);

canStatus CANLIBAPI canParamCommitChanges (void);

canStatus CANLIBAPI canParamDeleteEntry (int index);

canStatus CANLIBAPI canParamCreateNewEntry (void);

canStatus CANLIBAPI canParamSwapEntries (int index1, int index2);

canStatus CANLIBAPI canParamGetName (int index, char *buffer, int maxlen);

canStatus CANLIBAPI canParamGetChannelNumber (int index);

canStatus CANLIBAPI canParamGetBusParams (int index,
                                          long* bitrate,
                                          unsigned int *tseg1,
                                          unsigned int *tseg2,
                                          unsigned int *sjw,
                                          unsigned int *noSamp);

canStatus CANLIBAPI canParamSetName (int index, const char *buffer);

canStatus CANLIBAPI canParamSetChannelNumber (int index, int channel);

canStatus CANLIBAPI canParamSetBusParams (int index,
                                          long bitrate,
                                          unsigned int tseg1,
                                          unsigned int tseg2,
                                          unsigned int sjw,
                                          unsigned int noSamp);

canStatus CANLIBAPI canParamFindByName (const char *name);



// Frees all object buffers associated with the specified handle.
canStatus CANLIBAPI canObjBufFreeAll(int handle);

// Allocates an object buffer of the specified type.
canStatus CANLIBAPI canObjBufAllocate(int handle, int type);
#define canOBJBUF_TYPE_AUTO_RESPONSE            0x01
#define canOBJBUF_TYPE_PERIODIC_TX              0x02

// Deallocates the object buffer with the specified index.
canStatus CANLIBAPI canObjBufFree(int handle, int idx);

// Writes CAN data to the object buffer with the specified index.
canStatus CANLIBAPI canObjBufWrite(int handle, int idx, int id, void* msg,
                                   unsigned int dlc, unsigned int flags);

// For an AUTO_RESPONSE buffer, set the code and mask that together define
// the identifier(s) that trigger(s) the automatic response.
canStatus CANLIBAPI canObjBufSetFilter(int handle, int idx,
                                       unsigned int code, unsigned int mask);

// Sets buffer-speficic flags.
canStatus CANLIBAPI canObjBufSetFlags(int handle, int idx, unsigned int flags);
// The buffer responds to RTRs only, not regular messages.
// AUTO_RESPONSE buffers only
#define canOBJBUF_AUTO_RESPONSE_RTR_ONLY        0x01

// Sets transmission period for auto tx buffers.
canStatus CANLIBAPI canObjBufSetPeriod(int hnd, int idx, unsigned int period);

// Sets message count for auto tx buffers.
canStatus CANLIBAPI canObjBufSetMsgCount(int hnd, int idx, unsigned int count);

// Enable object buffer with index idx.
canStatus CANLIBAPI canObjBufEnable(int handle, int idx);

// Disable object buffer with index idx.
canStatus CANLIBAPI canObjBufDisable(int handle, int idx);

// For certain diagnostics.
canStatus CANLIBAPI canObjBufSendBurst(int hnd, int idx, unsigned int burstlen);


// Check for specific version(s) of CANLIB.
#define canVERSION_DONT_ACCEPT_LATER      0x01
#define canVERSION_DONT_ACCEPT_BETAS      0x02
BOOL CANLIBAPI canProbeVersion(int hnd, int major, int minor, int oem_id, unsigned int flags);

// Try to "reset" the CAN bus.
canStatus CANLIBAPI canResetBus(int handle);

// Convenience function that combines canWrite and canWriteSync.
canStatus CANLIBAPI canWriteWait(int handle, long id, void * msg,
                                 unsigned int dlc, unsigned int flag,
                                 unsigned long timeout);

// Tell canlib32.dll to unload its DLLs.
canStatus CANLIBAPI canUnloadLibrary(void);

canStatus CANLIBAPI canSetAcceptanceFilter(int hnd, unsigned int code,
                                           unsigned int mask, int is_extended);


canStatus CANLIBAPI canFlushReceiveQueue(int hnd);
canStatus CANLIBAPI canFlushTransmitQueue(int hnd);


canStatus CANLIBAPI kvGetApplicationMapping(int busType,
                                  char *appName,
                                  int appChannel,
                                  int *resultingChannel);

canStatus CANLIBAPI kvBeep(int hnd, int freq, unsigned int duration);

canStatus CANLIBAPI kvSelfTest(int hnd, unsigned long *presults);


#define kvLED_ACTION_ALL_LEDS_ON    0
#define kvLED_ACTION_ALL_LEDS_OFF   1  
#define kvLED_ACTION_LED_0_ON       2
#define kvLED_ACTION_LED_0_OFF      3
#define kvLED_ACTION_LED_1_ON       4
#define kvLED_ACTION_LED_1_OFF      5
#define kvLED_ACTION_LED_2_ON       6
#define kvLED_ACTION_LED_2_OFF      7
#define kvLED_ACTION_LED_3_ON       8
#define kvLED_ACTION_LED_3_OFF      9

canStatus CANLIBAPI kvFlashLeds(int hnd, int action, int timeout);

canStatus CANLIBAPI canRequestChipStatus(int hnd);

canStatus CANLIBAPI canRequestBusStatistics(int hnd);

typedef struct canBusStatistics_s {
  unsigned long  stdData;
  unsigned long  stdRemote;
  unsigned long  extData;
  unsigned long  extRemote;
  unsigned long  errFrame;      // Error frames
  unsigned long  busLoad;       // 0 .. 10000 meaning 0.00-100.00%
  unsigned long  overruns;
} canBusStatistics;

canStatus CANLIBAPI canGetBusStatistics(int hnd, canBusStatistics *stat, size_t bufsiz);

canStatus CANLIBAPI canSetBitrate(int hnd, int bitrate);

canStatus CANLIBAPI kvAnnounceIdentity(int hnd, void *buf, size_t bufsiz);

canStatus CANLIBAPI canGetHandleData(int hnd, int item, void *buffer, size_t bufsize);


typedef void *kvTimeDomain;
typedef canStatus kvStatus;

typedef struct kvTimeDomainData_s {
  int nMagiSyncGroups;
  int nMagiSyncedMembers;
  int nNonMagiSyncCards;
  int nNonMagiSyncedMembers;
} kvTimeDomainData;

kvStatus CANLIBAPI kvTimeDomainCreate(kvTimeDomain *domain);
kvStatus CANLIBAPI kvTimeDomainDelete(kvTimeDomain domain);

kvStatus CANLIBAPI kvTimeDomainResetTime(kvTimeDomain domain);
kvStatus CANLIBAPI kvTimeDomainGetData(kvTimeDomain domain, kvTimeDomainData *data, size_t bufsiz);

kvStatus CANLIBAPI kvTimeDomainAddHandle(kvTimeDomain domain, int handle);
kvStatus CANLIBAPI kvTimeDomainRemoveHandle(kvTimeDomain domain, int handle);


typedef void (CANLIBAPI *kvCallback_t)(int handle, void* context, unsigned int notifyEvent);
kvStatus CANLIBAPI kvSetNotifyCallback(int hnd, kvCallback_t callback, void* context, unsigned int notifyFlags);


//
// The following functions are not yet implemented. Do not use them.
//
canStatus CANLIBAPI canReadEvent(int hnd, CanEvent *event);
void CANLIBAPI canSetDebug(int d);
canStatus CANLIBAPI canSetNotifyEx(int handle, HANDLE event, unsigned int flags);
canStatus CANLIBAPI canSetTimer(int hnd, DWORD interval, DWORD flags);
#define canTIMER_CYCLIC             0x01
#define canTIMER_EXPENSIVE          0x02
int CANLIBAPI canSplitHandle(int hnd, int channel);
int CANLIBAPI canOpenMultiple(DWORD bitmask, int flags);

#ifdef __cplusplus
}
#endif

#include "obsolete.h"

#endif



