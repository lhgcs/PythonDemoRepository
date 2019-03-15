#import cPickle
import numpy as np
import os
import matplotlib.pyplot as plt
from matplotlib.pyplot import imshow
#%matplotlib inline

'''

机器学习：无序数据转为价值的方法
价值：从数据抽取规律，预测未来

应用：
分类：图像识别、垃圾邮件分类
回归：股价预测、房价预测  预测出来的是实数
排序：点击率预估、推荐
生成：图像生成、图像风格转换、图像文字描述生成

应用流程：
训练数据 --》机器学习算法（目标函数） --》模型 --》线上服务

岗位职责：
数据处理（采集+去噪）
模型训练（特征+模型）
模型评估优化（MSSE、F1-score、AUC+调参）  提取特征或改进模型
模型应用（A/B测试）

深度学习算法：
卷积神经网络  图像生成识别
循环神经网络  处理不定长数据
自动编码器
稀疏编码
深度信念网络
限制玻尔兹曼机
深度学习+强化学习=深度强化学习

深度学习：
图像分类
机器翻译
图像生成



神经元：最小结构
逻辑回归模型：深度学习之前最赚钱的算法    广告点击率预估
神经网络训练

多个输入加权求和，再经过函数得到输出（x0=1,w0=b)  x特征，w权重 b偏置

二分类：
神经网络：神经元 --》 激活函数sigmoid --> 二分类逻辑斯底回归模型
f(x) = 1/(1+e^(-x))   x=0时 y=0.5     0<=y<=1
P(Y=0|x)=h(x)=f(w^Tx)=1/(1+e^(-w^Tx))
P(Y=1|x) = 1-P(Y=0|x)

多分类：
多分类 --》 加神经元
神经网络：多输出神经元 --》 激活函数sigmoid --> 多分类逻辑斯底回归模型
w从向量扩展为矩阵
输出w*x变成向量

X=【3,1,2]
w=[[0.4,0.6,0.5],[0.3,0.2,0.1]]

Y=[y0,y1]=[2.8,1.3]
y0 = w[0]*x=2.8
y1 = w[1]*x

e^y = [e^-2.8,e^-1.3]=[0.06,0.27]
sum=1+0.06+0.27=1.33
p(Y=0|x)=0.06/1.33=0.045
p(Y=1|x)=0.27/1.33=0.203
p(Y=2|x)=1/1.33=0.752

P(Y=k|x)=1/(1+∑e^(-w^Tx))   k=1,2....k-1


衡量对数据的拟合程度：损失函数
二分类：
(x1,y1)=([10,3,9,20...],1)
y1'=Model(x1) =0.8    预测属于1的概率是0.8
loss=y1-y1'=0.2       真实值-预测值   损失函数，调整model使得loss越小
多分类：
(x1,y1)=([10,3,9,20...],3)
y1'=Model(x1) =[0.1,0.2,0.25,0.4,0.05]
loss=abs(y1-y1')=[0,0,0,1,0]-y1'=[0.1,0.2,0.25,0.6,0.05]=1.2
                 #编码 3 --》 [0,0,0,1,0]

常用损失函数：
平方差损失：（1/n)∑0.5*（y-model(x))^2
交叉熵损失：（1/n)∑y㏑model(x)

神经网络训练：调整参数使模型在训练集上的损失函数最小 ---》预测结果和真实结果距离越小

下山算法：方向，走一步
梯度下降算法：θ=θ-α*∂L(x,y)/∂θ
θ参数  α学习率即步长
方向：导数方向

TensorFlow： 开源  Google二代机器学习框架  扩展性好

计算图模型：声明式编程 --》 先构建计算图，再输入数值计算


多层神经网络TensorFlow

sudo easy_install pip
pip install --



数据：cifar  32*32 的图片  60000张 10类   50000张是训练数据 10000张是测试数据
cifar-10-batches-py
data_batch_ 训练数据
test_batch   测试


'''

'''
pip install tensorflow
pip install matplotlib
pip install tensorboard 可视化工具
'''

CIFAR_DIR = "./cifar-10-batches-py"
print(os.listdir(CIFAR_DIR))  #有多少个文件

# with open(os.path.join(CIFAR_DIR,'data_batch_1'),'rb') as f:
#     data = cPickle.load(f)
#     print(type(data))
#     print(data.keys())
#     print(data['data'])         #numpy.ndarray  10000 * 3072      32*32=1024  * 3 (rgb) = 3072
#     print(data['labels'])       #属于10类中的哪一类
#     print(data['batch_label'])
#     print(data['filenames'])    #文件名

image_arr = data['data'][0]
image_arr = image_arr.reshape((3,32,32))#3通道
image_arr = image_arr.transpose((1,2,0))#通道交换   32,32,3
imshow(image_arr)


import tensorflow as tf
def load_data(filename):
    with open(filename,'rb') as f:
        data = cPickle.load(f)
        return data['data'],data['labels']

x = tf.placeholder(tf.float32,[None,3072])#占位符   Node样本数据不固定
y = tf.placeholder(tf.int64,[None])       #占位符
#tf.get_variable  如果没定义'w'，就定义一个新的，否则使用
#w = tf.get_variable('w',[x.get_shape()[-1],1],initialize = tf.random_normal_initializer(0,1))#initialize初始化 均值0 方差1
#b = tf.get_variable('b',[1],                  initialize = tf.constant_initializer(0,0))#常量0
#y_=tf.matmul(x,w)+b   #矩阵乘法
#p_y_1 = tf.nn.sigmoid(y_)

'''
w:[3072*1]  b:[1,] 
y=x*w  -->  [None,3072] * [3072,1] = [None,1]
p_y_1:[None,1]
y：[None]
'''


#3072,10
w = tf.get_variable('w',[x.get_shape()[-1],10],initialize = tf.random_normal_initializer(0,1))#initialize初始化 均值0 方差1
#10
b = tf.get_variable('b',[10],                  initialize = tf.constant_initializer(0,0))#常量0
y_=tf.matmul(x,w)+b   #矩阵乘法
#e^x/sum(e^x)
p_y_1 = tf.nn.softmax(y_)
y_one_hot = tf.one_hot(y,10,dtype = tf.float32)
loss =tf.reduce_mean(tf.square(y_one_hot-p_y_1))  #平方差损失函数
predict = tf.argmax(y_,1)
corrent_predict = tf.equal(predict,y)
accurary = tf.reduce_mean(tf.cast(corrent_predict,tf.float64))#准确率



# #[None,1]
# y_reshape = tf.reshape(y,(-1,1))
# y_reshape_float = tf.cast(y_reshape,tf.float32)#转换类型
# loss =tf.reduce_mean(tf.square(y_reshape-p_y_1))  #平方差损失函数
# predict = p_y_1 > 0.5  #预测值
# corrent_predict = tf.equal(tf.cast(predict,tf.int64),y_reshape)#是否相等  [0,0,1,1,0]
# accurary = tf.reduce_mean(tf.cast(corrent_predict,tf.float64))#准确率



#梯度下降
with tf.name_scope('train_op'):
    train_op=tf.train.AdamOptimizer(1e-3).minimize(loss)#10的-3次方



class CifarData:
    def __init__(self,filenames,need_shuffle):#need_shuffle数据更乱
        all_data = []
        all_labels = []
        for filename in filenames:
            data,label = load_data(filename)
            all_data.append(data)
            all_labels.append(label)
            # for item,label in zip(data,label):
            #     if label in [0,1]:
            #         all_data.append(item)
            #         all_labels.append(label)
        self._data =np.vstack(all_data)#向量和平成矩阵
        self._data = self._data / 127.5 -1 # 0~255 ---> -1~1  归一化
        self._label =np.hstack(all_labels)#向量和平成矩阵
        self._num_example = self._data.shape[0]
        self._need_shuffle = need_shuffle
        self._indicator = 0
        if self._need_shuffle:
            self._shuffle_data()
    def _shuffle_data(self):
        p = np.random.permutation(self._num_example)#混排
        self._data = self._data[p]
        self._label = self._label[p]


    def next_batch(self,batch_size):
        end_indicator = self._indicator +batch_size
        if end_indicator > self._num_example:
            if self._need_shuffle:
                self._shuffle_data()
                self._indicator = 0
                end_indicator = batch_size
            else:
                raise Exception("end")
        if end_indicator > self._num_example:
            raise Exception("end")
        batch_data  = self._data[self._indicator:end_indicator]
        batch_label = self._label[self._indicator:end_indicator]
        self._indicator = end_indicator
        return batch_data,batch_label

train_filename = [os.path.join(CIFAR_DIR,'data_batch_%d' %i) for i in range(1,6)]
test_filename = [os.path.join(CIFAR_DIR,'test_batch')]
train_data = CifarData(train_filename,True)
test_data = CifarData(test_filename,False)
#batch_data,batch_abel = train_data.next_batch(10)

init = tf.global_variable_initializer()
batch_size = 20
train_steps = 100000
test_step = 100
with tf.Session() as sess:#会话
    sess.run(init)
    for i in range(train_steps):
        batch_data, batch_abel = train_data.next_batch(batch_size)
        loss_val,acc_val = sess.run([loss,accurary,train_op],feed_dict={x:batch_data,y:batch_abel})#运行计算图
        if i % 500 == 0:
            print('step:%d loss:%4.5f acc:%4.5f' \
                  %(i,loss_val,acc_val))

        if i % 5000 == 0:
            test_data = CifarData(test_filename,False)
            all_test_acc_val = []
            for j in range(test_step):
                test_batch_data,test_batch_label = train_data.next_batch(batch_size)
                test_acc_val = sess.run([accurary],feed_dict = {x:test_batch_data,y:test_batch_label})
                all_test_acc_val.append(test_acc_val)
            test_acc = np.mean(all_test_acc_val)
            print('step:%d acc:%4.5f' \
                  % (i, test_acc))


