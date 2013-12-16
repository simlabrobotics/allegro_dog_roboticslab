
// rPanelManipulatorDef.h
//
#pragma once

typedef struct NTGraphProperty
{
	long         FrameStyle;
	COLORREF     PlotAreaColor;
	short        YGridNumber;
	double       Xmin;
	double       Xmax;
	double       Ymin;
	double       Ymax;
	long         ElementWidth;
	COLORREF     LineColor;
	long         LineType;
	BOOL         AutoScroll;
	BOOL         AutoRange;
	BOOL         FlagReset;
} NTGraphProperty;

#define BLACK			RGB(  0,   0,   0)
#define WHITE			RGB(255, 255, 255)
#define GRAY			RGB(222, 215, 200)

#define RED				RGB(255,   0,   0)
#define GREEN			RGB(  0, 255,   0)
#define BLUE			RGB(  0,   0, 255)

#define YELLOW			RGB(255, 255,   0)  //´«¾ÆÇÄ
#define CYAN			RGB(  0, 255, 255)	//´«¾ÆÇÄ
#define MAGENTA			RGB(255,   0, 255)

#define DARKGREEN		RGB(  0, 100,   0)
#define MAROON			RGB(128,   0,   0)
#define PURPLE			RGB(128,   0, 128)

#define HONEYDEW		RGB(230, 255, 230)
#define LIME_GREEN		RGB( 51, 204,  51)

#define HOT_PINK		RGB(255, 102, 179)
#define PALE_VIOLET_RED	RGB(230, 102, 153)
#define ROYAL_BLUE		RGB( 77, 102, 230)

static const COLORREF GraphLineColorArr[] = {
	RED,		GREEN,			BLUE,		YELLOW, 
	CYAN,		MAGENTA,		DARKGREEN,	MAROON, 
	PURPLE,		GRAY,			HONEYDEW,	LIME_GREEN,
	HOT_PINK,	PALE_VIOLET_RED,ROYAL_BLUE, GRAY
};

#define PI					3.14159265358979323846
#define DTR(x)				((x) * PI / 180.0)	// degree to radian
#define D2R(x)				((x) * PI / 180.0)	// degree to radian
#define RTD(x)				((x) * 180.0 / PI)	// radian to degree
#define R2D(x)				((x) * 180.0 / PI)	// radian to degree
