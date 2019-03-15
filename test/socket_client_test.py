#!/usr/bin/python
#_*_ coding:utf-8 _*_

#你的源码文件不要用socket.py，他跟系统模块名字冲突了。改个名字，比如sock_test.py
import socket  # socket模块
import commands  # 执行系统命令模块

# 通信链句柄，特殊文件
# TCP客户端：sock---》connecharacterct---->write read --->close
# 服务器：socket对象--->bind绑定IP、端口（监听这个）---》listen最大连接数，超过不处理
# --->accept阻塞，等待连接（阻塞直到客户端连接---》read  write read  close


sk = socket.socket()
ip_port = ('127.0.0.1', 9999)
# 绑定
sk.bind(ip_port)
sk.listen(5)

while True:
    # conn,address=sk.accept()
    result = sk.accept()  # 元祖
    print type(result)
    result[0].send("hello")  # 给客户端conn发信息
    flag = True
    while flag:
        data = result[0].recv(1024)
        if data == 'exit':
            flag = False
    result[1].close()

import SocketServer


# TCPServer  ThreadingMixIn
class MyServer:
    pass


server = SocketServer.ThreadingTCPServer(('127.0.0.1', 9999), MyServer)
server.serve_forever()

base_path = 'G:/temp'
conn = self.request

while True:
    pre_data = conn.recv(2048)
    cmd, file_name, file_size = pre_data.split('|')
    recv_size = 0
    file_dir = os.path.join(base_path, file_name)
    f = file(file_dir, 'wb')
    Flag = True
    while Flag:
        if int(file_size) > recv_size:
            data = conn.recv(1024)
            recv_size += len(data)
        else:
            recv_size = 0
            Flag = False
        f.write(data)
    f.close()


#服务端代码
HOST='10.0.0.245'
PORT=50007
s= socket.socket(socket.AF_INET,socket.SOCK_STREAM)   #定义socket类型，网络通信，TCP
s.bind((HOST,PORT))   #套接字绑定的IP与端口
s.listen(1)         #开始TCP监听,监听1个请求

while 1:
    conn,addr=s.accept()   #接受TCP连接，并返回新的套接字与IP地址
    print'Connectedby',addr    #输出客户端的IP地址

while 1:
    data=conn.recv(1024)    #把接收的数据实例化
    cmd_status,cmd_result=commands.getstatusoutput(data)
     #commands.getstatusoutput执行系统命令（即shell命令），返回两个结果，第一个是状态，成功则为0，第二个是执行成功或失败的输出信息
    if len(cmd_result.strip()) ==0:
    #如果输出结果长度为0，则告诉客户端完成。此用法针对于创建文件或目录，创建成功不会有输出信息
        conn.sendall('Done.')
    else:
       conn.sendall(cmd_result)
       #否则就把结果发给对端（即客户端）
    conn.close()     #关闭连接



#客户端代码
HOST='10.0.0.245'
PORT=50007
s=socket.socket(socket.AF_INET,socket.SOCK_STREAM)      #定义socket类型，网络通信，TCP
s.connect((HOST,PORT))       #要连接的IP与端口

while 1:
    cmd=raw_input("Please input cmd:")       #与人交互，输入命令
    s.sendall(cmd)      #把命令发送给对端
    data=s.recv(1024)     #把接收的数据定义为变量
    print data         #输出变量
    s.close()   #关闭连接




#FTP客户端

cmd,path=input.split('|')

file_name=os.path.basename(path)
file_size=os.stat(path).st_size

sk.send(cmd+'|'+file_name+'|'+str(file_size))

send_size=0
f=file(path,'rb')
Flag=True

while Flag:
    if send_size+1024 >file_size:
        data=f.read(file_size-send_size)
        Flag = False
    else:
        data=f.read(1024)#字节
        send_size+=1024
    sk.send(data)#瓶颈在IO和网络，读IO所有线程都可以访问，写IO需要加锁
f.close()

sk.close()


import SocketServer
import os

class MyServer(SocketServer.BaseRequestHandler):
    def handle(self):
        base_path='G:/temp'
        conn = self.request

    file_dir=os.path.join(base_path,file_name)
    f=file(file_dir,'wb')
    Flag=True
    while Flag:
        if int(file_size)>recv_size:
            data=conn.recv(1024)
            recv_size+=len(data)
            f.write(data)
        else:
            recv_size=0
            Flag=False
'''
某个程序可以有多个进程，一个进程有多个线程
工作最小单元：线程

分片
多个CPU：同一时间处理多个线程

全局解释器锁GIL：防止一个进程中的多个进程同时被进行（同一时间只能执行一个线程吧 ）

计算密集：CPU  多进程
IO密集：IO不占有CPU（写文件）  多线程







'''
