#ifndef __ICCARDROTOCOL_H__
#define __ICCARDROTOCOL_H__


#include "network/ProtocolInterface.h"
#include <QString>
#include <QString>
#include <QThread>
#include <QDebug>


class  DataNotify : public QObject{
        Q_OBJECT
public:
   DataNotify(QObject *parent);
   ~DataNotify();


signals:
    void icCardNumberReadable(QString number);
};


class IcCardInterface : public ProtocolInterface{

public:
    IcCardInterface(int fd);
	~IcCardInterface();

	
	//发送协议
	 int sendCmd(unsigned long cmd, void* arg);
	
     unsigned char checkSum(unsigned char *Data, int DataLength);

	//校验数据
	 int validReceiveCmd(const unsigned char* data, unsigned int len);
	
	//获取指令码
	 CmdTypeValue getReceiveCmdCode(const unsigned char* data, unsigned int len);
	
	//获取指令码内容
	 unsigned char* praserReceiveCmd(const unsigned char* data, unsigned int len);
	
	//解析指令
	 int  praserReceiveCmd(CmdTypeValue value, const unsigned char* data, unsigned int len);
	 
	 CmdSignal *getCmdSignalHandle(unsigned long cmd);

     DataNotify *getNotifyHandle();


private:
	 int mSerialFd;
     DataNotify *notify;
};

#endif
