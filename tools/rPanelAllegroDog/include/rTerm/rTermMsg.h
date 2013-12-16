/* RoboticsLab, Copyright 2008-2011 SimLab Co., Ltd. All rights reserved.
 *
 * This library is commercial and cannot be redistributed, and/or modified
 * WITHOUT ANY ALLOWANCE OR PERMISSION OF SimLab Co., LTD.
 */

/**
 * @file rTermMsg.h
 * @brief rTerm Message classes definition.
 */
#ifndef __rTermMSG_H__
#define __rTermMSG_H__

#include "rTermDef.h"
#include <streambuf>

class rTermSession;

/**
 * @brief A class wrapping kaiMsg.
 * @author Sangyup Yi
 * @version 1.2
 * @since 2009/02/23
 * @see RecvMsg
 * @see AckMsg
 * @see SendMsg
 * @see ReqMsg
 * @see QueryMsg
 * @see DeliverMsg
 * @see DeliverRequestMsg
 */
class RTERMEXPORT rTermMsg
{
public:
    /** Default constructor. The message id is set to -1 and size to 0.
     */
	rTermMsg();
    /** Constructor with parameters to initialize this.
     */
    rTermMsg(unsigned short id, unsigned short size, const unsigned char* buff);
    /** Destructor.
     */
	virtual ~rTermMsg();

	/** Return the message id.
     * @return It returns the message id.
	 */
	unsigned int id() { return _id; }

    /** Return the size of the message.
     * @return It returns the data size currently set.
	 */
    unsigned int size() { return _size; }
    
    /** Reset the message and initialize buffer pointer.
	 */
    void reset();
	
	/** Assignment operator.
     * @param msg Reference to rTermMsg to assign to this.
     * @return Referenct to this.
     *
	 */
	rTermMsg& operator=(const rTermMsg &msg);

protected:
	unsigned short _id;         ///< message id.
    unsigned short _size;       ///< data size.
    unsigned char* _buffer;     ///< pointer to initial position of data buffer.
    unsigned char* _bufptr;     ///< pointer to data buffer where to write to or read data from.
	static unsigned int _sizeMax;
};


/**
 * @brief A message received from rKernel.
 * @author Sangyup Yi
 * @version 1.2
 * @since 2009/02/23
 * @see rTermMsg
 * @see AckMsg
 * @see SendMsg
 * @see ReqMsg
 * @see QueryMsg
 * @see DeliverMsg
 * @see DeliverRequestMsg
 */
class RTERMEXPORT RecvMsg : public rTermMsg
{
public:
    /** Default constructor.
     */
    RecvMsg();
    /** Destructor.
     */
    virtual ~RecvMsg();

    /** Reset read pointer.
      */
    void begin();

    /** Read an integer(4 byte) from data buffer.
      * @return The value read.
      */
    int				readInt();

    /** Read an unsigned char(1 byte) from data buffer.
      * @return The value read.
      */
    unsigned char	readByte();

    /** Read a short integer(2 byte) from data buffer.
      * @return The value read.
      */
    short			readShort();

    /** Read a charater string from data buffer.
      * @return The pointer to character string.
      */
    const char		*readString();

    /** Read a float(4 byte) from data buffer.
      * @return The value read.
      */
    float			readFloat();

    /** Read a float(double precision) from data buffer.
      * @return The value read.
      */
    double 			readDouble();

    /** Read byte stream.
      * @return The read size.
      * @param[out] buff The pointer to data buffer.
      * @param size The buffer size.
      */
    int 			readData(unsigned char* buff, int size);

#ifndef SWIG
    /** Operator overloading to read stadard data type from data buffer.
	 * @param[out] val value to store the value read.
     */
    template<typename T>
    RecvMsg& operator>>(T& val)
	{
		size_t size = sizeof(T);
		if ((_bufptr + size) > (_buffer + _sizeMax))
			return *this;
		memcpy(&val, _bufptr, size);
		_bufptr += size;
		return *this;
	}

    /** Operator overloading to read stl string from data buffer.
	 * @param[out] str stl string to stord the string read.
     */
    template<>
    RecvMsg& operator>>(std::string& str)
	{
		int len;
		if ((_bufptr + sizeof(len)) > (_buffer + _sizeMax))
			return *this;
		memcpy((void*)&len, _bufptr, sizeof(len));
		if (len < 0 || (_bufptr + sizeof(len) + len) > (_buffer + _sizeMax))
			return *this;

		char* buff = new char[len+1];
		memset(buff, 0, len+1);
		memcpy(buff, _bufptr + sizeof(len), len);
		str = buff;
		delete [] buff;

		_bufptr += (sizeof(len) + len);
		return *this;
	}
#endif
};


/**
 * @brief An acknowledge message received from rKernel.
 * Normally ReqMsg::request() functions call returns reference to AckMsg object.
 * @author Sangyup Yi
 * @version 1.2
 * @since 2009/02/23
 * @see rTermMsg
 * @see RecvMsg
 * @see SendMsg
 * @see ReqMsg
 * @see QueryMsg
 * @see DeliverMsg
 * @see DeliverRequestMsg
 */
class RTERMEXPORT AckMsg : public RecvMsg
{
	friend	class rTermSession;

public:
    /** Constructor.
     */
	AckMsg();
    /** Destructor.
     */
	virtual ~AckMsg();

	/** Return the result code.
     * @return It returns result code from rKernel.
	  */
	unsigned char	retCode() { return _retCode;}
    /** Set return code.
     * @param rtn Return code.
     */
	void			retCode(unsigned char rtn) { _retCode = rtn; }

    /** Assignment operator.
	  */
	AckMsg& operator=(const rTermMsg &msg);

protected:
	unsigned char	_retCode;   ///< Return code.
};


/**
 * @brief A message class to send a message to rKernel.
 * @author Sangyup Yi
 * @version 1.2
 * @since 2009/02/23
 * @see rTermMsg
 * @see RecvMsg
 * @see AckMsg
 * @see ReqMsg
 * @see QueryMsg
 * @see DeliverMsg
 * @see DeliverRequestMsg
 */
class RTERMEXPORT SendMsg : public rTermMsg
{
public:
    /** Constructor.
     * @param msgtype rProtocol message id.
     */
    SendMsg(unsigned short msgtype);
    /** Destructor.
     */
    virtual ~SendMsg();

    /** Write integer data.
      * @return The data size written.
      * @param value The integer data.
      */
    int writeInt(int value);

    /** Write Byte data.
      * @return The data size written.
      * @param value The byte data.
      */
    int writeByte(unsigned char value);

    /** write short data.
      * @return The data size written.
      * @param value The short data.
      */
    int writeShort(short value);

    /** write float data.
      * @return The data size written.
      * @param value The float data.
      */
    int writeFloat(float value);

    /** write float data(double precision, 64bit).
      * @return The data size written.
      * @param value The float data.
      */
    int writeDouble(double value);

    /** write string.
      * @return The data size written.
      * @param value The string.
      */
    int writeString(const char* value);

    /** write byte stream.
      * @return The data size written.
      * @param buff The pointer to data buffer.
      * @param size The buffer size.
      */
    int writeData(const unsigned char* buff, int size);

#ifndef SWIG
	/** Operator overloading to write stadard data type to data buffer.
	 * @param val value to write.
     */
    template<typename T>
    SendMsg& operator<<(const T& val)
	{
		int size = (int)sizeof(T);
		if ((_bufptr + size) > (_buffer + _sizeMax))
			return *this;
		memcpy(_bufptr, &val, size);
		_bufptr += size;
		_size += size;
		return *this;
	}

	/** Operator overloading to write stl string to data buffer.
	 * @param val stl string to write.
     */
    template<>
    SendMsg& operator<<(const std::string& val)
	{
		int len = (int)val.size();
		if ((_bufptr + sizeof(len) + len) > (_buffer + _sizeMax))
			return *this;
		memcpy(_bufptr, &len, sizeof(len));
		_bufptr += sizeof(len);
		_size += sizeof(len);
		memcpy(_bufptr, val.c_str(), len);
		_bufptr += len;
		_size += len;
		return *this;
	}
#endif

	/** Send this message to rKernel.
	  * @return The message length sent.
	  */
	int send();
};


/**
 * @brief A message class to send a request(RP_REQ_) to rKernel.
 * @author Sangyup Yi
 * @version 1.2
 * @since 2009/02/23
 * @see rTermMsg
 * @see RecvMsg
 * @see AckMsg
 * @see SendMsg
 * @see QueryMsg
 * @see DeliverMsg
 * @see DeliverRequestMsg
 */
class RTERMEXPORT ReqMsg : public SendMsg
{
public:
    /** Constructor.
     * @param msgtype rProtocol message id.
     */
	ReqMsg(unsigned short msgtype);
    /** Destructor.
     */
	virtual ~ReqMsg();

	/** Send this message to rKernel's plug-in module and wait until receiving acknowledge.
     * If there is no responding acknowledgement from rKernel after requesting, this fuctions
     * call is blocked by default. If you want to alter this feature and set the timeout value,
     * you can set the environment variable with argument type id of ACK_TIMEOUT.
	 * @return The pointer to the AckMsg.
	 */
	virtual AckMsg* request();
};


/**
 * @brief A message class to send an extended rProtocol message(RP_REQ_DELIVER_MESSAGE, 
 * RP_REQ_DELIVER_REQEUST, RP_REQ_NTY_MESSAGE) to rKernel extensions.
 * @author Sangyup Yi
 * @version 1.2
 * @since 2009/02/23
 * @see rTermMsg
 * @see RecvMsg
 * @see AckMsg
 * @see SendMsg
 * @see ReqMsg
 * @see DeliverMsg
 * @see DeliverRequestMsg
 */
class RTERMEXPORT QueryMsg : public ReqMsg
{
public:
    /** Constructor.
     * @param msgtype rProtocol message id.
     * @param target Extension handle where send message.
     * @param msgid Extended rProtocol message id.
     */
	QueryMsg(unsigned short msgtype, unsigned long target, unsigned short msgid);
    /** Destructor.
     */
	virtual ~QueryMsg();
	
private:
	unsigned long _target;      ///< Target extension handle.
	unsigned short _msgid;      ///< Extended rProtocol message id
};


/**
 * @brief A message class to send an extended rProtocol message, RP_REQ_DELIVER_MESSAGE to rKernel extensions.
 * @author Sangyup Yi
 * @version 1.2
 * @since 2009/02/23
 * @see rTermMsg
 * @see RecvMsg
 * @see AckMsg
 * @see SendMsg
 * @see ReqMsg
 * @see QueryMsg
 * @see DeliverRequestMsg
 */
class RTERMEXPORT DeliverMsg : public QueryMsg
{
public:
    /** Constructor.
     * @param target Extension handle where send message.
     * @param msgid Extended rProtocol message id.
     */
	DeliverMsg(unsigned long target, unsigned short msgid);
    /** Destructor.
     */
	virtual ~DeliverMsg();
};


/**
 * @brief A message class to send an extended rProtocol message, RP_REQ_DELIVER_REQUEST to rKernel extensions.
 * @author Sangyup Yi
 * @version 1.2
 * @since 2009/02/23
 * @see rTermMsg
 * @see RecvMsg
 * @see AckMsg
 * @see SendMsg
 * @see ReqMsg
 * @see QueryMsg
 * @see DeliverRequestMsg
 */
class RTERMEXPORT DeliverRequestMsg : public QueryMsg
{
public:
    /** Constructor.
     * @param target Extension handle where send message.
     * @param msgid Extended rProtocol message id.
     */
	DeliverRequestMsg(unsigned long target, unsigned short msgid);
    /** Destructor.
     */
	virtual ~DeliverRequestMsg();

    /** It sends this request message and waits until rKernel extension responds with proper acknowledge message. The return code in AckMsg object is replaced with the value from rKernel extension when the return code is RP_RETCODE_DELIVER_REQUEST_SUCCESS.
     */
    virtual AckMsg* request();
};

#endif // __rTermMSG_H__
