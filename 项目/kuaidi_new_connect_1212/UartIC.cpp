#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <linux/ioctl.h>
#include <linux/types.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/serial.h>
#include <termios.h> 
#include "UartIC.h"
#include <QMessageBox>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QtDebug>
#include <QFile>
#include <QTextStream>
#include "configure.h"
#include "filewr.h"
#include "json.h"
#include "cJSON.h"
#include <time.h>
#include "uart.h"


#define   TEMP_STR_LEN  512
#define   NONE                      0
#define   HARDWARE                  1
#define   SOFTWARE                  2

#define B64_DEF_LINE_SIZE   72
#define B64_MIN_LINE_SIZE    4

static const char
  cb64[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
static const char
  cd64[] =
  "|$$$}rstuvwxyz{$$$$$$$>?@ABCDEFGHIJKLMNOPQRSTUVW$$$$$$XYZ[\\]^_`abcdefghijklmnopq";

//构造函数
UartIC::UartIC()
{
    this->ic_manager = new QNetworkAccessManager(this);

    this->msgtimer = new QTimer(this);
    this->msgBox=new myDialog();
    connect(this->msgtimer,SIGNAL(timeout()),this,SLOT(msgTimeout()));
}
//析够函数
UartIC::~UartIC()
{
}
  
static void
encodeblock (unsigned char in[3], unsigned char out[4], int len)
{
  out[0] = cb64[in[0] >> 2];
  out[1] = cb64[((in[0] & 0x03) << 4) | ((in[1] & 0xf0) >> 4)];
  out[2] =
    (unsigned char) (len >
		     1 ? cb64[((in[1] & 0x0f) << 2) | ((in[2] & 0xc0) >> 6)] :
		     '=');
  out[3] = (unsigned char) (len > 2 ? cb64[in[2] & 0x3f] : '=');
}

static int
base64_encode_str (unsigned char *instr, int inlen, unsigned char *outstr)
{
  unsigned char in[3], out[4];
  int inc = 0, outc = 0, i, len;

  /* only size of buffer required */
    if (!outstr)
    {
        while (inc < inlen)
        {
            len = 0;
            for (i = 0; i < 3; i++)
            {
                if (inc++ < inlen)
                {
                  len++;
                }
            }
            if (len)
            {
              outc += 4;
            }
        }
        return outc;
    }

  //assert(outstr);   
    while (inc < inlen)
    {
        len = 0;
        for (i = 0; i < 3; i++)
        {
            in[i] = instr[inc];
            if (inc++ < inlen)
            {
                len++;
            }
            else
            {
                in[i] = 0;
            }
        }
        if (len)
        {
            encodeblock (in, out, len);
            for (i = 0; i < 4; i++)
            {
                outstr[outc++] = out[i];
            }
        }
    }
    return outc;
}

static void
decodeblock (unsigned char in[4], unsigned char out[3])
{
  out[0] = (unsigned char) (in[0] << 2 | in[1] >> 4);
  out[1] = (unsigned char) (in[1] << 4 | in[2] >> 2);
  out[2] = (unsigned char) (((in[2] << 6) & 0xc0) | in[3]);
}

static int
base64_decode_str (const unsigned char *instr, int inlen,
		   unsigned char *outstr)
{
  unsigned char in[4], out[3], v;
  int inc = 0, outc = 0, i, len;

  /* only size of buffer required */
  if (!outstr)
    {
      while (inc < inlen)
	{
	  for (len = 0, i = 0; i < 4 && inc < inlen; i++)
	    {
	      v = 0;
	      while ((inc < inlen) && (v == 0))
		{
		  v = instr[inc++];
		  v =
		    (unsigned char) ((v < 43 || v > 122) ? 0 : cd64[v - 43]);
		  if (v)
		    {
		      v = (unsigned char) ((v == '$') ? 0 : v - 61);
		    }
	      } if (inc < inlen)
		{
		  len++;
		}
	    }
	  if (len > 1)
	    {
	      outc += (len - 1);
	    }
	}
      return outc;
    }

  //assert(outstr);   
  while (inc < inlen)
    {
      for (len = 0, i = 0; i < 4 && inc < inlen; i++)
	{
	  v = 0;
	  while ((inc < inlen) && (v == 0))
	    {
	      v = instr[inc++];
	      v = (unsigned char) ((v < 43 || v > 122) ? 0 : cd64[v - 43]);
	      if (v)
		{
		  v = (unsigned char) ((v == '$') ? 0 : v - 61);
		}
	  } if (inc < inlen)
	    {
	      len++;
	      if (v)
		{
		  in[i] = (unsigned char) (v - 1);
		}
	    }

	  else
	    {
	      in[i] = 0;
	    }
	}
      if (len)
	{
	  decodeblock (in, out);
	  for (i = 0; i < len - 1; i++)
	    {
	      outstr[outc++] = out[i];
	    }
	}
    }
  return outc;
}
  
static int
InitSerialPort (char *ttyname, int BaudRate, char cBits, char cCheck,char cStops, int FlowCtrl)
{
    int fd;
    struct termios sp_settings;
    bzero( &sp_settings, sizeof(sp_settings));//初始化

    fd = open (ttyname, O_RDWR | O_NOCTTY | O_NONBLOCK);//非阻塞
    if (fd < 0)
    {
      printf ("Can't open device %s\n", ttyname);
      return -1;
    }
    memset (&sp_settings, 0, sizeof (sp_settings));
    tcflush (fd, TCIFLUSH);

    /* Set input mode to raw */
    sp_settings.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);

    /* Set baudrate */
    cfsetispeed (&sp_settings, BaudRate);
    cfsetospeed (&sp_settings, BaudRate);
    sp_settings.c_cflag |= (CLOCAL | CREAD);

    /* Set cBits */
    if (cBits == '8')
    {
        sp_settings.c_cflag |= CS8;
    }
    else
    {
        sp_settings.c_cflag |= CS7;
    }

    /* Set cCheck */
    if ((cCheck == 'N') || (cCheck == 'n'))
    {
        sp_settings.c_cflag &= ~PARENB;
    }
    else
    {
        sp_settings.c_cflag |= PARENB;
        if ((cCheck == 'E') || (cCheck == 'e'))
        {
            sp_settings.c_cflag &= ~PARODD;
        }
        else
        {
            sp_settings.c_cflag |= PARODD;
        }
            sp_settings.c_iflag |= (INPCK | ISTRIP);
    }

    /* Set cStops */
    if (cStops == '1')
    {
        sp_settings.c_cflag &= ~CSTOPB;
    }
    else
    {
        sp_settings.c_cflag |= CSTOPB;
    }

    /* Set FlowCtrl */  //硬件流控制
    if (FlowCtrl == HARDWARE)
    {
        sp_settings.c_cflag |= CRTSCTS;                // 开启硬件流控
        sp_settings.c_iflag &= ~(IXON | IXOFF | IXANY);// 关闭软件流控
    }
    else if (FlowCtrl == SOFTWARE)
    {
        sp_settings.c_cflag &= ~CRTSCTS;              // 关闭硬件流控
        sp_settings.c_iflag |= (IXON | IXOFF | IXANY);// 开启软件流控
    }
    else
    {
        sp_settings.c_cflag &= ~CRTSCTS;               // 关闭硬件流控
        sp_settings.c_iflag &= ~(IXON | IXOFF | IXANY);// 关闭软件流控
    }

    sp_settings.c_cc[VTIME] = 0;// 设置等待时间和最小接收字符
    sp_settings.c_cc[VMIN] = 0;
    tcflush (fd, TCIFLUSH);

    if( tcsetattr (fd, TCSANOW, &sp_settings)!=0)//激活新配置
    {
        printf("\nserialport set error\n");
        return -1;
    }
    return fd;
} 

int UartIC::openserialport ()//加命名空间
{
  char ttyname[TEMP_STR_LEN];
  int BaudRate;
  char cBits;
  char cCheck;
  char cStops;
 // int FlowCtrl;
  memset (ttyname, 0, TEMP_STR_LEN);
  sprintf (ttyname, "/dev/ttyAMA2");

  BaudRate = B115200;
  cBits = '8';
  cCheck = 'N';
  cStops = '1';
  return InitSerialPort (ttyname, BaudRate, cBits, cCheck, cStops, NONE);
}

static int 
FindFirstChar(unsigned char *str, int len,  unsigned char c)
{
     int i;
     for(i=0;i<len;i++)
     {
          if(str[i] == c)
          {
                return i;
          }
     }
     return -1;
}

void UartIC::ReplyIC(QNetworkReply* reply)//接受服务器数据，JON解析,开箱，开哪个
{
    QByteArray data = reply->readAll();
    //这里会输出百度首页的HTML网页代码
    reply->close();
    reply->deleteLater();
    QString str=QByteArray(data);
    str.replace("\n","");

    char logger[200];
    int result=2;
    memset(logger, 0x0, sizeof(logger));

    cJSON* pArray = cJSON_CreateArray();
    cJSON *json;

    QByteArray ba = str.toUtf8();
    char *text = ba.data();

    json=cJSON_Parse(text);

    char bbuf[100];
    memset(bbuf, 0x0, sizeof(bbuf));
    sprintf(bbuf,"receive data=%s\n", text);
    printf("%s\n",bbuf);

    if (!json) 
    {
        sprintf(logger, "Error before:[%s]\n", cJSON_GetErrorPtr());
        printf("%s\n", logger);
        qDebug(logger);
        if(this->msgtimer->isActive())
        {
           this->msgtimer->stop();
        }
        this->msgBox->setText("服务器正在维护，请稍后再取件");
    }
    else
    {
        result = cJSON_GetObjectItem(json, "result")->valueint;

        if(result==1)
        {
            pArray = cJSON_GetObjectItem(json, "lockRoom");
            if (NULL == pArray)
            {
                return;
            }

            int iCount = cJSON_GetArraySize(pArray);

            for (int i = 0; i < iCount; ++i)
            {
             cJSON* pItem = cJSON_GetArrayItem(pArray, i);
             if (NULL == pItem)
             {
                 continue;
             }

             char* lockRoomId = cJSON_GetObjectItem(pItem, "lockRoomId")->valuestring;
             char* boxId = cJSON_GetObjectItem(pItem, "boxIds")->valuestring;
             char* errMsg = cJSON_GetObjectItem(json, "errMsg")->valuestring;
             if(this->msgtimer->isActive())
            {
                this->msgtimer->stop();
            }
             this->msgBox->setText(QString::fromUtf8(errMsg));

             //sprintf(logger, "open the box:lockRoomId=%s, boxId=%s\n",lockRoomId, boxId);
             //printf("%s\n", logger);
             //qDebug(logger);
             uart_ctrl(atoi(lockRoomId), atoi(boxId));//开箱
            }
         }
         else
         {
            printf("result=2\n");
            char* errMsg = cJSON_GetObjectItem(json, "errMsg")->valuestring;
            if(this->msgtimer->isActive())
           {
               this->msgtimer->stop();
           }
            this->msgBox->setText(QString::fromUtf8(errMsg));
         }
    }
    if((msgBox->text())!=NULL)
    {
        this->msgBox->show();
        this->msgtimer->start(10000);
    }
    cJSON_Delete(json);
}

void UartIC::msgTimeout()
{
    this->msgBox->close();
    this->msgtimer->stop();
}

void UartIC::PostIC(QString iccode)//有IC,发送数据
{
        QNetworkRequest request;//创建一个请求

        FileWR fr,fr_uid;
        QString sys_info = fr.Read("/opt/sys_info.txt");
        QString uid = fr_uid.Read("/opt/uid.txt");

        if(sys_info!="error")
        {
            QStringList sections = sys_info.split(",");
            //QString strurl = "http://192.168.1.52:80/app_set_barcode.jsp?uid=1$585601225&barCode=31263434";
            QString strurl = "http://"+sections[0]+":"+sections[4]+"/app_get_goods_by_sn.jsp?uid="+uid+"&sn="+iccode;
            qDebug()<<strurl;
            printf("%s\n", strurl.toLatin1().data());
            request.setUrl(QUrl(strurl));

            QEventLoop loop;                //使用事件循环使得网络通讯同步进行
            QTimer requestTime;
            requestTime.setSingleShot(true);//只会触发一次
            QNetworkReply *reply = this->ic_manager->get(request);
            connect(this->ic_manager, SIGNAL(finished(QNetworkReply*)), &loop, SLOT(quit()));
            connect(&requestTime, SIGNAL(timeout()), &loop, SLOT(quit()));
            requestTime.start(4000);
            loop.exec();                    //进入事件循环， 直到reply的finished()信号发出， 这个语句才能

            if(requestTime.isActive())      //定时器还在工作，即在规定时间内下载完成
            {
                requestTime.stop();
                this->ReplyIC(reply);
            }
            else                            //timeout，没下载完成
            {
                reply->close();//abort不行
                if(this->msgtimer->isActive())
                {
                this->msgtimer->stop();
                }
                this->msgBox->setText("请求超时，网络不稳定或者网络断开");
                this->msgBox->show();
                this->msgtimer->start(10000);
            }
            //printf("IC request success\n");
        }
}

int UartIC::ParseRecvData(unsigned char *data, int len)//串口数据解密,获得IC卡号，发送请求                                                                                   接受服务器数据
{
    unsigned char tmpstr[TEMP_STR_LEN]; 
    char ic_code[32];
    QString nn;
    
    if(data[0] != 0x20 || data[len-1] != 0x03)//'{'   '}'
    {
         printf("Parse Recv Data Err\r\n");
         return -1;
    }

    switch(data[1])
    {
         case 0x00:  //reset msg 
             printf("MCU Reset\r\n");        
             break;
         case 0x01:  //uart params
             printf("UART%x:\r\n", data[2]);
             switch(data[3])
             {
                  case 0x00:
                      printf("300,");
                      break;
                  case 0x01:
                      printf("1200,");
                      break;
                  case 0x02:
                      printf("4800,");
                      break;
                  case 0x03:
                      printf("9600,");
                      break;
                  case 0x04:
                      printf("19200,");
                      break;
                  case 0x05:
                      printf("38400,");
                      break;
                  case 0x06:
                      printf("57600,");
                      break;
                  case 0x07:
                      printf("115200,");
                      break;
             }
             printf("%x, %c, %x", data[4], data[5], data[6]);
             break;
         case 0x02:  //recv data
             memset(tmpstr, 0, TEMP_STR_LEN);
             base64_decode_str (data + 3, len-4, tmpstr);
             printf("UART%x: %s\r\n", data[2], tmpstr);

             #if 0
			 for(i=0; i<sizeof(tmpstr); i++)
			 {
				 printf("%02X,", tmpstr[i]);
			 }
             #else

             tmpstr[0]=tmpstr[0];
             memset(ic_code,0,sizeof(ic_code));
             if(tmpstr[0]==0x20)
			 {
				 printf("IC No: %02X%02X%02X%02X\n", tmpstr[5],tmpstr[6],tmpstr[7],tmpstr[8]);					 
				 sprintf(ic_code, "%02X%02X%02X%02X", tmpstr[5],tmpstr[6],tmpstr[7],tmpstr[8]);	
				 nn = QString(QLatin1String(ic_code)); 			 
                this->PostIC(nn);//////////IC，请求
			 }
             #endif

             break;
         case 0x03:  //IO data
             printf("IO%x=%x\r\n", data[2], data[3]);
             break;            
    }
    return 0;
}

typedef struct {
  int fd;
} param_t;

void UartIC::run()//读取IC
{
    int pos, pos1;
    unsigned char recvstr[TEMP_STR_LEN]; 
    unsigned char recvbuf[TEMP_STR_LEN*2];
    unsigned char data[TEMP_STR_LEN];

    unsigned char sendbuf[10]={0x20, 0x00, 0x26, 0x04, 0x00, 0x00, 0x01, 0x03, 0xDF, 0x03};
                            //起始符         命令 数据长度                        校验  结束符
    int fd = openserialport (); //ttyAMA2
    if (fd <= 0)
    {
      printf ("Can't open serialport ttyAMA2 IC.\r\n");
      return;
    }

    //设置自动采集数据
    SendStrToUart(fd, 0, (char *)sendbuf);	//向串口UartNo=0写入数据//设置自动读卡
    //SendStrToUart(fd, 1, (char *)sendbuf);
    int res;
    int recv_len = 0;
    int data_len;
    memset(recvbuf, 1, TEMP_STR_LEN*2);//每个字节的内容全部设置为0

    while(1)
    {
        res = read (fd, recvstr, TEMP_STR_LEN);//串口读IC数据
        if (res > 0)
        {
            printf("read IC\n");

            printf("data len=%d\n",res);
            for(int j=0;j<res;j++)
            {
             printf("%2x ",recvstr[j]);
            }
            printf("\n");

            memcpy(recvbuf + recv_len, recvstr, res);//拷贝res个字节到recvbuf + recv_len
            recv_len += res;
            if(recv_len>TEMP_STR_LEN)//读到的长度太长   帧长最大不能超过64字节，且帧长必须等于数据长度加6
            {
              recv_len = 0;
            }

            pos = FindFirstChar(recvbuf, recv_len,0x03);   //数据里有 ‘}’  终止符
            if(pos >= 0)
            {
                printf("IC's data has ending flag\n");
                pos1 = FindFirstChar(recvbuf, pos,0x20);  //‘}’ 之前的数据里有 '{' 开始符
                if(pos1 == -1) //err data
                {
                    printf("IC's data error,it has not starting flag\n");
                      memcpy(recvbuf, recvbuf+pos+1,  recv_len - pos -1);
                      recv_len = recv_len - pos -1;
                }
                else
                {
                      data_len = pos - pos1 + 1;            //{}之间的数据长度
                      memcpy(data, recvbuf + pos1,  data_len);

                      for(int j=0;j<data_len;j++)
                      {
                       printf("%2x ",data[j]);
                      }
                      printf("\n");
                      printf("parsed IC's data\n");
                      this->ParseRecvData(data,  data_len); //base64  解码{}之间的数据
                      memcpy(recvbuf, recvbuf + pos + 1, recv_len - pos -1 ); //recvbuf的}之后的数据放到recvbuf中
                      recv_len = recv_len - pos -1;
                }
            }
            else
            {
                printf("IC's data has not ending flag\n");
            }
       }
      else if(res<0)
      {
           this->msgBox->setText("读IC卡出错");
           this->msgBox->show();
           this->msgtimer->start(100);
           printf("IC read error\n");

        usleep(500000);
      }

       usleep(500000);//500ms
    }
}


//主机发送命令至模块，连续的发送STX (0x20)+数据块+0x03(结束符)。通过判断TX522B返回数据的正确性来判断TX522B是否正确执行了本条命令
int UartIC::SendStrToUart(int fd, int UartNo, char *str)//设置串口IC,自动读卡
{
     unsigned char tmpstr[TEMP_STR_LEN]; 
     unsigned char sendstr[TEMP_STR_LEN]; 
     memset (tmpstr, 0, TEMP_STR_LEN);
     memset (sendstr, 0, TEMP_STR_LEN); 
     int len = base64_encode_str ((unsigned char*)str, strlen(str), tmpstr);//str----->tmpstr   //base64加密
     sendstr[0] = 0x7b;
     sendstr[1] = 0x02;
     sendstr[2] = UartNo;
     memcpy(&(sendstr[3]), tmpstr, len);//tmpstr
     sendstr[len+3] = 0x7d;         

    if(write(fd, sendstr, len+4)<=0)//往串口写入   { 02, UartNo=0,命令数据 }
        printf("write error\n");

    for(int i=0;i<len+4;i++)
    {
      printf("%.2x,", sendstr[i]);
    }
    printf("");
    printf("\r\nset auto read IC\n");
    return 0;
}


static int 
SetUartParam(int fd, int UartNo)
{ 
     unsigned char sendstr[TEMP_STR_LEN];  
     memset (sendstr, 0, TEMP_STR_LEN); 
     sendstr[0] = 0x7b;   //'{'
     sendstr[1] = 0x00;
     sendstr[2] = UartNo;

     sendstr[3] = 0x04;   //B19200
     sendstr[4] = 0x08;   //8
     sendstr[5] = 'N';    //N
     sendstr[6] = 1;      //1

     sendstr[7] = 0x00; 
     sendstr[8] = 0x00; 
     sendstr[9] = 0x00; 
     sendstr[10] = 0x00;  
     sendstr[11] = 0x7d;   //'}'         
     write(fd, sendstr, 12);
     return 0;
}

static int 
SetUartParam2(int fd, int UartNo)
{ 
     unsigned char sendstr[TEMP_STR_LEN];  
     memset (sendstr, 0, TEMP_STR_LEN); 
     sendstr[0] = 0x7b;   //'{'
     sendstr[1] = 0x00;
     sendstr[2] = UartNo;

     sendstr[3] = 0x03;   //B9600
     sendstr[4] = 0x08;   //8
     sendstr[5] = 'N';    //N
     sendstr[6] = 1;      //1

     sendstr[7] = 0x00; 
     sendstr[8] = 0x00; 
     sendstr[9] = 0x00; 
     sendstr[10] = 0x00;  
     sendstr[11] = 0x7d;   //'}'         
     write(fd, sendstr, 12);
     return 0;
}

static int 
SendIOData(int fd, int IONo, int IOData)
{ 
     unsigned char sendstr[TEMP_STR_LEN];  
     memset (sendstr, 0, TEMP_STR_LEN); 
     sendstr[0] = 0x7b;   //'{'
     sendstr[1] = 0x03;
     sendstr[2] = IONo;
     sendstr[3] = IOData;  
     sendstr[4] = 0x7d;   //'}'         
     write(fd, sendstr, 5);
     return 0;
}

static int 
GetUartParam(int fd, int UartNo)
{ 
     unsigned char sendstr[TEMP_STR_LEN];  
     memset (sendstr, 0, TEMP_STR_LEN); 
     sendstr[0] = 0x7b;   //'{'
     sendstr[1] = 0x01;
     sendstr[2] = UartNo; 
     sendstr[3] = 0x7d;   //'}'         
     write(fd, sendstr, 4);
     return 0;
}

