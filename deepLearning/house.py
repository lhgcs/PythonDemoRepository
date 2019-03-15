import tensorflow as tf
from mpl_toolkits.mplot3d import Axes3D  # 空间三维画图
import matplotlib.pyplot as plt
import numpy as np
from numpy.random import RandomState
import pandas as pd

#获取数据
df = pd.read_csv(r'C:\Users\Administrator.000\Desktop\训练营\第一周20190128\week01作业第二题的数据\train.csv',engine='python',encoding = "utf-8",header = 0)
# print(X.info)

#数据清洗
df = df[['LotFrontage','LotArea','SalePrice']]
df.apply(pd.to_numeric, errors='ignore')                              #转换为数字类型，无效数据转忽略
#缺失值
df.dropna(axis=0,how='any',subset=['LotArea','SalePrice'],thresh=2)  #删除有空值的行,至少2个缺失值才删除
df['LotArea'].fillna(df['LotArea'].mean())                           #平均值填充缺失值
#异常值
df[df['LotArea'] < 50000]                                            #过滤异常值
df[df['SalePrice'] > 500000] = df['SalePrice'].mean()                #平均值填充异常值

# for column in list(df.columns[df.isnull().sum() > 0]):            #用平均值填充缺失值
#     mean_val = df[column].mean()
#     df[column].fillna(mean_val, inplace=True)

'''
选择行 df[0:5]  df.loc[0:5]  df.iloc[[0,1]]
选择列 df[['LotArea']]
选择行列 df.iloc[1:5,1:5]
'''
X = df[['LotFrontage','LotArea']]
y = df.ix[:,['SalePrice']]

#支持中文显示
from pylab import *
mpl.rcParams['font.sans-serif'] = ['SimHei']
#训练数据散点图
fig = plt.figure()
ax = Axes3D(fig)
# ax.legend(loc='upper left')
ax.set_title('结果',color='r',size=16)
ax.set_zlabel('Y', fontdict={'size': 15, 'color': 'red'})
ax.set_ylabel('X2', fontdict={'size': 15, 'color': 'red'})
ax.set_xlabel('X1', fontdict={'size': 15, 'color': 'red'})
ax.set_xlim(min(X.ix[:,0]),max(X.ix[:,0]))
ax.set_ylim(min(X.ix[:,1]),max(X.ix[:,1]))
ax.set_zlim(min(y.ix[:,0]),max(y.ix[:,0]))
ax.scatter(X.ix[:,0],X.ix[:,1],y.ix[:,0],color='g')
plt.show()


#定义训练数据 batch 的大小
batch_size = 10


x = tf.placeholder (tf.float32 , shape=(None , 2) , name= 'x-input')#存放输入数据   2个输入
y_ = tf.placeholder(tf.float32 , shape=(None , 1) , name= 'y-input')#存放预测值     1个输出

#2层结构，权重w1,w2（从服从指定正太分布的数值中取出指定个数的值）
w1 = tf.Variable(tf.random_normal([2 ,3], stddev=1, seed=1)) #2输入 3输出
w2 = tf.Variable (tf.random_normal([3,1], stddev=1, seed=1)) #3输入 1输出

#定义神经网络前向传播的过程，神经元加权求和
a = tf.matmul (x , w1)
y = tf.matmul (a , w2)
#用 sigmoid 函数（y = 1/(1 + exp (-x))）将值转换为0~1之间的数值。转换后y代表预测是正样本的概率，1-y代表负样本的概率
y=tf.sigmoid(y)

# pd.get_dummies()独热编码
# loss = tf.reduce_mean(tf.aquare(y-y_))
# train_step = tf.train.GradientDescentOptimizer(1e-15).minimize(loss)
# .eval()

#损失函数loss:预测值与真实值的差距  ---> 求取loss最小值  --》 对w求导求导（斜率）等于0
#把y值压缩到0~1之间，再求交叉熵平均值
cross_entropy = -tf.reduce_mean(y * tf.log(tf.clip_by_value(y ,1e-10,1.0))+(1-y)*tf.log(tf.clip_by_value(1-y ,1e-10,1.0 )))
# 定义Optimizer
opt = tf.train.AdamOptimizer(0.001)     #添加操作节点，寻找全局最优点的优化算法  学习率(步长)0.001
# 定义train
train_step = opt.minimize(cross_entropy)#最小化loss
'''
tf.train.GradientDescentOptimizer 实现 梯度下降算法
tf.train.MomentumOptimizer        实现 动量梯度下降算法
tf.train.AdamOptimizer            实现 Adam优化算法

tf.reduce_mean 函数用于计算张量tensor沿着指定的数轴（tensor的某一维度）上的的平均值，主要用作降维或者计算tensor（图像）的平均值。
第二个参数axis： 指定的轴，如果不指定，则计算所有元素的均值;
第三个参数keep_dims：是否降维度，设置为True，输出的结果保持输入tensor的形状，设置为False，输出结果会降低维度;

tf.log函数可以计算元素的自然对数,自然对数以常数e为底数的对数,该函数返回一个张量,并且与x具有相同的类型
tf.clip_by_value(A, min, max)：输入一个张量A，把A中的每一个元素的值都压缩在min和max之间。小于min的让它等于min，大于max的元素的值等于max。
'''

loss = []
XX = []
YY = []
W1 = 0
W2 = 0
#创建会话来运行 TensorFlow 程序
with tf. Session() as sess :
    init_op = tf .global_variables_initializer()#初始化所有变量
    sess.run(init_op)
    #
    # XX = sess.run(np.array(X[:]))  #TypeError: The value of a feed cannot be a tf.Tensor object.
    # YY = sess.run(np.array(y[:]))
    # XX = sess.run(X[:])  #TypeError: The value of a feed cannot be a tf.Tensor object.
    # YY = sess.run(y[:])
    # XX = zip(X.ix[:,0],X.ix[:,1])               #转成坐标
    XX = X[:]
    YY = y[:]

    print('start train data')
    for i in range(1000):
        #选取的样本训练神经网络并更新参数
        #feed_dict：给使用placeholder创建出来的tensor赋值

        #sess.run(train_step,feed_dict= {x:[[1,2],[3,4]], y_:np.array([0,1]).reshape(-1,1)})


        sess.run(train_step,feed_dict= {x:XX, y_:y})#TypeError: The value of a feed cannot be a tf.Tensor object.


        # if i % 100 == 0 :
        #     #计算在所有数据上的交叉煽并输出
        #     total_cross_entropy =sess.run(cross_entropy, feed_dict={x : X, y_ : Y})
        #     print("after %d step entroy is %g" % (i, total_cross_entropy))
        #     loss.append(total_cross_entropy)
    W1 = sess.run(w1)
    W2 = sess.run(w2)
    print(W1)
    print(W2)
# tf.cast(corrent_predict,tf.float64)
# corrent_predict = tf.equal(predict,y)

    def get_predict(x1,x2):
        temp = np.matmul(np.array(x1,x2), W1)
        return np.matmul(temp, W2)
    df = pd.read_csv(r'C:\Users\Administrator.000\Desktop\训练营\第一周20190128\week01作业第二题的数据\test.csv', engine='python',encoding="utf-8", header=0)
    # 数据清洗
    df = df[['LotFrontage', 'LotArea', 'SalePrice']]
    df.dropna(axis=0, how='any')
    right_cnt = 0
    predict_list = []
    for i in range(df.shape[0]):
        y_predict = get_predict(tf.cast(df.ix[i,0],float64),tf.cast(df.ix[i,1]))
        if tf.equal(y_predict,tf.cast(df.ix[i,2],float64)):
            right_cnt +=1
        predict_list.append(y_predict)
    print('test right:',right_cnt/df.shape[0])

    fig = plt.figure() # 得到画面
    ax = fig.gca(projection='3d') # 得到3d坐标的图
    ax.scatter(df.ix[:,0],df.ix[:,1],df.ix[:,2],  c='r',size=20)# 画点
    ax.scatter(df.ix[:,0],df.ix[:,1],predict_list,c='g',size=10)# 画点
    ax.set_title('预测', size=16)
    ax.set_xlabel('x1', size=16)
    ax.set_ylabel('x2', size=16)
# fig1 = plt.figure()
# ax1 = fig1.add_subplot(111)
# #损失函数
#
# ax1.set_ylabel('误差',size=16)
# ax1.plot(range(len(loss)),loss, color='red',marker='o',label='setosa')#随着训练的进行，交叉煽是逐渐变小的。交叉炳越小说明,预测的结果和真实的结果差距越小


# 构建平面
#     surf = ax.plot_surface(X, Y, Z, rstride=1, cstride=1, cmap=cm.jet, linewidth=0, antialiased=True)
#     fig.colorbar(surf, shrink=0.5, aspect=5) # 图例

# 曲面，x,y,z坐标，横向步长，纵向步长，颜色，线宽，是否渐变
#     surf = ax.plot_surface(X, Y, Z, rstride=1, cstride=1, cmap=cm.coolwarm, linewidth=0, antialiased=False)
#     ax.set_zlim(-1.01, 1.01)
#
#     ax.set_xlabel("x-label", color='r')
#     ax.set_ylabel("y-label", color='g')
#     ax.set_zlabel("z-label", color='b')
#
#     ax.zaxis.set_major_locator(LinearLocator(10))  # 设置z轴标度
#     ax.zaxis.set_major_formatter(FormatStrFormatter('%0.02f'))  # 设置z轴精度
#     # shrink颜色条伸缩比例0-1, aspect颜色条宽度（反比例，数值越大宽度越窄）
#     fig.colorbar(surf, shrink=0.5, aspect=5)

plt.savefig('fig.png',bbox_inches='tight')
plt.show()




