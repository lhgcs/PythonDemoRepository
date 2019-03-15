#include "module_4g.h"

QString Module_4G::imeiStr="";
Module_4G::Module_4G(){
}

#include <stdio.h>
#include <errno.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <termios.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>


/**************************************************************************************
 *  Description: 串口参数配置
 *  Input Args: fd:open打开的文件描述符 nspeed:波特率 nBits:数据位数 nEvent:奇偶校验 nStop:停止位
 *  Output Argtingzhis: 串口参数设置失败返回-1
 * Return Value:
 *************************************************************************************/
//int set_serial(int fd,int nSpeed,int nBits,char nEvent,int nStop)
//{
//    struct termios newttys1,oldttys1;

//    if(tcgetattr(fd,&oldttys1)!=0)           //保存原先串口配置 /*该函数与设备文件绑定*/
//    {
//        perror("Setupserial 1");
//        return -1;
//    }

//    bzero(&newttys1,sizeof(newttys1));       //将一段内存区域的内容全清为零
//    newttys1.c_cflag|=(CLOCAL|CREAD );       //CREAD 开启串行数据接收，CLOCAL并打开本地连接模式   /*这两个保证程序不会成为终端的所有者，用于本地接受和发送*/
//    newttys1.c_cflag &=~CSIZE;               //设置数据位数 /*字符大小屏蔽*/
//    switch(nBits)     //选择数据位
//    {
//            case 7:
//                    newttys1.c_cflag |=CS7;
//                    break;
//            case 8:
//                    newttys1.c_cflag |=CS8;  /* 把数据设置为8位*/
//                    break;
//    }
//    switch( nEvent )   //设置校验位
//    {
//       case '0':       //奇校验
//               newttys1.c_cflag |= PARENB;             //开启奇偶校验
//               newttys1.c_iflag |= (INPCK | ISTRIP);   //INPCK打开输入奇偶校验；ISTRIP去除字符的第八个比特
//               newttys1.c_cflag |= PARODD;             //启用奇校验(默认为偶校验)
//               break;
//      case 'E' :       //偶校验
//               newttys1.c_cflag |= PARENB;             //开启奇偶校验
//               newttys1.c_iflag |= ( INPCK | ISTRIP);  //打开输入奇偶校验并去除字符第八个比特
//               newttys1.c_cflag &= ~PARODD;            //启用偶校验；
//               break;
//      case 'N':        //关闭奇偶校验
//               newttys1.c_cflag &= ~PARENB;
//               break;
//    }

//     switch( nSpeed )        //设置波特率
//     {
//        case 2400:
//                 cfsetispeed(&newttys1, B2400);           //设置输入速度
//                 cfsetospeed(&newttys1, B2400);           //设置输出速度
//                 break;
//        case 4800:
//                 cfsetispeed(&newttys1, B4800);
//                 cfsetospeed(&newttys1, B4800);
//                 break;
//        case 9600:
//                 cfsetispeed(&newttys1, B9600);
//                 cfsetospeed(&newttys1, B9600);
//                 break;
//        case 115200:
//                 cfsetispeed(&newttys1, B115200);
//                 cfsetospeed(&newttys1, B115200);
//                 break;
//        default:
//                 cfsetispeed(&newttys1, B9600);
//                 cfsetospeed(&newttys1, B9600);
//                 break;
//     }

//     if( nStop == 1)                      //设置停止位；若停止位为1，则清除CSTOPB，若停止位为2，则激活CSTOPB。
//     {
//        newttys1.c_cflag &= ~CSTOPB;      //默认为送一位停止位； /*空格校验*/
//     }
//     else if( nStop == 2)
//     {
//        newttys1.c_cflag |= CSTOPB;       //CSTOPB表示送两位停止位；
//     }

//     //设置最少字符和等待时间，对于接收字符和等待时间没有特别的要求时
//     newttys1.c_cc[VTIME] = 0;        //非规范模式读取时的超时时间；
//     newttys1.c_cc[VMIN]  = 0;        //非规范模式读取时的最小字符数；
//     tcflush(fd ,TCIFLUSH);           //tcflush清空终端未完成的输入/输出请求及数据；TCIFLUSH表示清空正收到的数据，且不读取出来

//     // 在完成配置后，需要激活配置使其生效  /*激活配置，使其生效，第二个参数为 更改立即生效*/
//     if((tcsetattr( fd, TCSANOW,&newttys1))!=0) //TCSANOW不等数据传输完毕就立即改变属性
//     {
//         perror("com set error");
//         return -1;
//     }
//     return 0;
 /*   }*/ /* ----- End of if()  ----- */

int set_serial(int fd,int BaudRate, char cBits, char cCheck, char cStops, int FlowCtrl)
{
    struct termios sp_settings;
    memset (&sp_settings, 0, sizeof (sp_settings));
    tcflush (fd, TCIFLUSH);//丢弃所有未读取的接收到的数据

    sp_settings.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);//规范输入|回送输入数据|回送擦除字符|使能SIGINTR，SIGSUSP， SIGDSUSP和 SIGQUIT 信号，这些都关闭
    //sp_settings.c_oflag &= ~OPOST; //原始数据（RAW）输出
    cfsetispeed (&sp_settings, BaudRate);//设置端口的输入波特率
    cfsetospeed (&sp_settings, BaudRate);//设置端口的输出波特率
    sp_settings.c_cflag |= (CLOCAL | CREAD);//忽略modem信号线|打开接收功能

    switch (cBits) {//设置数据位数
        case '8':sp_settings.c_cflag |= CS8;//数据位为8bit
            break;
        case '7':sp_settings.c_cflag |= CS7;//数据位为7bit
            break;
        default: sp_settings.c_cflag |= CS8;//数据位为8bit
            break;
    }
    switch (cCheck) {//奇偶校验位
        case 'N':
        case 'n':sp_settings.c_cflag &= ~PARENB;//无校验
            break;
        case 'O':
        case 'o':sp_settings.c_cflag |= PARODD; //奇校验
                 sp_settings.c_iflag |= (INPCK | ISTRIP);
            break;
        case 'E':
        case 'e':sp_settings.c_cflag &= ~PARODD;//偶校验
                 sp_settings.c_iflag |= (INPCK | ISTRIP);
            break;
        default: sp_settings.c_cflag &= ~PARENB;//无校验
            break;
    }
    switch (cStops) {//设置停止位
        case '1':sp_settings.c_cflag &= ~CSTOPB; //1位停止位
            break;
        case '2':sp_settings.c_cflag |= CSTOPB;  //2位停止位
            break;
        default: sp_settings.c_cflag &= ~CSTOPB; //1位停止位
            break;
    }
    switch (FlowCtrl) {//数据流控制
        case 1:sp_settings.c_cflag |= CRTSCTS;//硬件流控制
            break;
        case 2:sp_settings.c_iflag |= (IXON | IXOFF | IXANY);//软件流控制 IXON 打开输出控制，IXOFF 打开输入控制
            break;
        default:      sp_settings.c_iflag &= ~(IXON | IXOFF | IXANY);//不使用流控制
                      sp_settings.c_cflag &= ~CRTSCTS;
            break;
    }
    //只有阻塞时，下面两个才有效，且针对读。
    sp_settings.c_cc[VMIN] = 0;   //读取字符的最小数量，至少要读到0个字符才会返回
    sp_settings.c_cc[VTIME] =0;   //读取第一个字符的等待时间*(1/10)s
    if(tcsetattr (fd, TCSANOW, &sp_settings)!=0)//将修改后数据设置到串口中
    {
        perror("set uart error!!!");
        return -1;
    }
    tcflush (fd, TCIFLUSH);
   // 用fcntl函数:
//    fcntl(fd,F_SETFL,0); //F_SETFL：设置文件flag为0，即默认，即阻塞状态
    return fd;
}


//打开并初始化设备
//R_OK 只判断是否有读权限
//W_OK 只判断是否有写权限
//X_OK 判断是否有执行权限
//F_OK 只判断是否存在
int Module_4G::module_4G_open(const char *const device){
    int cnt=0;
    do{
        if((access(device,F_OK))!=-1)   {//确定文件或文件夹的访问权限
            printf("%s exist \n",device);
            break;
        }
        else    {
            printf("%s no exist\n",device);
            sleep(1);
            cnt++;
        }
    }while (cnt<5);

    if(cnt>=5)   {
        return -1;
    }

    if(access(device,R_OK)!=-1)    {
        printf("%s read power\n",device);
    }
    else  {
        printf("%s no read power\n",device);
        return -1;
    }

    if(access(device,W_OK)!=-1)    {
        printf("%s write power\n",device);
    }
    else  {
        printf("%s no write power\n",device);
        return -1;
    }
    //O_NOCTTY 表示不能把本串口当成控制终端  O_NONBLOCK非阻塞方式
    int fd = open(device, O_RDWR|O_NOCTTY);/*ttyUSB2为AT指令口*/
    if(fd<0){
        perror("Can't Open Serial Port \n");
        return -1;
    }
    //配置串口
    int res=set_serial(fd,115200,8,'N',1,0);
    if(res<0){
        printf("Can't Set Serial Port %s\n",device);
        return -1;
    }
    else{
        printf("Set Serial Port %s\n",device);
    }
    return fd;
}
//提取字符串
int get_substr(char *str,char *substr,char a,char b,int str_len){
    int substr_len=0;
    if(str_len<=0){
        return 0;
    }
    char *p=str,*q=substr;
    int contain_a=0,contain_b=0;
    for(;*(p+str_len-1);p++){
        if(*p==a){
            contain_a=1;
        }
        else if(*p==b){
            contain_b=1;
        }
    }
    if(contain_a>0&&contain_b>0){
        int flag=0;
        p=str;
        for(;*(p+str_len-1);p++){
            if(*p==a){
                flag=1;
                continue;
            }
            if(flag>0){
                if(!*q){
                  break;
                }
                if(*p==b){
                  break;
                }
                *q=*p;
                q++;
                substr_len++;
            }
        }
        *q='\0';
    }
    return substr_len;
}
//截取字符串中指定字符串之后的字符
int get_substr_after(char *str,char *substr,char *prefix){
    char *p=str;
    char *q=prefix;

    int p_len=strlen(p);
    int q_len=strlen(q);

    printf("p:%d,q:%d",p_len,q_len);
    int substr_len=-1;

    if((p_len<=0)||(q_len>p_len)){
        return -1;
    }

    if(p_len<=0){
        strcpy(substr,p);
        substr_len=p_len;
    }
    else{
        char *p_index = strstr(p, q); //首次出现地址
        if (p_index != NULL){
            p_index+=q_len;
            substr_len=0;

            printf("%s \n",p_index);

            while(*p_index){
//                printf("%c ",*p_index);
                *substr=*p_index;
                p_index++;
                substr++;
                substr_len++;
            }
            substr='\0';
        }
    }
    return substr_len;
}
//截取字符串中指定字符串之前的字符
int get_substr_before(char *str,char *substr,char *postfix){
    char *p=str;
    char *q=postfix;
    int p_len=strlen(p);
    int q_len=strlen(q);
    int substr_len=-1;
    if((p_len<=0)||(q_len>p_len)){
        return -1;
    }
    if(p_len<=0){
        strcpy(substr,p);
        substr_len=p_len;
    }
    else{
        char *p_index = strstr(p, q); //首次出现地址
        if (p_index != NULL){
            printf("%s \n",p);
            substr_len=0;
            for(;p<p_index;p++){
//                printf("%c ",*p);
                *substr=*p;
                substr++;
                substr_len++;
            }
            substr='\0';
        }
    }
    return substr_len;
}

//提取字符串
int get_substr2(char *str,char *substr,char *prefix,char *postfix){
    int str_len=strlen(str);
    printf("str:%d",str_len);
    if(str_len<=0){
        return -1;
    }
    char *p=str;
    char *q=substr;
    char temp[256];
    memset(temp,0,sizeof(temp));
    printf("str:%s",str);
    if(get_substr_after(p,temp,prefix)>=0){
        printf("prefix");
        int res=get_substr_before(temp,q,postfix);
        if(res<0){
           printf("no postfix");
        }
        else{
            printf("postfix");
        }
        return res;
    }
    else{
        printf("no prefix");
    }
    return -1;
}
//去除空格
void str_delete_all_space(char *sStr)
{
    int len,k,i;
    if (sStr == NULL)
        return;
    len = strlen(sStr);
    k = 0;
    for (i=0; i<len; i++){
        if ((sStr[i] != ' ')&&(sStr[i] != '\n')&&(sStr[i] != '\r')){
           sStr[k]=sStr[i];
           k++;
        }
    }
    sStr[k]='\0';
}
//启动初始化SIM模块命令，该命令不管有没有插卡都会返回OK；
void cmd_init(int fd){
    system("killall pppd");
    system("killall chat");
    int nwrite;
    char buffWrite[20];
    memset(buffWrite,0,sizeof(buffWrite));
    strcpy(buffWrite,"AT+CFUN=1\r");
    nwrite = write(fd,buffWrite,strlen(buffWrite));
    printf("nwrite=%d,%s\n",nwrite,buffWrite);
    sleep(3);
}
//检查SIM卡
/*
输入如下指令， 查询 PIN 码信息：
AT+CPIN?
将得到如下信息：
+CPIN: READY
*/
int cmd_check_sim_card(int fd){
    int cnt=0;
    int nread,nwrite;
    char buffRead[128];
    char buffWrite[20];
    char *p=NULL;
    while (cnt<5) {
        cnt++;
        memset(buffWrite,0,sizeof(buffWrite));
        strcpy(buffWrite,"AT+CPIN?\r");
        nwrite = write(fd,buffWrite,strlen(buffWrite));
        printf("nwrite=%d,%s\n",nwrite,buffWrite);
        sleep(2);

        memset(buffRead,0,sizeof(buffRead));
        nread = read(fd,buffRead,sizeof(buffRead));
        if(nread>0){
            printf("nread=%d,%s\n",nread,buffRead);
            p=NULL;
            p=strstr(buffRead,"READY");
            if(p){
                printf("SIM card ready ok\n");
                return 0;
            }
        }
    }
    return 1;
}
//信号强度
/*
输入如下指令， 查询信号：
AT+CSQ
将得到如下信号强度和误码率信息：
+CSQ: 31,99
*/
void cmd_signal(int fd){
    int nread,nwrite;
    char buffRead[128];
    char buffWrite[20];
    memset(buffWrite,0,sizeof(buffWrite));
    strcpy(buffWrite,"AT+CSQ\r");
    nwrite = write(fd,buffWrite,strlen(buffWrite));
    printf("nwrite=%d,%s\n",nwrite,buffWrite);
    sleep(1);

    memset(buffRead,0,sizeof(buffRead));
    nread = read(fd,buffRead,sizeof(buffRead));
    if(nread>0){
        printf("nread=%d,%s\n",nread,buffRead);
        str_delete_all_space(buffRead);

        char substr[128];
        memset(substr,0,sizeof(substr));
        char after[10]="+CSQ:";
        char before[10]=",";
        if(get_substr2(buffRead,substr,after,before)>=0){
            printf("************\n");
            printf("signal:%s\n",substr);
        }
    }
}
/*
    输入如下指令， 查询 PS 域（数据域） 注册状态：
    AT+CGREG?
    将得到如下注册信息：
    +CGREG: 0,1
*/
int cmd_check_register(int fd){
    int cnt=0;
    int nread,nwrite;
    char buffRead[128];
    char buffWrite[20];
    char *p=NULL;
    while (cnt<5) {
        cnt++;
        memset(buffWrite,0,sizeof(buffWrite));
        strcpy(buffWrite,"AT+CGREG?\r");
        nwrite = write(fd,buffWrite,strlen(buffWrite));
        printf("nwrite=%d,%s\n",nwrite,buffWrite);
        sleep(1);

        memset(buffRead,0,sizeof(buffRead));
        nread = read(fd,buffRead,sizeof(buffRead));
        if(nread>0){
            printf("nread=%d,%s\n",nread,buffRead);
            str_delete_all_space(buffRead);
            p=NULL;
            p=strstr(buffRead,"+CGREG:0,1");
            if(p){
                printf("register ok\n");
                return 0;
            }
        }
    }
    return 1;
}
//获取IMEI
//+CGSN	+CGSN:<IMEI>
int Module_4G::cmd_get_IMEI(int fd){
    int nread,nwrite;
    char buffRead[256];
    char buffWrite[20];
    memset(buffWrite,0,sizeof(buffWrite));
    strcpy(buffWrite,"AT+CGSN\r");
    nwrite = write(fd,buffWrite,strlen(buffWrite));
    printf("nwrite=%d,%s\n",nwrite,buffWrite);
    sleep(1);

    memset(buffRead,0,sizeof(buffRead));
    nread = read(fd,buffRead,sizeof(buffRead));
    if(nread>0){
        printf("nread=%d,%s\n",nread,buffRead);
        str_delete_all_space(buffRead);
        char imei[256];
        memset(imei,0,sizeof(imei));
        char after[10]="AT+CGSN";
        char before[10]="OK";
        if(get_substr2(buffRead,imei,after,before)>=0){
            printf("************\n");
            printf("IMEI:%s\n",imei);
            if(strlen(imei)==15){
                this->imeiStr=imei;
                return 0;
            }
            else{
                printf("IMEI_len:%d\n",strlen(imei));
            }
        }
    }
    return 1;
}
//拨号

int cmd_pppd(){
    char buffWrite[512];
    memset(buffWrite,0,sizeof(buffWrite));
    //strcpy(buffWrite,"pppd connect 'chat -s -v  \"ABORT\" \"NO CARRIER\" \"\" AT OK AT+CGDCONT=1,\"IP\",\"\" OK ATD*99**1# CONNECT' /dev/ttyUSB0 115200 nodetach debug noauth defaultroute usepeerdns  crtscts user card password card -pc -ac $* &");
    strcpy(buffWrite,"/home/pppd/pppd_on.sh");
    printf("pppd:%s\n",buffWrite);

    pid_t status=system(buffWrite);
    /*
    判断一个system函数调用shell脚本是否正常结束的方法应该是如下3个条件同时成立：
    （1）-1 != status
    （2）WIFEXITED(status)为真
    （3）0 == WEXITSTATUS(status)
    */
    if (-1 == status)  {
        printf("system error!");
    }
    else  {
        printf("exit status value = [0x%x]\n", status);
        if (WIFEXITED(status))  {
            if (0 == WEXITSTATUS(status))  {
                printf("run shell script successfully.\n");
            }
            else  {
                printf("run shell script fail, script exit code: %d\n", WEXITSTATUS(status));
            }
        }
        else  {
            printf("exit status = [%d]\n", WEXITSTATUS(status));
        }
    }
    sleep(5);
    return 0;
}



//int cmd_pppd(){
//    char buffWrite[512];
//    memset(buffWrite,0,sizeof(buffWrite));
//    //strcpy(buffWrite,"pppd connect 'chat -s -v  \"ABORT\" \"NO CARRIER\" \"\" AT OK AT+CGDCONT=1,\"IP\",\"\" OK ATD*99**1# CONNECT' /dev/ttyUSB0 115200 nodetach debug noauth defaultroute usepeerdns  crtscts user card password card -pc -ac $* &");
//    //strcpy(buffWrite,"/home/pppd/pppd_on.sh");

//    strcpy(buffWrite,"/home/pppd/pppd connect \'/home/pppd/chat -s -v  \"ABORT\" \"NO CARRIER\" \"\" AT OK AT+CGDCONT=1,\"IP\",\"\" OK ATD*99**1# CONNECT\' /dev/ttyUSB0 115200 nodetach debug noauth defaultroute usepeerdns  crtscts user card password card persist -pc -ac $* &");
//    printf("pppd:%s\n",buffWrite);

//    pid_t status=system(buffWrite);
//    /*
//    判断一个system函数调用shell脚本是否正常结束的方法应该是如下3个条件同时成立：
//    （1）-1 != status
//    （2）WIFEXITED(status)为真
//    （3）0 == WEXITSTATUS(status)
//    */
//    if (-1 == status)  {
//        printf("system error!");
//    }
//    else  {
//        printf("exit status value = [0x%x]\n", status);
//        if (WIFEXITED(status))  {
//            if (0 == WEXITSTATUS(status))  {
//                printf("run shell script successfully.\n");
//            }
//            else  {
//                printf("run shell script fail, script exit code: %d\n", WEXITSTATUS(status));
//            }
//        }
//        else  {
//            printf("exit status = [%d]\n", WEXITSTATUS(status));
//        }
//    }
//    sleep(5);
//    return 0;
//}


//expect (ABORTNO CARRIER)
//alarm
//Failed

//pppd:/home/pppd/pppd connect 'chat -s -v  "ABORTNO CARRIER" AT OK AT+CGDCONT=1,"IP","" OK ATD*99**1# CONNECT' /dev/ttyUSB0 115200 nodetach debug noauth defaultroute usepeerdns  crtscts user card password card persist -pc -ac $* > $FILE &


//dial_on()
//{
//        FILE="/tmp/a"
//        if [ ! -f "$FILE" ]; then
//               touch "$FILE"
//        fi

//        echo "YUGA 4G start pppd ......"
//        CMD_PATH=$(cd `dirname $0`; pwd)
//        echo "current cmd path:$CMD_PATH"
//        export PATH=$CMD_PATH:$PATH
//        pppd connect 'chat -s -v  "ABORT" "NO CARRIER" "" AT OK AT+CGDCONT=1,\"IP\",\"\" OK ATD*99**1# CONNECT' /dev/ttyUSB0 115200 nodetach debug noauth defaultroute usepeerdns  crtscts user card password card persist -pc -ac $* > $FILE &
//        echo "ppp is starting..."


//#       tmp=`cat $FILE | grep 'CONNECT finished'`
//#       if [ -n "$tmp" ]; then
//#           echo 'pppd connect ok !!!'
//##          echo "" > '/etc/resolv.conf'
//##          cat '/etc/ppp/resolv.conf' >> '/etc/resolv.conf'
//#           break
//#       else
//#           echo 'pppd connect failed'
//#           #route del default
//#           #route add default dev ppp0
//#       fi

//#       echo "" > '/etc/resolv.conf'
//        cat '/etc/ppp/resolv.conf' >> '/etc/resolv.conf'
//        cat 'nameserver 114.114.114.114' >> '/etc/resolv.conf'
//        route add default dev ppp0
//}



//写入DNS
int write_DNS(){
    char buff[256];
    int len;
    int res=1;
    char const *src_path = "/etc/ppp/resolv.conf";
    char const *des_path = "/etc/resolv.conf";
    int fd_src = open(src_path,O_RDONLY);
    int fd_des = open(des_path,O_RDWR|O_CREAT);
    if(fd_des>0){
        ftruncate(fd_des,0);     //清空文件
        lseek(fd_des,0,SEEK_SET);//重新设置文件偏移量

        memset(buff,0,sizeof(buff));
        strcpy(buff,"nameserver 114.114.114.114\r\n");
        len=write(fd_des,buff,strlen(buff));
        if(len > 0){
            res=0;
        }
        if(fd_src > 0){
            while((len = read(fd_src,buff,sizeof(buff)))>0 )  {
                write(fd_des,buff,len);
            }
        }
    }
    close(fd_src);
    close(fd_des);
    return res;
}

//获取wifi网络状态

//#include <string.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include <sys/types.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <QDebug>

int Module_4G::check_network_connect(char *device)
{
    int fp;
    //一定要只读模式打开,读写模式打开不可以
    ///sys/class/net/wlan0/carrier  0:down 1:up
    char network[50];
    memset(network,0,sizeof(network));
    sprintf(network,"/sys/class/net/%s/operstate",device);
    fp = open ("/sys/class/net/ppp0/operstate",O_RDONLY);
    if (fp<0) {
        printf("open file operstate failure%d\n",fp);
        return 1;
    }
    char status[20];
    memset(status,0,sizeof(status));
    read (fp,status,strlen(status));
    printf("status:%s\n",status);
    int res=0;
    if (NULL != strstr(status,"up"))   {
        printf("on line now \n");
    }
    else if (NULL != strstr(status,"down"))  {
        printf("off off \n");
        res=1;
    }
    else if(NULL != strstr(status,"unknown"))  {//unknown
        printf("unknow error\n");
    }
    close (fp);
    return res;
}



int Module_4G::check_network_connect2(char *device)
{
    system("cat /sys/class/net/ppp0/operstate > /home/eth0.txt");
    int fd;
    char *path="/home/eth0.txt";
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
        }
        else if (NULL != strstr(result,"down"))  {
            printf("off off \n");
        }
        else if (NULL != strstr(result,"unknown"))  {
            printf("unknow error\n");
            res=0;
        }
        close (fd);
    }
    return res;
}




//设备检查
int Module_4G::module_4G_check_status(const int fd){
    int res=1;
    int cnt=0;
    while (cnt<5) {
        cnt++;
        cmd_init(fd);
//        if(cmd_get_IMEI(fd)){
//            res=MODULE_4G_NO_IMEI;
//            continue;
//        }
        if(cmd_check_sim_card(fd)>0){
            qDebug()<<"no sim card";
            res=MODULE_4G_NO_SIM_CARD;
            continue;
        }
        cmd_signal(fd);
        if(cmd_check_register(fd)>0){
             qDebug()<<"register failed";
            res=MODULE_4G_FAILED_REGISTER;
            continue;
        }
        if(cmd_pppd()>0){
             qDebug()<<"pppd failed";
            res=MODULE_4G_FAILED_PPPD;
            continue;
        }

        for(int i=0;i<3;i++){
            if(write_DNS() == 0){
                 qDebug()<<"dns ok";
                 break;
            }else{
                qDebug()<<"dns failed";
   //            res=MODULE_4G_FAILED_DNS;
   //            continue;
            }
            sleep(1);
        }

        for(int i=0;i<5;i++){
            if(check_network_connect2("ppp0") == 0){
                return 0;
            }else{
                 qDebug()<<"ppp0 not up";
                res=MODULE_4G_FAILED_PING;
                sleep(1);
            }
        }
    }
    return res;
}
