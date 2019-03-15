#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#include <netinet/in.h>
#include <net/if.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <netinet/ip.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>

//#include "applog.h"
#include "netutil.h"
//#include "gobal.h"


int Net_isValidPort(unsigned int Port)
{
	int isRet = 0;

	if( Port > 0 && Port < 65536)
	{
        isRet = -1;    
    }

    return isRet;
}		


bool Net_isValidIpAddr(NetIPAddress *IpAddr)
{

	if ( ( NULL == IpAddr ))
	{
			return false;
	}
	
	if ( ( IpAddr->addr[0] > 255 ) || ( IpAddr->addr[1] > 255 )||
			 ( IpAddr->addr[2] > 255 ) || ( IpAddr->addr[0] > 255 ) )
	{
		return false;
	}
	
		return true;
}



int Net_isValidIpString(char *IpString, int Len)
{
	int ret = 0;
	struct in_addr inp;
	
	if(IpString == NULL || Len <=0 )
	{
		return -1;
	}

	ret = inet_aton(IpString, &inp);
	if (0 == ret)
	{
		return -1;
	}
	
	return 0;
}



int Net_isValidNetMask(char *NetMaskString, int Len)
{
	unsigned int b = 0, i, n[4];
	int ret = -1;
	
	if(NetMaskString == NULL || Len <=0 )
	{
		return -1;
	}
	
    if((ret = Net_isValidIpString(NetMaskString, Len) )< 0)
    {
    	return ret ;
    }
	
   	sscanf(NetMaskString, "%u.%u.%u.%u", &n[3], &n[2], &n[1], &n[0]);
    for(i = 0; i < 4; ++i)
    	b += n[i] << (i * 8);
    b = ~b + 1;
    if((b & (b - 1)) != 0)
    {
       return -1;
    }


    return 0;

}



int Net_IpStringToAddr(char *IpString, NetIPAddress *IpAddr, int StringLen)
{
	int ret = 0;
	
    if(IpString == NULL || IpAddr == NULL || StringLen == 0)
	{
		return -1;
	}

	ret = sscanf(IpString,  "%u.%u.%u.%u", &(IpAddr->addr[3]),  &(IpAddr->addr[2]),  &(IpAddr->addr[1]),  &(IpAddr->addr[0]));
	if(ret < 0)
	{
		//log_printf(LOG_LEVEL_ERROR, LOG_MODULE_NET, "(%s:%d):Ip string to addr failed\n", __func__, __LINE__);
		return -1;
	}

	return 0;
}



int Net_isExistNetCable(char *NetTypeName)
{
	struct ifreq ifr;
	int sock = 0;
//	int ret = 0;

    if(NetTypeName == NULL )
    {
        //log_printf(LOG_LEVEL_ERROR, LOG_MODULE_NET, "(%s:%d): Incorrect params!\n", __func__, __LINE__);
        return -1;
    }

    sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock < 0)
    {
       //log_printf(LOG_LEVEL_DEBUG, LOG_MODULE_NET, "(%s:%d):Create socket failed!errno=%d", __func__, __LINE__, errno);
       return  -1;
    }

    strcpy(ifr.ifr_name, NetTypeName);
	
    if(ioctl(sock, SIOCGIFFLAGS, &ifr) < 0)
    {
    	//log_printf(LOG_LEVEL_ERROR, LOG_MODULE_NET, "(%s:%d):Get net cable iotcl failed!errno=%s\n", __func__, __LINE__, strerror(errno));
		return -1;
    }
	
	if(!(ifr.ifr_flags & IFF_RUNNING))
	{
    	//log_printf(LOG_LEVEL_DEBUG, LOG_MODULE_NET, "(%s:%d):No exist net cable!\n", __func__, __LINE__);

		return -1;
	}
	

    close(sock);
	
	return 0;
}





//int Net_InitNetCard(NetDevice *Device, NetCard Type, char *Name)
//{
////	int isExistNetCable = 0;
	
//	if(Device == NULL || Name == NULL)
//	{
//		return -1;
//	}

//	Device->NetCardType = Type;
//	strcpy(Device->NetCardName, Name);
//	Device->NetSocketFd = -1;
	
//	//检查是否存在网线
//	//isExistNetCable = Net_isExistNetCable(Device);
	
//	int sock = socket(AF_INET, SOCK_DGRAM, 0);
//    if (sock < 0)
//    {
//       //log_printf(LOG_LEVEL_DEBUG, LOG_MODULE_NET, "(%s:%d):Create socket failed!errno=%d", __func__, __LINE__, errno);
//       return  -1;
//    }
	
//	Device->NetSocketFd = sock;
	
//	return 0;
//}

int Net_InitNetCard(NetDevice *Device, char *Name)
{
//	int isExistNetCable = 0;

    if(Device == NULL || Name == NULL)
    {
         return -1;
    }

    strcpy(Device->NetCardName, Name);
    Device->NetSocketFd = -1;

    //检查是否存在网线
    //isExistNetCable = Net_isExistNetCable(Device);

    int sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock < 0)
    {
       //log_printf(LOG_LEVEL_DEBUG, LOG_MODULE_NET, "(%s:%d):Create socket failed!errno=%d", __func__, __LINE__, errno);
       return  -1;
    }

    Device->NetSocketFd = sock;

    return 0;
}


int Net_DelInitNetCard(NetDevice *Device)
{
	Device->ConnectStatus = NET_CONNECT_FAIL;
	close(Device->NetSocketFd);
	
	return 0;
}





int Net_getMacAddr(NetDevice *Device, NetMacAddress *MacAddr)
{
	struct ifreq ifr;
	int sock = 0;
//	int ret = 0;

	if(Device == NULL || MacAddr == NULL || Device->NetSocketFd == -1)
	{
		//log_printf(LOG_LEVEL_ERROR, LOG_MODULE_NET, "(%s:%d): Incorrect params!\n", __func__, __LINE__);
		return -1;
	}

	sock = Device->NetSocketFd;
	strcpy(ifr.ifr_name, Device->NetCardName);
	
    if(ioctl(sock, SIOCGIFHWADDR, &ifr) < 0)
    {
    	//log_printf(LOG_LEVEL_ERROR, LOG_MODULE_NET, "(%s:%d):Get Local ip iotcl failed!errno=%s\n", __func__, __LINE__, strerror(errno));
		return -1;
    }
	
	memcpy((void *)MacAddr->mac, (void *)ifr.ifr_hwaddr.sa_data, sizeof(MacAddr->mac));

	printf("[%02x-%02x-%02x-%02x-%02x-%02x]\n", MacAddr->mac[0], MacAddr->mac[1], MacAddr->mac[2], MacAddr->mac[3], MacAddr->mac[4], MacAddr->mac[5]);
	
	return 0;
}

int ifc_get_ifindex(NetDevice *Device, int *if_indexp)
{
    int r;
    struct ifreq ifr;
	int sock = 0;

	memset(&ifr, 0, sizeof(struct ifreq));
	sock = Device->NetSocketFd;
	strcpy(ifr.ifr_name, Device->NetCardName);
	 
    r = ioctl(sock, SIOCGIFINDEX, &ifr);
    if(r < 0) 
	return -1;

    *if_indexp = ifr.ifr_ifindex;
    return 0;
}


int Net_getIpAddrString(NetDevice *Device, char *AddrString , int Len)
{
	struct ifreq ifr;
	struct sockaddr_in *ip_addr = NULL;
	char *ip_string = NULL;
	int sock = 0;
	int ret = 0;

	if(Device == NULL || AddrString == NULL || Device->NetSocketFd == -1)
	{
		//log_printf(LOG_LEVEL_ERROR, LOG_MODULE_NET, "(%s:%d): Incorrect params!\n", __func__, __LINE__);
		return -1;
	}

	sock = Device->NetSocketFd;
	strcpy(ifr.ifr_name, Device->NetCardName);
	
    if(ioctl(sock, SIOCGIFADDR, &ifr) < 0)
    {
    	//log_printf(LOG_LEVEL_ERROR, LOG_MODULE_NET, "(%s:%d):Get Local ip iotcl failed!errno=%s\n", __func__, __LINE__, strerror(errno));
		return -1;
    }
	
	ip_addr = (struct sockaddr_in *)(&(ifr.ifr_addr));
	ip_string = (char *)inet_ntoa(ip_addr->sin_addr);
	
	ret = strlen(strncpy(AddrString, ip_string, Len));
	if(ret == 0)
	{
		//log_printf(LOG_LEVEL_ERROR, LOG_MODULE_NET, "(%s:%d):Copy ip  failed!\n", __func__, __LINE__);
		return -1;
	}

	return 0;
}



int Net_setIpAddrString(NetDevice *Device, char *AddrString , int Len)
{
	struct ifreq ifr;
	struct sockaddr_in net_ip;
//	NetIPAddress IpAddr;
	int ret = 0;
	int sock = 0;

	if(Device == NULL || AddrString == NULL || Device->NetSocketFd == -1)
	{
		//log_printf(LOG_LEVEL_ERROR, LOG_MODULE_NET, "(%s:%d): Incorrect params!\n", __func__, __LINE__);
		return -1;
	}

	//检查是否合法IP地址
	if((ret = Net_isValidIpString(AddrString, Len)) < 0)
	{
		//log_printf(LOG_LEVEL_ERROR, LOG_MODULE_NET, "(%s:%d): Incorrect Ip Address!\n", __func__, __LINE__);
		return ret;
	}
	
	sock = Device->NetSocketFd;
	strcpy(ifr.ifr_name, Device->NetCardName);

	//初始化struct sockaddr_in
	memset(&net_ip, 0 , sizeof(struct sockaddr));
	net_ip.sin_family = AF_INET;
	inet_aton(AddrString, &(net_ip.sin_addr));
	memcpy((void *)&ifr.ifr_addr, (void *)&net_ip, sizeof(struct sockaddr));

	
	//设置激活标志
	//ifr.ifr_flags |= IFF_UP|IFF_RUNNING;
	//设置地址
	if(ioctl(sock, SIOCSIFADDR, &ifr) < 0)
    {
    	//log_printf(LOG_LEVEL_ERROR, LOG_MODULE_NET, "(%s:%d):Set Local ip iotcl failed! errno=%s\n", __func__, __LINE__, strerror(errno));
		return -1;
    }

	/*
	snprintf(cmd, sizeof(cmd)-1, "ifconfig %s down", Device->NetCardName);
	system(cmd);

	snprintf(cmd, sizeof(cmd)-1, "ifconfig %s up", Device->NetCardName);
	system(cmd);
	*/

	return 0;
}



int Net_getSubNetMaskString(NetDevice *Device , char *AddrString , int Len)
{
	struct ifreq ifr;
	struct sockaddr_in *ip_addr = NULL;
	char *ip_string = NULL;
	int sock = 0;
	int ret = 0;
	
	
	if(Device == NULL || AddrString == NULL || Device->NetSocketFd == -1)
	{
		//log_printf(LOG_LEVEL_ERROR, LOG_MODULE_NET, "(%s:%d): Incorrect params!\n", __func__, __LINE__);
		return -1;
	}

	sock = Device->NetSocketFd;
	strcpy(ifr.ifr_name, Device->NetCardName);
	
	if(ioctl(sock, SIOCGIFNETMASK, &ifr) < 0)
    {
    	//log_printf(LOG_LEVEL_ERROR, LOG_MODULE_NET, "(%s:%d):Get subnet mask iotcl failed!errno=%s\n", __func__, __LINE__, strerror(errno));
		return -1;
    }


	ip_addr = (struct sockaddr_in *)(&(ifr.ifr_addr));
	ip_string = (char *)inet_ntoa(ip_addr->sin_addr);


	ret = strlen(strncpy(AddrString, ip_string, Len));
	if(ret == 0)
	{
		//log_printf(LOG_LEVEL_ERROR, LOG_MODULE_NET, "(%s:%d):Copy ip  failed!\n", __func__, __LINE__);
		return -1;
	}
	
	return 0;

}




int Net_setSubNetMaskString(NetDevice *Device , char *AddrString , int Len)
{
	struct ifreq ifr;
	struct sockaddr_in net_mask ;
//	char *ip_string = NULL;
	int sock = 0;
	int ret = 0;

	
	if(Device == NULL || AddrString == NULL || Device->NetSocketFd == -1)
	{
		return -1;
	}

	//检查是否合法掩码地址
	if((ret = Net_isValidNetMask(AddrString, Len)) < 0)
	{
		//log_printf(LOG_LEVEL_ERROR, LOG_MODULE_NET, "(%s:%d): Incorrect SubNetMask Address!\n", __func__, __LINE__);
		return ret;
	}	
		
	sock = Device->NetSocketFd;
	strcpy(ifr.ifr_name, Device->NetCardName);
		
	//初始化struct sockaddr_in

	//初始化struct sockaddr_in
	memset(&net_mask, 0 , sizeof(struct sockaddr));
	net_mask.sin_family = AF_INET;
	inet_aton(AddrString, &(net_mask.sin_addr));
	memcpy((void *)&ifr.ifr_addr, (void *)&net_mask, sizeof(struct sockaddr));
		
	
	//设置激活标志
	//ifr.ifr_flags |= IFF_UP|IFF_RUNNING;
	
		
	//设置地址
	if(ioctl(sock, SIOCSIFNETMASK, &ifr) < 0)
	{
		//log_printf(LOG_LEVEL_ERROR, LOG_MODULE_NET, "(%s:%d):Set Local ip iotcl failed!errno=%s \n", __func__, __LINE__, strerror(errno));
		return -1;
	}

	
	return 0;

}




int Net_getDNSString(char *DNSString, int Len)
{
	FILE * fp = NULL;
    int ret = 0;
	unsigned char Line[200] = {0};
	unsigned int DNSValue[200] ={0};
//	unsigned char *CurStringPos = NULL;
	char *CurStringPos = NULL;
	int isFindNameServer = 0;
	int LineNum = 0;
		
    if(DNSString == NULL || Len == 0)
    {
        return -1;
    }


	fp = fopen(DNS_CONFIG_FILE, "r+");
    if(NULL == fp)
    {
        //log_printf(LOG_LEVEL_ERROR, LOG_MODULE_NET, "(%s:%d):DNS file [%s] open fail!\n", __func__, __LINE__, DNS_CONFIG_FILE);
        return -1;
    }


	// 读取DNS配置行 
    while(fgets((char *)Line, sizeof(Line), fp))
    {
        LineNum++;
		
		//跳过注释行
		if(Line[0] == '#')
		{	
			continue;
		}	
      //  if((CurStringPos = (unsigned char *)strstr((char *)Line, "nameserver"))!= NULL)
         if((CurStringPos = (char *)strstr((char *)Line, "nameserver"))!= NULL)
        {
            isFindNameServer = 1;
          //log_printf(LOG_LEVEL_DEBUG, LOG_MODULE_NET, "[%s]%d: Find DNS line!\n", __func__, __LINE__); 
            break;
        }
        
        if(LineNum > 100)
        {
  //        log_printf(LOG_LEVEL_ERROR, LOG_MODULE_NET, "[%s]%d: Too many invalid information!\n", __func__, __LINE__); 
            break;
        }
        
    }

	if(isFindNameServer == 0)
	{
		fclose(fp);
		return -1;
	}

	//CurStringPos = (unsigned char *)strstr((char *)CurStringPos, " ");
	CurStringPos = (char *)strstr((char *)CurStringPos, " ");
	ret = sscanf((char *)CurStringPos, "%u.%u.%u.%u", &DNSValue[0], &DNSValue[1], &DNSValue[2], &DNSValue[3] );
	snprintf((char *)DNSString, Len, "%u.%u.%u.%u", DNSValue[0], DNSValue[1], DNSValue[2] ,DNSValue[3]);
	//log_printf(LOG_LEVEL_DEBUG, LOG_MODULE_NET, "[%s]%d: Find DNS info:[%s]!\n", __func__, __LINE__, DNSString);
	
	fclose(fp);
	
	return 0;
}



/*
int Net_setDNSString(char *DNSString, int Len)
{

	FILE * fp = NULL;
//	unsigned char Line[200] = {0};
//	unsigned char DNSModifyString[200] ={0};
//	unsigned char *CurStringPos = NULL;

	char Line[200] = {0};
    char DNSModifyString[200] ={0};
	char *CurStringPos = NULL;
	int isFindNameServer = 0;
	int LineNum = 0;
	int ret = 0;
	int dns_pos = 0;
	int offset = 0;
	unsigned int line_len = 0;
	struct stat dns_stat;
	int trunc_size = 0;

	//检查是否合法IP地址
	if((ret = Net_isValidIpString(DNSString, Len)) < 0)
	{
		log_printf(LOG_LEVEL_ERROR, LOG_MODULE_NET, "(%s:%d): Incorrect Ip Address!\n", __func__, __LINE__);
		return ret;
	}

	
	fp = fopen(DNS_CONFIG_FILE, "r+");
    if(NULL == fp)
    {
      	log_printf(LOG_LEVEL_ERROR, LOG_MODULE_NET, "(%s:%d):DNS file [%s] open fail!\n", __func__, __LINE__, DNS_CONFIG_FILE);
        return ERRCODE_NET_DNS_NOT_EXIST;
    }
	
	//获取DNS配置文件大小
	stat(DNS_CONFIG_FILE, &dns_stat);

	// 读取DNS配置行 
    while(fgets((char *)Line, sizeof(Line), fp))
    {
        LineNum++;
			
		//跳过注释行
		if(Line[0] == '#')
		{	
			continue;
		}
        if((CurStringPos = (char *)strstr((char *)Line, "nameserver"))!= NULL)
        {
            isFindNameServer = 1;
			line_len = strlen(Line);
	        log_printf(LOG_LEVEL_DEBUG, LOG_MODULE_NET, "(%s:%d): Find DNS line!\n", __func__, __LINE__); 
            break;
        }
        
        if(LineNum > 100)
        {
            log_printf(LOG_LEVEL_ERROR, LOG_MODULE_NET, "(%s:%d): Too many invalid information!\n", __func__, __LINE__); 
            break;
        }
        
    }

	if(isFindNameServer == 0)
	{
		fprintf(fp, "%s %s\n", "nameserver", DNSString);
	}
	else
	{
		CurStringPos = (char *)strstr((char *)CurStringPos, " ");
		//找不到nameserver后面的空格，直接加入地址
		if(CurStringPos == NULL)
		{
			snprintf(DNSModifyString, sizeof(DNSModifyString), " %s", DNSString);
			trunc_size = -1;
		}
		else
		{
			CurStringPos += 1;
			dns_pos = CurStringPos - Line;
			offset = line_len- dns_pos;
			
			//截取之前的IP地址
			truncate(DNS_CONFIG_FILE, (dns_stat.st_size - offset));
			snprintf(DNSModifyString, sizeof(DNSModifyString), "%s", DNSString);
		}
		
		fclose(fp);

		//重新写入IP地址
		fp = fopen(DNS_CONFIG_FILE, "r+");
		fseek(fp, trunc_size, SEEK_END);
		fprintf(fp,"%s", DNSModifyString);

	}	
	fclose(fp);
	
	return ERRCODE_OK;

}
*/

int Net_setDNSString(char *DNSString, int Len)
{
	FILE * fp = NULL;
	char DNSModifyString[200] ={0};

    if(DNSString == NULL || Len == 0)
    {
        return -1;
    }

	truncate(DNS_CONFIG_FILE, 0);
	snprintf(DNSModifyString, sizeof(DNSModifyString), "%s %s", "nameserver",DNSString);

	fp = fopen(DNS_CONFIG_FILE, "r+");
	if(fp == NULL)
	{
        printf("(%s:%d):DNS file [%s] open fail!\n", __func__, __LINE__, DNS_CONFIG_FILE);
		return -1;
	}
	
	fprintf(fp,"%s", DNSModifyString);
	
	fclose(fp);

	
	return 0;
}



int Net_getDefaultGateWay(char *NetGateString, int Len)
{
	FILE *fp;
	char buf[512];
	char cmd[128];
	char gateway[30];
	char *tmp;
    int isFindGW = 0;

    if(NetGateString == NULL || Len == 0)
    {
        return -1;
    }

	strcpy(cmd, "ip route");
	fp = popen(cmd, "r");
	if(NULL == fp)
	{
		//log_printf(LOG_LEVEL_DEBUG, LOG_MODULE_NET, "(%s:%d): popen error", __func__, __LINE__);
		return -1;
	}
    
	while(fgets(buf, sizeof(buf), fp) != NULL)
	{
		tmp =buf;
		while(*tmp && isspace(*tmp))
			++ tmp;
		if(0 == strncmp(tmp, "default", strlen("default")))
        {
            isFindGW = 1;
			break;
        }
	}
	sscanf(buf, "%*s%*s%s", gateway);	
	pclose(fp);

    if(1 == isFindGW)
    {
        snprintf((char *)NetGateString, Len, "%s", gateway);
    }
    else
    {
        snprintf((char *)NetGateString, Len, "%s", "0.0.0.0");
		
		//log_printf(LOG_LEVEL_ERROR, LOG_MODULE_NET, "(%s:%d):Get default gateway failed\n", __func__, __LINE__);
		
		return -1;
    }

    //log_printf(LOG_LEVEL_DEBUG, LOG_MODULE_NET, "(%s:%d): default gateway:[%s]\n", __func__, __LINE__, NetGateString);
	
	return 0;
}



int Net_setDefaultGateWay(char *NetGateString, int Len)
{
    char cmd[128]        = { 0 };
    char DefaultGW[MAX_IP_ADDR_STRING_LEN] = { 0 };
	int ret = 0;

    if(NetGateString == NULL || Len == 0)
    {
        return -1;
    }

	//检查是否合法IP地址
	if((ret = Net_isValidIpString(NetGateString, Len)) < 0)
	{
		//log_printf(LOG_LEVEL_ERROR, LOG_MODULE_NET, "(%s:%d): Incorrect Ip Address!\n", __func__, __LINE__);
		return ret;
	}
	
	ret =Net_getDefaultGateWay(DefaultGW, MAX_IP_ADDR_STRING_LEN);
	if(ret == 0)
	{
		if(0 == strcmp((char *)DefaultGW, (char *)NetGateString))
		{
			//log_printf(LOG_LEVEL_DEBUG, LOG_MODULE_NET, "(%s:%d): Is the same gateway!\n", __func__, __LINE__);
			return 0;
		}
				
		printf("ret :%d\n",ret);
		snprintf( (char*)cmd, sizeof(cmd), "route del default" );
    	system( (char*)cmd );
	}
	
    snprintf( (char*)cmd, sizeof(cmd), "route add default gw %s", NetGateString );
    system( (char*)cmd );


    /* 再次刷新网关, 查看是否配置成功 */
    Net_getDefaultGateWay(DefaultGW, MAX_IP_ADDR_STRING_LEN);
    if(0 == strcmp((char *)DefaultGW, (char *)NetGateString))
    {
        //log_printf(LOG_LEVEL_DEBUG, LOG_MODULE_NET, "(%s:%d): Modify default gw [%s] successfully !\n", __func__, __LINE__, (char *)NetGateString);
        return 0;
    }
    else
    {
        //log_printf(LOG_LEVEL_ERROR, LOG_MODULE_NET, "(%s:%d): Modify default gw [%s] failure!\n", __func__, __LINE__, (char *)NetGateString); 
        return -1;
    }
}



//int Net_paramsConfig(NetDevice *Device, NetCard type, char *NetDeviceName, char *IpAddr, char *SubNetMask, char *NetGateWay, char *DNS)
//{
//	int ret = 0;
//	int errcode = 0;
	
//	if(Device == NULL || NetDeviceName == NULL || IpAddr == NULL || SubNetMask == NULL || NetGateWay == NULL || DNS == NULL)
//	{
//		//log_printf(LOG_LEVEL_ERROR, LOG_MODULE_NET, "(%s:%d): Incorrect params!\n", __func__, __LINE__);
//		return -1;
//	}

//	/*
//	ret = Net_InitNetCard(Device, type, NetDeviceName);
//	if(ret < 0)
//	{
//		log_printf(LOG_LEVEL_ERROR, LOG_MODULE_NET, "(%s:%d): Init netcard failed\n", __func__, __LINE__);
//		errcode = ret;
//	}
//	*/

//	//Set IP
//	ret = Net_setIpAddrString(Device, IpAddr , strlen(IpAddr));
//	if(ret < 0)
//	{
//		//log_printf(LOG_LEVEL_ERROR, LOG_MODULE_NET, "(%s:%d): Set ip  failed\n", __func__, __LINE__);
//		errcode = ret;
//	}

//	//Set Mask
//	ret = Net_setSubNetMaskString(Device , SubNetMask , strlen(SubNetMask));
//	if(ret < 0)
//	{
//		//log_printf(LOG_LEVEL_ERROR, LOG_MODULE_NET, "(%s:%d): Set subnetmask failed\n", __func__, __LINE__);
//		errcode = ret;
//	}

//	//Set GateWay
//	ret = Net_setDefaultGateWay(NetGateWay, strlen(NetGateWay));
//	if(ret < 0)
//	{
//		//log_printf(LOG_LEVEL_ERROR, LOG_MODULE_NET, "(%s:%d): Set DefautGateWay failed\n", __func__, __LINE__);
//		errcode = ret;
//	}

//	//Set DNS
//	ret = Net_setDNSString(DNS, strlen(DNS));
//	if(ret < 0)
//	{
//		//log_printf(LOG_LEVEL_ERROR, LOG_MODULE_NET, "(%s:%d): Set DNS failed\n", __func__, __LINE__);
//		errcode = ret;
//	}
	
	
//	return errcode;
//}


int Net_PingIp(char *IpString)
{
    return 0;
}
