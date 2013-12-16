#ifndef __RDEVICEALLEGRODOGCANAPI_H__
#define __RDEVICEALLEGRODOGCANAPI_H__

#include "canAPI.h"

CANAPI_BEGIN

/************************************/
/* AllegroDog CAN utility functions */
/************************************/
int parse_position(unsigned char* data);
void dump_message(int id, int len, unsigned char* data);
void dump_message_str(int id, int len, unsigned char* data);

/************************************/
/* AllegroDog CAN device API        */
/************************************/
int command_system_init(int bus, int id);
int command_set_respond_address(int bus, int id);
int command_activate_pmode(int bus, int id);
int command_deactivate_pmode(int bus, int id);
int command_motor_on(int bus, int id);
int command_motor_off(int bus, int id);
int command_set_mode_of_operation(int bus, int id, int m1, int m2);
int command_set_p2p_velocity(int bus, int id, int v1, int v2);
int command_set_p2p_position_relative(int bus, int id, int mid, int pulse);
int command_set_p2p_position_absolute(int bus, int id, int pulse1, int pulse2);
int command_home(int bus, int id, int enable1, int enable2, int dir1, int dir2, int offset1, int offset2);
int command_hb(int bus, int id);
int command_reset_encoder(int bus, int id);
int command_query_status(int bus, int id);
int write_control_cmds(int bus, int id, int cmd1, int cmd2);

CANAPI_END

#endif
