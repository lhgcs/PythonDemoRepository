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
#include "UartQrcode.h"
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
#define   NONE          0
#define   HARDWARE      1
#define   SOFTWARE      2

#define B64_DEF_LINE_SIZE   72
#define B64_MIN_LINE_SIZE    4

static const char
  cb64[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
static const char
  cd64[] =
  "|$$$}rstuvwxyz{$$$$$$$>?@ABCDEFGHIJKLMNOPQRSTUVW$$$$$$XYZ[\\]^_`abcdefghijklmnopq";

//构造函数
UartQrcode::UartQrcode()
{
	this->qrcode_manager = new QNetworkAccessManager(this);

    this->msgtimer = new QTimer(this);
    this->msgBox=new myDialog();
    connect(this->msgtimer,SIGNAL(timeout()),this,SLOT(msgTimeout()));
}
//析够函数
UartQrcode::~UartQrcode()
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
InitSerialPort (char *ttyname, int BaudRate, char cBits, char cCheck,
		char cStops, int FlowCtrl)
{
    int fd;
    struct termios sp_settings;
    fd = open (ttyname, O_RDWR | O_NOCTTY | O_NONBLOCK);
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

  /* Set FlowCtrl */
    if (FlowCtrl == SOFTWARE)
    {
      sp_settings.c_cflag |= CRTSCTS;
    }
    else if (FlowCtrl == SOFTWARE)
    {
      sp_settings.c_iflag |= (IXON | IXOFF | IXANY);
    }
    else
    {
      sp_settings.c_iflag &= ~(IXON | IXOFF | IXANY);
      sp_settings.c_cflag &= ~CRTSCTS;
    }
    tcsetattr (fd, TCSANOW, &sp_settings);
    tcflush (fd, TCIFLUSH);
    return fd;
} 

int UartQrcode::openserialport ()
{
  char ttyname[TEMP_STR_LEN];
  int BaudRate;
  char cBits;
  char cCheck;
  char cStops;
  //int FlowCtrl;
  memset (ttyname, 0, TEMP_STR_LEN);
  sprintf (ttyname, "/dev/ttyAMA3");
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

void UartQrcode::ReplyQrcode(QNetworkReply* reply)
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

    char bbuf[100];/////////////////////////////////////////
    memset(bbuf, 0x0, sizeof(bbuf));
    sprintf(bbuf,"QR receive buff=%s\n", text);
    printf("%s\n",bbuf);

    if (!json) 
    {
        printf("json error\n");
        sprintf(logger, "Error before:[%s]", cJSON_GetErrorPtr());
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
        printf("json ok\n");
        sprintf(logger, "JSON item:%s\n",(str.toLatin1()).data());
        printf("%s\n", logger);
        qDebug(logger);
			
        result = cJSON_GetObjectItem(json, "result")->valueint;
        if(result==1)
        {
            printf("result=1\n");
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
                uart_ctrl(atoi(lockRoomId), atoi(boxId));
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
 
            //memset(logger, 0x0, sizeof(logger));
            //sprintf(logger, "errMsg:%s\n",errMsg);
            //printf("%s\n", logger);
            //qDebug(logger);
         }
    }
    if((msgBox->text())!=NULL)
    {
        this->msgBox->show();
        this->msgtimer->start(10000);
    }
    cJSON_Delete(json);
}

void UartQrcode::msgTimeout()
{
    this->msgBox->close();
    this->msgtimer->stop();
}


void UartQrcode::PostQrcode(QString qrcode)
{
        QNetworkRequest request;

        FileWR fr,fr_uid;
        QString sys_info = fr.Read("/opt/sys_info.txt");
        QString uid = fr_uid.Read("/opt/uid.txt");

        if(sys_info!="error")
        {
            QStringList sections = sys_info.split(",");
            QString strurl = "http://"+sections[0]+":"+sections[4]+"/app_set_barcode.jsp?uid="+uid+"&barCode="+qrcode;
            request.setUrl(QUrl(strurl));

            QEventLoop loop;
            QTimer requestTime;
            requestTime.setSingleShot(true);//只会触发一次
            QNetworkReply *reply = this->qrcode_manager->get(request);
            connect(this->qrcode_manager, SIGNAL(finished(QNetworkReply*)), &loop, SLOT(quit()));
            connect(&requestTime, SIGNAL(timeout()), &loop, SLOT(quit()));
            requestTime.start(4000);
            loop.exec();                    // 进入事件循环， 直到reply的finished()信号发出， 这个语句才能

            if(requestTime.isActive())
            {
                requestTime.stop();
                this->ReplyQrcode(reply);
            }
            else                             //timeout
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
        }
}
		
int UartQrcode::ParseRecvData(unsigned char *data, int len)
{
    unsigned char tmpstr[TEMP_STR_LEN]; 
    QString nn;
    
    if(data[0] != '{' || data[len-1] != '}')
    {
         printf("ParseRecvData Err\r\n");
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
         nn = QString(QLatin1String((char *)tmpstr));
        this->PostQrcode(nn);
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

void UartQrcode::run()
{
    int pos, pos1;
    unsigned char recvstr[TEMP_STR_LEN]; 
    unsigned char recvbuf[TEMP_STR_LEN*2];
    unsigned char data[TEMP_STR_LEN];

    int fd = openserialport (); 
    if (fd <= 0)
    {
      printf ("Can't open serialport ttyAMA3 QR_CORE.\r\n");
      return;
    }

    int res;
    int recv_len = 0;
    int data_len;
    memset(recvbuf, 0, TEMP_STR_LEN*2);
    while(1)
    {
       res = read (fd, recvstr, TEMP_STR_LEN);
       if (res > 0)
       { 
          memcpy(recvbuf + recv_len, recvstr, res);
          recv_len += res; 
          if(recv_len>TEMP_STR_LEN)
          {
              recv_len = 0;
          }

          pos = FindFirstChar(recvbuf, recv_len,  '}');
          if(pos >= 0)
          {
                pos1 = FindFirstChar(recvbuf, pos,  '{');
                if(pos1 == -1) //err data
                {
                      memcpy(recvbuf, recvbuf+pos+1,  recv_len - pos -1);
                      recv_len = recv_len - pos -1; 
                }
                else
                {
                      data_len = pos - pos1 + 1;
                      memcpy(data, recvbuf + pos1,  data_len);
                      this->ParseRecvData(data,  data_len);
                     
                      memcpy(recvbuf, recvbuf + pos + 1, recv_len - pos -1 ); 
                      recv_len = recv_len - pos -1;
                }
          } 
       }
        else if(res<0)
       {
           this->msgBox->setText("读二维码出错");
           this->msgBox->show();
           this->msgtimer->start(8);
         printf("read error\n");
       }

        usleep(100000);//10ms   扫二维码不灵敏速度要快些
    }  
}  


static int 
SendStrToUart(int fd, int UartNo, char *str)
{
     unsigned char tmpstr[TEMP_STR_LEN]; 
     unsigned char sendstr[TEMP_STR_LEN]; 
     memset (tmpstr, 0, TEMP_STR_LEN);
     memset (sendstr, 0, TEMP_STR_LEN); 
     int len = base64_encode_str ((unsigned char*)str, strlen(str), tmpstr);
     sendstr[0] = 0x7b;
     sendstr[1] = 0x02;
     sendstr[2] = UartNo;
     memcpy(&(sendstr[3]), tmpstr, len);
     sendstr[len+3] = 0x7d;         
     write(fd, sendstr, len+4);
     int i;
     for(i=0;i<len+4;i++)
     {
          printf("%.2x,", sendstr[i]);
     }
     printf("\r\n");
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

