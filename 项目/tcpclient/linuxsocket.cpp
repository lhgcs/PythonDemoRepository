/*
 * socket
 * 连接服务器
 * 2017-06-01
 */

#include "linuxsocket.h"

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#define IP "192.168.1.227"
#define SERVPORT 55555
#define MAXDATASIZE 50

#include <pthread.h>

#include <QDebug>

#include <unistd.h>

/*线程一*/
void* thread1(void *)//param
{
    int sockfd;                   //socket描述符
    struct hostent *host;
    struct sockaddr_in serv_addr;

    /*地址解析函数*/
    if((host=gethostbyname(IP))==NULL)
    {
        perror("gethostbyname");
        exit(1);
    }
    else
    {
        qDebug("host name!\n");
    }
    /*创建 socket*/
    if((sockfd=socket(AF_INET,SOCK_STREAM,0))== -1)//IPv4协议,字节流套接字
    {
        perror("socket");
        exit(1);
    }
    else
    {
        qDebug("socket ok!\n");
    }
    /*设置 sockaddr_in 结构体中相关参数*/
    serv_addr.sin_family=AF_INET;                          //地址族
    serv_addr.sin_port=htons(SERVPORT);                    //端口号
    serv_addr.sin_addr=*((struct in_addr *)host->h_addr);  //IP地址
    bzero(&(serv_addr.sin_zero),8);                        //填充0以保持与 struct sockaddr 同样大小
    /*调用 connect 函数主动发起对服务器端的连接*/
    if(connect(sockfd,(struct sockaddr *)&serv_addr,sizeof(struct sockaddr))==-1)//服务器端地址,地址长度
    {
        qDebug("connect error!\n");
        perror("connect");
        //exit(1);
    }
    else
    {
        qDebug("connect ok!\n");
    }

    fd_set readfd;
    int recvbytes;
    char buf[MAXDATASIZE];
    memset(buf,0,strlen(buf));
    /*将调用 socket 函数的描述符作为文件描述符*/
    FD_ZERO(&readfd);        //清除一个文件描述符集
    FD_SET(sockfd,&readfd);  //将一个文件描述符加入文件描述符集中
    struct timeval time;
    time.tv_sec=0;
    time.tv_usec=100000;
    while(1)
    {
        /*调用 select 函数*/
        //最高的文件描述符加1,读文件描述符集合,写文件描述符集合,异常处理文件描述符集合
        if(select(sockfd+1,&readfd,NULL,NULL,&time)>0)//准备好的文件描述符
        {
            if(FD_ISSET(sockfd,&readfd)>0)            //测试该集中的一个给定位是否有变化
            {
                recvbytes=recv(sockfd,buf,MAXDATASIZE,0);
                if(recvbytes<0)//== -1
                {
                    qDebug("recv error");
                }
                else if(recvbytes==0)
                {
                    qDebug("recv 0");
                }
                else
                {
                   // printf("received a connection :%s\n",buf);
                    qDebug()<<recvbytes;
                    qDebug()<<buf;
                    memset(buf,0,strlen(buf));
                }
            }
         }
    }
}

/*发送消息给服务器端*/
void LinuxSocket::send_data(char buf[])
{
    int sendbytes=send(sockfd,buf,strlen(buf),0);
    if(sendbytes<0)//设置为异步方式，就可以一边读，一边写
    {
        perror("send");
        qDebug("send error");
    }
    else if(sendbytes==0)
    {
        qDebug("send 0");
    }
    else
    {
        printf("send 0k\n");
        qDebug("send 0k\n");
    }
}

LinuxSocket::LinuxSocket()
{
    pthread_t id1;
    pthread_attr_t attr;
    /*初始化线程*/
    pthread_attr_init(&attr);
    /*设置线程绑定属性*/
    pthread_attr_setscope(&attr, PTHREAD_SCOPE_SYSTEM);
    /*设置线程分离属性*/
    pthread_attr_setdetachstate(&attr,PTHREAD_CREATE_DETACHED);
    /*创建线程*/
    int ret=pthread_create(&id1,&attr,thread1,NULL);//创建线程成功时，函数返回0
    if(ret!=0)
    {
        printf ("Create pthread error!\n");
        qDebug("Create pthread error!\n");
        exit (1);
    }
    else
    {
        qDebug("Create pthread ok!\n");
    }
    /*等待线程结束*/
    pthread_join(id1,NULL);
    close(sockfd);
}

//if(inet_pton(AF_INET,argv[1],&ip_addr)<0){
//               printf("inet_pton failed\n");
//               exit(1);
//       }

//       if((host=gethostbyaddr(&ip_addr,4,AF_INET))==NULL){
//               printf("%s\n",hstrerror(h_errno));
//               exit(1);
//       }
//       printf("official hostname :%s\n",host->h_name);
//       for(pptr=host->h_aliases;*pptr!=NULL;pptr++){
//       printf("\t alais:%s\n",*pptr);
//       }

