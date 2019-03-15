#ifndef __NETUTIL_H__
#define __NETUTIL_H__

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* End of #ifdef __cplusplus */


//String  Len
#define MAX_NET_CARD_NAME_LEN					100
#define MAX_IP_MAC_ADDR_LEN						6
#define MAX_IP_ADDR_LEN							4

#define MAX_IP_ADDR_STRING_LEN					100
#define MAX_IP_MASK_STRING_LEN					100

#define IP_ADDR_STRING_LEN						20



/****************ICMP*****************/
#define PACKET_SIZE     256
#define MAX_WAIT_TIME   5
#define MAX_NO_PACKETS 	4


#define DNS_CONFIG_FILE									"/etc/resolv.conf"
#define NET_DEVICE_NAME									"eth0"


/* net status */
typedef enum{
    NET_CONNECT_FAIL,
    NET_CONNECT_SUCCESS,
    NET_CONNECT_LIMITED,
}NetConnectStatus;


///* netcard */
typedef enum{
    NETCARD_NO,
    NETCARD_ETH,
    NETCARD_WIFI,
    NETCARD_4G,
}NetCardType;

/*[%02x-%02x-%02x-%02x-%02x-%02x]
  mac[0] mac[1] mac[2] mac[3] mac[4] mac[5]*/
typedef struct{
	unsigned char mac[6];
}NetMacAddress;


typedef struct{
	unsigned int addr[4];
}NetIPAddress;





typedef struct
{
//	NetCard    			NetCardType;
    char  				NetCardName[MAX_NET_CARD_NAME_LEN];
	int 				NetSocketFd;
    NetConnectStatus	ConnectStatus;
}NetDevice;

//int ping(char *ipaddr, int timeout,int sendcount);


int Net_isValidPort(unsigned int Port);
int Net_isValidIpString(char *IpString, int Len);


int Net_isExistNetCable(char *NetTypeName);
int Net_CheckNetWorkIsAvailable(NetDevice *Device);


//int Net_InitNetCard(NetDevice *Device, NetCard Type, char *Name);
int Net_InitNetCard(NetDevice *Device, char *Name);
int Net_DelInitNetCard(NetDevice *Device);



//MAC
int Net_getMacAddr(NetDevice *Device, NetMacAddress *MacAddr);

//IP Address
int Net_getIpAddrString(NetDevice *Device, char *AddrString , int Len);
int Net_setIpAddrString(NetDevice *Device, char *AddrString , int Len);

//SubNetMask
int Net_getSubNetMaskString(NetDevice *Device , char *AddrString , int Len);
int Net_setSubNetMaskString(NetDevice *Device , char *AddrString , int Len);

//DNS
int Net_getDNSString(char *NetGateString, int Len);
int Net_setDNSString(char *NetGateString, int Len);

//DefaultGateWay
int Net_getDefaultGateWay(char *NetGateString, int Len);
int Net_setDefaultGateWay(char *NetGateString, int Len);

//int Net_paramsConfig(NetDevice *Device, NetCard type, char *NetDeviceName, char *IpAddr, char *SubNetMask, char *NetGateWay, char *DNS);
int Net_PingIp(char *IpString);


int ifc_get_ifindex(NetDevice *Device, int *if_indexp);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */



#endif
