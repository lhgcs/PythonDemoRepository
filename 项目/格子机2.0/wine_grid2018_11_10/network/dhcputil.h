#ifndef __DHCPUTIL_H__
#define __DHCPUTIL_H__

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* End of #ifdef __cplusplus */

#include <stdint.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
	   
#include "netutil.h"


typedef struct dhcp_info {
    uint32_t type;

    uint32_t ipaddr;						//ip地址
    uint32_t gateway;						//网关
    uint32_t prefixLength;			
	uint32_t mask;							//子网掩码

    uint32_t dns1;							//DNS1
    uint32_t dns2;							//DNS2

    uint32_t serveraddr;
    uint32_t lease;
}dhcp_info;


typedef void (*DhcpClientHandler)(NetDevice *, dhcp_info *,void *);

void DhcpHandler(NetDevice *Interface, dhcp_info *info, void *arg);


int DhcpClient(NetDevice *Interface, DhcpClientHandler Handler, void *arg);


char *ip2addr(in_addr_t addr);


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */

#endif
