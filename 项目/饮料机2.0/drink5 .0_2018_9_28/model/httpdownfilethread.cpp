#include "httpdownfilethread.h"

int HttpDownFileThread::downThreadNum=0;
QMutex HttpDownFileThread::mutex;//这句必须存在！！！


HttpDownFileThread::HttpDownFileThread()
{

}



