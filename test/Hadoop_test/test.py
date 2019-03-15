#_*_ coding:utf-8 _*_
__author__ = 'Administrator'
'''

Hadoop
是一个开源大数据框架
是一个分布式计算的解决方案
Hadoop=HDFS（分布式文件系统）+MapReduce（分布式计算）

Hadoop核心：
HDFS：存储是大数据技术的基础
MapReduce：分布式计算是大数据应用的解决方案

HDFS：
成百上千台机器
TB甚至PB为单位的大量数据
简单便捷的文件获取

HDFS慨念：
数据块：抽象块而非整个文件作为存储单元；默认大小64M，一般为128M，备份X3
分布式--》主从模式
NameNode：主，管理文件系统的命名空间，存放文件元数据；维护着文件系统的所有文件和目录，文件和数据块的映射；记录每个文件中各个块所在数据节点的信息
DataNode：从，存储并检索数据块；想NameNode更新所存储块的列表


优点：适合大文件存储，支持TB、PB级的数据存储，并有副本策略；可构建在廉价的机器上，并有硬顶的容错和回复机制；支持流式数据访问，一次写入，多次读取最搞笑
缺点：不适合大量小文件存储；不适合并发写入，不支持文件随机修改，只能后续添加；不支持随机读等低延时的访问方式

数据块一般设置128M，设置太小，访问时要查找多个数据块的地址；对NameNode的内存损耗比较大。设置过大对并行支持不太好，系统重启时加载数据耗时比较长
Hadoop2容错机制：配置为高可用的集群，有主节点和备用节点

HDFS写流程：

Data--》Client---》NameNode---》各个节点的状态，哪个节点还有空间---》client---》client对数据分块---》client发数据给DataNo_1
--->DataNode_1传给DataNode_2---》DataNode_2传给NameNode----》nameNode传给Client（数据块1已经存储完了）

客户端想NameNode发起写数据请求
分块写入DataNode节点，DataNode自动完成副本备份
DataNode想NameNode汇报存储完成，NameNode通知客户端


HDFS读流程：

Data--》Client---》NameNode发起请求:要取文件---》节点信息：在哪个节点---》client---》client想DataNode_1发起请求,取回数据1---》client发数据给DataNo_1
--->client想DataNode_2发起请求,取回数据_2---》假如DataNode_2挂掉----》client想DataNode_3(备份，包含数据1，数据2）发起请求,取回数据DataNode_2

客户端向NameNode发起读数据请求
DataNode找出距离最近的DataNode节点信息
客户端从dataNode分块下载文件


例子：
目标：过滤含有Hadoop字符串的行
解决方案1：Linux grep命令
解决方案2：编写java/python程序


实战
常用HDFS Shell命令

类Linux系统：ls cat mkdir rm chmod chown
HDFS文件交互：copyFromLoad 从本地复制到HDFS copyToLoad 从FDFS复制到本地  get 下载 put 上传

/usr/local/hadoop/sbin   启动文件
/usr/local/hadoop/bin
帮助文档 hdfs dfs -help

hdfs dfs -ls /
hdfs dfs -mkdir /test
hdfs dfs -copyFromLocal /home/hadoop/mk.txt /test
hdfs dfs -cat /test/mk.txt
hdfs dfs -copyToLocal /test/mk.txt /home/hadoop/mk2.txt
hdfs dfs -chmod 777 /test/mk.txt





'''
from hdfs3 import HDFilesSystem   #HDFS文件系统客户端

test_host='loalhost'
test_port=9000

#文件是否存在
def hdfs_exists(hdfs_client):
    path='/tmp/test'
    if hdfs_client.exists(path):
        hdfs_client.rm(path)
    hdfs_client.makedirs(path)

#读写
def hdfs_write_read(hdfs_client):
    data=b"hello"
    file_a='/tmp/test/file_a'
    with hdfs_client.open(file_a,'wb',replication=1) as f:
        f.write(data)
    with hdfs_client.open(file_a,'rb') as f:
        out= f.read(len(data))
        assert out == data


def hdfs_reradline(hdfs_client):
    data=b"hello\n hadoop"
    file_a='/tmp/test/file_b'
    with hdfs_client.open(file_a,'wb',replication=1) as f:
        f.write(data)
    with hdfs_client.open(file_a,'rb') as f:
        out= f.readlines()
        assert len(out) ==2


if __name__ == '__main__':
    hdfs_client=HDFilesSystem(host=test_host,port=test_port)

    hdfs_exists(hdfs_client)
    hdfs_write_read(hdfs_client)
    hdfs_reradline(hdfs_client)
    hdfs_client.disconnect()#端口

'''

mapReduce
是一种编程模型，一种编程方法，是抽象的理论
YARN
Hadoop2.0后的资源管理器
可扩展性，可靠性，资源利用率提升，支持多种计算框架；主从架构；高可用

ResourceManager：分配调度资源；启动并监控ApplicationMaster；监控NodeManager
ApplicationManager：为MR类型的程序申请资源，并分配给内部任务；负责数据切分；监控任务的执行及容错
NodeManager：管理单个节点的资源；处理来自ResourceManager的命令；处理来自ApplManager的命令

编程模型：分片---》分别处理---》汇总
输入一个大文件，通过split之后，将其分为多个分片
每个文件分片由单独的机器处理，这就是map方法
将各个机器计算的结果进行汇总并得到最终的结果，这就是reduce方法


如何通过Hadoop存储小文件
当有节点故障的时候，集群是如何继续提供服务的
哪些影响MapReduce性能



Hbase
高性能，搞可靠面向列，可伸缩，实时读写的分布式数据库
利用HDFS作为其文件存储系统，支持MR程序读取数据
存储非结构化和半结构化数据

RowKey：数据唯一标识，按字典排序
Column Family：列族，多个列的集合，最多不要超过3个
TimeStamp：时间戳

Spark
基于内存计算的大数据并行计算框架
是MapReduce的替代方案，兼容HDFS，HIVE等数据源

优势：基于内存计算的分布式计算框架；抽象出分布式内存存储数据结构，弹性分布式数据集RDD；基于事件驱动，通过线程池复用线程提高性能



'''

l = ["a","bb","cc"]
l_count=map(len,l)
l_sum=reduce(lambda x,y:x+y,l_count)





