/* Allegro, Copyright 2012 SimLab Co., Ltd. All rights reserved.
 *
 * This library is commercial and cannot be redistributed, and/or modified
 * WITHOUT ANY ALLOWANCE OR PERMISSION OF SimLab Co., LTD.
 */

/**
 * @file AllegroDogWalk.h
 * @author SimLab
 * @brief Class definition of BHand.
 *
 * @mainpage BHand
 * @section INTRODUCTION
 * Implementation of Allegro Dog walking algorithm.<br>
 * 
 * @section CREATEINFO
 * - Author: Sangyup (Sean) Yi
 * - Data: 2012/11/29
 * @section MODIFYINFO
 * - 2012/11/29: Commit this document. (by Sean)
 */

#ifndef __ALLEGRODOGWALK_H__
#define __ALLEGRODOGWALK_H__

#include "AllegroDogWalkDef.h"

/**
 * AllegroDogWalk class.
 * @brief Allegro Dog walking algorithm.
 * @author Vo Gia Loc, Sangyup Yi
 */
class ALLEGRODOGWALKEXPORT AllegroDogWalk
{
public:
	AllegroDogWalk(eDogType type);
	~AllegroDogWalk(void);

	/**
	 * Get the type of dog.
	 * @return It returns dog type.
	 * @see eDogType
	 */
	eDogType GetType();

	/**
	 * Set time interval.
	 * Algorithm assumes UpdateControl() is called once in this time interval.
	 * @param dT Time interval in seconds.
	 */
	void SetTimeInterval(double dT);

	/**
	 * Set Turning Arg. // Huan New
	 * Algorithm assumes UpdateControl() is called once in this time interval.
	 * @param dT Time interval in seconds.
	 */
	void SetTurningArg(int turningArg);

	/**
	 * Set Turning Arg. // Huan New
	 * Algorithm assumes UpdateControl() is called once in this time interval.
	 * @param dT Time interval in seconds.
	 */
	void SetSpeedArg(int speedArg);

	/**
	 * Set motion type.
	 * @param motionType Type of motion to set.
	 * @see eMotionType
	 */
	void SetMotionType(int motionType);

	/**
	 * Set Angle Reference.
	 * @param row_ref, pitch_ref, yaw_ref: Angles to set.
	 */
	void SetAngelRef(float row_ref, float pitch_ref, float yaw_ref);

	/**
	 * Set joint position of each joint. 
	 * The dog has twelve joints totally. 
	 * The dimension of 'q' should be twelve.
	 * This function should be called once in a control loop before UpdateControl() is called.
	 * @param q Array of joint positions in radian.
	 */ 
	void SetJointPosition(double* q);

	/**
	 * Set joint OFFSET position of each joint. 
	 * The dog has twelve joints totally. 
	 * The dimension of 'offset' should be twelve.
	 * This function should be called once in a control loop before UpdateControl() is called.
	 * @param offset Array of joint positions in radian.
	 
	void SetJointOffsetPosition(double* q_offset);*/

	/**
	 * Set body posture. 
	 * This function should be called once in a control loop before UpdateControl() is called.
	 * @param pitch Pitch angle.
	 * @param roll Roll angle.
	 * @param yaw Yaw angle.
	 * @param droll Derivative of pitch angle.
	 */
	void SetBodyPosture(double pitch, double roll, double yaw, double droll);

	/**
	 * Get desired(computed) joint positions.
	 * @param qd [out] Array of desired joint positions.
	 */
	void GetJointDesiredPosition(double* qd);

	/**
	 * Update control algorithm callback. This function should be called in each control time step manually.
	 * Desired joint torques are computed once in every control loop.
	 * @param time Current time in second.
	 */
	void UpdateControl(double time);

	/**
	 * Get desired(computed) joint torques.
	 * This function should be called once in a control loop after UpdateControl() is called.
	 * @param tau [out] Array of desired joint torques.
	 */
	void GetJointTorque(double* tau);

	/**
	 * Get forward kinematic solution for each leg.
	 * @param x [out] x coordinate of center of each footpad.
	 * @param y [out] y coordinate of center of each footpad.
	 * @param z [out] z coordinate of center of each footpad.
	 */
	void GetFKResult(double x[4], double y[4], double z[4]);

	/**
	 * Set desired joint position.
	 * @param q Array of desired joint positions in radian.
	 */
	void SetJointDesiredPosition(double* q);

	/**
	 * Set control gains explicitly.
	 * @param kp proportional gains
	 * @param ki integral gains
	 * @param kd derivative gains
	 */
	void SetGains(double* kp, double* ki, double* kd);

	/**
	 * Set gain for balancing.
	 * @param kp proportional gain
	 * @param kd derivative gain
	 */
	void SetBalanceGain(double kp, double kd);

	/**
	 * Set gain for compensating turning
	 * @param kp proportional gain
	 */
	void SetTurnGain(double kp);

	/**
	 * Set the kinematic information of the dog.
	 * @param l0 length of 1st link of each leg
	 * @param l1 length of 2nd link of each leg
	 * @param l2 length of 3rd link of each leg
	 * @param bw body width
	 * @param bl body length
	 */
	void SetKinematics(double l0, double l1, double l2, double bw, double bl);

	/**
	 * Set parameters of walking algorithm.
	 * @param stride_length stride step length
	 * @param stride_height stride step height
	 * @param stride_freq stride step frequency
	 * @param stride_inc_X stride step increment in X direction
	 * @param stride_inc_Z stride step increment in Z direction
	 */
	void SetWalkingParameters(double stride_length, double stride_height, double stride_freq, double stride_inc_X, double stride_inc_Z);

	/**
	 * Set desired home position
	 * @param height body height of home position from the center of base body frame in Z direction (normally negative number)
	 * @param front front foot position from the center of base body frame in X direction (normally positive number)
	 * @param hind hind foot position from the center of base body frame in X direction (normally negative number)
	 */
	void SetDesiredHomePosition(double height, double front, double hind);

private:
	void StartGaitGeneration();

	void Oscillator();
	void BalanceModule();
	void ControlDirectionCurveMethod(double desired_angle);
	void ControlDirectionSelfRotationMethod(int left_right);

	void Motion_HomePosition();
	void Motion_Ready();
	void Motion_Ready_Up();
	void Motion_Walk();
	void Motion_Run();
	void Motion_Hop();
	void Motion_Stop();

	void SolveFK();
	void SolveIK();

private:
	double _dT;							///< control time step (second)
	double _curT;
	int _turningArg;					/// Turning Argument

	double	_stride_freq;				/// frequency of stride
	double	_stride_step_height;		/// height of stride
	double	_stride_step_length;		/// length of stride
	double	_stride_step_inc_X;			/// increament unit in X direction of stride
	double	_stride_step_inc_Z;			/// increament unit in Z direction of stride
	double	_home_position_height;		/// height position of the body base
	double	_home_position_front;		/// front position of the body base
	double	_home_position_hind;		/// gear position of the body base
	double	_L0;						///> length of the first link in a leg
	double	_L1;						///> length of the second link in a leg
	double	_L2;						///> length of the third link in a leg
	double	_half_of_body_width;
	double	_half_of_body_length;

	float	_rpy_Ref[3];
	
	eDogType _dogType;					///> type of dog
	eMotionType _motionType;			///< type of motion currently set
	
//	double _q_offset[NOL][NOJ];			///< current joint offset angle (radian)
	double _q[NOL][NOJ];				///< current joint angle (radian)
	double _q_filtered[NOL][NOJ];		///< current joint angle (radian, low pass filtered)
	double _q_pre[NOL][NOJ];			///< previous(last) joint angle (radian)
	double _q_filtered_pre[NOL][NOJ];	///< previous(last) joint angle (radian, low pass filtered)
	double _qdot[NOL][NOJ];				///< joint velocity (radian/sec)
	double _qdot_filtered[NOL][NOJ];	///< joint velocity (radian/sec, low pass filtered)
	double _qdot_pre[NOL][NOJ];			///< previous(last) joint velocity (radian/sec)
	double _tau_des[NOL][NOJ];			///< desired joint torque
	double _q_des[NOL][NOJ];			///< desired joint position used in joint pd control motion

	//NEW CODE
	double _err_dot[NOL][NOJ];
	double _err_old[NOL][NOJ];
	double _err[NOL][NOJ];

	double _kp[NOL][NOJ];				///< proportional control gain for each joint
	double _ki[NOL][NOJ];				///< integral control gain for each joint
	double _kd[NOL][NOJ];				///< derivative control gain for each joint

	double _kp_balance;
	double _kd_balance;
	double _kp_turn;

	double _x[NOL];						///< x position of the center of each footpad along with cartesian space
	double _y[NOL];						///< y position of the center of each footpad along with cartesian space
	double _z[NOL];						///< z position of the center of each footpad along with cartesian space
	double _x_filtered[NOL];			///< x position of the center of each footpad along with cartesian space (low pass filtered)
	double _y_filtered[NOL];			///< y position of the center of each footpad along with cartesian space (low pass filtered)
	double _z_filtered[NOL];			///< z position of the center of each footpad along with cartesian space (low pass filtered)
	double _x_pre[NOL];					///< previous x position of the center of each footpad along with cartesian space
	double _y_pre[NOL];					///< previous y position of the center of each footpad along with cartesian space
	double _z_pre[NOL];					///< previous z position of the center of each footpad along with cartesian space
	double _x_filtered_pre[NOL];		///< previous x position of the center of each footpad along with cartesian space (low pass filtered)
	double _y_filtered_pre[NOL];		///< previous y position of the center of each footpad along with cartesian space (low pass filtered)
	double _z_filtered_pre[NOL];		///< previous z position of the center of each footpad along with cartesian space (low pass filtered)
	
	double _xdot[NOL];					///< velocity of the center of each footpad along with x-axis
	double _ydot[NOL];					///< velocity of the center of each footpad along with y-axis
	double _zdot[NOL];					///< velocity of the center of each footpad along with z-axis
	double _xdot_filtered[NOL];			///< velocity of the center of each footpad along with x-axis (low pass filtered)
	double _ydot_filtered[NOL];			///< velocity of the center of each footpad along with y-axis (low pass filtered)
	double _zdot_filtered[NOL];			///< velocity of the center of each footpad along with z-axis (low pass filtered)
	double _xdot_pre[NOL];				///< previous velocity of finger tip along with x-axis
	double _ydot_pre[NOL];				///< previous velocity of finger tip along with y-axis
	double _zdot_pre[NOL];				///< previous velocity of finger tip along with z-axis

	double _x_des[NOL];					///< desired x position in cartesian coordinate for the center of each footpad
	double _y_des[NOL];					///< desired y position in cartesian coordinate for the center of each footpad
	double _z_des[NOL];					///< desired z position in cartesian coordinate for the center of each footpad

	double _runTime;

	class CPosition {
	public:
		double x;
		double y;
		double z;
	};
	CPosition _footPosition[NOL];
	double _desiredAngle[NOL][NOJ];

	double _pitchAngle;			/// Pitch angle received from GYRO
	double _rollAngle;			/// Roll angle received from GYRO
	double _yawAngle;			/// Yaw angle received from GYRO
	double _dRollAngle;			/// Velocity in Roll angle received from GYRO
	double _desiredHeight[NOL]; /// Height desire
	double _stopDeepen[NOL];	/// Limit the height step
	double _strideLength;		/// moving along x- direction of foot
	double _stepHeight;			/// moving along z- direction of foot 
	double _turningMagnitude;	/// used for turning motion
	double _rotationMagnitude;	/// used for self turning
};

ALLEGRODOGWALK_EXTERN_C_BEGIN

/**
 * @brief Creates an instance of Allegro Dog walking algorithm.
 */
ALLEGRODOGWALKEXPORT AllegroDogWalk* CreateAllegroDogWalk();

ALLEGRODOGWALK_EXTERN_C_END

#endif // __ALLEGRODOGWALK_H__
