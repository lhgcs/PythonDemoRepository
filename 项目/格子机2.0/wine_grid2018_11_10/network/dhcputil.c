#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>
#include <poll.h>
#include <sys/types.h>
#include <netinet/in.h>

#include <dirent.h>


#include "dhcppacket.h"

#include "dhcpmsg.h"
#include "dhcputil.h"


typedef unsigned long long msecs_t;
static int verbose = 1;


msecs_t get_msecs(void)
{
    struct timespec ts;

    if (clock_gettime(CLOCK_MONOTONIC, &ts)) {
        return 0;
    } else {
        return (((msecs_t) ts.tv_sec) * ((msecs_t) 1000)) +
            (((msecs_t) ts.tv_nsec) / ((msecs_t) 1000000));
    }
}

int ipv4NetmaskToPrefixLength(in_addr_t mask)
{
    int prefixLength = 0;
    uint32_t m = (uint32_t)ntohl(mask);
    while (m & 0x80000000) {
        prefixLength++;
        m = m << 1;
    }
    return prefixLength;
}

void printerr(char *fmt, ...)
{
	char errmsg[256];
	
    va_list ap;

    va_start(ap, fmt);
    vsnprintf(errmsg, sizeof(errmsg), fmt, ap);
    va_end(ap);

    printf("%s", errmsg);
}


int fatal(const char *reason)
{
    printerr("%s: %s\n", reason, strerror(errno));
    return -1;
}

char *ip2addr(in_addr_t addr)
{
    struct in_addr in_addr;

    in_addr.s_addr = addr;
    return (char *)inet_ntoa(in_addr);
}



dhcp_info last_good_info;

void get_dhcp_info(uint32_t *ipaddr, uint32_t *gateway, uint32_t *prefixLength,
                   uint32_t *dns1, uint32_t *dns2, uint32_t *server,
                   uint32_t *lease)
{
    *ipaddr = last_good_info.ipaddr;
    *gateway = last_good_info.gateway;
    *prefixLength = last_good_info.prefixLength;
    *dns1 = last_good_info.dns1;
    *dns2 = last_good_info.dns2;
    *server = last_good_info.serveraddr;
    *lease = last_good_info.lease;
}



static const char *dhcp_type_to_name(uint32_t type)
{
    switch(type) {
    case DHCPDISCOVER: return "discover";
    case DHCPOFFER:    return "offer";
    case DHCPREQUEST:  return "request";
    case DHCPDECLINE:  return "decline";
    case DHCPACK:      return "ack";
    case DHCPNAK:      return "nak";
    case DHCPRELEASE:  return "release";
    case DHCPINFORM:   return "inform";
    default:           return "???";
    }
}




int decode_dhcp_msg(dhcp_msg *msg, int len, dhcp_info *info)
{
    uint8_t *x;
    unsigned int opt;
    int optlen;

    memset(info, 0, sizeof(dhcp_info));
    if (len < (DHCP_MSG_FIXED_SIZE + 4)) return -1;

    len -= (DHCP_MSG_FIXED_SIZE + 4);

    if (msg->options[0] != OPT_COOKIE1) return -1;
    if (msg->options[1] != OPT_COOKIE2) return -1;
    if (msg->options[2] != OPT_COOKIE3) return -1;
    if (msg->options[3] != OPT_COOKIE4) return -1;

    x = msg->options + 4;

    while (len > 2) {
        opt = *x++;
        if (opt == OPT_PAD) {
            len--;
            continue;
        }
        if (opt == OPT_END) {
            break;
        }
        optlen = *x++;
        len -= 2;
        if (optlen > len) {
            break;
        }
        switch(opt) {
        case OPT_SUBNET_MASK:
            if (optlen >= 4) {
				memcpy(&info->mask, x, 4);
                info->prefixLength = ipv4NetmaskToPrefixLength(info->mask);
            }
            break;
        case OPT_GATEWAY:
            if (optlen >= 4) memcpy(&info->gateway, x, 4);
            break;
        case OPT_DNS:
            if (optlen >= 4) memcpy(&info->dns1, x + 0, 4);
            if (optlen >= 8) memcpy(&info->dns2, x + 4, 4);
            break;
        case OPT_LEASE_TIME:
            if (optlen >= 4) {
                memcpy(&info->lease, x, 4);
                info->lease = ntohl(info->lease);
            }
            break;
        case OPT_SERVER_ID:
            if (optlen >= 4) memcpy(&info->serveraddr, x, 4);
            break;
        case OPT_MESSAGE_TYPE:
            info->type = *x;
            break;
        default:
            break;
        }
        x += optlen;
        len -= optlen;
    }

    info->ipaddr = msg->yiaddr;

    return 0;
}


static void hex2str(char *buf, const unsigned char *array, int len)
{
    int i;
    char *cp = buf;

    for (i = 0; i < len; i++) {
        cp += sprintf(cp, " %02x ", array[i]);
    }
}

void dump_dhcp_msg(dhcp_msg *msg, int len)
{
    unsigned char *x;
    unsigned int n,c;
    int optsz;
    const char *name;
    char buf[2048];

    printf("===== DHCP message:");
    if (len < DHCP_MSG_FIXED_SIZE) {
        printf("Invalid length %d, should be %d", len, DHCP_MSG_FIXED_SIZE);
        return;
    }

    len -= DHCP_MSG_FIXED_SIZE;

    if (msg->op == OP_BOOTREQUEST)
        name = "BOOTREQUEST";
    else if (msg->op == OP_BOOTREPLY)
        name = "BOOTREPLY";
    else
        name = "????";
    printf("op = %s (%d), htype = %d, hlen = %d, hops = %d",
           name, msg->op, msg->htype, msg->hlen, msg->hops);
    printf("xid = 0x%08x secs = %d, flags = 0x%04x optlen = %d",
           ntohl(msg->xid), ntohs(msg->secs), ntohs(msg->flags), len);
    printf("ciaddr = %s", ip2addr(msg->ciaddr));
    printf("yiaddr = %s", ip2addr(msg->yiaddr));
    printf("siaddr = %s", ip2addr(msg->siaddr));
    printf("giaddr = %s", ip2addr(msg->giaddr));

    c = msg->hlen > 16 ? 16 : msg->hlen;
    hex2str(buf, msg->chaddr, c);
    printf("chaddr = {%s}", buf);

    for (n = 0; n < 64; n++) {
        if ((msg->sname[n] < ' ') || (msg->sname[n] > 127)) {
            if (msg->sname[n] == 0) break;
            msg->sname[n] = '.';
        }
    }
    msg->sname[63] = 0;

    for (n = 0; n < 128; n++) {
        if ((msg->file[n] < ' ') || (msg->file[n] > 127)) {
            if (msg->file[n] == 0) break;
            msg->file[n] = '.';
        }
    }
    msg->file[127] = 0;

    printf("sname = '%s'", msg->sname);
    printf("file = '%s'", msg->file);

    if (len < 4) return;
    len -= 4;
    x = msg->options + 4;

    while (len > 2) {
        if (*x == 0) {
            x++;
            len--;
            continue;
        }
        if (*x == OPT_END) {
            break;
        }
        len -= 2;
        optsz = x[1];
        if (optsz > len) break;
        if (x[0] == OPT_DOMAIN_NAME || x[0] == OPT_MESSAGE) {
            if ((unsigned int)optsz < sizeof(buf) - 1) {
                n = optsz;
            } else {
                n = sizeof(buf) - 1;
            }
            memcpy(buf, &x[2], n);
            buf[n] = '\0';
        } else {
            hex2str(buf, &x[2], optsz);
        }
        if (x[0] == OPT_MESSAGE_TYPE)
            name = dhcp_type_to_name(x[2]);
        else
            name = NULL;
        printf("op %d len %d {%s} %s", x[0], optsz, buf, name == NULL ? "" : name);
        len -= optsz;
        x = x + optsz + 2;
    }
}

static int send_message(int sock, int if_index, dhcp_msg  *msg, unsigned char *mac,int size)
{

    //dump_dhcp_msg(msg, size);

    return send_packet(sock, if_index, msg, size, mac, INADDR_ANY, INADDR_BROADCAST,
                       PORT_BOOTP_CLIENT, PORT_BOOTP_SERVER);
}

static int is_valid_reply(dhcp_msg *msg, dhcp_msg *reply, int sz)
{
    if (sz < DHCP_MSG_FIXED_SIZE) {
        if (verbose) printf("netcfg: Wrong size %d != %d\n", sz, DHCP_MSG_FIXED_SIZE);
        return 0;
    }
    if (reply->op != OP_BOOTREPLY) {
        if (verbose) printf("netcfg: Wrong Op %d != %d\n", reply->op, OP_BOOTREPLY);
        return 0;
    }
    if (reply->xid != msg->xid) {
        if (verbose) printf("netcfg: Wrong Xid 0x%x != 0x%x\n", ntohl(reply->xid),
                          ntohl(msg->xid));
        return 0;
    }
    if (reply->htype != msg->htype) {
        if (verbose) printf("netcfg: Wrong Htype %d != %d\n", reply->htype, msg->htype);
        return 0;
    }
    if (reply->hlen != msg->hlen) {
        if (verbose) printf("netcfg: Wrong Hlen %d != %d\n", reply->hlen, msg->hlen);
        return 0;
    }
    if (memcmp(msg->chaddr, reply->chaddr, msg->hlen)) {
        if (verbose) printf("netcfg: Wrong chaddr %x != %x\n", *(reply->chaddr),*(msg->chaddr));
        return 0;
    }
    return 1;
}

#define STATE_SELECTING  1
#define STATE_REQUESTING 2

#define TIMEOUT_INITIAL   4000
#define TIMEOUT_MAX      32000



int dhcp_init_ifc(NetDevice *Interface, const char *ifname, dhcp_info *result)
{
    dhcp_msg discover_msg;
    dhcp_msg request_msg;
    dhcp_msg reply;
    dhcp_msg *msg;
    dhcp_info info;
    int s, r, size;
    int valid_reply;
    uint32_t xid;
    struct pollfd pfd;
    unsigned int state;
    unsigned int timeout;
    int if_index = 0;
	int index = 0;
	
	NetMacAddress macaddr;
	
    xid = (uint32_t) get_msecs();

    Net_getMacAddr(Interface, &macaddr);
   
   if(ifc_get_ifindex(Interface, &if_index))
   {
	   printf("get ifindex failed\n");
	   return -1;
   }

	printf("if index :%d\n", if_index);
    s = open_raw_socket(ifname, macaddr.mac, if_index);
	if(s <0)
	{
		printf("open_raw_socket filed \n");
		return -1;
	}
		
    timeout = TIMEOUT_INITIAL;
    state = STATE_SELECTING;
    info.type = 0;

	for(index = 0; index< 5; index++)
	{
	   size = 0;
       msg = NULL;
	   
        switch(state) 
		{
            case STATE_SELECTING:
                msg = &discover_msg;
                size = init_dhcp_discover_msg(msg, macaddr.mac, xid);
                break;
            case STATE_REQUESTING:
                msg = &request_msg;
                size = init_dhcp_request_msg(msg, macaddr.mac, xid, info.ipaddr, info.serveraddr);
                break;
            default:
				break;
        }
		
        if (size != 0) 
		{
            r = send_message(s, if_index, msg, macaddr.mac, size);
            if (r < 0) 
			{
                printf("error sending dhcp msg: %s\n", strerror(errno));
            }
		}
		
		
		pfd.fd = s;
        pfd.events = POLLIN;
        pfd.revents = 0;
        r = poll(&pfd, 1, timeout);
		
		if (r < 0) 
		{
            if ((errno == EAGAIN) || (errno == EINTR)) {
                continue;
            }
			printf("poll failed\n");
            return -3;
        }
		
	    errno = 0;
        r = receive_packet(s, &reply);
        if (r < 0) {
            if (errno != 0) {
                printf("receive_packet failed (%d): %s\n", r, strerror(errno));
                if (errno == ENETDOWN || errno == ENXIO) {
                    return -1;
                }
            }
            continue;
        }
		
		decode_dhcp_msg(&reply, r, &info);
		if (state == STATE_SELECTING)			
		{
            valid_reply = is_valid_reply(&discover_msg, &reply, r);
        } else 
		{
            valid_reply = is_valid_reply(&request_msg, &reply, r);
        }
		
        if (!valid_reply) {
            printf("invalid reply\n");
            return -1;
        }
		
		
		switch(state) 
		{
			case STATE_SELECTING:
			if (info.type == DHCPOFFER) 
			{
                state = STATE_REQUESTING;
                timeout = TIMEOUT_INITIAL;
                xid++;
            }
            break;
			case STATE_REQUESTING:
				if (info.type == DHCPACK)				
				{
					printf("configuring %s\n", ifname);
					*result = info;
					close(s);
					return 0;
				} 
				else if (info.type == DHCPNAK) 
				{
					printf("configuration request denied\n");
					close(s);
					return -1;
				} 
				else 
				{
					printf("ignoring %s message in state %d\n", dhcp_type_to_name(info.type), state);
					return -1;
				}
				break;
		}
	}

	
    return 0;
}

void dump_dhcp_info(dhcp_info *info)
{
    char addr[20], gway[20], mask[20];
    printf("--- dhcp %s (%d) ---\n", dhcp_type_to_name(info->type), info->type);
    strcpy(addr, ip2addr(info->ipaddr));
    strcpy(gway, ip2addr(info->gateway));
    printf("ip %s  gw %s  prefixLength %d\n", addr, gway, info->prefixLength);

	strcpy(mask, ip2addr(info->mask));
	printf("mask: %s\n", mask);
	
    if (info->dns1) 
	printf("dns1: %s\n", ip2addr(info->dns1));

    if (info->dns2) 
	printf("dns2: %s\n", ip2addr(info->dns2));
    printf("server %s, lease %d seconds\n", ip2addr(info->serveraddr), info->lease);
}


int DhcpClient(NetDevice *Interface, DhcpClientHandler Handler, void *arg)
{
	dhcp_info info; 
	int ret =0;
	
    ret = dhcp_init_ifc(Interface, Interface->NetCardName, &info);
	if(ret < 0)
	{
		printf("dhcp get network failed\n");
		return -1;
	}
	
    if(Handler != NULL)
    {
        Handler(Interface, &info, arg);
        printf("dhcp get network Handler\n");
    }

	dump_dhcp_info(&info);
	
	return 0;
}

void DhcpHandler(NetDevice *Interface, dhcp_info *info, void *arg)
{
	char IpString[20], GatewayString[20], MaskString[20], DnsString[20];
	
    strcpy(IpString, ip2addr(info->ipaddr));
    strcpy(GatewayString, ip2addr(info->gateway));
    strcpy(MaskString, ip2addr(info->mask));
    strcpy(DnsString, ip2addr(info->dns1));


    Net_setIpAddrString(Interface, IpString , strlen(IpString));
	
    Net_setSubNetMaskString(Interface , MaskString , strlen(MaskString));

    Net_setDNSString(DnsString, strlen((DnsString)));
	
    Net_setDefaultGateWay(GatewayString, strlen((GatewayString)));
}

/*
int main()
{
	DhcpClient("eth0");
	
	return 0;
}*/
