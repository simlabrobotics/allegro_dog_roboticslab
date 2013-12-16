/* RoboticsLab, Copyright 2008-2013 SimLab Co., Ltd. All rights reserved.
 *
 * This library is commercial and cannot be redistributed, and/or modified
 * WITHOUT ANY ALLOWANCE OR PERMISSION OF SimLab Co., LTD.
 */

/**
 * @file rPanelManipulatorCmd.h
 * @brief Robot command and status definition.
 * @author Sangyup Yi
 * @version 2.0
 * @date 2013/10/15 : version 2.0
 *                    - introduce a new constant RP_MANIPULATOR_DATA_VERSION.
 *                    - increased MAX_SLAVE_COUNT from 16 to 32.
 *                    - introduce a new constant MAX_BRANCH_COUNT and rPanelManipulatorState_t has end-effector states of MAX_BRANCH_COUNT.
 *                    - modified rPanelManipulatorState_t.
 * @date 2012/04/15 : version 1.2
 * @date 2012/03/30 : version 1.1
 */
#pragma once

#define RP_MANIPULATOR_DATA_VERSION (0x00020000)

#define MAX_SLAVE_COUNT 32	///< maximum number of slaves.
#define MAX_BRANCH_COUNT 4	///< maximum number of hierachical branches.
#define MAX_TRAJ_COUNT	256 ///< maximum number of tasks which can be set simultaneously.

#define CMD_NULL				(0x0000)
#define CMD_ENABLE				(0x0001)
#define CMD_RESET_ENCODER		(0x0002)
#define CMD_DISABLE				(0x0003)
#define CMD_SERVO_ON			(0x0004)
#define CMD_SERVO_OFF			(0x0005)
#define CMD_GO					(0x0006)
#define CMD_CLEAR_ERR			(0x0007)
#define CMD_CLEAR_STATISTICS	(0x0008)
#define CMD_EXIT				(0x000f)

#define CMD_CMD_1				(0x1001)
#define CMD_CMD_2				(0x1002)
#define CMD_CMD_3				(0x1003)
#define CMD_CMD_4				(0x1004)
#define CMD_CMD_5				(0x1005)
#define CMD_CMD_6				(0x1006)
#define CMD_CMD_7				(0x1007)
#define CMD_CMD_8				(0x1008)
#define CMD_CMD_9				(0x1009)
#define CMD_CMD_10				(0x100a)
#define CMD_CMD_11				(0x100b)
#define CMD_CMD_12				(0x100c)
#define CMD_CMD_13				(0x100d)
#define CMD_CMD_14				(0x100e)
#define CMD_CMD_15				(0x100f)

/**
 * Enumeration of current state of main application.
 */
typedef enum eApStatus
{
	eApStatus_START = 0,
	eApStatus_IDLE,
	eApStatus_INIT,
	eApStatus_STABLIZING,
	eApStatus_HOMING,
	eApStatus_READY_TO_SERVO_ON,
	eApStatus_SERVO_ON,
	eApStatus_FINALIZED,
	eApStatus_END,
	eApStatus_EMERGENCY_STOP,
	eApStatus_FALUT
} eApStatus;
/**
 * Strings of current state of main application.
 */
static const TCHAR* szApStatus[] =
{
	_T(""),
	_T(""),
	_T(""),
	_T(""),
	_T(""),
	_T(""),
	_T(""),
	_T(""),
	_T(""),
	_T(""),
	_T("")
};

/**
 * Enumeration of current AL(communication) status.
 */
typedef enum eAlStatus
{
	eAlStatus_INIT		= 0x0001, ///< Init.
	eAlStatus_PRE_OP	= 0x0002, ///< pre-operational.
	eAlStatus_BOOTSTRAP	= 0x0003, ///< bootstral mode
	eAlStatus_SAFE_OP	= 0x0004, ///< safe-operational.
	eAlStatus_OP		= 0x0008, ///< operational.
	eAlStatus_ERR		= 0x0100  ///< error.
} eAlStatus;
/**
 * Strings of current AL(communication) status.
 */
static const TCHAR* szAlStatus[] =
{
	_T("Init"),
	_T("Pre Operational"),
	_T("Bootstrap mode"),
	_T("Safe Operational"),
	_T("Operational"),
	_T("Error")
};

/**
 * Control words(CoE).
 */
typedef enum eOpControl
{
	eOpControl_SHUTDOWN,
	eOpControl_SWITCH_ON,
	eOpControl_ENABLE_OPERATION,
	eOpControl_SWITCH_ON_ENABLE_OPERATION,
	eOpControl_DISABLE_VOLTAGE,
	eOpControl_QUICK_STOP,
	eOpControl_DISABLE_OPERATION,
	eOpControl_FAULT_RESET
} eOpControl;
/**
 * Strings of control words(CoE).
 */
static const TCHAR* szOpControl[] =
{
	_T("Shutdown"),
	_T("Switch On"),
	_T("Enable Operation"),
	_T("Switch On + Enable Operation"),
	_T("Disable Voltage"),
	_T("Quick Stop"),
	_T("Disable Operation"),
	_T("Fault Reset")
};

/**
 * Status words(CoE).
 */
typedef enum eOpStatus
{
	eOpStatus_START = 0,
	eOpStatus_NOT_READY_TO_SWITCH_ON,
	eOpStatus_SWITCH_ON_DISABLED,
	eOpStatus_READY_TO_SWITCH_ON,
	eOpStatus_SWITCHED_ON,
	eOpStatus_OPERATION_ENABLED,
	eOpStatus_QUICK_STOP_ACTIVE,
	eOpStatus_FAULT_REACTION_ACTIVE,
	eOpStatus_FAULT,
	eOpStatus_UNKNOWN
} eOpStatus;
/**
 * Strings of status words(CoE).
 */
static const TCHAR* szOpStatus[] =
{
	_T("Init"),
	_T("Not Ready to Switch On"),
	_T("Switch On Disabled"),
	_T("Ready to Switch On"),
	_T("Switched On"),
	_T("Operation Enabled"),
	_T("Quick Stop Active"),
	_T("Fault Reaction Active"),
	_T("Fault"),
	_T("Unknown")
};

/**
 * Mode of operation.
 */
typedef enum eOpMode
{
	eOpMode_POSITION = 0, ///< position servo mode.
	eOpMode_VELOCITY, ///< velocity servo mode.
	eOpMode_TORQUE ///< torque(current) servo mode.
} eOpMode;
/**
 * Strings of mode of operation.
 */
static const TCHAR* szOpMode[] =
{
	_T("Position"),
	_T("Velocity"),
	_T("Torque")
};

/**
 * Error codes.
 */
typedef enum eErrCode
{
	eErrCode_NONE = 0
} eErrCode;
/**
 * Strings of error codes.
 */
static const TCHAR* szErrCode[] = 
{
	_T("No error")
};

/**
 * Control command for each slave.
 */
typedef struct tagPanelManipulatorSlaveCmd
{
	double position; ///< position demand value.
	double velocity; ///< velocity demand value.
	double torque; ///< torque demand value.
	eOpMode OP_mode; ///< active mode of operation.
} rPanelManipulatorSlaveCmd_t;

/**
 * Control command for whole system.
 */
typedef struct tagPanelManipulatorCmd
{
	int command; ///< command.
	int reserved_1; ///< reserved.
	int reserved_2; ///< reserved.
	unsigned short slave_count; ///< number of slaves.
	rPanelManipulatorSlaveCmd_t slave_command[MAX_SLAVE_COUNT]; ///< control command for each slave.
} rPanelManipulatorCmd_t;


/**
 * Current state for master(main application).
 */
typedef struct tagPanelManipulatorMasterState
{
	eAlStatus AL_status; ///< master communication status (New in version 2)
	eApStatus AP_status; ///< master application status (New in version 2)
	int error_count; ///< error count (New in version 2)
	int error_count_continuous; ///< continuous error count (New in version 2)
	float frame_rate_recv; ///< received data frames per a second (New in version 2)
	float frame_rate_send; ///< sent data frames per a second (New in version 2)
	int frames_recv; ///< total received data frames (New in version 2)
	int frames_send; ///< total sent data frames (New in version 2)
} rPanelManipulatorMasterState_t;

/**
 * Current state for each slave.
 */
typedef struct tagPanelManipulatorSlaveState
{
	int id; ///< slave id. (New in version 2)
	double position; ///< position actual value.
	double velocity; ///< velocity actual value.
	double torque; ///< torque actual value.
	eOpMode OP_mode; ///< current actual control(servo) mode.
	eErrCode error_code; ///< current error code set.
	eOpStatus OP_status; ///< status word(CoE).
	eAlStatus AL_status; ///< AL status.
	int errcount; ///< error count. (New in version 2)
} rPanelManipulatorSlaveState_t;

/**
 * Current state for Endeffector.
 */
typedef struct tagPanelManipulatorEndeffectorState
{
	double x;		///< position actual value aligned x axis.
	double y;		///< position actual value aligned y axis.
	double z;		///< position actual value aligned z axis.
	double roll;	///< rotation actual value of roll in euler angle.
	double pitch;	///< rotation actual value of pitch in euler angle.
	double yaw;		///< rotation actual value of yaw in euler angle.
} rPanelManipulatorEndeffectorState_t;

/**
 * Current state for whole system.
 */
typedef struct tagPanelManipulatorState
{
	double time; ///< current time.
	unsigned short slave_count; ///< number of active slaves.
	rPanelManipulatorMasterState_t master_state;
	rPanelManipulatorSlaveState_t slave_state[MAX_SLAVE_COUNT]; ///< state for each slave.
	rPanelManipulatorEndeffectorState_t endeffector_state[MAX_BRANCH_COUNT];
} rPanelManipulatorState_t;

/**
 * Enumeration of type of trajectory.
 */
typedef enum eTrajType
{
	eTrajType_TASK, ///< task-space trajectory.
	eTrajType_JOINT, ///< joint-space trajectory.
	eTrajType_TASK_FILE, ///< task-space trajectory stored in a file.
	eTrajType_JOINT_FILE, ///< joint-space trajectory stored in a file.
} eTrajType;

/**
 * Enumeration of type of trajectory interpolator.
 */
typedef enum eTrajIntpType
{
	eTrajIntpType_None = 0, ///<
	eTrajIntpType_Linear, ///< 1st order linear interpolator.
	eTrajIntpType_Quadratic, ///< 2nd order quadratic interpolator.
	eTrajIntpType_Cubic, ///< 3rd order cubic interpolator.
	eTrajIntpType_Quintic, ///< 5th order quintic interpolator.
	eTrajIntpType_Jerk, ///< 7th order interpolator.
	eTrajIntpType_UserDefined, ///<
	eTrajIntpType_Trapezoid ///< trapezoidal interpolator.
} eTrajIntpType;

/**
 * Enumeration of rotation specifier.
 */
typedef enum eTrajRotationType
{
	eTrajRotationType_ZYX, ///< Euler angle.
	eTrajRotationType_RotationMatrix, ///< Rotation matrix.
} eTrajRotationType;

/**
 * Task description.
 */
typedef struct tagPanelManipulatorTaskTarget
{
	double xg; ///< position x of base frame(body) relative to the global frame.
	double yg; ///< position y of base frame(body) relative to the global frame.
	double zg; ///< position z of base frame(body) relative to the global frame.
	double thg; ///< heading angle, theta, of base frame(body) relative to the global frame.
	double x; ///< position x of task(tool) frame relative to the base frame.
	double y; ///< position y of task(tool) frame relative to the base frame.
	double z; ///< position z of task(tool) frame relative to the base frame.
	double R[9]; ///< rotation matrix of task(tool) frame relative to the base frame.
	double roll; ///< euler-angle, roll.
	double pitch; ///< euler-angle, pitch.
	double yaw; ///< euler-angle, yaw.
	unsigned int DO; ///< digital output.
	double duration; ///< time duration of motion assigned to this task in seconds.
	eTrajRotationType rotation_type; ///< type of rotation specifier valid.
	eTrajIntpType interpolator_type; ///< type of interpolator which is used to interpolate tasks.
} rPanelManipulatorTaskTarget_t;

/**
 * Snapshot of joint angles.
 */
typedef struct tagPanelManipulatorJointTarget
{
	double joint_rad[MAX_SLAVE_COUNT]; ///< joint position demand value in radian. 
	unsigned int DO; ///< digital output.
	double duration; ///< duration of motion.
	eTrajIntpType interpolator_type[MAX_SLAVE_COUNT]; ///< type of interpolator which is used to interpolate joint angles in seconds.
} rPanelManipulatorJointTarget_t;

/**
 * Motion trajectory.
 */
typedef struct tagPanelManipulatorTraj
{
	eTrajType type; ///< type of trajectory activated currently.
	unsigned int num_task_traj; ///< number of task-space trajectory elements.
	unsigned int num_joint_traj; ///< number of joint-space trajectory elements.
	rPanelManipulatorTaskTarget_t task_traj[MAX_TRAJ_COUNT]; ///< task-space trajectory.
	rPanelManipulatorJointTarget_t joint_traj[MAX_TRAJ_COUNT]; ///< joint-space trajectory.
	TCHAR filename[MAX_PATH]; ///< file name which stores trajectory information.
} rPanelManipulatorTraj_t;

/**
 * Control and Joint parameters.
 */
typedef struct tagPanelManipulatorParam
{
	double kp[MAX_SLAVE_COUNT]; ///< propotional  gains
	double kv[MAX_SLAVE_COUNT]; ///< derivative gains.
	double ki[MAX_SLAVE_COUNT]; ///< integral gains.
	double invL2[MAX_SLAVE_COUNT]; ///< inverse L2 gains.
	double pos_ubound[MAX_SLAVE_COUNT]; ///< position upper bound for each joint.
	double pos_lbound[MAX_SLAVE_COUNT]; ///< position lower bound for each joint.
	double vel_ubound[MAX_SLAVE_COUNT]; ///< velocity upper bound for each joint.
	double vel_lbound[MAX_SLAVE_COUNT]; ///< velocity lower bound for each joint.
	double acc_ubound[MAX_SLAVE_COUNT]; ///< acceleration upper bound for each joint.
	double acc_lbound[MAX_SLAVE_COUNT]; ///< acceleration lower bound for each joint.
	double link_len[MAX_SLAVE_COUNT+1]; ///< length of each link.
} rPanelManipulatorParam_t;

/**
 * Robot command and status definition.
 */
typedef struct tagPanelManipulatorData
{
	rPanelManipulatorCmd_t cmd;
	rPanelManipulatorState_t state;
	rPanelManipulatorTraj_t traj;
	rPanelManipulatorParam_t param;
} rPanelManipulatorData_t;
