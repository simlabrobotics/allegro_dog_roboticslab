#include <stdio.h>
#include <string.h>
#include "rDeviceAllegroDogCANAPI.h"

CANAPI_BEGIN

/************************************/
/* AllegroDog CAN utility functions */
/************************************/

void dump_message(int id, int len, unsigned char* data)
{
	printf("[%d]", id);
	for (int i=0; i<len; i++)
		printf(" %02x", data[i]);
	printf("\n");
}
void dump_message_str(int id, int len, unsigned char* data)
{
	printf("[%d]", id);
	for (int i=0; i<len; i++)
		printf(" %c", data[i]);
	printf("\n");
}

typedef union _buf32_type
{
   unsigned int uint32_value;
   int int32_value;
   float float_value;
   unsigned char arr[4];
} buf32_type;

int parse_position(unsigned char* data)
{
	buf32_type value;

	value.arr[0] = data[3];
	value.arr[1] = data[2];
	value.arr[2] = data[1];
	value.arr[3] = data[0];

	return value.int32_value;
}

/*****************************/
/* AllegroDog CAN device API */
/*****************************/
/// set communication frequency, number of channels and error count
int command_system_init(int bus, int id)
{
	unsigned long Txid = (unsigned long)(0xFF & id);
	static unsigned char data[8] = {'S', 't', '2', ',', '2', ',', '5', ';'};
	/// 2: communication frequency(2 msec)
	/// 2: number of channels
	/// 5: error count(this is using to fire an error in p(cyclic) mode.
	///    if cyclic p commands are not received by a motor drive '5' times sequencially, 
	///    it will report an error.
	return canSendMsg(bus, Txid, 8, data, TRUE);
}

/// activate p(cyclic) command mode
int command_activate_pmode(int bus, int id)
{
	unsigned long Txid = (unsigned long)(0xFF & id);
	static unsigned char data[7] = {'S', 'e', '0', '0', '3', '3', ';'};
	return canSendMsg(bus, Txid, 7, data, TRUE);
}

/// deactivate p(cyclic) command mode
int command_deactivate_pmode(int bus, int id)
{
	unsigned long Txid = (unsigned long)(0xFF & id);
	static unsigned char data[4] = {'P', 'P', 'E', '!'}; /// terminating semicolon is missing. no response will be got from the motor drive.
	return canSendMsg(bus, Txid, 4, data, TRUE);
}

/// set address of interest (encoders)
int command_set_respond_address(int bus, int id)
{
	unsigned long Txid;
	unsigned char data[8];
	int rtn;

	Txid = (unsigned long)(0xFF & id);

	data[0]='S';
	data[1]='r';
	data[2]='8'; /// address of the encoders
	data[3]='2';
	data[4]='9';
	data[5]='B';
	data[6]=',';
	data[7]='8';
	rtn = canSendMsg(bus, Txid, 8, data, TRUE);

	data[0]='2';
	data[1]='9';
	data[2]='A';
	data[3]=',';
	data[4]='8';
	data[5]='2';
	data[6]='A';
	data[7]='D';
	rtn = canSendMsg(bus, Txid, 8, data, TRUE);

	data[0]=',';
	data[1]='8';
	data[2]='2';
	data[3]='A';
	data[4]='C';
	data[5]=';';
	rtn = canSendMsg(bus, Txid, 6, data, TRUE);

	return rtn;
}

/// servo on motor
int command_motor_on(int bus, int id)
{
	unsigned long Txid = (unsigned long)(0xFF & id);
	unsigned char data[8];
	sprintf_s((char*)data, 8, "PE%04d;", id);
	return canSendMsg(bus, Txid, 7, data, TRUE);
}

/// servo off motor
int command_motor_off(int bus, int id)
{
	unsigned long Txid = (unsigned long)(0xFF & id);
	unsigned char data[8];
	sprintf_s((char*)data, 8, "PD%04d;", id);
	return canSendMsg(bus, Txid, 7, data, TRUE);
}

/// set mode of operation
int command_set_mode_of_operation(int bus, int id, int m1, int m2)
{
	unsigned long Txid = (unsigned long)(0xFF & id);
	unsigned char data[8];
	sprintf_s((char*)data, 8, "SM%02d%02d;", m1, m2);
	return canSendMsg(bus, Txid, 7, data, TRUE);
}

// set velocity in point to point position mode
int command_set_p2p_velocity(int bus, int id, int v1, int v2)
{
	unsigned long Txid = (unsigned long)(0xFF & id);
	unsigned char data[32];
	unsigned char* pdata = (unsigned char*)data;
	int rtn, len;
	sprintf_s((char*)data, 32, "SS%d,%d;", v1, v2);
	len =  strlen((char*)data);
	while (len > 0)
	{
		rtn = canSendMsg(bus, Txid, (len>8?8:len), pdata, TRUE);
		if (rtn) return rtn;
		len -= (len>8?8:len);
		pdata += 8;
	}
	return 0;
}

// set relative target position
int command_set_p2p_position_relative(int bus, int id, int mid, int pulse)
{
	if (mid != 1 && mid != 2) return -1;
	unsigned long Txid = (unsigned long)(0xFF & id);
	unsigned char data[32];
	unsigned char* pdata = (unsigned char*)data;
	int rtn, len;
	sprintf_s((char*)data, 32, "P%d%d;", mid, pulse);
	len =  strlen((char*)data);
	while (len > 0)
	{
		rtn = canSendMsg(bus, Txid, (len>8?8:len), pdata, TRUE);
		if (rtn) return rtn;
		len -= (len>8?8:len);
		pdata += 8;
	}
	return 0;
}

// set absolute target position
int command_set_p2p_position_absolute(int bus, int id, int pulse1, int pulse2)
{
	unsigned long Txid = (unsigned long)(0xFF & id);
	unsigned char data[32];
	unsigned char* pdata = (unsigned char*)data;
	int rtn, len;
	pulse1 += 5000000;
	pulse2 += 5000000;
	sprintf_s((char*)data, 32, "PA%d,%d;", pulse1, pulse2);
	len =  strlen((char*)data);
	while (len > 0)
	{
		rtn = canSendMsg(bus, Txid, (len>8?8:len), pdata, TRUE);
		if (rtn) return rtn;
		len -= (len>8?8:len);
		pdata += 8;
	}
	return 0;
}

// send home command
int command_home(int bus, int id, int enable1, int enable2, int dir1, int dir2, int offset1, int offset2)
{
	unsigned long Txid = (unsigned long)(0xFF & id);
	unsigned char data[32];
	unsigned char* pdata = (unsigned char*)data;
	int rtn, len;
	int enable = (enable1 ? 0x01 : 0) | (enable2 ? 0x02 : 0);
	int dir = (dir1 ? 1 : 0) + (dir2 ? 10 : 0);
	sprintf_s((char*)data, 32, "GJ%02d,%02d,%d,%d;", enable, dir, offset1, offset2);
	len =  strlen((char*)data);
	while (len > 0)
	{
		rtn = canSendMsg(bus, Txid, (len>8?8:len), pdata, TRUE);
		if (rtn) return rtn;
		len -= (len>8?8:len);
		pdata += 8;
	}
	return 0;
}

/// send heart beat command
int command_hb(int bus, int id)
{
	unsigned long Txid = (unsigned long)(0xFF & id);
	static unsigned char data[1] = {';'};
	return canSendMsg(bus, Txid, 1, data, TRUE);
}

/// reset current value of encoder as zero
int command_reset_encoder(int bus, int id)
{
	unsigned long Txid = (unsigned long)(0xFF & id);
	static unsigned char data[7] = {'Q', 'E', 'A', '5', '5', 'A', ';'};
	/// "QEA55A;": both channels
	/// "QEA5A5;": the first channel only
	/// "QE5A5A;": the second channel only
	return canSendMsg(bus, Txid, 7, data, TRUE);
}

/// query operation status
int command_query_status(int bus, int id)
{
	unsigned long Txid = (unsigned long)(0xFF & id);
	static unsigned char data[3] = {'Q', '1', ';'};
	return canSendMsg(bus, Txid, 3, data, TRUE);
}

// send control inputs
int write_control_cmds(int bus, int id, int cmd1, int cmd2)
{
	unsigned long Txid = (unsigned long)(0xFF & id);
	unsigned char data[8];
	cmd1 += 5000000;
	cmd2 += 5000000;
	data[0]='p';
	data[1]=(unsigned char)(0xFF & (cmd1 >> 16));
	data[2]=(unsigned char)(0xFF & (cmd1 >> 8));
	data[3]=(unsigned char)(0xFF & (cmd1));
	data[4]=(unsigned char)(0xFF & (cmd2 >> 16));
	data[5]=(unsigned char)(0xFF & (cmd2 >> 8));
	data[6]=(unsigned char)(0xFF & (cmd2));
	data[7]=(unsigned char)(~(data[1] + data[2] + data[3] + data[4] + data[5] + data[6]) & 0xFF);
	// EXAMPLE #1
	/*data[1]=(unsigned char)(0x4C);
	data[2]=(unsigned char)(0x4A);
	data[3]=(unsigned char)(0xEE);
	data[4]=(unsigned char)(0x4C);
	data[5]=(unsigned char)(0x49);
	data[6]=(unsigned char)(0x8C);
	data[7]=(unsigned char) (~(0x4C + 0x4A + 0xEE + 0x4C + 0x49 + 0x8C) & 0xFF);*/
	// EXAMPLE #2: ZERO CURRENT
	/*data[1]= (unsigned char)(0x4C);
	data[2]=(unsigned char)(0x4B);
	data[3]=(unsigned char)(0x40);
	data[4]=(unsigned char)(0x4C);
	data[5]=(unsigned char)(0x4B);
	data[6]=(unsigned char)(0x40);
	data[7]=(unsigned char) (~(0x4C + 0x4B + 0x40 + 0x4C + 0x4B + 0x40) & 0xFF);*/
	return canSendMsg(bus, Txid, 8, data, TRUE);
}

CANAPI_END
