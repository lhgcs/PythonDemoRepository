#ifndef __PROTOCOLINTERFACE_H__
#define __PROTOCOLINTERFACE_H__

#include <pthread.h>
//#include "serialclient.h"

typedef struct{
	int CmdSize;
	unsigned long Cmd[3];
}CmdTypeValue;

typedef struct{
	int Cmd;
	pthread_mutex_t Mutex;
	pthread_cond_t	Cond;
	void *PrivateData;
	int PrivateDataSize;
}CmdSignal;


class ProtocolInterface{



public:
	//ProtocolInterface(SerialClient *Client);
	//ProtocolInterface();
	/*
	virtual int receiveStorageGoodsData(unsigned long cmd, const unsigned char* data, unsigned int len) = 0;		//存货消息返回
	virtual int receivePickUpGoodsData(unsigned long cmd, const unsigned char* data, unsigned int len) = 0;		//取货消息返回
	virtual int receiveTemperatureData(unsigned long cmd, const unsigned char* data, unsigned int len) = 0;		//温度消息返回
	virtual int receiveDoorSignalData(unsigned long cmd, const unsigned char* data, unsigned int len) = 0;			//温度消息返回
	virtual int receivePlcData(unsigned long cmd, const unsigned char* data, unsigned int len) = 0;				//PLC消息返回
	virtual int receiveFreezingData(unsigned long cmd, const unsigned char* data, unsigned int len) = 0;			//冰箱冷冻消息返回
	*/
	//接收信号处理
	int beginWaitCmd(unsigned long cmd);

    int waitTimeOutCmd(unsigned int cmd, unsigned int ms ,void *data, int dat_buffer_length, int *data_lenth);

	int waitCmd(unsigned int cmd, void *data, int *data_lenth);

	int endWaitCmd(unsigned long cmd);
	
	int sendCmdSignal(unsigned long cmd, int cmd_size, const unsigned char* data, unsigned int len);

	
	//发送指令码
	virtual int sendCmd(unsigned long cmd, void* arg) = 0;
	
	//校验接收数据
	virtual int validReceiveCmd(const unsigned char* data, unsigned int len) = 0;
	
	//获取接收指令码
	virtual CmdTypeValue getReceiveCmdCode(const unsigned char* data, unsigned int len) = 0;
	
	//解析指令
	virtual int  praserReceiveCmd(CmdTypeValue value, const unsigned char* data, unsigned int len) = 0;
	
	virtual  CmdSignal *getCmdSignalHandle(unsigned long cmd) = 0;
	


protected:
	CmdSignal 	*mCmdSignal;
};

#endif
