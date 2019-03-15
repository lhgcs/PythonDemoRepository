#!/usr/bin/env python
#*_* coding:utf-8 _*_


__author__ = 'Administrator'

from threading import Thread

def Foo(arg):
    print arg

t1 = Thread(target=Foo,args=(1,))#函数名 序列
#主线程创建子线程后就不管子线程
#主线程执行完了等待子线程退出才退出
#t1.setDaemon(False) #默认是False，不是守护进程

#设置主线程不等待子线程
t1.setDaemon(True) #子线程是守护线程    必须在start前设置    主线程一结束，子线程跟着借宿
t1.start() #有可能立即执行  同一时刻只有一个线程跑
t1.join(5)#主线程阻塞，等待子线程，子线程最多执行5s，然后继续执行主线程

print "1111"  #主线程    主线程创建子线程
print t1.getName()  #线程名字
#线程想建多少就建多少，不要建太多   需要保持状态（上下文），等下接着继续运行，上下文切换需要时间


import time

class MyThread(Thread):
    def run(self):#run中执行target
        time.sleep(10)
        print '我是线程'

def Bar():
    print 'Bar'

t1=MyThread(target=Bar,args=())
t1.start()





from Queue import Queue
que = Queue(maxsize=100)
que.put(1)
que.put(1)
que.get()
que.get()
que.empty()

class Procuder(Thread):
    def __init__(self,name,queue):
        self.__Name=name
        self.__Queue=queue
        super(Procuder, self).__init__()

    def run(self):
        while True:
            if self.__Queue.full():
                time.sleep(1)
            else:
                self.__Queue.put("包子")
        Thread.run(self)

class Consumer(Thread):
    def __init__(self, name, queue):
        self.__Name = name
        self.__Queue=queue
        super(Consumer, self).__init__()

    def run(self):
        while True:
            if self.__Queue.empty():
                time.sleep(1)
            else:
                self.__Queue.get()
        Thread.run(self)

p1 = Procuder("p1",que)
p1.start
p2 = Procuder("p2",que)
p3 = Procuder("p3",que)


p2.start
p3.start

for item in xrange(20):
    name = 'consumer%d' %(item,)
    temp = Consumer(name, que)
    temp.start


#服务器公钥给用户  公钥只能加密，谁发送谁就要获取对方的公钥
#import paramiko

#ssh.


#线程是共享内存的，需要线程安全

import threading
#全局变量   global
num=0

lock = threading.Lock()   #只能加一次锁，再次加锁需要释放之前的锁
#lock=threading.RLock  递归锁，可以加几次锁

samp=threading.BoundedSemaphore(4)    #同一时刻可以有四个线程


def run1():
    global num
    lock.acquire()#加锁    阻塞获取锁
    num +=1
    lock.release()





for i in range(200):
    t = threading.Thread(target=run1)
    t.start()















