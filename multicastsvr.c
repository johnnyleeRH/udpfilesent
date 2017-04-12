#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>

#include "log.h"

static int udpfd = -1;
static struct sockaddr_in destaddr;

int InitialMulticastSvr(char* addr, unsigned short port) {
    udpfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (-1 == udpfd) {
        ERROR("create multicast socket failed error %d.", errno);
        return -1;
    }
    INFO("multicast addr %s, port %u.", addr, port);
    memset(&destaddr, 0, sizeof(destaddr));
    destaddr.sin_family = AF_INET;
    destaddr.sin_addr.s_addr = inet_addr(addr);
    destaddr.sin_port = htons(port);
    return 0;
}

void SendMsg(char *msg, int len) {
    ssize_t sentsize = sendto(udpfd, msg, len, 0, (struct sockaddr *)&destaddr, sizeof(destaddr));
    if (-1 == sentsize)
        ERROR("send msg failed %d.", errno);
    else if (sentsize != len)
        INFO("sent size %d not equal to need size %d.", sentsize, len);
}

void CloseMulticastSvr() {
    close(udpfd);
}