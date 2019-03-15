#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#include "IcCardProtocol.h"

DataNotify::DataNotify(QObject *parent):QObject(parent)
{

}

DataNotify::~DataNotify()
{
}

IcCardInterface::IcCardInterface(int fd)
{
    notify = new DataNotify(NULL);
}

CmdSignal* IcCardInterface::getCmdSignalHandle(unsigned long cmd)
{
	return NULL;
}


DataNotify *IcCardInterface::getNotifyHandle()
{
    return notify;
}

IcCardInterface::~IcCardInterface()
{	
    delete notify;
}

//unsigned char IcCardInterface::checkSum(unsigned char *Data, int DataLength)
//{
//    int i = 0;
//    unsigned char CheckSum = 0;

//    CheckSum = Data[0];
//    for(i =1; i< DataLength; i++)
//    {
//        CheckSum ^= Data[i];
//    }

//    return CheckSum;
//}

//int IcCardInterface::validReceiveCmd(const unsigned char* data, unsigned int len)
//{
//    int data_length;
//    int calculate_checksum;

//    if(!(data[0] == 0xa0 && data[1] == 0x01 && data[2] == 0x01))
//    {
//        printf("header error\n");
//        return -1;
//    }

//    data_length = data[3];
//    calculate_checksum = checkSum((unsigned char*)data + 4, data_length);

//    if(calculate_checksum != data[4+data_length])
//    {
//        printf("check sum error send :%x, calculate :%x", data[4+data_length], calculate_checksum);
//        return -1;
//    }

//	return 0;
//}

unsigned char IcCardInterface::checkSum(unsigned char *Data, int DataLength)
{
    int i = 0;
    unsigned char CheckSum = 0;

    for(i =0; i< DataLength-1; i++)
    {
        CheckSum ^= Data[i];
    }
    return ~CheckSum;
}

int IcCardInterface::validReceiveCmd(const unsigned char* data, unsigned int len)
{
    int data_length;
    int calculate_checksum;

    if(!(data[0] == 0x04  && data[2] == 0x02))
    {
        printf("header error\n");
        return -1;
    }

    data_length = data[1];
    calculate_checksum = checkSum((unsigned char*)data, data_length);

    if(calculate_checksum != data[data_length-1])
    {
        printf("check sum error send :%x, calculate :%x", data[data_length-1], calculate_checksum);
        return -1;
    }
    return 0;
}

CmdTypeValue IcCardInterface::getReceiveCmdCode(const unsigned char* data, unsigned int len)
{
	CmdTypeValue Value;
	return Value;
}

int  IcCardInterface::praserReceiveCmd(CmdTypeValue value, const unsigned char* data, unsigned int len)
{
	int i;
    int data_length;
    data_length = data[3];
    size_t BufSize =20;
    char buf[BufSize];

		printf("\n-------------SerialRecvData---------------\n");
        printf("DataLength : %d\n", len);
	//	printf("DataString : %s\n", DataBuf);
			
			for(i = 0; i< len; i++)
			{
 				printf( "0x%02x ", data[i]);
				if(0 == (i + 1) % 8)
        		{
            		printf("\n");
        		}
			}

		printf("\n------------------------------------\n\n");	

    ::snprintf(buf, BufSize, "%02x%02x%02x%02x", data[7],data[8], data[9], data[10]);
    QString str = QString::fromUtf8(buf);

    emit notify->icCardNumberReadable(str);
	return 0;
		
}

int IcCardInterface::sendCmd(unsigned long cmd, void* arg)
{
	
}
