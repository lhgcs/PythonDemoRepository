#ifndef LINUXSOCKET_H
#define LINUXSOCKET_H

class LinuxSocket
{
public:
    LinuxSocket();
    void send_data(char buf[]);
private:
    int sockfd;
};

#endif // LINUXSOCKET_H
