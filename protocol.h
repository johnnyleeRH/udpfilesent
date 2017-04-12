#ifndef UDPFILESENT_PROTOCOL_H
#define UDPFILESENT_PROTOCOL_H

#include <stdint.h>

#pragma pack(push,1)

struct proto {
	uint8_t id;
	uint8_t round;
	uint16_t fileno;
	int  packetno;
	char reserved[2];
	uint16_t len; 
};

#pragma pack(pop)
#endif
