#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#include "MobileNetWorkProtocol.h"


MobileNetWorkProtocol::MobileNetWorkProtocol(int fd)
{
	mSerialFd = fd;
	mTimeOut = 1000;
	
	mCmdSignal = new CmdSignal;
	
	mCmdSignal->Cmd = 0;
    mCmdSignal->PrivateData = NULL;
    mCmdSignal->PrivateDataSize = 0;

	pthread_mutexattr_t attr;
	pthread_mutexattr_init( &attr );
	pthread_mutexattr_settype( &attr, PTHREAD_MUTEX_RECURSIVE );
	

	pthread_mutex_init( &mCmdSignal->Mutex, &attr );
	pthread_cond_init( &mCmdSignal->Cond, NULL );
	
	
}


MobileNetWorkProtocol::~MobileNetWorkProtocol()
{	

	pthread_mutex_destroy( &mCmdSignal->Mutex);
	pthread_cond_destroy( &mCmdSignal->Cond );
	
	delete mCmdSignal;
	mCmdSignal = NULL;
}


int MobileNetWorkProtocol::validReceiveCmd(const unsigned char* data, unsigned int len)
{

	
	return 0;
}


CmdTypeValue MobileNetWorkProtocol::getReceiveCmdCode(const unsigned char* data, unsigned int len)
{
	CmdTypeValue Value;
	Value.CmdSize = 0;

	return Value;
}


CmdSignal* MobileNetWorkProtocol::getCmdSignalHandle(unsigned long cmd)
{

	return mCmdSignal;
}


int MobileNetWorkProtocol::praserReceiveCmd(CmdTypeValue value, const unsigned char* data, unsigned int len)
{
	unsigned long cmd = 0;

	
	if(value.CmdSize != 0)
		return -1;

	
	sendCmdSignal(cmd, 7, data, len);
	
	return 0;
		
}


int MobileNetWorkProtocol::sendCmd(unsigned long cmd, void* arg)
{
	
	int retval;
	
	switch(cmd) {
		case MOBILE_NET_WORK_GET_SIM_STATUS: 
			if(arg != NULL) 
			{
				retval = requestSIMStatus((int *)arg);
			}
			break;
			
		case MOBILE_NET_WORK_SETUP_DEFAULT_PDP:
			if(arg != NULL) 
			{
				retval = requestNetWorkSetupUp();
			}
			break;
			
		case MOBILE_NET_WORK_GET_IMSI:

	
			break;
			
		case MOBILE_NET_WORK_GET_IMEI:

	
			break;
			
		case MOBILE_NET_WORK_GET_SIGNAL_STRENGTH:
			if(arg != NULL) 
			{
				retval = requestSignalStrength((SignalStrength *)arg);
				printf("retval :%d\n", retval);
			}
			break;
			
		case MOBILE_NET_WORK_GET_REGISTRATION_STATE:
			if(arg != NULL)
			{
				retval = requestRegistrationState((RegistrationState *)arg);
			}
			break;
		case MOBILE_NET_WORK_GET_SIM_CARD_INFO:
			if(arg != NULL)
			{
				requestSimCardInfo((SimCardInfo *)arg);
			}
		default: break;	
	}
	
	return retval;
}

int MobileNetWorkProtocol::getResponse(unsigned long cmd, unsigned char *response,int response_buffer_length, int *response_length, unsigned int ms)
{
	int ret = 0;
	
	beginWaitCmd(cmd);
    ret = waitTimeOutCmd(cmd, ms, response, response_buffer_length, response_length);		// 2000 ms
	
//    printf("data ret :%d\n", ret);
	if(ret == 0)
	{
//        printf("get response data ok\n");
	}
	else if(ret < 0)
	{
        printf("Get response data error\n");
	}
	else if(ret == 1)
	{
//        printf("Get response data timeout\n");
	}
	
	endWaitCmd(cmd);
	
	return ret;
}

int MobileNetWorkProtocol::requestNetWorkSetupUp()
{
	int ret =0;
	
	//"AT+CGDCONT=1,\"IP\",\"%s\",,0,0"
	ret = write(mSerialFd, CMD_DEFINE_PDP_CONTEXT, strlen(CMD_DEFINE_PDP_CONTEXT));
	if(ret < 0 )
	{
		printf("write error\n");
		return -1;
	}
	
	//"AT+CGQREQ=1"
	ret = write(mSerialFd, CMD_REQUIRED_QOS_PARAMS, strlen(CMD_REQUIRED_QOS_PARAMS));
	if(ret < 0 )
	{
		printf("write error\n");
		return -1;
	}

	//"AT+CGQMIN=1"
	ret = write(mSerialFd, CMD_MINIMUN_QOS_PARAMS, strlen(CMD_MINIMUN_QOS_PARAMS));
	if(ret < 0 )
	{
		printf("write error\n");
		return -1;
	}
	
	//"AT+CGEREP=1,0"
	ret = write(mSerialFd, CMD_PACKET_DOMAIN_REPORT, strlen(CMD_PACKET_DOMAIN_REPORT));
	if(ret < 0 )
	{
		printf("write error\n");
		return -1;
	}
	
	//"ATD*99***1#"
	ret = write(mSerialFd, CMD_INIT_DATA_CONNECT, strlen(CMD_INIT_DATA_CONNECT));
	if(ret < 0 )
	{
		printf("write error\n");
		return -1;
	}
	
	return 0;
}


int MobileNetWorkProtocol::requestRegistrationState(RegistrationState *registrationState)
{
    unsigned char response[256] = {0};
	int response_length;
	int ret = -1, read_num = 0;
	char *pStr = NULL;
	int cmd = 0;
	int isfind = -1;
	
	//"AT+CREG?"
	ret = write(mSerialFd, CMD_NET_WORK_REGISTER_STATUS, strlen(CMD_NET_WORK_REGISTER_STATUS));
	if(ret < 0 )
	{
		printf("write error\n");
		return -1;
	}

	while(1)
	{
        ret = getResponse(cmd, response, 255, &response_length, mTimeOut);
		if(ret != 0)
		{
			break;
		}
		read_num++;
		
		if(response_length > 1)
		{
	
			pStr = strstr((char *)response, ":");
			if(pStr !=  NULL)
			{
				if(sscanf(pStr + 1, "%d,%d", &registrationState->ci,&registrationState->stat) == 2)
				{
					isfind = 0;
					//break;
				}
			}
		}

	}
	
	if(read_num == 0)
	{
		isfind = -2;
	}
	
	return isfind;
}


int MobileNetWorkProtocol::requestSignalStrength(SignalStrength *signalStrength) 
{
    unsigned char response[256] = {0};
	int response_length;
	int ret = -1, read_num = 0;
	char *pStr = NULL;
	int cmd = 0;
	int isfind = -1;
	
	//"AT+CSQ"
	ret = write(mSerialFd, CMD_SINAL_STENGTH, strlen(CMD_SINAL_STENGTH));
	if(ret < 0 )
	{
		printf("write error\n");
		return -1;
	}
	
	while(1)
	{
        ret = getResponse(cmd, response, 255, &response_length, mTimeOut);
		if(ret != 0)
		{
			break;
		}
		read_num++;
		
       // printf("response :%s\n", (char *)response);
		if(response_length > 1)
		{
			pStr = strstr((char *)response, ":");
			if(pStr !=  NULL)
			{
                if(sscanf(pStr + 1, "%d,%d", &signalStrength->rssi, &signalStrength->ber) == 2)
				{
					isfind = 0;
					ret = 0;
				}
			}
		}

	}
	
	if(read_num == 0)
	{
		isfind = -2;
	}
	
	
	return isfind;
}

int MobileNetWorkProtocol::requestSIMStatus(int *Status) 
{
    unsigned char response[256] = {0};
	int response_length;
	int ret =-1, read_num = 0;
	int cmd = 0;
	int isfind = -1;
	char *pStr = NULL;
	
	//"AT+CPIN?"
	ret = write(mSerialFd, CMD_CHECK_SIM_STATUS, strlen(CMD_CHECK_SIM_STATUS));
	if(ret < 0 )
	{
		printf("write error\n");
		return -1;
	}
    printf("cmd :%s :%d \n", CMD_CHECK_SIM_STATUS, strlen(CMD_CHECK_SIM_STATUS));
	*Status = false;
	
	while(1)
	{
        ret = getResponse(cmd, response, 255, &response_length, mTimeOut);
		if(ret != 0)
		{
			break;
		}
		read_num++;
		
        printf("response :%s\n", (char *)response);
		if(response_length > 1)
		{
			pStr = strstr((char *)response, "READY");
			if(pStr !=  NULL)
			{
				*Status = true;
				isfind = 0;
			}
		}

	}
	
	if(read_num == 0)
	{
		isfind = -2;
	}
	
	
	return isfind;
}


int MobileNetWorkProtocol::requestSimCardInfo(SimCardInfo *info) 
{
    unsigned char response[256] = {0};
	int response_length;
	int ret =-1, read_num = 0;
	int cmd = 0;
	int isfind = -1;
	char *pStr = NULL;
	int mode, format;
	
	//"AT+CPIN?"
	ret = write(mSerialFd, CMD_CHECK_SIM_STATUS, strlen(CMD_CHECK_SIM_STATUS));
	if(ret < 0 )
	{
		printf("write error\n");
		return -1;
	}
	
	
	while(1)
	{
        ret = getResponse(cmd, response, 255, &response_length, mTimeOut);
		if(ret != 0)
		{
			break;
		}
		read_num++;
		
		//printf("response :%s\n", (char *)response);
		if(response_length > 1)
		{
			pStr = strstr((char *)response, ":");
			if(pStr !=  NULL)
			{
				if(sscanf(pStr + 1, "%d,%d,%s,%d", &mode, &format, info->MobileOperatorInfo,&info->CardType) == 4)
				{
					isfind = 0;
					ret = 0;
				}
			}
		}

	
	}
	
	if(read_num == 0)
	{
		isfind = -2;
	}
	
	
	return isfind;
}
