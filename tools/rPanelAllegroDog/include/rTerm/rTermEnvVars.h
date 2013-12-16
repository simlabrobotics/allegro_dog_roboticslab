/* RoboticsLab, Copyright 2008-2011 SimLab Co., Ltd. All rights reserved.
 *
 * This library is commercial and cannot be redistributed, and/or modified
 * WITHOUT ANY ALLOWANCE OR PERMISSION OF SimLab Co., LTD.
 */

/**
 * @file rTermEnvVars.h
 * @brief defines rTerm environment variables.
 */
#ifndef __rTermENVVARS_H__
#define __rTermENVVARS_H__

RTERM_EXTERN_C_BEGIN

/*! \enum eVARS_INDEX
 *  \brief type id of environment variables
 *
 */
typedef enum eVARS_INDEX
{
    IP_ADDR = 0,        ///< default rKernel IP address
    PORT_NUM,           ///< default rKernel port number
    IP_ADDR_LAUNCHER,   ///< default rSpy IP address
    PORT_NUM_LAUNCHER,  ///< default rSpy port number
    ROOT_PATH,          ///< resource root path
    ACK_TIMEOUT,        ///< time limit how long wait acknowledge in milliseconds
    MAX_ROBOT,          ///< maximum number of robots to load
    MAX_ENV,            ///< maximum number of environments to load
	NETWORK_SLEEP_MSEC,	///< sleep duration between each "recv()" call in milliseconds
    VARS_COUNT
} eVARS_INDEX;

RTERM_EXTERN_C_END

#endif // __rTermENVVARS_H__
