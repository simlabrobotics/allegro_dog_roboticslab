/* Kai, Copyright 2004-2011 SimLab Co., Ltd. All rights reserved.
 *
 * This library is commercial and cannot be redistributed, and/or modified
 * WITHOUT ANY ALLOWANCE OR PERMISSION OF SimLab Co., LTD.
 */

/** 
@file kaiSocket.h
@brief Kai network socket class.
@author SimLab
@date 2010/12/10
*/
#ifndef __KAISOCKET_H__
#define __KAISOCKET_H__

#include <string>
using namespace std;

class kaiBuffer;
/**
 * kaiSocket class. 
 */
class KAIEXPORT kaiSocket
{
public:
	/** 
	* kaiMsg constructor.
	* Default constructor, do nothing.
	*/
	kaiSocket();

	/** 
	* kaiMsg destructor.
	* Default destructor. do nothing.
	*/
	virtual ~kaiSocket();

	/**
	 * Called after constructing kaiSocket object to create socket and attach it.
	 * @param bTCP true for TCP socket, false for UDP socket.
	 * @param lEvent A bitmask which specifies a combination of network event in which the application is interested. 
	 * <pre>
	 * kaiON_SEND for send callback, 
	 * kaiON_MESSAGE for message callback,
	 * kaiON_RECV for receive callback,
	 * kaiON_ACCEPT for accept callback,
	 * kaiON_CLOSE for close. 
	 * </pre>
	 * @param buffersize Size of receive buffer.
	 * @param maxdatasize Maximum massage size that this socket can handle.
	 */
	void create(
		bool bTCP = true, 
		long lEvent = kaiNO_EVENT, 
		unsigned buffersize = kaiBUFFER_SIZE, 
		unsigned maxdatasize = kaiMAX_MSG_SIZE);

	/**
	 * Send data through a connected socket.
	 * The onSend callback function is called if kaiON_SEND flag is set when created. 
	 * @param[in] buf pointer to a buffer containing data to be transmitted. 
	 * @param len size of data to be transmitted.
	 * @return It returns the size of data sent.
	 */
	int send(const char* buf, int len);
	
	/** 
	 * Send data through a connected socket.
	 * The onSend callback function is called if kaiON_SEND flag is set when created. 
	 * @param[in] msg kaiMsg object containing data to be transmitted.
	 * @return It returns the size of data sent.
	 */
	int send(const kaiMsg& msg);

	/** 
	 * Send data through a connected socket. The data to be transmitted should already be in _sendBuffer.
	 * The onSend callback function is called if kaiON_SEND flag is set when created. 
	 * @return It returns the size of data sent.
	 * @see putMessage()
	 */
	int send();

	/**
	 * Send data to a specific destination. Created socket should be UDP socket.
	 * The onSend callback function is called if kaiON_SEND flag is set when created. 
	 * @param[in] buf Pointer to a buffer containing data to be transmitted.
	 * @param len Size of data to be transmitted.
	 * @param addr The network address of the destination to which the data is transmitted.
	 * @param port Port number of destination.
	 * @return It returns the size of data sent.
	 */
	int sendTo(const char* buf, int len, const char* addr, unsigned short port);

	/**
	 * Send data to a specific destination. Created socket should be UDP socket.
	 * The onSend callback function is called if kaiON_SEND flag is set when created. 
	 * @param[in] msg kaiMsg object containing data to be transmitted.
	 * @param addr The network address of the destination to which the data is transmitted.
	 * @param port Port number of destination.
	 * @return It returns the size of data sent.
	*/
	int sendTo(const kaiMsg& msg, const char* addr, unsigned short port);

	/**
	 * Send data to a specific destination. Created socket should be UDP socket.
	 * The data to be transmitted should already be in _sendBuffer.
	 * The onSend callback function is called if kaiON_SEND flag is set when created. 
	 * @param addr The network address of the destination to which the data is transmitted.
	 * @param port Port number of destination.
	 * @return It returns the size of data sent.
	*/
	int sendTo(const char* addr, unsigned short port);
		
	/**
	 * Send data to a specific destination. Created socket should be UDP socket.
	 * The onSend callback function is called if kaiON_SEND flag is set when created. 
	 * @param[in] buf Pointer to a buffer containing data to be transmitted.
	 * @param len Size of data to be transmitted.
	 * @param lpSockAddr Pointer to sockaddr_in structure that contains the network address of destination.
	 * @return It returns the size of data sent.
	 */
	int sendTo(const char* buf, int len, const sockaddr_in* lpSockAddr);

	/**
	 * Send data to a specific destination. Created socket should be UDP socket.
	 * The onSend callback function is called if kaiON_SEND flag is set when created. 
	 * @param[in] msg kaiMsg object containing data to be transmitted.
	 * @param lpSockAddr Pointer to sockaddr_in structure that contains the network address of destination.
	 * @return It returns the size of data sent.
	 */
	int sendTo(const kaiMsg& msg, const sockaddr_in* lpSockAddr);

	/**
	 * Send data to a specific destination. Created socket should be UDP socket. The data to be transmitted should already be in _sendBuffer.
	 * The onSend callback function is called if kaiON_SEND flag is set when created. 
	 * @param lpSockAddr Pointer to sockaddr_in structure that contains the network address of destination.
	 * @return It returns the size of data sent.
	 */
	int sendTo(const sockaddr_in* lpSockAddr);
	
	/**
	 * Send data to host that already sent data to this socket. Created socket should be UDP socket.  
	 * The onSend callback function is called if kaiON_SEND flag is set when created. 
	 * @param[in] msg kaiMsg object containing data to be transmitted.
	 * @return It returns the size of data sent.
	 */
	int sendTo(const kaiMsg& msg);

	/**
	 * Receive data from a socket. Socket should be TCP socket. Data is written in _recvBuffer.
	 * The onMessage function is called if kaiON_MESSAGE flag is set and the onReceive function is called if kaiON_RECV flag is set when crated.
	 * If both are set, kaiON_RECV is ignored. Recommend to use this function when one of above flags is set. 
	 * @return It returns the number of bytes received.
	 */
	int recv();

	/**
	 * Receive data from a socket. Socket should be TCP socket. 
	 * No callback function is called. 
	 * @param[out] buf Pointer to buffer to receive data.
	 * @param len Maximum data size to receive.
	 * @return It returns the number of bytes received.
	 */
	int recv(void* buf, int len);

	/**
	 * Receive data from a socket. Socket should be UDP socket. 
	 * The onMessage function is called if kaiON_MESSAGE flag is set and the onReceive function is called if kaiON_RECV flag is set when crated.
	 * If both are set, kaiON_RECV is ignored. Recommend to use this function when one of above flags is set. 
	 * @param[out] addr String object that receives a dotted number ip address.
	 * @param[out] port Port number of source address.
	 * @return It returns the number of bytes received.
	 */
	int recvFrom(string& addr, unsigned short& port);

	/**
	 * Receive data from a socket. Socket should be UDP socket. 
	 * No callback function is called. 
	 * @param[out] buf Pointer to buffer to receive data.
	 * @param len Maximum data size to receive.
	 * @param[out] addr String object that receives a dotted number ip address.
	 * @param[out] port Port number of source address.
	 * @return It returns the number of bytes received.
	 */
	int recvFrom(void* buf, int len, string& addr, unsigned short& port);
	
	/**
	 * Receive data from a socket. Socket should be UDP socket. 
	 * The onMessage function is called if kaiON_MESSAGE flag is set and the onReceive function is called if kaiON_RECV flag is set when crated.
	 * If both are set, kaiON_RECV is ignored. Recommend to use this function when one of above flags is set. 
	 * @param[out] lpSockAddr Pointer to sockaddr_in structure.
	 * @return It returns the number of bytes received.
	 */
	int recvFrom(sockaddr_in* lpSockAddr);

	/**
	 * Receive data from a socket. Socket should be UDP socket. 
	 * No callback function is called. 
	 * @param[out] buf Pointer to buffer to receive data.
	 * @param len Maximum data size to receive.
	 * @param[out] lpSockAddr Pointer to sockaddr_in structure.
	 * @return It returns the number of bytes received.
	 */
	int recvFrom(void* buf, int len, sockaddr_in* lpSockAddr);

	/** 
	 * Set socket handle manually. Socket handle assigned before is released. 
	 * @param sock New socket handle to be assigned.
	 */
	void setSocket(SOCKET sock);

	/** 
	 * Get socket handle contained in this object.
	 * @return It returns the current socket handle.
	 */
	SOCKET getSocket();

	/** 
	 * Close the socket.
	 */ 
	void close();

	/**
	 * Establish a connection. It can be used for both TCP and UDP socket.
	 * @param addr The network IP address.
	 * @param port Port number to connect.
	 * @return It returns kaiSUCCESS if the connection is established successfully. Otherwise it returns kaiFAIL.
	 */
	int connect(const char* addr, unsigned short port);

	/** 
	 * Listen for incoming connection requests.
	 * @return It returns kaiSUCCESS if there is no error. Otherwise it returns kaiFAIL.
	 */
	int listen();

	/** 
	 * Associate a local address with the socket.
	 * @param port Port number to bind.
	 * @return It returns kaiSUCCESS if there is no error. Otherwise it returns kaiFAIL.
	 */
	int bind(unsigned short port);

	/** 
	 * Accept a connector on a socket.
	 * @param[out] sock A reference identifying a new socket that is available for connection.
	 */
	void accept(kaiSocket& sock);
	
	/** 
	 * Set blocking mode for socket.
	 * @param blocking Blocking mode.
	 * <pre>
	 *     kaiBLOCKING_SOCKET : blocking mode.
	 *     kaiNON_BLOCKING_SOCKET : non-blocking mode.
	 * </pre>
	 */
	void setBlockingMode(int blocking);
	
	/** 
	 * Set id for this object.
	 * @param id Id to be set.
	 */
	void setID(int id);

	/** 
	 * Get id.
	 * @return Id of this object.
	 */
	int getID() const;
	
	/** 
	 * Get message from _recvBuffer manually. This function pops one message from _recvBuffer.
	 * @param[out] msg Reference to kaiMsg object.
	 * @return It return true when it pops a message successfully. Otherwise it returns false. In this context false means not a error. Even if there is no message arrived, it returns false.
	 */
	bool getMessage(kaiMsg& msg);
	
	/** 
	 * Get message from _recvBuffer manually. 
	 * This function tries to get data from _recvBuffer as much as possible.
	 * The maximun data size to get at once is limited up to kaiMAX_MSG_SIZE.
	 * @param[out] data Double pointer for data. Get address to data buffer.
	 * @param[out] len Size of data received.
	 */
	void getMessage(void** data, int& len);

	/** 
	 * Put message to _sendBuffer.
	 * @param[in] msg kaiMsg object to append to _sendBuffer.
	 */
	void putMessage(const kaiMsg& msg);

	/** 
	 * Put data to _sendBuffer.
	 * @param[in] data Pointer to data buffer to append to _sendBuffer.
	 * @param len Size of data to put.
	 */
	void putMessage(const void* data, int len);
	
	/** 
	 * Set user data.
	 * @param param User parameter.
	 */
	void setUserParam(int param);

	/** 
	 * Get user data.
	 * @return It returns an integer value which is set by user.
	 */
	int getUserParam() const;

	/**
	 * Reset this socket.
	 * It delete buffers and initialize socket handle.
	 */
	void reset();

	/**
	 * Set socket option, TCP_NODELAY.
	 */
	void setNoDelay();

	/**
	 * Set the manual close flag.
	 * @param b Whether to set the flag or not.
	 */
	void setManualClose(bool b);

	/**
	 * Check the socket handle and returns whether it is valid or not.
	 * @return It returns true when the sockeck handle is valid. Otherwise it returns false.
	 */
	bool isValid();

	/** 
	 * Call to get sockaddr struct which has ip address
	 * This function is run at only UDP mode.
	 * @return It returns sockaddr struct with valid information when mode is UDP.
	 */
	sockaddr_in getSocketAddress();

protected:
	/** 
	 * Called when there is message in _recvBuffer. 
	 * Override this function to process the message.
	 * @param msg kaiMsg object to be read from _recvBuffer.
	 */
	virtual void onMessage(kaiMsg& msg){}

	/** 
	 * Called when sending data.
	 * Override this function to do something when application send data .
	 * @param len Size of data sent.
	 */
	virtual void onSend(int len){}

	/** 
	 * Called when closing socket.
	 * Override this function to do something when this socket is closed.
	 */
	virtual void onClose(){}

	/** 
	 * Called when accepting new connection.
	 * Override this function to do something when new socket connection is accepted.
	 * @param sock kaiSocket object for new socket. 
	 */
	virtual void onAccept(kaiSocket& sock){}

	/** 
	 * Called when data is received.
	 * Override this function to do something when application receive new data from socket.
	 * @param data Pointer to data received.
	 * @param len Size of data received.
	 */
	virtual void onReceive(void* data, int len){}

private:
	long			_event;			///< Event flag. On this flag, event callback is called when a certain event is occurred.
	SOCKET			_socket;		///< Socket handle.
	kaiBuffer*		_recvBuffer;	///< Receive buffer.
	kaiBuffer*		_sendBuffer;	///< Send buffer.
	sockaddr_in		_addr;			///< Socket address.
	int				_id;			///< Socket id.
	
	int			_userParam;		///< User parameter. It is used for user specific purpose.
	bool		_bManualClose;	///< Whether this sockeck is closed by user manually or not.

};

#endif
