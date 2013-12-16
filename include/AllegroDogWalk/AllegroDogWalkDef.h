/* Allegro, Copyright 2012 SimLab Co., Ltd. All rights reserved.
 *
 * This library is commercial and cannot be redistributed, and/or modified
 * WITHOUT ANY ALLOWANCE OR PERMISSION OF SimLab Co., LTD.
 */

/**
 * @file AllegroDogDef.h
 * @author SimLab
 * @brief Definitions.
 */
#ifndef __ALLEGRODOGWALKDEF_H__
#define __ALLEGRODOGWALKDEF_H__

/* DLL export */
#if defined(WIN32) || defined(WINCE)
#    if defined(ALLEGRODOGWALK_EXPORTS)
#        define ALLEGRODOGWALKEXPORT __declspec(dllexport)
#    elif defined(ALLEGRODOGWALK_IMPORTS)
#        define ALLEGRODOGWALKEXPORT __declspec(dllimport)
#    else
#        define ALLEGRODOGWALKEXPORT
#    endif
#else
#    define ALLEGRODOGWALKEXPORT
#endif


#ifdef __cplusplus
#    define ALLEGRODOGWALK_EXTERN_C_BEGIN    extern "C" {
#    define ALLEGRODOGWALK_EXTERN_C_END    }
#else
#    define ALLEGRODOGWALK_EXTERN_C_BEGIN
#    define ALLEGRODOGWALK_EXTERN_C_END
#endif

#ifndef DEG2RAD
#define DEG2RAD (3.141592f/180.0f)
#endif

#ifndef RAD2DEG
#define RAD2DEG (180.0f/3.141592f)
#endif

#define		NOL 4 // number of legs
#define		NOJ 3 // number of joints in each leg
#define		SIZEOF_VARRAY (NOL*NOJ*8)


////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////
// Some default values(constants)
#define		BX					0.2		// half of body length
#define		BY					0.120	// half of body width

#define		L0					0.07	// length of 1st link
#define		L1					0.2		// length of 2nd link
#define		L2					0.23	// length of 3rd link

#define		INC_POS_X			0.00004	// constant value to increase speed in x direction 0.00001 // 1mps:0.000017
#define		INC_POS_Z			0.00005	// constant value to increase body height in z direction

#define		STRIDE_STEP_LENGTH	0.08	// stride length 0.04 // 1mps:0.16 // normal:0.03
#define		STRIDE_STEP_HEIGHT	0.03	// step height 0.03   // 1mps:0.025 //normal: 0.03
#define		STRIDE_FREQ			1.4		// frequency of stride 1.8  // 1mps:2.4 //normal: 1.8

#define		SP_HEIGHT			-0.47	// desired z-coordinate of leg // -0.47
#define		SP_FRONT			BX		// desired x-coordinate of front leg
#define		SP_HIND				-BX		// desired x-coordinate of hind leg
////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////


/**
 * Type of motion.
 * @brief Motion type set by user.
 */
enum eMotionType
{
	eMotionType_NONE,				///< power off
	eMotionType_HOME,				///< go to home position
	eMotionType_READY,				///< init posture
	eMotionType_UP,					///< Up posture
	eMotionType_DN,					///< Dn posture
	eMotionType_WALK,				///< walking
	eMotionType_RUN,				///< running
	eMotionType_HOP,				///< slow down robot and hopping
	eMotionType_STOP,				///< Slow STOP
	eMotionType_READY_UP,			///< Up posture
	eMotionType_LEFT,				///< Turn left
	eMotionType_RIGHT,				///< Turn right
	eMotionType_BACK,				///< Back
	NUMBER_OF_MOTION_TYPE
};

/**
 * Type of dog. It is input argument to instanciate AllegroDogWalk class.
 * @brief It specifies the type of dog.
 */
enum eDogType
{
	eDogType_AllegroDog = 0,		///< Allegor Dog
};

#endif // __ALLEGRODOGWALKDEF_H__
