/* Kai, Copyright 2004-2011 SimLab Co., Ltd. All rights reserved.
 *
 * This library is commercial and cannot be redistributed, and/or modified
 * WITHOUT ANY ALLOWANCE OR PERMISSION OF SimLab Co., LTD.
 */

/**
@file kaiServer.h
@brief Network server class.
@author SimLab
@date 2010/12/10
*/
#ifndef __KAISERVER_H__
#define __KAISERVER_H__
#include <map>
#include <vector>
#include <queue>

using namespace std;
class kaiIDGenerator;

/**
 * kaiServer class. 
 */
class KAIEXPORT kaiServer
{
	typedef map<int, kaiSocket*> ClientList;

public:
	/** 
	 * kaiServer constructor.
	 * Default constructor, do nothing.
	 */
	kaiServer();

	/** 
	 * kaiServer destructor.
	 * Default destructor, do nothing.
	 */
	virtual ~kaiServer();

	/** 
	 * It creates a server socket and start to listen to the connection request from the clients.
	 * @param port Port number. If TCP, listen socket is binded to this port, otherwise newly opened UDP sockets are binded to port number increased one by one from this number.
	 * @param event A bitmask which specifies a combination of network event in which application is interested.
	 * <pre>
	 *     kaiON_ACCEPT for accept callback, 
	 *     kaiON_MESSAGE for message callback, 
	 *     kaiON_CLOSE for close callback.
	 * </pre>
	 * @param bTCP If to create TCP server set this value true, otherwise false.
	 * @return It returns kaiSUCCESS when it starts the server successfully. Otherwise it returns kaiFAIL.
	 */
	int create(
		unsigned short port, 
		long event = kaiON_ACCEPT | kaiON_MESSAGE | kaiON_CLOSE, 
		bool bTCP = true);
	
	/**
	 * Run the server.
	 * @param bThreaded if true, server spawn the new thread. Otherwise server runs on current thread.
	 * @param loopPerSec Determine how many loop is processed per one second.
	 */
	void run(bool bThreaded = false, int loopPerSec = 50);

	/**
	 * Stop the server process.
	 */
	void stop();

	/**
	 * Open a new UDP socket.
	 * @return It returns the binded port number if it opens an UDP channel successfully. Otherwise it returns 0.
	 */
	unsigned short openUDPChannel();

	/**
	 * Close a client and remove it from the connected client list.
	 * @param id Client id to close.
	 */
	void closeClient(int id);

protected:
	/** 
	 * Called when there is client that has message. 
	 * Override this function to process the message of each client.
	 * @param id Unique id for client.
	 * @param msg kaiMsg object to be read from _recvBuffer.
	 */
	virtual void onMessage(int id, kaiMsg& msg){}

	/** 
	 * Called when there is client that received new incoming data.
	 * Override this function to do something when client receive new data.
	 * @param id Unique id for client. 
	 * @param data Pointer to data received
	 * @param len Size of data received
	 */
	virtual void onReceive(int id, const void* data, int len){}
	
	/** 
	 * Called when accepting new connection.
	 * Override this function to do something when new socket connection is accepted.
	 * @param id Unique id for client. 
	 */
	virtual void onAccept(int id){}

	/** 
	 * Called when closing socket.
	 * Override this function to do something when client is closed.
	 * @param id Unique id for client.
	 */
	virtual void onClose(int id){}
	
    /** 
	 * Call this function to get kaiSocket object with id.
	 * @param id Unique id for client.
	 * @return It returns a client socket id of which is 'id'.
	 */
	kaiSocket* findClient(int id);

private:
	void _run();
	void _closeClient();

	fd_set _rset;
	fd_set _temp;
	kaiSocket _serverSocket;
	
	int _curClientNum;
	int _maxClientNum;
	timeval _timeout;
	
	ClientList _clientList;
	ClientList _clientListByID;
	kaiIDGenerator* _idGen;
	
	long _event;
	unsigned short _port;
	bool _bTCP;
	int _hThread;
	int _loopPerSec;
	bool _bStop;
	int _sleepTime;
	queue<int> _closeQueue;
	
#ifndef WIN32
	vector<int> _fdSets;
	int _maxfd;
#endif

#ifdef WIN32
	CRITICAL_SECTION _cs;
#else
    pthread_mutex_t _lock;
#endif

#ifdef WIN32
	friend void threadFunc(void* param);
#else 
	friend void* threadFunc(void* param);
#endif
	
};


#endif
