#include "stdafx.h"
#include "rPlotServer.h"
#include "rProtocol/rProtocol.h"
#include "rProtocol/rpPhysics.h"
#include "rTerm/rTerm.h"

CrPlotServer::CrPlotServer(unsigned short port, bool start_rplot)
{
	kaiInitialize();
	_clientLastConnected = NULL;
	_msgSend.allocateMemory();
	create(port, kaiON_ACCEPT|kaiON_MESSAGE|kaiON_CLOSE, true);
	run(true);
	if (start_rplot)
	{
		char arg[256];
		sprintf_s(arg, 256, "-ip 127.0.0.1 -port %d", port);
		rTerm::startPlot(arg, false);
	}
}

CrPlotServer::~CrPlotServer()
{
	stop();
	kaiShutdown();
}

int CrPlotServer::ackHello(kaiSocket* client)
{
	if (!client) return 0;

	kaiMsg msgSend;
	char buf[256];
	msgSend.setBuffer(buf, 256);
	msgSend.id(RP_ACK_HELLO);
	msgSend.begin();
	msgSend << RP_RETCODE_HELLO_SUCCESS;
	msgSend.end();
	return client->send(msgSend);
}

int CrPlotServer::ntyPlotInfo(kaiSocket* client)
{
	if (!client) return 0;

	RPUINT32 plotId;
	std::string plotName;
	RPBYTE plotType;
	RPBYTE plotLayout;
	RPINT16 plotGroupId;
	RPINT16 plotNOV;
	std::string legend;
	RPBYTE lineType;
	RPREAL32 lineColorR;
	RPREAL32 lineColorG;
	RPREAL32 lineColorB;
	
	kaiMsg msgSend;
	char buf[256];

	msgSend.setBuffer(buf, 256);
	msgSend.id(RP_NTY_MESSAGE);
	msgSend.begin();
	
	plotId = 1;
	plotName = "joint angles";
	plotType = RPPYS_PLOT_TIMELINE;
	plotLayout = RPPYS_PLOT_LAYOUT_VERTICAL;
	plotGroupId = 1;
	plotNOV = 1;

	msgSend << (RPMID)RPPYS_NTY_NEW_PLOT
			<< plotId
			<< plotName
			<< plotType
			<< plotLayout
			<< plotGroupId
			<< plotNOV;

	for (int i=0; i<plotNOV; i++)
	{
		legend = "angle";
		lineType = RPPYS_PLOT_LINE_TYPE_SOLID;
		lineColorR = 0.5;
		lineColorR = 0.0;
		lineColorR = 0.0;

		msgSend << legend
				<< lineType
				<< lineColorR
				<< lineColorG
				<< lineColorB;
	}

	msgSend.end();
	return client->send(msgSend);
}

int CrPlotServer::ntyPlotData(unsigned int pid, double time, unsigned short count, double* values)
{
	if (!_clientLastConnected) return 0;

	kaiMsg msgSend;
	char buf[256];
	msgSend.setBuffer(buf, 256);
	msgSend.id(RP_NTY_PLOT_VALUE);
	msgSend.begin();

	msgSend << (RPUINT32)pid
			<< (RPREAL64)time
			<< (RPINT16)count;

	for (int i=0; i<count; i++)
	{
		msgSend << (RPREAL64)values[i];
	}

	msgSend.end();
	return _clientLastConnected->send(msgSend);
}


void CrPlotServer::onMessage(int id, kaiMsg& msg)
{
#ifdef _DEBUG
	TCHAR dmsg[256];
	_stprintf_s(dmsg, 256, _T("Message (%d) is received from client (%d).\n"), msg.id(), id);
	OutputDebugString(dmsg);
#endif

	switch (msg.id())
	{
	case RP_REQ_HELLO:
		{
			kaiSocket* client = findClient(id);
			ackHello(client);
			ntyPlotInfo(client);
		}
		break;
	}
}

void CrPlotServer::onAccept(int id)
{
#ifdef _DEBUG
	TCHAR dmsg[256];
	_stprintf_s(dmsg, 256, _T("New client (%d) is connected.\n"), id);
	OutputDebugString(dmsg);
#endif

	_clientLastConnected = findClient(id);
}

void CrPlotServer::onClose(int id)
{
#ifdef _DEBUG
	TCHAR dmsg[256];
	_stprintf_s(dmsg, 256, _T("Client (%d) is disconnected.\n"), id);
	OutputDebugString(dmsg);
#endif

	kaiSocket* client = findClient(id);
	if (client == _clientLastConnected)
		_clientLastConnected = NULL;
}
