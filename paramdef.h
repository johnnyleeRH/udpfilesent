#ifndef UDPFILESENT_PARAMDEF_H
#define UDPFILESENT_PARAMDEF_H

#include <stdint.h>

#define TIME_FORMAT_LEN 20
#define MAX_IP_LEN 16

struct inputparam {
    uint8_t multnum;
    uint8_t afiles;
    uint8_t atimes;
    uint8_t pausecmd;
    uint16_t mpsize;
    uint16_t sfiles;
    uint16_t multport;
    uint64_t mbandwidth;
    uint8_t starttime[21];
    uint8_t endtime[21];
    uint8_t multip[17];
    uint8_t smpath[1024];
};

struct calcparam {
    uint64_t sfilesize;
    uint64_t stotalpackets;
    uint64_t filesize;
    uint64_t fileptotal;
};

struct outputparam {
    uint16_t filenum;
    uint64_t smpnum;
    uint64_t rbandwidth;
    uint64_t mduringtime;
};

#endif
