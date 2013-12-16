
#pragma once

#include "resource.h"
#include "rPanelManipulatorCmdUtil.h"
#include "kai2/kai.h"

class CrPlotServer : public kaiServer
{
public:
	CrPlotServer(unsigned short port, bool start_rplot);
	virtual ~CrPlotServer();

	virtual void onMessage(int id, kaiMsg& msg);
    virtual void onAccept(int id);
    virtual void onClose(int id);

	int ackHello(kaiSocket* client);
	int ntyPlotInfo(kaiSocket* client);
	int ntyPlotData(unsigned int pid, double time, unsigned short count, double* values);

private:
	kaiSocket*	_clientLastConnected;
	kaiMsg		_msgSend;
};
