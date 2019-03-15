#include "serial.h"

Serial::Serial(){
    this->fd=-1;
}
//尝试通信,检测状态
void Serial::cmd_package(uchar cmd, uchar pack[]){
    pack[0]=0xAB;                   //包头AB
    pack[1]=0x01;                   //包标识
    pack[2]=0x01;                   //数据长度1
    pack[3]=cmd;                    //指令
    pack[4]=pack[1]+pack[2]+pack[3];//校验和
    pack[5]=0xBA;                   //包尾BA
}
//商品出货
void Serial::open_package(uchar cmd,uchar data,uchar pack[]){
    pack[0]=0xAB;                           //包头AB
    pack[1]=0x01;                           //包标识
    pack[2]=0x02;                           //数据长度 2
    pack[3]=cmd;                            //指令
    pack[4]=data;                           //
    pack[5]=pack[1]+pack[2]+pack[3]+pack[4];//校验和
    pack[6]=0xBA;                            //包尾BA
}

void Serial::open_package(uchar cmd,uchar data1,uchar data2,uchar pack[]){
    pack[0]=0xAB;                           //包头AB
    pack[1]=0x01;                           //包标识
    pack[2]=0x03;                           //数据长度 2
    pack[3]=cmd;                            //指令
    pack[4]=data1;
    pack[5]=data2;
    pack[6]=pack[1]+pack[2]+pack[3]+pack[4]+pack[5];//校验和
    pack[7]=0xBA;                                   //包尾BA
}

int Serial::send_cmd(int fd,uchar cmd){
    if(fd>0){
        uchar pack[6]={0};
        cmd_package(cmd,pack);
        int len=write(fd, pack, sizeof(pack));
        usleep(100000);
//        printf("cmd=%x\n",cmd);
//        for(unsigned int i=0;i<sizeof(pack);i++){
//            printf("%02x ",pack[i]);
//        }
//        printf("\n");
        printf_hex(pack,6);
        return len;
    }
    else{
        return -1;
    }
}
int Serial::send_open(int fd, uchar cmd, uchar data1){
    if(fd>0){
        uchar pack[8]={0};
        open_package(cmd,data1,pack);
        usleep(100000);
        int len=write(fd, pack, sizeof(pack));
        usleep(1000);
        printf_hex(pack,8);
        return len;
    }
    else{
        return -1;
    }
}
//int Serial::send_open(int fd, uchar cmd, uchar data1,uchar data2){
//    if(fd>0){
//        uchar pack[8]={0};
//        open_package(cmd,data1,data2,pack);
//        usleep(100000);
//        int len=write(fd, pack, sizeof(pack));
//        usleep(1000);
//        printf_hex(pack,8);
//        return len;
//    }
//    else{
//        return -1;
//    }
//}
int Serial::send_open(int fd, int cmd, int data1, int data2){
    if(fd>0){
        uchar pack[8]={0};
        open_package(cmd,data1,data2,pack);
        usleep(100000);
        int len=write(fd, pack, sizeof(pack));
        usleep(1000);
        printf_hex(pack,8);
        return len;
    }
    else{
        return -1;
    }
}
//int Serial::send_open(int fd, uchar cmd, int type, uint weight){
//    if(fd>0){
//        uchar pack[13];
//        memset(pack,0,13);
//        pack[0]=0xAB;  //包头AB
//        pack[1]=0x01;  //包标识
//        pack[2]=0x08;  //数据长度 8
//        pack[3]=cmd;   //指令
//        pack[4]=type;

//        pack[5]=weight/100000%10;
//        pack[6]=weight/10000%10;
//        pack[7]=weight/1000%10;
//        pack[8]=weight/100%10;
//        pack[9]=weight/10%10;
//        pack[10]=weight%10;

//        pack[11]=pack[1]+pack[2]+pack[3]+pack[4]+pack[5]+pack[6]+pack[7]+pack[8]+pack[9]+pack[10];//校验和
//        pack[12]=0xBA;

//        usleep(100000);
//        int len=write(fd, pack, sizeof(pack));
//        usleep(1000);
//        printf_hex(pack,13);
//        return len;
//    }
//    else{
//        qDebug()<<"fd=-1";
//        return -1;
//    }
//}

int Serial::data_prser(QVector<uchar> &pack,QVector<uchar> &data)//提取有效数据
{
    data.clear();
    int data_start=pack.indexOf(0xAB);  //包头AB
    if(data_start>=0){
        int data_end=pack.indexOf(0xBA);//包尾BA
        if((data_end>=0)&&(data_start<data_end)){
            printf("data=");
            for(int i=data_start;i<=data_end;i++){
                data.append(pack.at(i));
                printf("%02x ",pack.at(i));
            }
            printf("\n");

            uchar sum_check=pack.at(data_end-1);
            uchar sum=0;
            for(int i=data_start+1;i<data_end-1;i++){//校验
                sum+=pack.at(i);
            }
            pack.erase(pack.begin(),pack.begin()+data_end+1);//删除[beg,end)区间的数据，传回下一个数据的位置
            if(sum==sum_check){
                return 0;
            }
            else{
                return -3;//校验出错
            }
        }
        else{
            return -2;//无包尾BA
        }
    }
    else{
        pack.clear();//vector的尺寸(size)将变成zero. 但它的容量(capacity)却并不发生变化, vector本身并不释放任何内存
        return -1; //无包头AB
    }
}

int Serial::InitSerialPort(char *ttyname, int BaudRate, char cBits, char cCheck, char cStops, int FlowCtrl)
{
    struct termios sp_settings;
    fd = open (ttyname, O_RDWR | O_NOCTTY);  //O_NOCTTY 表示不能把本串口当成控制终端  O_NONBLOCK非阻塞方式
    if (fd < 0){
      printf ("Can't open serial\n");
      return -1;
    }
    memset (&sp_settings, 0, sizeof (sp_settings));
    tcflush (fd, TCIFLUSH);                 //丢弃所有未读取的接收到的数据
    sp_settings.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);//规范输入|回送输入数据|回送擦除字符|使能SIGINTR，SIGSUSP， SIGDSUSP和 SIGQUIT 信号，这些都关闭
    //sp_settings.c_oflag &= ~OPOST;        //原始数据（RAW）输出
    cfsetispeed (&sp_settings, BaudRate);   //设置端口的输入波特率
    cfsetospeed (&sp_settings, BaudRate);   //设置端口的输出波特率
    sp_settings.c_cflag |= (CLOCAL | CREAD);//忽略modem信号线|打开接收功能
    //设置数据位数
    switch (cBits) {
        case '8':sp_settings.c_cflag |= CS8;//数据位为8bit
            break;
        case '7':sp_settings.c_cflag |= CS7;//数据位为7bit
            break;
        default:sp_settings.c_cflag |= CS8;//数据位为8bit
            break;
    }
    //奇偶校验位
    switch (cCheck) {
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
    //设置停止位
    switch (cStops) {
        case '1':sp_settings.c_cflag &= ~CSTOPB; //1位停止位
            break;
        case '2':sp_settings.c_cflag |= CSTOPB;  //2位停止位
            break;
        default: sp_settings.c_cflag &= ~CSTOPB; //1位停止位
            break;
    }
    //数据流控制
    switch (FlowCtrl) {
        case HARDWARE:sp_settings.c_cflag |= CRTSCTS;               //硬件流控制
            break;
        case SOFTWARE:sp_settings.c_iflag |= (IXON | IXOFF | IXANY);//软件流控制 //IXON 打开输出控制，IXOFF 打开输入控制
            break;
        default:      sp_settings.c_iflag &= ~(IXON | IXOFF | IXANY);//不使用流控制
                      sp_settings.c_cflag &= ~CRTSCTS;
            break;
    }
    sp_settings.c_cc[VMIN] = 0;                //读取字符的最小数量，至少要读到0个字符才会返回
    sp_settings.c_cc[VTIME] =0;                //读取第一个字符的等待时间*(1/10)s
    if(tcsetattr (fd, TCSANOW, &sp_settings)!=0)//将修改后数据设置到串口中
    {
        perror("set uart error!!!");
        return -1;
    }
    tcflush (fd, TCIFLUSH);
    return fd;
}


void Serial::printf_hex(uchar *Str, int len){
    QString buff;
    for(int i=0;i<len;i++){
//            buff.append(QString::number(Str[i],16));
         buff.append(QString("%1 ").arg(Str[i],2,16,QLatin1Char('0')));//2代表宽度，16表示16进制，空位补零
    }
    qDebug()<<buff;
}
