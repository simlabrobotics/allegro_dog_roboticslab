#pragma once

#include <string>
using namespace std;

class CrPanelAllegroDogAppParam
{
public:
	string strRtssProcessManagerPath;
	string strRTXPropertiesPath;
	string strRtssRunPath;
	int nGraphUpdateMethod; //0:use HPET(High Precision Event Timer) callback(default), 1: use event polling
	int nGraphSamplingPeriod;
	int nGraphSamplingDuration;
	double dblGraphXmin;
	double dblGraphXmax;
	double dblGraphYmin;
	double dblGraphYmax;
	bool bRPlotEnable;
	bool bRPlotAutoStart;
	unsigned short nRPlotPort;
	string strUserCmdLabel[16];
};

#define JDOF 12
