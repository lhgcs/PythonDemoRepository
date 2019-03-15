#ifndef __SERIALCLIENT_H__
#define __SERIALCLIENT_H__

#include <sys/stat.h>
#include <fcntl.h>

#include <QString>
#include <QThread>

#include "network/ProtocolInterface.h"

typedef enum{
	BITRATE_2400	= 2400,
	BITRATE_4800	= 4800,
	BITRATE_9600 	= 9600,
	BITRATE_19200 	= 19200,
	BITRATE_38400 	= 38400,
	BITRATE_57600 	= 57600,	
	BITRATE_115200 	= 115200,	
}UartBitrate;

typedef enum{
	DATA_5BIT 	= 5,
	DATA_6BIT	= 6,
	DATA_7BIT 	= 7,
	DATA_8BIT 	= 8,
}UartDataBit;

typedef enum{
	STOP_1BIT		= 0,
	STOP_2BIT		,	
}UartStopBit;

typedef enum{
	HARDWARE_FLOW_CONTROL	= 0,
	SOFTWARE_FLOW_CONTROL	,	
	NO_ANY_FLOW_CONTROL	,	
}UartFlowControl;

typedef enum{
	PARITY_NONE	= 0,		//无校验
	PARITY_ODD	,			//奇校验
	PARITY_EVEN	,			//偶校验
}UartParity;

class SerialClient: public QThread
{
    Q_OBJECT
public:	
    SerialClient(const QString &Device,QObject *parent);
	~SerialClient();
	int  sendData(unsigned long cmd, void* arg);
    int  getResponseData(unsigned long cmd, unsigned char *response, int response_data, int *response_length, unsigned int ms);
	int getFd();
	int setInterface(ProtocolInterface *Interface);
	int  ClientLoop(void);
	int setParams(UartBitrate Rate, UartDataBit DBit, UartStopBit SBit, UartFlowControl Fcontrl, UartParity Parity);
    void eixt();
    void run();

private:
	/*
	typedef struct {
		int     		UseSize;
		int     		Overflow;
		unsigned char	Buffer[ SERIAL_CLIENT_BUFFERSIZE + 1 ];
	}dataBuffer;
	*/
	
	
	int findProtocol(int ProtocolType);

	int receiveDataParser(unsigned char *Data, int DataLenth);
	
	int		 	mFd;
	int 		mEpollFd;
	//dataBuffer	mInBuffer;
	ProtocolInterface *mInterface;
    int         isRun;
};

#endif
