#include "serialclient.h"
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>

#include <termios.h>
#include <errno.h>
#include <sys/select.h>
#include <string.h>
#include <QByteArray>

SerialClient::SerialClient(const QString &Device, QObject *parent):QThread(parent)
{
	int ret = 0;
	/*
	//打开串口
	mFd = open(Device, O_RDWR| O_NOCTTY );
	if(mFd < 0)
	{
		printf("can't open device\r\n");
	   
	}*/
    QByteArray ba = Device.toLatin1();

		//打开串口
    mFd = open(ba.data(), O_RDWR| O_NOCTTY | O_NDELAY);
	if(mFd < 0)
	{
		printf("can't open device\r\n");
		return ;	
	}

	//  2. 判断串口是否为阻塞状态 
	ret = fcntl(mFd, F_SETFL, 0);
	if(ret < 0)
	{
		printf("fcntl failed!\r\n");
		return ;
	}
	else
	{
		printf("fcntl =%d\r\n", ret);
	}

    isRun = 1;
}

SerialClient::~SerialClient()
{
	if(mFd > 0)
	{
        eixt();
		close(mFd);
	}
}

int SerialClient::getFd()
{
	return mFd;
}

int SerialClient::setInterface(ProtocolInterface *Interface)
{
	mInterface = Interface;

    return 0;
}

int SerialClient::setParams(UartBitrate Rate, UartDataBit DBit, UartStopBit SBit, UartFlowControl Fcontrl, UartParity Parity)
{

	struct termios newtio, oldtio;
    if(mFd < 0)
    {
        return -1;
    }

	/* get the default setting for the uart */
	if(tcgetattr(mFd, &oldtio) != 0)
	{
		perror("Setup Serial 1");
		return -1;	
	}
	
	/* CLOCAL : no modem; 
	 * CREAD  : can read from the uart
	 */
	bzero(&newtio, sizeof(newtio));
	newtio.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);
	newtio.c_cflag |= CLOCAL | CREAD;

	//设置流控
	switch(Fcontrl)
	{
		/* with hardware flow control */
		case HARDWARE_FLOW_CONTROL:
			newtio.c_cflag |= CRTSCTS;
			printf(" with hardware flow control!\n");
			break;
		/* software flow control */
		case SOFTWARE_FLOW_CONTROL:
			newtio.c_cflag |= IXON|IXOFF|IXANY;
			printf(" software flow control!\n");
			break;
		case NO_ANY_FLOW_CONTROL:
			newtio.c_cflag &= ~(CRTSCTS|IXON|IXOFF|IXANY);
			printf("no any flow control!\n");
			break;
		default:
			newtio.c_cflag &= ~(CRTSCTS|IXON|IXOFF|IXANY);
			printf("no any flow control!\n");
			break;
	}
	
	//设置数据位
	if(DBit == DATA_8BIT)
	{
		newtio.c_cflag &= ~CSIZE;
		newtio.c_cflag |= CS8;
	}
	else if(DBit == DATA_7BIT)
	{
		newtio.c_cflag	&= ~CSIZE;
		newtio.c_cflag |= CS7;
	}
	
	//设置校验方式
	if(Parity == PARITY_ODD)
	{
		newtio.c_cflag |= PARENB;
		newtio.c_cflag |= PARODD;
		newtio.c_iflag |= (INPCK | ISTRIP);
	}
	else if(Parity == PARITY_EVEN)
	{
		newtio.c_iflag |= (INPCK | ISTRIP);
		newtio.c_cflag |= PARENB;
		newtio.c_cflag &= ~PARODD;
	}
	else if(Parity == PARITY_NONE)
	{
		newtio.c_cflag &= ~PARENB;
		
	}
	else
	{
		perror("Parity is not support!");
		return -2;
	}
	
	/* set one bit stop, or it is two bits */
	//设置停止位
	if(SBit == STOP_1BIT)
		newtio.c_cflag &=~CSTOPB;
	else if(SBit == STOP_2BIT)
		newtio.c_cflag |=CSTOPB;

	//设置波特率
	switch(Rate)
	{
		case BITRATE_2400:
			cfsetispeed(&newtio, B2400);
			cfsetospeed(&newtio, B2400);
			break;
		case BITRATE_4800:
			cfsetispeed(&newtio, B4800);
			cfsetospeed(&newtio, B4800);
			break;
		case BITRATE_9600:
			cfsetispeed(&newtio, B9600);
			cfsetospeed(&newtio, B9600);
			break;
		case BITRATE_115200:
			cfsetispeed(&newtio, B115200);
			cfsetospeed(&newtio, B115200);
			break;
		default:
			cfsetispeed(&newtio, B9600);
			cfsetospeed(&newtio, B9600);
			break;
	}
	
	newtio.c_cc[VTIME] = 0;
	//newtio.c_cc[VMIN]  = 1;
	newtio.c_cc[VMIN]  = 0;
	
	tcflush(mFd, TCIFLUSH);
	
	if(tcsetattr(mFd, TCSANOW, &newtio) != 0)
	{
		perror("Uart set error!");
		return -3;	
	}


	printf("Uart set done\n");
	
	return 0;
}

/*
int SerialClient::detectProtocol(char *Data, int DataLenth)
{
	
	if(Data[0] == 0xA0)
	{
		return 0;
	}
	else if(Data[0] == 0xAB)
	{
		mInterface = new WeiDingInterface();
		return 0;
	}
	
	return -1;
}
*/
int SerialClient::sendData(unsigned long cmd, void* arg)
{
	int ret = 0;
	if(mFd < 0)
	{
		return -1;
	}
	
	ret = mInterface->sendCmd(cmd, arg);
	if(ret < 0) 
	{
		return -1;
	}

	return 0;
}

int SerialClient::getResponseData(unsigned long cmd, unsigned char *response, int response_data, int *response_length, unsigned int ms)
{
	int ret = 0;
	
	mInterface->beginWaitCmd(cmd);
    ret = mInterface->waitTimeOutCmd(cmd, ms, response, response_data, response_length);		// 2000 ms
	
	printf("data ret :%d\n", ret);
	if(ret == 0)
	{
//		printf("get response data ok\n");
	}
	else if(ret < 0)
	{
//		printf("Get response data error\n");
	}
	else if(ret == 1)
	{
//		printf("Get response data timeout\n");
	}
	
	mInterface->endWaitCmd(cmd);
	
	return ret;
}

int SerialClient::receiveDataParser(unsigned char *Data, int DataLenth)
{
	
	CmdTypeValue Value;
	
	/*
	//探测是什么类型的协议
	if(detectProtocol(Data, DataLenth))
	{
		printf("detect Protocol failed\n");
		return -1;
	}*/
	
	//判断是不是有效指令
	if(mInterface->validReceiveCmd(Data, DataLenth))
	{
		printf("receive data in not valid\n");
		return -1;
	}
	
	//获取指令码
	Value = mInterface->getReceiveCmdCode(Data, DataLenth);
	
	//解析指令码
	mInterface->praserReceiveCmd(Value , Data, DataLenth);


    return 0;
}



int SerialClient::ClientLoop(void)
{
	int    fs_sel;
	fd_set fs_read;
	struct timeval time;
    unsigned int u32RDLen   = 0;
	unsigned char DataBuf[1024+1] = {0};
	int TimeOut = 1;

    if(mFd < 0)
    {
        return -1;
    }

	while( TimeOut > 0 )
    {
		FD_ZERO(&fs_read);
		FD_SET(mFd, &fs_read);
		
		time.tv_sec  = 1;
		time.tv_usec = 0;
	
        fs_sel = select(mFd+1, &fs_read, NULL, NULL, &time);
       	if(fs_sel)
        {
            //tcflush(mFd, TCIOFLUSH);
        	u32RDLen = read(mFd, (void *)DataBuf, 1024);
			DataBuf[u32RDLen] = '\0';
          //  tcflush(mFd, TCIOFLUSH);
			break;
       	}
        else if(fs_sel == 0)
        {
			//printf("TimeOut ；%d\n", TimeOut);
        	TimeOut--;
        }
    }

    /*
    //读取包头超时
    if (0 == TimeOut)
    {
        //清理串口数据
        
        printf( "Read Data time out\n" );

		return -2;
    }
    */

    if(u32RDLen > 0)
    {
        receiveDataParser(DataBuf , u32RDLen);

    }

    return 0;
}

void SerialClient::run()
{
    while(1)
    {
        ClientLoop();
    }
}

void SerialClient::eixt()
{
    isRun = 0;
    wait();
}
