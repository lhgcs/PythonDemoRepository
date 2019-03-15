#include "pingutil.h"

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/ip_icmp.h>
#include <netdb.h>
#include <setjmp.h>
#include <errno.h>
#include <sys/time.h>


#include "netutil.h"


int datalen=56;
pid_t pid;



void statistics(int sendcout, int recvcount)
{
    printf("\n--------------------PING statistics-------------------\n");
    printf("%d packets transmitted, %d received , %%%d lost\n", sendcout, recvcount, (sendcout-recvcount)/sendcout*100);
}

/*两个timeval结构相减*/
void tv_sub(struct timeval *out,struct timeval *in)
{       if( (out->tv_usec-=in->tv_usec)<0)
        {       --out->tv_sec;
                out->tv_usec+=1000000;
        }
        out->tv_sec-=in->tv_sec;
}


//校验和算法
unsigned short icmp_chksum(unsigned short *addr,int len)
{
    int nleft=len;
    int sum=0;
    unsigned short *w=addr;
    unsigned short answer=0;

    //把ICMP报头二进制数据以2字节为单位累加起来
    while(nleft>1)
    {
        sum+=*w++;
        nleft-=2;
    }
    //若ICMP报头为奇数个字节，会剩下最后一字节。把最后一个字节视为一个2字节数据的高字节，这个2字节数据的低字节为0，继续累加
    if(nleft == 1)
    {
        *(unsigned char *)(&answer)=*(unsigned char *)w;
        sum+=answer;
    }
    sum=(sum>>16)+(sum&0xffff);
    sum+=(sum>>16);
    answer=~sum;

    return answer;
}


//设置ICMP报头
int PackIcmpHeader(char *databuffer, int pack_no)
{
    int packsize;
    struct icmp *icmp;
    struct timeval *tval;

    icmp=(struct icmp*)databuffer;
    icmp->icmp_type=ICMP_ECHO;
    icmp->icmp_code=0;
    icmp->icmp_cksum=0;
    icmp->icmp_seq=pack_no;
    icmp->icmp_id=pid;

    packsize=8+datalen;

    tval= (struct timeval *)icmp->icmp_data;
    gettimeofday(tval,NULL);    /*记录发送时间*/

    icmp->icmp_cksum=icmp_chksum( (unsigned short *)icmp,packsize); /*校验算法*/
    printf("icmp_type :%d\n", icmp->icmp_type);

    return packsize;
}

//剥去ICMP报头
int UnPackIcmpHeader(char *buf,int len, struct sockaddr_in *src, PingReslut *Reslut)
{
    int iphdrlen;
    struct ip *ip;
    struct icmp *icmp;
    struct timeval *tvsend;
    double rtt;
    struct timeval tvrecv;


    ip=(struct ip *)buf;
    iphdrlen=ip->ip_hl<<2;    //求ip报头长度,即ip报头的长度标志乘4
    icmp=(struct icmp *)(buf+iphdrlen); //越过ip报头,指向ICMP报头
    len-=iphdrlen;            //ICMP报头及ICMP数据报的总长度
    if(len < 8)                //小于ICMP报头长度则不合理
    {
        printf("ICMP packets\'s length is less than 8\n");
        return -1;
    }
    //确保所接收的是我所发的的ICMP的回应
    if( (icmp->icmp_type != ICMP_ECHOREPLY) || (icmp->icmp_id != pid) )
    {
        return -1;
    }


    gettimeofday(&tvrecv,NULL); /*记录接收时间*/

    tvsend=(struct timeval *)icmp->icmp_data;
    tv_sub(&tvrecv,tvsend); //接收和发送的时间差
    rtt= tvrecv.tv_sec*1000+ tvrecv.tv_usec/1000; //以毫秒为单位计算rtt

    //显示相关信息
    printf("%d byte from %s: icmp_seq=%u ttl=%d rtt=%.3f ms\n",
                        len,
                        inet_ntoa(src->sin_addr),
                        icmp->icmp_seq,
                        ip->ip_ttl,
                        rtt);
    Reslut->AverTTL = ip->ip_ttl;
    Reslut->AverRTT = rtt;


    return 0;
}


/*发送三个ICMP报文*/
void send_packet(int sock, struct sockaddr_in *dst, char *data, int datasize, int sendcount)
{
    int index = 0, packetsize;

    while( index<sendcount)
   {
        index++;
        packetsize=PackIcmpHeader(data, index); /*设置ICMP报头*/
        if( sendto(sock, data, packetsize, 0, (struct sockaddr *)dst,sizeof(struct sockaddr_in) )<0 )
        {
            perror("sendto error");
            continue;
        }
       //sleep(1); //每隔一秒发送一个ICMP报文
   }
}

/*接收所有ICMP报文*/
int  recv_packet(int sock, struct sockaddr_in *src, char *data, int datasize, int sendcount, PingReslut *Reslut)
{
    int index = 0, ret;
    int recvcount =0;
    extern int errno;
    socklen_t addr_size;
    PingReslut CurrentReslut;

    addr_size =  sizeof(struct sockaddr_in);
    while( index<sendcount)
    {
        index++;

        if( (ret=recvfrom(sock, data, datasize, 0, (struct sockaddr *)src, &addr_size )) <0)
        {
            if (ret == EWOULDBLOCK || ret == EAGAIN)
                printf("recvfrom timeout\n");
            else
                printf("recvfrom err:%d\n", ret);
            return -1;
        }

        if(UnPackIcmpHeader(data, datasize, src, Reslut) !=-1)
        {
            Reslut->AverRTT += CurrentReslut.AverRTT;
            Reslut->AverTTL += CurrentReslut.AverTTL;
            recvcount++;
        }

    }

    Reslut->AverRTT /= recvcount;
    Reslut->AverTTL /= recvcount;

    return recvcount;
}


int ping(char *ipaddr, int timeout,int sendcount)
{
    PingReslut *Reslut=(PingReslut *)malloc(sizeof(PingReslut));

    struct hostent *host;
    unsigned long  inaddr;
    int size=50*1024;
    char sendpacket[PACKET_SIZE] ={0};
    char recvpacket[PACKET_SIZE] = {0};
    int recvcount = 0;
    struct timeval recv_timeout;
    struct sockaddr_in dest_addr;

    struct sockaddr_in src_addr;
    int sockfd;
    int packetindex = 0;

    if(ipaddr == NULL){
        return -1;
    }

    if( (sockfd=socket(AF_INET,SOCK_RAW,IPPROTO_ICMP) )<0){
        perror("socket error");
        return -1;
    }

   // 回收root权限,设置当前用户权限
    setuid(getuid());

    /*扩大套接字接收缓冲区到50K这样做主要为了减小接收缓冲区溢出的可能性,若无意中ping一个广播地址或多播地址,将会引来大量应答*/
    if (setsockopt(sockfd,SOL_SOCKET,SO_RCVBUF,&size,sizeof(size) )< 0){
        printf("socket option  SO_RCVBUF not support\n");
        return -1;
    }

    //设置接收超时时间
    recv_timeout.tv_sec = timeout;
    recv_timeout.tv_usec = 0;

    //设置接收icmp数据包超时时间
    if (setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, &recv_timeout, sizeof(recv_timeout)) < 0){
        printf("socket option  SO_RCVTIMEO not support\n");
        return -1;
    }

    bzero(&dest_addr,sizeof(dest_addr));
    dest_addr.sin_family=AF_INET;

    //判断是主机名还是ip地址
    if( (inaddr=inet_addr(ipaddr))==INADDR_NONE)
    {
        if((host=gethostbyname(ipaddr) )==NULL) //是主机名
        {
            perror("gethostbyname error");
            return -1;
        }
        memcpy( (char *)&dest_addr.sin_addr, host->h_addr, host->h_length);
    }
    else    //是ip地址
    {
        dest_addr.sin_addr.s_addr = inet_addr(ipaddr);
    }

    //获取main的进程id,用于设置ICMP的标志符
    pid=getpid();

    //printf("PING %s(%s): %d bytes data in ICMP packets.\n", ipaddr, inet_ntoa(dest_addr.sin_addr), datalen);

    while(packetindex <  sendcount)
    {
        send_packet(sockfd, &dest_addr, sendpacket, PACKET_SIZE, 1); //发送所有ICMP报文

        recvcount += recv_packet(sockfd, &src_addr, recvpacket, PACKET_SIZE, 1, Reslut); //接收所有ICMP报文
        packetindex++;
    }

    close(sockfd);

    free(Reslut);
    return recvcount;

}


/*
int main(int argc,char *argv[])
{
    int rec;

    if(argc<2)
    {
        printf("usage:%s hostname/IP address\n",argv[0]);
        exit(1);
    }

    rec = ping(argv[1], MAX_NO_PACKETS);
    if(rec >= 0)
        statistics(MAX_NO_PACKETS, rec); //进行统计

     return 0;
}
*/



