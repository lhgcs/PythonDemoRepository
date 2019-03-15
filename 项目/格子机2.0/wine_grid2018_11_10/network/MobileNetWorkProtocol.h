#ifndef __MOBILENETWORKPROTOCOL_H__
#define __MOBILENETWORKPROTOCOL_H__


#include "ProtocolInterface.h"

#define BAI_SHI_TONG_CMD_SIZE				(5)

typedef enum{
	MOBILE_NET_WORK_GET_SIM_STATUS					=   0x01	,
	MOBILE_NET_WORK_SETUP_DEFAULT_PDP				=   0x02	,
	MOBILE_NET_WORK_GET_IMSI						=   0x03	,
	MOBILE_NET_WORK_GET_IMEI						=   0x04	,
	MOBILE_NET_WORK_GET_SIGNAL_STRENGTH				=   0x05	,
	MOBILE_NET_WORK_GET_REGISTRATION_STATE			=   0x06	,
	MOBILE_NET_WORK_GET_SIM_CARD_INFO				=   0x07	,
}MobileNetWorkCmdType;

//CMD STRING
#define CMD_REQUIRED_QOS_PARAMS					"AT+CGQREQ=1\n"
#define CMD_MINIMUN_QOS_PARAMS					"AT+CGQMIN=1\n"
#define CMD_PACKET_DOMAIN_REPORT				"AT+CGEREP=1,0\n"
#define CMD_INIT_DATA_CONNECT					"ATD*99***1#\n"
#define CMD_DEFINE_PDP_CONTEXT					"AT+CGDCONT=1,\"IP\",\"\",,0,0\n"
#define CMD_SINAL_STENGTH						"AT+CCSQ\n"								//查询信号强度
#define CMD_NET_WORK_REGISTER_STATUS			"AT+CREG?\n"							//查询是否注册成功
#define CMD_CHECK_SIM_STATUS					"AT+CPIN?\n"							//查询是否插卡
#define CMD_CHECK_NETWORK_OPERATOR				"AT+COPS?\n"							//查询网络信息及运营商信息



//信号强度
typedef struct{
	int rssi, ber;
}SignalStrength;


//注册状态
typedef struct{
	int ci, stat;
}RegistrationState;

//卡信息和运营商
typedef struct{
	char MobileOperatorInfo[128];
	int CardType;
}SimCardInfo;

class MobileNetWorkProtocol : public ProtocolInterface{
public:
	MobileNetWorkProtocol(int fd);
	~MobileNetWorkProtocol();
/*
	bool 				mSimStatus;
	SignalStrength 		mSignal;
	RegistrationState	mRegistration;
*/
	
	//发送协议
	 int sendCmd(unsigned long cmd, void* arg);
	
	//校验数据
	 int validReceiveCmd(const unsigned char* data, unsigned int len);
	
	//获取指令码
	 CmdTypeValue getReceiveCmdCode(const unsigned char* data, unsigned int len);
	
	//获取指令码内容
	 unsigned char* praserReceiveCmd(const unsigned char* data, unsigned int len);
	
	//解析指令
	 int  praserReceiveCmd(CmdTypeValue value, const unsigned char* data, unsigned int len);
	 
	 CmdSignal *getCmdSignalHandle(unsigned long cmd);
	 
private:
	 int requestNetWorkSetupUp();
	 int requestRegistrationState(RegistrationState *registrationState);
	 int requestSignalStrength(SignalStrength *signalStrength);
	 int requestSIMStatus(int *Status) ;
	 int requestSimCardInfo(SimCardInfo *info);
     int getResponse(unsigned long cmd, unsigned char *response,int response_buffer_length, int *response_length, unsigned int ms);
	 
	 int mSerialFd;
	 int mTimeOut;
	 CmdSignal *mCmdSignal;
};

#endif
