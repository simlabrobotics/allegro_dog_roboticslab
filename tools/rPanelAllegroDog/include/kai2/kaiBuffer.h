/* Kai, Copyright 2004-2011 SimLab Co., Ltd. All rights reserved.
 *
 * This library is commercial and cannot be redistributed, and/or modified
 * WITHOUT ANY ALLOWANCE OR PERMISSION OF SimLab Co., LTD.
 */

/** 
@file kaiBuffer.h
@brief Ring buffer class.
@author SimLab
@date 2008/2/26
*/
#ifndef __KAIBUFFER_H
#define __KAIBUFFER_H

class KAIEXPORT kaiBuffer
{
public:
	/** 
	 * kaiBuffer constructor.
	 * Default constructor, do nothing.
	 */
	kaiBuffer();

	/** 
	* kaiBuffer destructor.
	* Default destructor, do nothing.
	*/
	virtual ~kaiBuffer();
	
	/** 
	 * Initialize buffer. Allocate memory for buffer if it is necessary.
 	 * @param buffersize Total buffer size to allocate.
 	 * @param maxdatasize Maximum data size to be handled per one time .
	 */
	void init(unsigned buffersize = kaiBUFFER_SIZE, unsigned maxdatasize = kaiMAX_MSG_SIZE);

	/** 
	 * Refresh member variables.
	 */
	void refresh();

	/**
	 * Do defragmentation if it is necessary. It assembles fragmented messages into a sequencial data buffer.
	 * Buffer fragmentation happens at the boundary of this ring buffer.
	 * @param size Size of data block to defragment. It can be up to _maxDataSize. If it is greater than _maxDataSize, buffer overun can happen.
	 */
	void defrag(unsigned size = kaiMAX_MSG_SIZE);

	/** 
	 * Get data size stored in buffer.
	 * @return It returns the size of data stored in buffer.
	 */
	unsigned getDataSize();

	/**
	 * Get maximum data size it can handle at once.
	 * @return It returns maximum data size.
	 */
	unsigned getMaxDataSize();
	
	/** 
	 * Get pointer to buffer for putting data.
	 * @return It returns the pointer to buffer where data is put.
     */
	char* bufferIn();

	/** 
	 * Get pointer to buffer for getting data.
	 * @return It returns the point to buffer where data is get.
	 */
	const char* bufferOut() const;
	
	/** 
	 * Put data to buffer as much as size specified.
	 * @param[in] Data pointer to data to put.
	 * @param size Data size to put.
	 */
	void put(const void* data, unsigned short size);

	/**
	 * Increase data-in buffer pointer.
	 * @param size Size of data to put. 
	 */
	void put(unsigned short size);

	/**
	 * Get data from receive buffer.
	 * Make it sure that 'size' is not greater than the real data size in this object.
	 * @param[out] data Pointer to data buffer to store received data.
	 * @param[in] size Data size to get.
	 */
	void get(void* data, unsigned short size);

	/**
	 * Pop data out from receive buffer.
	 * @param size Data size to pop.
	 */
	void pop(unsigned short size);

private:
	kaiBuffer(const kaiBuffer& q){}

	char* _buffer;			///< Buffer.
	char* _bufferEnd;		///< End of buffer. The actual data buffer exceeds this buffer end limit to prevent buffer overflow.
	char* _bufferEndValid;	///< Real end of buffer where valid data is stored at this moment. Before this limit data stored in this buffer is valid.

	char* _bufferInPtr;		///< Where to put data.
	char* _bufferOutPtr;	///< Where to get data.

	unsigned _bufferSize;	///< Size of buffer.
	unsigned _maxDataSize; ///< Maximum size of message it can handle at once.
};

#endif
