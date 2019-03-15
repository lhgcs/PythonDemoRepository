#ifndef SHELLUTIL_H
#define SHELLUTIL_H

#include <stdio.h>
#include <stdlib.h>
#include<string.h>

#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <linux/if.h>
#include <linux/sockios.h>
#include <linux/ethtool.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/stat.h>

class ShellUtil
{
public:
    ShellUtil();
    //    翻了一下ifconfig和route的源代码
    //    获取MAC地址和IP分两大步
    //    1 读取/proc/net/dev文件 获得网络设备的名字
    //    2 根据名字调用ioctl系列函数获得MAC, IP, MTU, METRIC等信息
    //    获取路由则比较方便
    //    直接读取/proc/net/route就可以了

        int get_ip( ){
            FILE* netinfo = popen("/sbin/ifconfig", "r");
            if(!netinfo){
                puts("error while open pipe");
                return 1;
            }
            char str[200];
            while(fgets(str, 199, netinfo) != NULL ){
                printf("%s", str);
            }
            pclose(netinfo);
            return 0;
        }

        //检测网卡与网线是否连接
        //使用ioctl向socket发送SIOCETHTOOL命令字

    // if_name like "ath0", "eth0". Notice: call this function
    // need root privilege.
    // return value:
    // -1 -- error , details can check errno
    // 1 -- interface link up
    // 0 -- interface link down.
    /*  缺点:read长度0,但是 cat到数据,似乎是回车的原因 */
    static int get_netlink_status(const char *if_name){
        if(getuid()!= 0){
            fprintf(stderr,"Netlink Status Check Need Root Power.\n");
            return 1;
        }
        int skfd;
        struct ifreq ifr;
        struct ethtool_value edata;

        edata.cmd = ETHTOOL_GLINK;
        edata.data = 0;

        memset(&ifr, 0,sizeof(ifr));
        strncpy(ifr.ifr_name, if_name,sizeof(ifr.ifr_name)- 1);
        ifr.ifr_data =(char *) &edata;

        if (( skfd= socket(AF_INET, SOCK_DGRAM, 0 )) < 0)
            return -1;

        if(ioctl( skfd, SIOCETHTOOL,&ifr ) == -1)  {
            close(skfd);
            return -1;
        }
        close(skfd);
        return edata.data;
    }

    // 方法二： 使用ifconfigl命令能很方便的查看网卡与网线是否连通：
    //其中的RUNNING就表示网卡与网线正常链接，拔掉网线再运行此命令就会发现RUNNING不在了。
    //linux系统提供了popen/pclose进程管道让C和shell很方便的交互，下面C代码结合shell命令检测网卡与网线连通状况：

    static int get_net_status( )  {
        char buffer[BUFSIZ];
        FILE *read_fp;
        int  chars_read;
        int  ret;
        memset( buffer, 0, BUFSIZ );
        read_fp = popen("/sbin/ifconfig eth0 | grep RUNNING", "r");
        if ( read_fp != NULL )  {
            chars_read = fread(buffer, sizeof(char), BUFSIZ-1, read_fp);
            if (chars_read > 0)  {
                ret = 0;
            }  else  {
                ret = -1;
            }
            pclose(read_fp);
        }  else  {
            ret = -1;
        }
        return ret;
    }

    /**
      杀死进程
     * @brief kill_cmd
     * @param pidName
     * @return
     */
    /*  缺点:不一定成功  */
    static int kill_cmd(char *pidName)  {
        char buff[50];
        memset(buff,0,sizeof(buff));
        sprintf(buff,"/usr/bin/killall %s",pidName);
        FILE *read_fp = popen(buff, "r");
        if ( read_fp != NULL )  {
            pclose(read_fp);
            return 0;
        }
        return 1;
    }

    static int check_network_connect1(char *device){
        int fp;
        //一定要只读模式打开,读写模式打开不可以
        ///sys/class/net/wlan0/carrier  0:down 1:up
        char network[50];
        memset(network,0,sizeof(network));
                         ///sys/class/net/eth0/operstate
        sprintf(network,"/sys/class/net/%s/operstate",device);
        fp = open ("/sys/class/net/eth0/operstate",O_RDONLY);
        if (fp<0) {
            printf("open file operstate failure%d\n",fp);
            return 1;
        }
        char status[50];
        memset(status,0,sizeof(status));
        int len=read(fp,status,strlen(status));
        printf("status:%s\n",status);
        printf("len:%d\n",len);
        int res=1;
        if (NULL != strstr(status,"up"))   {//失败
            printf("on line now \n");
            res=0;
        }else if (NULL != strstr(status,"down"))  {
            printf("off off \n");
        }else if (NULL != strstr(status,"unknow error"))  {
            printf("unknow error\n");
        }
        close (fp);
        return res;
    }

    static int check_network_connect2(char *device){
        system("cat /sys/class/net/eth0/operstate > /home/eth0.txt");
        int fd;
        char *path=(char *)"/home/eth0.txt";
        char result[20];
        fd=open(path,O_RDONLY);
        int res=1;
        if(fd>0){
            int len=read(fd,result,20);
            printf("The content is %s",result);
            printf("len:%d\n",len);
            if (NULL != strstr(result,"up"))   {//失败
                printf("on line now \n");
                res=0;
            }else if (NULL != strstr(result,"down"))  {
                printf("off off \n");
            }else if (NULL != strstr(result,"unknow error"))  {
                printf("unknow error\n");
            }
            close (fd);
        }
        return res;
    }

    static int network_by_eth(char *eth0,int ms=5000){
        int cnt=ms/1000;
        printf("kill udhcpc");
        //kill_cmd((char *)"udhcpc");//无法kill
        system("/usr/bin/killall udhcpc");
        if(system("/sbin/udhcpc eth0 &") == 0){
            printf("udhcpc eth0 ok");
            for(int i=0;i<cnt;i++){
               //if(get_net_status() == 0){   //没反应
               // if(get_netlink_status(eth0) == 1){//崩溃
                sleep(2);
                if(check_network_connect2((char *)"eth0") ==0 ){//未测试
                    printf("eth0 ok");
                    return 0;
                }else{
                    printf("eth0 fail");
                }
            }
        }else{
            printf("udhcpc eth0 failed");
        }
        return 1;
    }

};

#endif // SHELLUTIL_H
