/* RoboticsLab, Copyright 2008-2011 SimLab Co., Ltd. All rights reserved.
 *
 * This library is commercial and cannot be redistributed, and/or modified
 * WITHOUT ANY ALLOWANCE OR PERMISSION OF SimLab Co., LTD.
 */

/**
 * @file rTerm.h
 * @brief rTerm native API's declaration.
 */

/**
 * @mainpage rTerm (RoboticsLab Terminal) C++ native library
 *
 * @section INTRODUCTION
 * rTerm is designed to help RoboticsLab user to communicate with rKernel more easily.
 * C++ programmer can easily create rKernel RSP client program using rTerm library.
 * For this, rTerm defines message object classes such as rTermMsg, RecvMsg, SendMsg, 
 * AckMsg, QueryMsg, DeliverMsg, and DeliverRequestMsg. Programmers can create message
 * object and send it to rKernel or other rKernel RSP clients handling rProtocol messages.
 * It also defines several help functions such as connect, disconnect, startKernel,
 * stopKernel, startPlayer, and stopPlayer.
 * 
 * @section CREATEINFO
 * - Author : Sangyup Yi
 * - Date : 2009/02/23
 *
 * @section MODIFYINFO
 * - 2011/09/21 : version 1.9
 * - 2011/07/04 : version 1.8.4
 * - 2011/01/24 : version 1.7.1
 * - 2010/09/14 : version 1.6
 * - 2009/11/09 : version 1.2
 * - 2009/09/11 : version 1.0.9
 * - 2009/08/19 : version 1.0.8
 * - 2009/07/01 : version 1.0.7
 * - 2009/06/11 : version 1.0.6
 * - 2009/03/20 : version 1.0.4
 * - 2009/02/23 : Commit this document.
 */
#ifndef __RTERM_H__
#define __RTERM_H__

#include "rTermDef.h"
#include "rTermEnvVars.h"

RTERMAPI_BEGIN

/**
 * @brief Change current working directory to RoboticsLab bin path.
 */
RTERMAPI void chDir();

/**
 * @brief Get current working directory.
 */
RTERMAPI const char* getCwd();

/**
 * @brief Set environment variables such as ip address and port number to connect to rKernel.
 * @param varType Type id of environment variable.
 * @param value Value value of the environment variable in null terminated character string.
 * @return It returns true when succeeded or false when failed.
 */
RTERMAPI bool setEnv(eVARS_INDEX varType, char *value);

/**
 * @brief Get environment variables such as ip address and port number to connect to rKernel.
 * @param varType Type id of environment variable.
 * @return It returns the environment variable.
 */
RTERMAPI const char* getEnv(eVARS_INDEX varType);

/**
 * @brief Print all rTerm environment variables.
 */
RTERMAPI void printEnvs();

/**
 * @brief Connect to rKernel and set rTerm internal state as 'connected'.
 * @return It returns true when succeeded or false when failed.
 */
RTERMAPI bool connect();

/**
 * @brief Disconnect from rKernel and reset rTerm internal state as 'not-connected'.
 * @return It returns true when succeeded or false when failed.
 */
RTERMAPI bool disconnect();

/**
 * @brief Request rKernel to start rKernel extension.
 * @param pluginName Installed rKernel extension name.
 * @param extensionName Unique instance name or rKernel extension.
 * @return It returns the handle of rKernel extension started or -1 when failed to start it.
 */
RTERMAPI unsigned long startExtension(const char *pluginName, const char *extensionName);

/**
 * @brief Request rKernel to stop rKernel extension.
 * @param extensionID rKernel extension handle to stop.
 * @return It returns true when succeeded or false when failed.
 */
RTERMAPI bool stopExtension(unsigned long extensionID);

/**
 * @brief Get rKernel extension handle the name of which is 'extensionName'.
 * @param extensionName Instance name of rKernel extension.
 * @return It returns the handle of rKernl extension or -1 when failed to get the handle.
 */
RTERMAPI unsigned long queryExtension(const char *extensionName);

/**
 * @brief Request rKernel to load a model file.
 * @param filePath Filepath including file extension such as .aml, .eml relative to "$(RLAB_BIN_PATH)".
 * @param modelName The name of the model.
 * @param T0 H-Transform of base body. It is a double array, the dimension of which is 12. Displacement vector follows rotation matrix.
 * @param isStatic Whether the system is a static system or not. This parameter holds good only if the model is loaded from AML file. 
 * @return It returns a model id to identify loaded model or -1 when failed to load it.
 */
RTERMAPI int load(const char *filePath, const char *modelName, const double T0[12], const bool isStatic);

/**
 * @brief Request rKernel to unload model and free system resources.
 * @param modelID Model id.
 * @return it returns true when succeeded or false when failed.
 */
RTERMAPI bool unload(int modelID);

/**
 * @brief Request rKernel to register this as a data service listener.
 * @param serviceID Service type id.
 *     <pre>
 *     RP_SERVICE_TYPE_WATCH_JOINT					: Joint values.
 *     RP_SERVICE_TYPE_WATCH_CONTACT_FORCE	: Contact forces.
 *     RP_SERVICE_TYPE_WATCH_WRENCH				: Wrenches.
 *     RP_SERVICE_TYPE_WATCH_TWIST					: Twists.
 *     RP_SERVICE_TYPE_WATCH_HTRANSFORM		: H-Transforms.
 *     RP_SERVICE_TYPE_WATCH_DEVICE				: Device values.
 *     RP_SERVICE_TYPE_WATCH_EVENT					: Event.
 *     RP_SERVICE_TYPE_WATCH_PLOT					: Plot data.
 *    </pre>
 * @param timeStep Time interval in microsecond to receive the data.
 *    <pre>
 *    -1              : Disable.
 *    0               : Everytime.
 *    (microsecond)   : The data is served in every time step.
 *    </pre>
 * @return it returns true when succeeded or false when failed.
 * @see listenJoint
 * @see listenDevice
 * @see listenContactForce
 * @see listenWrench
 * @see listenTwist
 * @see listenHTransform
 * @see listenEvent
 * @see listenPlot
 */
RTERMAPI bool requestService(const unsigned char serviceID, const int timeStep);

/**
 * @brief Register callback function for being notified when joint values have arrived.
 * @param f user-defined callback function called when new joint values are received.
 *    <pre>
 *    The callback function prototype is as follows.
 *    void (*f)(double time,            // current time
 *              unsigned int rid,       // robot id
 *              unsigned int jid,       // joint id
 *              unsigned short jdof,    // degree of freedom of the joint 
 *              float* jval,            // joint value (angle or position)
 *              void* udata);           // user-defined parameter
 *    </pre>
 * @param udata user-defined data with which the callback is called.
 * @return it returns the callback registered.
 * @see requestService
 */
RTERMAPI void* listenJoint(void (*f)(double, unsigned int, unsigned int, unsigned short, float*, void*), void* udata);

/**
 * @brief Register callback function for being notified when device values have arrived.
 * @param f user-defined callback function called when new device values are received.
 *    <pre>
 *    The callback function prototype is as follows.
 *    void (*f)(unsigned int rid,           // robot id
 *              unsigned int did,           // device id
 *              const char* dname,          // device name
 *              int dsize,                  // size of device buffer
 *              unsigned char* dbuff,       // pointer to device buffer
 *              void* udata);               // user-defined parameter
 *    </pre>
 * @param udata user-defined data with which the callback is called.
 * @return it returns the callback registered.
 * @see requestService
 */
RTERMAPI void* listenDevice(void (*f)(unsigned int, unsigned int, const char*, int, unsigned char*, void*), void* udata);

/**
 * @brief Register callback function for being notified when contact force values have arrived.
 * @param f user-defined callback function called when new contact force values are received.
 *    <pre>
 *    The callback function prototype is as follows.
 *    void (*f)(double time,           // current time
 *              unsigned int sid,      // show id
 *              unsigned int rid,      // robot id
 *              unsigned int jid,      // joint id
 *              float* r,              // displacement
 *              float* n,              // normal force vector
 *              void* udata);          // user-defined parameter
 *    </pre>
 * @param udata user-defined data with which the callback is called.
 * @return it returns the callback registered.
 * @see requestService
 */
RTERMAPI void* listenContactForce(void (*f)(double, unsigned int, unsigned int, unsigned int, float*, float*, void*), void* udata);

/**
 * @brief Register callback function for being notified when wrench values have arrived.
 * @param f user-defined callback function called when new wrench values are received.
 *    <pre>
 *    The callback function prototype is as follows.
 *    void (*f)(double time,           // current time
 *              unsigned int sid,      // show id
 *              unsigned int rid,      // robot id
 *              unsigned int bid,      // body id
 *              bool trace,            // trace it or not
 *              float* f,              // linear force
 *              float* t,              // angular moment
 *              void* udata);          // user-define parameter
 *    </pre>
 * @param udata user-defined data with which the callback is called.
 * @return it returns the callback registered.
 * @see requestService
 */
RTERMAPI void* listenWrench(void (*f)(double, unsigned int, unsigned int, unsigned int, bool, float*, float*, void*), void* udata);

/**
 * @brief Register callback function for being notified when twist values have arrived.
 * @param f user-defined callback function called when new twist values are received.
 *    <pre>
 *    The callback function prototype is as follows.
 *    void (*f)(double time,           // current time
 *              unsigned int sid,      // show id
 *              unsigned int rid,      // robot id
 *              unsigned int bid,      // body id
 *              float* HT,             // H-Transform of reference frame
 *              float* v,              // linear velociy 
 *              float* w,              // angular velociy
 *              void* udata);          // user-defined parameter
 *    </pre>
 * @param udata user-defined data with which the callback is called.
 * @return it returns the callback registered.
 * @see requestService
 */
RTERMAPI void* listenTwist(void (*f)(double, unsigned int, unsigned int, unsigned int, bool, float*, float*, float*, void*), void* udata);

/**
 * @brief Register callback function for being notified when H-Transform values have arrived.
 * @param f user-defined callback function called when new H-Transform values are received.
 *    <pre>
 *    The callback function prototype is as follows.
 *    void (*f)(unsigned int sid       // show id
 *              unsigned int rid,      // robot id
 *              unsigned int bid,      // body id
 *              bool trace,            // whether to trace this H-Transform or not
 *              float* HT,             // H-Transform value
 *              void* udata);          // user-defined parameter
 *    </pre>
 * @param udata user-defined data with which the callback is called.
 * @return it returns the callback registered.
 * @see requestService
 * @see RP_NTY_HTRANSFORM
 */
RTERMAPI void* listenHTransform(void (*f)(unsigned int, unsigned int, unsigned int, bool, float*, void*), void* udata);

/**
 * @brief Register callback function for being notified when events have arrived.
 * @param f user-defined callback function called when new event is received.
 *    <pre>
 *    The callback function prototype is as follows.
 *    void (*f)(unsigned int eid,        // event id 
 *              unsigned char etype,     // event type (timer, collision, custom event, etc. Please refer to RP_NTY_EVENT for the detail.)
 *              unsigned int param1,     // the first event parameter
 *              unsigned int param2,     // the second event parameter
 *              void* udata);            // user-defined parameter
 *    </pre>
 * @param udata user-defined data with which the callback is called.
 * @return it returns the callback registered.
 * @see requestService
 * @see RP_NTY_EVENT
 */
RTERMAPI void* listenEvent(void (*f)(unsigned int, unsigned char, unsigned int, unsigned int, void*), void* udata);

/**
 * @brief Register callback function for being notified when plot datum have arrived.
 * @param f user-defined callback function called when plot datum have arrived.
 *    <pre>
 *    The callback function prototype is as follows.
 *    void (*f)(unsigned int pid,        // plot id 
 *              double time,             // current time
 *              unsigned short count,    // number of values
 *              double* values,          // values
 *              void* udata);            // user-defined parameter
 *    </pre>
 * @param udata user-defined data with which the callback is called.
 * @return it returns the callback registered.
 * @see requestService
 * @see RP_NTY_PLOT_VALUE
 */
RTERMAPI void* listenPlot(void (*f)(unsigned int, double, unsigned short, double*, void*), void* udata);

/**
 * @brief Register callback function for being notified when annonymous notification message has arrived.
 * @param f user-defined callback function called when notification message has arrived.
 *    <pre>
 *    The callback function prototype is as follows.
 *    void (*f)(void* buf,            // message buffer
 *              int size,             // size of message buffer
 *              void* udata);         // user-defined parameter
 *    </pre>
 * @param udata user-defined data with which the callback is called.
 * @return it returns the callback registered.
 * @see RP_NTY_MESSAGE
 */
RTERMAPI void* listenNtyMessage(void (*f)(void*, int, void*), void* udata);

/**
 * @brief Register callback function for being notified when the network connection closes.
 * @param f user-defined callback function called when the network connection closes.
 *    <pre>
 *    The callback function prototype is as follows.
 *    void (*f)(void* udata);         // user-defined parameter
 *    </pre>
 * @param udata user-defined data with which the callback is called.
 * @return it returns the callback registered.
 */
RTERMAPI void* listenConnectionClosed(void (*f)(void*), void* udata);

/**
 * @brief Starts a new process on the local system.
 * @param procname process(program) name to create(start).
 * @param arg command argument to the new process.
 * @param multiproc whether to allow creating a new process when a process of the same name already exists on the system.
 * @return it returns true when succeeded or false when failed.
 */
RTERMAPI bool startProcess(const char* procname, const char *arg, bool multiproc);

/**
 * @brief Starts a new process on the local system.
 * @param procname process(program) name to create(start).
 * @param arg command argument to the new process.
 * @param multiproc whether to allow creating a new process when a process of the same name already exists on the system.
 * @param forceRestart whether to kill the running process previous launched and restart it.
 * @return it returns true when succeeded or false when failed.
 */
RTERMAPI bool startProcess(const char* procname, const char *arg, bool multiproc, bool forceRestart);

/**
 * @brief Stop a processes currently running on the local system.
 * @param procname process(program) name to stop.
 * @return it returns true when succeeded or false when failed.
 */
RTERMAPI bool stopProcess(const char* procname);

/**
 * @brief Starts a new rPlayer process on the local system.
 * @param arg command argument to rPlayer.
 * @return it returns true when succeeded or false when failed.
 */
RTERMAPI bool startPlayer(const char *arg);

/**
 * @brief Starts a new rPlayer process on the local system.
 * @param arg command argument to rPlayer.
 * @param forceRestart whether to kill the running process previous launched and restart it.
 * @return it returns true when succeeded or false when failed.
 */
RTERMAPI bool startPlayer(const char *arg, bool forceRestart);

/**
 * @brief Stop rPlayer processes currently running on the local system.
 * @return it returns true when succeeded or false when failed.
 */
RTERMAPI bool stopPlayer();

/**
 * @brief Starts a new rPlot process on the local system.
 * @param arg command argument to rPlot.
 * @return it returns true when succeeded or false when failed.
 */
RTERMAPI bool startPlot(const char *arg);

/**
 * @brief Starts a new rPlot process on the local system.
 * @param arg command argument to rPlot.
 * @param forceRestart whether to kill the running process previous launched and restart it.
 * @return it returns true when succeeded or false when failed.
 */
RTERMAPI bool startPlot(const char *arg, bool forceRestart);

/**
 * @brief Stop rPlot processes currently running on the local system.
 * @return it returns true when succeeded or false when failed.
 */
RTERMAPI bool stopPlot();

/**
 * @brief Starts a new rBuilder process on the local system.
 * @param arg command argument to rBuilder.
 * @return it returns true when succeeded or false when failed.
 */
RTERMAPI bool startBuilder(const char *arg);

/**
 * @brief Stop rBuilder processes currently running on the local system.
 * @return it returns true when succeeded or false when failed.
 */
RTERMAPI bool stopBuilder();

/**
 * @brief Starts a new rKernel Script IDE(rScriptIDE) process on the local system.
 * @param arg command argument to rKernel Script IDE.
 * @return it returns true when succeeded or false when failed.
 */
RTERMAPI bool startScriptIDE(const char *arg);

/**
 * @brief Stop rBuilder processes currently running on the local system.
 * @return it returns true when succeeded or false when failed.
 */
RTERMAPI bool stopScriptIDE();

/**
 * @brief Starts a new rKernel process on the local system.
 * @param arg command argument to rKernel.
 * @return it returns true when succeeded or false when failed.
 */
RTERMAPI bool startKernel(const char *arg);

/**
 * @brief Starts a new rKernel process on the local system.
 * @param arg command argument to rKernel.
 * @param forceRestart whether to kill the running process previous launched and restart it.
 * @return it returns true when succeeded or false when failed.
 */
RTERMAPI bool startKernel(const char *arg, bool forceRestart);

/**
 * @brief Stop rKernel process currently running on the local system.
 * @return it returns true when succeeded or false when failed.
 */
RTERMAPI bool stopKernel();

/**
 * @brief Starts a new rPlayer process on the remote system.
 * @param arg Command-line argument string to feed the new rPlayer process.
 * @return it returns true when succeeded or false when failed.
 */
RTERMAPI bool startPlayerEx(const char *arg);

/**
 * @brief Starts a new rPlayer process on the remote system.
 * @param arg Command-line argument string to feed the new rPlayer process.
 * @param forceRestart whether to kill the running process previous launched and restart it.
 * @return it returns true when succeeded or false when failed.
 */
RTERMAPI bool startPlayerEx(const char *arg, bool forceRestart);

/**
 * @brief Stops rPlayer process running on the remote system.
 * @return it returns true when succeeded or false when failed.
 */
RTERMAPI bool stopPlayerEx();

/**
 * @brief Starts a new rPlot process on the remote system.
 * @param arg Command-line argument string to feed the new rPlot process.
 * @return it returns true when succeeded or false when failed.
 */
RTERMAPI bool startPlotEx(const char *arg);

/**
 * @brief Starts a new rPlot process on the remote system.
 * @param arg Command-line argument string to feed the new rPlot process.
 * @param forceRestart whether to kill the running process previous launched and restart it.
 * @return it returns true when succeeded or false when failed.
 */
RTERMAPI bool startPlotEx(const char *arg, bool forceRestart);

/**
 * @brief Stops rPlot process running on the remote system.
 * @return it returns true when succeeded or false when failed.
 */
RTERMAPI bool stopPlotEx();

/**
 * @brief Starts a new rKernel process on the remote system.
 * @param arg Command-line argument string to feed the new rKernel process.
 * @return it returns true when succeeded or false when failed.
 */
RTERMAPI bool startKernelEx(const char *arg);

/**
 * @brief Starts a new rKernel process on the remote system.
 * @param arg Command-line argument string to feed the new rKernel process.
 * @param forceRestart whether to kill the running process previous launched and restart it.
 * @return it returns true when succeeded or false when failed.
 */
RTERMAPI bool startKernelEx(const char *arg, bool forceRestart);

/**
 * @brief Stops rKernel process running on the remote system.
 * @return it returns true when succeeded or false when failed.
 */
RTERMAPI bool stopKernelEx();


#ifdef SWIGPYTHON
RTERMAPI PyObject* setRecvFunc(PyObject *pyfunc);
#endif

RTERMAPI void printExtensions();
RTERMAPI void printClients();
RTERMAPI void printModels();




RTERMAPI unsigned long createQueryMsg(unsigned short msgtype, unsigned long target, unsigned short msgid);
RTERMAPI unsigned long createDeliverRequestMsg(unsigned long target, unsigned short msgid);
RTERMAPI void destroyMsg(unsigned long msg);

RTERMAPI int writeMsgInt(unsigned long msg, int value);
RTERMAPI int writeMsgByte(unsigned long msg, unsigned char value);
RTERMAPI int writeMsgShort(unsigned long msg, short value);
RTERMAPI int writeMsgFloat(unsigned long msg, float value);
RTERMAPI int writeMsgDouble(unsigned long msg, double value);
RTERMAPI int writeMsgString(unsigned long msg, const char* value);
RTERMAPI int writeMsgFloat(unsigned long msg, const unsigned char* buff, int size);

RTERMAPI int requestQueryMsg(unsigned long msg);
RTERMAPI int requestDeliverRequestMsg(unsigned long msg);

RTERMAPI unsigned char getRetCode();
RTERMAPI unsigned char readAckByte();
RTERMAPI short readAckShort();
RTERMAPI int readAckInt();
RTERMAPI float readAckFloat();
RTERMAPI double readAckDouble();
RTERMAPI const char* readAckString();
RTERMAPI int readAckData(unsigned char* buff, int size);


RTERMAPI unsigned long createRobot();
RTERMAPI void destroyRobot(unsigned long robot);


#ifdef SWIG
%immutable;
#endif
/**
 * @brief rTerm version.
 */
extern char* version;
/**
 * @brief rTerm copyright.
 */
extern char* copyright;
#ifdef SWIG
%mutable;
#endif



RTERMAPI_END

#ifndef RTERM_EXPORTS
#if _MSC_VER == 1400

#ifdef _DEBUG
    #pragma comment(lib, "rTerm_8d.lib")
#else
    #pragma comment(lib, "rTerm_8.lib")
#endif

#elif _MSC_VER == 1500

#ifdef _DEBUG
    #pragma comment(lib, "rTerm_9d.lib")
#else
    #pragma comment(lib, "rTerm_9.lib")
#endif

#elif _MSC_VER == 1600

#ifdef _DEBUG
    #pragma comment(lib, "rTerm_10d.lib")
#else
    #pragma comment(lib, "rTerm_10.lib")
#endif

#endif
#endif

#endif // __RTERM_H__
