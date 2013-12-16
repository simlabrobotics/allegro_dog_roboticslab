/* Kai, Copyright 2004-2011 SimLab Co., Ltd. All rights reserved.
 *
 * This library is commercial and cannot be redistributed, and/or modified
 * WITHOUT ANY ALLOWANCE OR PERMISSION OF SimLab Co., LTD.
 */

/** 
@file kaiMsg.h
@brief Message class used in kai network library.
@author SimLab
@date 2010/12/10
*/
#ifndef __KAIMSG_H__
#define __KAIMSG_H__
#include "kaiDefinition.h"
#ifdef __GNUC__
#include <string.h>
#endif
#include <string>
#include <assert.h>

/**
 * kaiMsg class. 
 */
class KAIEXPORT kaiMsg
{
public:
	/** 
	 * kaiMsg default constructor.
	 * Before using kaiMsg instance, message buffer should be allocated by calling allocateMemory() explicitly.
	 */
	kaiMsg();
	/** 
	* kaiMsg destructor.
	* Message buffer is deallocated if it is necessary.
	*/
	virtual ~kaiMsg();
	/** 
	 * kaiMsg copy constructor.
	 * If the message buffer of 'msg' is allocated by itself, 
	 * the new instance also allocate its own memory.
	 * On the other hand, it just copy the buffer pointer from 'msg'.
	 * *param[in] msg
	 */
	kaiMsg(const kaiMsg& msg);

	/** 
	 * It prepares write/read operation by initializing several buffer pointers.
	 * It should be called before reading and writing data.
	 */
	void			begin();

	/** 
	 * It terminate write/read operation.
	 * When writing, it calculats the size of data buffer and set the value in the message header.
	 * It should be called after reading and writing data.
	 */
	void			end();
	
	/** 
	 * It allocates message buffer. This memory is released in the destructor.
	 * @param[in] size Size of memory to be allocated. Maximum size of message buffer is limited by kaiMAX_MSG_SIZE.
	 * @see kaiMAX_MSG_SIZE
	 */
	void			allocateMemory(kaiMsgSize size = kaiMAX_MSG_SIZE);
	
	/** 
	 * It sets 'buf' as buffer pointer.
	 * @param[in] buf Pointer to memory.
	 * @param[in] size Size of memory. Maximum size of message buffer is limited by kaiMAX_MSG_SIZE.
	 * @see kaiMAX_MSG_SIZE
	 */
	void			setBuffer(char* buf, kaiMsgSize size);

	/** 
	 * It gets message id.
	 * @return It returns message id.
	 */
	kaiMsgID		id() const;

	/** 
	 * It gets message size excluding message header.
	 * @return It returns message size.
	 * @see dataSize()
	 */
	kaiMsgSize		size() const;
	
	/** 
	 * It gets message flag.
	 * @return It returns message flag.
	 */
	kaiFlag			flag() const;

	/** 
	 * It sets message id.
	 * @param[in] id Message id.
	 */
	void			id(kaiMsgID id);

	/** 
	 * It sets message size manually.
	 * @param[in] size Message size excluding header size.
	 */
	void			size(kaiMsgSize size);

	/** 
	 * It sets message flag.
	 * @param[in] flag Message flag.
	 */
	void			flag(kaiFlag flag);

	/**
	 * It gets data size to be already read. Use only for reading not writing. 
	 * @return It returns the data size to be already read.
	 */
	kaiMsgSize		getReadSize() const;
	
	/**
	 * It retrieves the buffer pointer. It points the begining of the buffer including message header.
	 * @return It returns pointer to message buffer.
	 */
	const char*		buffer() const;
	
	/** 
	 * It retrieves the message size including message header.
	 * Don't confuse this function with kaiMsg::size(). kaiMsg::size() returns the message size excluding message header.
	 * @return It returns message size including message header.
	 * @see size()
	 */
	kaiMsgSize		dataSize() const;

	/**
	 * It reset message header and the position for read/write to the begining of message buffer.
	 */
	void			reset();

	/** 
	 * Overloaded operator for writing various types of data into this message.
	 * @code 
	 * kaiMsg msg;
	 * msg.allocate();
	 * msg.begin();
	 * char a='a'; short b=1; int c=2; double d=2.3;
	 * msg << a << b << c << d ;
	 * msg.end();
	 * @endcode
	 * @param[in] val Data to write.
	 * @return It returns the reference to this instance.
	 * @see operator<<(const char* const& str), operator<<(char* const& str), operator<<(const std::string& str)
	 */
    template<typename T>
	kaiMsg&		operator<<(T const& val)
	{
#ifdef _DEBUG
		assert(_bBeginCalled);
#endif
		size_t size = sizeof(T);
		if (_pos + size > _bufend)
			return *this;

		memcpy(_pos, &val, size);
		_pos += size;
		return *this;
	}

	/** 
	 * Overloaded operator for reading various types of data from this message.
	 * @param[out] val Data read from this message.
	 * @return It returns the reference to this instance.
	 * @see operator>>(std::string& str), operator>>(char *&buff)
	 */
	template<typename T>
	kaiMsg&		operator>>(T& val)
	{
#ifdef _DEBUG
		assert(_bBeginCalled);
#endif
		size_t size = sizeof(T);
		if (_pos + size > _bufend)
			return *this;

		memcpy(&val, _pos, size);
		_pos += size;

		return *this;
	}
	
	kaiMsg&		operator<< (const char* const& str);
	kaiMsg&		operator<< (char* const& str);
	kaiMsg&		operator<< (char* str);
	kaiMsg&		operator<< (const std::string& str);
	kaiMsg&		operator>> (std::string& str);
	kaiMsg&		operator>> (char*& buff);

	/** 
	 * Write data as much as size.
	 * @param[in] data Pointer to data buffer to write.
	 * @param[in] size Data size to be written.
	 */
	void writeData(const void* data, kaiMsgSize size);

	/** 
	 * Read data as much as size.
	 * @param[out] data Pointer to data buffer.
	 * @param[in] size Data size to be read.
	 */
	void readData(void* data, kaiMsgSize size);

	/**
	 * Write float.
	 * @param[in] val Float data.
	 * @return It returns the data size written.
	 */
	int writeFloat(float val);

	/**
	 * Read float.
	 * @param[out] val Float data.
	 * @return It returns the data size read.
	 */
	int readFloat(float& val);

	/**
	 * Write integer.
	 * @param[in] val Integer data.
	 * @return It returns the data size written.
	 */
	int writeInt(int val);

	/**
	 * Read integer.
	 * @param[out] val Integer data.
	 * @return It returns the data size read.
	 */
	int readInt(int& val);

	/**
	 * Write short integer.
	 * @param[in] val Short integer data.
	 * @return It returns the data size written.
	 */
	int writeShort(short val);

	/**
	 * Read short integer.
	 * @param[out] val Short integer data.
	 * @return It returns the data size read.
	 */
	int readShort(short& val);

	/**
	 * Write byte.
	 * @param[in] val Byte data.
	 * @return It returns the data size written.
	 */
	int writeByte(unsigned char val);

	/**
	 * Read one byte from the message buffer.
	 * @param[out] val Byte data.
	 * @return It returns the data size read.
	 */
	int readByte(unsigned char& val);

protected:
	kaiHeader _header;	///< Message header.
private:
	char* _buf;			///< Pointer to message buffer. Message buffer consists of kaiHeader and actual data buffer.
	char* _pos;			///< Current read/write position in the message buffer.
	char* _bufend;		///< Pointer to the end of message buffer.
	bool _bAllocated;	///< Whether message buffer is allocated by itself or not.
#ifdef _DEBUG
	bool _bBeginCalled;
	bool _bEndCalled;
#endif
};

#endif
