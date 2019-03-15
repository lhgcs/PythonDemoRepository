'''
1. 创建「计算图」，表示计算的数据流
2. 运行「会话」，执行图中的运算

pip install tensorflow
pip install tensorflow-gpu   gpu包

File->Settings->Project-〉Project interpreter选择anaconda自带的python
'''

import tensorflow as tf

#tf.enable_eager_execution() #动态图机制，在没有session时运行代码
#v = tf.add(1, 2).numpy()    #参数相加
v = tf.add(1, 2)    #参数相加
print(v)

'''
tf.add 和 tf.nn.bias_add 的区别
tf.nn.bias_add 是 tf.add 中的一个特例，tf.nn.bias_add 中 bias 一定是 1 维的张量；
tf.nn.bias_add 中 value 最后一维长度和 bias 的长度一定得一样；
tf.nn.bias_add 中，当 value 是 quantized type 时，bias 可以取不同的 quantized type
'''

gh = tf.Graph()          #生成新的计算图，不同计算图中的张量（多维数组）和运算是独立的。零阶张量（标量）：一个数
with gh.device('/gpu:0'):#指定在GPU上运行
    hello = tf.constant('Hello, TensorFlow!')  # 常量，tf.constant是一个op（算子）
    #print(hello)
    with tf.Session() as sess: #创建会话
        tf.global_variables_initializer()
        print(sess.run(hello))
    # 任务完成, 关闭会话.
    #sess.close()  with自动close了

sess = tf.InteractiveSession() #默认会话
a = tf.constant([1,2],name='a')#张量唯一标识a
b = tf.constant([3,4],name='b')#张量唯一标识b
print('a:',type(a))
print('a+b:',a+b)
c = tf.Variable(tf.ones([3]))
d = tf.Variable(tf.ones([3]))
# e = tf.matmul(c,d)#矩阵乘法
# print('e:',e)

'''
生成一个给定值的常量         tf.constant(value, dtype, shape, name)
生成一个全部为给定数字的数组 tf.fill(dims, value, name)
生成全1数组                 tf.ones(shape, dtype, name)
生成全0数组                 tf.zeros(shape, dtype, name)

tf.Variable是一个类，用于实例化对象，然后用对象初始化多个op
变量的类型是不可改变的
validate_shape= False 时维度可变
'''
x = tf.Variable(1)
# x.initializer          # 初始化单个变量
# x.value()              # 读取op
# x.assign()             # 写入op
# x.assign_add()         # 更多op
# x.eval()               # 输出变量内容
x2 = tf.Variable(x.initialized_value())
print('x2:',x2)

#通过随机数设置初始值
weights = tf.Variable(tf.random_normal([2, 3], stddev = 2, seed = 1))#2*3，元素均值为0，标准差为2的矩阵。参数seed设定随机种子
#通过常数设置初始值
biases = tf.Variable(tf.zeros([3]))
#通过其他变量设置初始值

#初始化全部变量
init = tf.global_variables_initializer()
with tf.Session() as sess:
    sess.run(init)

#初始化单独一个变量
W = tf.Variable(tf.zeros([784, 10]))
with tf.Session() as sess:
    sess.run(W.initializer)

a = tf.Variable(tf.random_uniform([3]))
b = tf.Variable(tf.random_uniform([3]))

#初始化一个变量子集
init_ab = tf.variables_initializer([a, b], name = "init_ab")
with tf.Session() as sess:
    sess.run(init_ab)

print(a)

from mpl_toolkits.mplot3d import Axes3D  # 空间三维画图
import matplotlib.pyplot as plt
import numpy as np
from numpy.random import RandomState

#定义训练数据 batch 的大小，一次8个
batch_size = 8
#在 shape 个维度上使用 None 可以方便使用不同的 batch 大小。在训练时需要把数据分成比较小的 batch
x = tf.placeholder (tf.float32 , shape=(None , 2) , name= 'x-input')#存放输入数据   2个输入
y_ = tf.placeholder(tf.float32 , shape=(None , 1) , name= 'y-input')#存放预测值     1个输出

'''
从服从指定正太分布的数值中取出指定个数的值
tf.random_normal(shape, mean=0.0, stddev=1.0, dtype=tf.float32, seed=None, name=None)
shape: 输出张量的形状，必选
mean: 正态分布的均值，默认为0
stddev: 正态分布的标准差，默认为1.0
dtype: 输出的类型，默认为tf.float32
seed: 随机数种子，是一个整数，当设置之后，每次生成的随机数都一样
name: 操作的名称
'''
#2层结构，权重w1,w2
w1 = tf.Variable(tf.random_normal([2 ,3], stddev=1, seed=1))   #2输入 3输出
w2 = tf.Variable (tf.random_normal([3,1] , stddev=1, seed=1)) #3输入 1输出

#定义神经网络前向传播的过程，神经元加权求和
a = tf.matmul (x , w1)
y = tf.matmul (a , w2)
#用 sigmoid 函数（y = 1/(1 + exp (-x))）将值转换为0~1之间的数值。转换后y代表预测是正样本的概率，1-y代表负样本的概率
y=tf.sigmoid(y)

#损失函数loss:预测值与真实值的差距  ---> 求取loss最小值  --》 对w求导求导（斜率）等于0
#把y值压缩到0~1之间，再求平均值
cross_entropy = -tf.reduce_mean(y * tf.log(tf.clip_by_value(y ,1e-10,1.0))+(1-y)*tf.log(tf.clip_by_value(1-y ,1e-10,1.0 )))
# 定义Optimizer
opt = tf.train.AdamOptimizer(0.001)#学习率(步长)0.001
# 定义train
train_step = opt.minimize(cross_entropy)
'''
tf.train.GradientDescentOptimizer 实现 梯度下降算法
tf.train.MomentumOptimizer        实现 动量梯度下降算法
tf.train.AdamOptimizer            实现 Adam优化算法

tf.reduce_mean 函数用于计算张量tensor沿着指定的数轴（tensor的某一维度）上的的平均值，主要用作降维或者计算tensor（图像）的平均值。
reduce_mean(input_tensor,
                axis=None,
                keep_dims=False,
                name=None,
                reduction_indices=None)
第一个参数input_tensor： 输入的待降维的tensor;
第二个参数axis： 指定的轴，如果不指定，则计算所有元素的均值;
第三个参数keep_dims：是否降维度，设置为True，输出的结果保持输入tensor的形状，设置为False，输出结果会降低维度;
第四个参数name： 操作的名称;
第五个参数 reduction_indices：在以前版本中用来指定轴，已弃用;

tf.log函数可以计算元素的自然对数,自然对数以常数e为底数的对数,该函数返回一个张量,并且与x具有相同的类型
tf.clip_by_value(A, min, max)：输入一个张量A，把A中的每一个元素的值都压缩在min和max之间。小于min的让它等于min，大于max的元素的值等于max。
'''

#随机数产生训练数据集
dataset_size = 128
rdm = RandomState(1)         #产生一个随机状态种子，只要随机种子seed相同，产生的随机数序列就相同
X = rdm.rand(dataset_size ,2)# 生成128*2的随机数
#X = rdm.uniform(0,1,(2,3))  # 2行3列,在[0,1]区间的均匀分布的随机数
#定义规样本的标签。在这里所有 xl+x2<1 都被认为是正样本
Y = [1 if xl+x2 < 1 else 0 for (xl , x2) in X]
Y = np.mat(Y).T  #list转为矩阵
print('X:',X)
print('Y:',Y)
li = [x1+x2 for (x1,x2) in X]

loss = []
XX = []
YY = []
W1 = 0
W2 = 0
#创建会话来运行 TensorFlow 程序
with tf. Session() as sess :
    init_op = tf .global_variables_initializer()#初始化所有变量
    sess.run(init_op)
    XX = X[:]
    YY = Y[:]
    print('w1:',sess.run(w1))
    print('w2:',sess.run(w2))

    print('start train data')
    STEPS = 5000  #训练5000次
    for i in range(STEPS):
        start = (i * batch_size % dataset_size)    #每次选取8个样本进行训练
        end = min (start+batch_size , dataset_size)
        #print(start,end)
        #选取的样本训练神经网络并更新参数
        #feed_dict：给使用placeholder创建出来的tensor赋值
        temp1 = X[start:end] #8行1列  1列中有2个元素
        temp2 = Y[start:end] #8行1列
        # print(temp1)
        # print(temp2)
        sess.run(train_step,feed_dict= {x:temp1, y_:temp2})
        if i % 100 == 0 :
            #计算在所有数据上的交叉煽并输出
            total_cross_entropy =sess.run(cross_entropy, feed_dict={x : X, y_ : Y})
            print("after %d step entroy is %g" % (i, total_cross_entropy))
            loss.append(total_cross_entropy)
    W1 = sess.run(w1)
    W2 = sess.run(w2)
    print(W1)
    print(W2)
    print(type(w1))
    print(type(W1))

fig1 = plt.figure()
ax1 = fig1.add_subplot(111)
#损失函数
ax1.set_title('误差',size=16)
ax1.set_xlabel('训练次数',size=16)
ax1.set_ylabel('误差',size=16)
ax1.plot(range(len(loss)),loss, color='red',marker='o',label='setosa')#随着训练的进行，交叉煽是逐渐变小的。交叉炳越小说明,预测的结果和真实的结果差距越小

#验证
#x1 = np.linspace(0,1,20)
#x = np.random.rand(100,2)#生成10x2的二维数组,[0,1)之间的随机浮点数
rdm = RandomState(1)
x = rdm.rand(100 ,2)
y = []   #计算值   y=x1+x2
y_ = []  #预测值
for i in x:
    y.append(i[0]+i[1])
    y_.append(np.dot(np.dot(i,W1),W2))

#支持中文显示
from pylab import *
mpl.rcParams['font.sans-serif'] = ['SimHei']

# 绘制三维散点图
fig = plt.figure()
ax = Axes3D(fig)
# 添加坐标轴(顺序是Z, Y, X)
plt.legend(loc='upper left')
ax.set_title('结果',size=16)
ax.set_zlabel('Y', fontdict={'size': 15, 'color': 'red'})
ax.set_ylabel('X2', fontdict={'size': 15, 'color': 'red'})
ax.set_xlabel('X1', fontdict={'size': 15, 'color': 'red'})
ax.set_zlim(0,3)
ax.scatter(XX[:,0], XX[:,1], YY)
ax.scatter(x[:,0], x[:,1], y,color='red')
ax.scatter(x[:,0], x[:,1], y_,color='green')

plt.savefig('fig.png',bbox_inches='tight')
plt.show()








