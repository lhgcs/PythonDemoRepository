#include "ProtocolInterface.h"

#include <sys/time.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/time.h>

/*
ProtocolInterface(SerialClient *Client)
{
}
*/

int ProtocolInterface::beginWaitCmd(unsigned long cmd)
{
	CmdSignal *pSignal = NULL;
	

	pSignal = getCmdSignalHandle(cmd);
    if(pSignal)
	pthread_mutex_lock( &pSignal->Mutex);

    return 0;
}

int ProtocolInterface::waitTimeOutCmd(unsigned int cmd, unsigned int ms ,void *data, int dat_buffer_length, int *data_lenth)
{
	int ret;
	long int out_usec;
	struct timeval now;
	struct timespec outtime;

	CmdSignal *pSignal = NULL;
    if(pSignal)
    {
	pSignal = getCmdSignalHandle(cmd);

	gettimeofday(&now, NULL);
	out_usec = now.tv_usec + ms*1000;
	outtime.tv_sec = now.tv_sec + out_usec / 1000000;
	outtime.tv_nsec = (out_usec % 1000000) *1000;
	
	//DEBUG_MSG("wait, ms = %u, outtime.tv_nsec: %ld\n", ms, outtime.tv_nsec);
	ret = pthread_cond_timedwait( &pSignal->Cond, &pSignal->Mutex, &outtime );
	
	if(pSignal->PrivateData != NULL && data != NULL)
	{
        if(dat_buffer_length >= pSignal->PrivateDataSize)
        {
            dat_buffer_length = pSignal->PrivateDataSize;
        }
        memcpy((void*)data, pSignal->PrivateData, dat_buffer_length);
        *data_lenth = dat_buffer_length;
	}
	
	switch( ret ) {
		case 0: return 0;
		case ETIMEDOUT: return 1;
		default: break;
	}
    }
	return -1;
}

int ProtocolInterface::waitCmd(unsigned int cmd, void *data, int *data_lenth)
{
	int ret = 0;
	
	CmdSignal *pSignal = NULL;
	pSignal = getCmdSignalHandle(cmd);
    if(pSignal)
    {
	ret = pthread_cond_wait( &pSignal->Cond, &pSignal->Mutex);
	
	if(pSignal->PrivateData != NULL && data != NULL)
	{
		memcpy((void*)data, pSignal->PrivateData, pSignal->PrivateDataSize);
		*data_lenth = pSignal->PrivateDataSize;
	}
	
	switch( ret ) {
		case 0: return 0;
		case ETIMEDOUT: return 1;
		default: break;
	}
    }
	return -1;
}


int ProtocolInterface::endWaitCmd(unsigned long cmd)
{
	CmdSignal *pSignal = NULL;
    if(pSignal)
    {
	pSignal = getCmdSignalHandle(cmd);
	
	pthread_mutex_unlock( &pSignal->Mutex);
    }
    return 0;
}


int ProtocolInterface::sendCmdSignal(unsigned long cmd, int cmd_size, const unsigned char* data, unsigned int len)
{
	CmdSignal *pSignal = NULL;
	
	pSignal = getCmdSignalHandle(cmd);
	
	pthread_cond_signal( &pSignal->Cond );
	if(pSignal->PrivateData != NULL)
	{
		free(pSignal->PrivateData);
		pSignal->PrivateData = NULL;
		pSignal->PrivateDataSize = 0;
	}
	
	if(data != NULL && len > 0)
	{
		pSignal->PrivateData = (void *)malloc(len);
		memcpy((void *)pSignal->PrivateData, (void *)data, len);
		pSignal->PrivateDataSize = len;
	}
	
	return 0;
}
