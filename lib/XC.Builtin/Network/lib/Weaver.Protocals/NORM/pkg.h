#pragma once
#include "../__common/DSP.h"

typedef struct{
	u8 version : 4;
	u8 type : 4;
	u8 service_command;
	u16 sequence;
	u32 source_id;
}NORM_common_header;

typedef struct{
	u16 instance_id;
	u8 grtt;
	u8 backoff : 4;
	u8 gsize : 4;
	u8 flags;
	u8 fec_id;
	u16 object_transport_id;
}NORM_INFO_header;

typedef struct{
	NORM_INFO_header info;
	u32 fec_payload_id;
}NORM_DATA_header;

typedef struct{
	NORM_INFO_header info;
	u8 sub_type;
}NORM_CTRL_header;

typedef struct{
	u32 server_id;
	u16 instance_id;
	u16 reserved;
	u32 grtt_response_sec;
	u32 grtt_response_usec;
}NORM_NACK_header;

typedef struct{
	u32 server_id;
	u16 instance_id;
	u8 ack_type;
	u8 ack_id;
	u32 grtt_response_sec;
	u32 grtt_response_usec;
}NORM_ACK_header;
