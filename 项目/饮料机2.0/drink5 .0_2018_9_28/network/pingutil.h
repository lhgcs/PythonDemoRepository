#ifndef PINGUTIL_H
#define PINGUTIL_H

#define MAX_NO_PACKETS 	4

typedef struct{
    int  	AverTTL;
    double  AverRTT;
}PingReslut;


int ping(char *ipaddr, int timeout, int sendcount);

#endif // PINGUTIL_H
