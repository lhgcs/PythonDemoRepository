'''
2019/01/14  初十 情人节
吴恩达作业
'''

'''
单变量线性回归
案例：开分店，根据城市的人口预测利润

训练数据集：
人口数据,利润   ---》 X，Y
6.11  17.59
5.52  9.13
8.51  13.66
7.00  11.85

得到线性回归函数：h(x)=W0 + W1*x  最大程度拟合数据的分布
求出 W1 W2
代价函数cost funtion J=(1/2m)sun(h(i)-y(i))^2  ---》计算每个样本的误差再求和
把每个训练样本的x代入h,与实际值y求差值（即x一样时，y的差值）。把所有的样本代入求平方和，再除m求平均，除2时因为求导时产生2，除不除2没关系
目标：最小化代价函数

第一步：确定代价函数
第二步：最小化代价函数

（1）初始化参数W0 W1 为0
（2）改变W0 W1 减小代价函数，直到最小值

梯度下降法：求最值
W = W -α*(∂J/∂W)   偏微分---》梯度方向
迭代到某一时刻，代价函数的值不变或者变化很小时迭代完成

h(x)=W0+W1*x = X*W
w=[W0
   W1]
X=[1 x]
代价函数：J=(1/2m)sun(X*W-y(i))^2
梯度下降公式：
W0=W0-α*(1/m)sum(h(xi)-y(i))x(i)0
W1=W1-α*(1/m)sum(h(xi)-y(i))x(i)1
w=[W0
   W1]
W=W-αX^T(X*W-y)
维度：X(m,n)
    y(m,1)
theta(n,1)
'''


import numpy as np    #科学计算库，处理多维数据
import pandas as pd   #基于numpy，二维数据，提供行标列标
import matplotlib.pyplot as plt
#matplotlib  2D绘图库
#matplot.pyplot 提供类似MATLAB的绘图框架

#sed 字符串分隔符
#header 指定第几行作为列名，没有指定列名默认header=0
#names 指定列名

#读取数据
data = pd.read_csv('data',names=['population','porfit'])
data.head()
data.tail()
data.describe()#均值方差等
data.info()

data.plot.scatter('population','porfit',label='population')
plt.show()

#添加1列
data.insert(0,'ones',1)
X=data.iloc[:,0:-1]
y=data.iloc[:,-1]
#转成数组
X=X.values
y=y.values
y=y.reshape(y.shape[0],1)

def costFuntion(X,y,theta):
    '''
    损失函数
    :param X:
    :param y:
    :param theta:
    :return:
    '''
    inner = np.power(X@theta - y,2)
    return np.sum(inner)/(2*len(X))

#初始化
theta = np.zeros((2,1))
cost_init = costFuntion(X,y,theta)

#梯度下降
def gradlientDescent(X,y,theta,alpha,iters):
    costs = []
    for i in range(iters):
        theta = theta - (X.T @ (X @ theta - y)) *alpha/len(X)
        cost = costFuntion(X,y,theta)
        costs.append(cost)
    return theta,costs

#训练
alpha = 0.01
iters = 2000  #迭代2000次
theta,costs=gradlientDescent(X,y,theta,alpha,iters)

# fig,ax=plt.subplots(2,3)#fig画布，ax实例
# ax1 = ax[0,0]
fig,ax = plt.subplot
ax.plot(np.arange(iters),costs)
ax.set(xlabel='iter',ylabel='cost',title='cost and iter')
plt.show()


x = np.linspace(y.min(),y.max(),100)
y_ = theta[0,0]+theta[1,0]*x
fig,ax=plt.subplot()
ax.scatter(X[:,1],y,label='train data')
ax.plot(x,y_,label='pridict')
ax.legend()
ax.set(xlabel='population',ylabel='porfit',title='cost and iter')


#用正规方程
def normalEquation(X,y):
    theta = np.linalg.inv(X.T @ X)@X.T @y
    return theta
'''
多变量线性回归

数据预处理：特征归一化
消除特征值之间量纲的影响，各特征值处于同一数量级
提升模型的收敛速度   ---》 等高线圆
提升模型的精度

归一化方法：
1.归一化到标准到正态分布 z=(x(i)-μ)/σ   μ特征的均值  σ特征的标准差   量化后的特征分布在[-1,1]区间
2.z=(x(i)-min(x))/(max(x)-min(x))    量化后特征分布到[0,1]

'''
#读取数据
data = pd.read_csv('txt',names=['size','bedrooms','price'])
data.head()

#特征归一化 --》 数据的量级在相同的范围
def normalize_feature(data):
    return (data-data.mean())/data.std()

data = normalize_feature(data)

data.plot.scatter('size','price',label='size')
data.plot.scatter('bedroom','price',label='size')
plt.show()

#添加全为1的列
data.insert(0,'ones',1)

#构造数据集
X=data.iloc[:,0:-1]
y=data.iloc[:,-1]

#转为数组
X = X.values
y=y.values
y=y.reshape(y.shape,1)

theta = np.zeros((3,1))
cost_init = costFuntion(X,y,theta)

#不同alpha的效果
alpha = [0.003,0.03,0.01,0.001,0.0001]
iters = 2000  #迭代2000次

fig,ax = plt.subplots()
for i in alpha:
    _,costs = gradlientDescent(X,y,theta,i,iters)
    ax.plot(np.arange(iters),costs,label=i)
ax.legend()
ax.set(xlabel='iter',ylabel='cost',title='cost and iter')
plt.show()


'''
正规方程  一下子算出w
w=(X^T X)^-1 X^T y  
如果不可逆，一般考虑两种情况：或求伪逆矩阵
1.移除冗杂特征，一些特征存在线性依赖，删除几个
2.特征太多时，删除一些特征。对于小样本数据使用正则化

numpy.linalg模块包含线性代数的函数，求逆矩阵，特征值，解线性方程组，求解行列式
inv函数计算逆矩阵
 
梯度下降：
需要选择学习率
需要多次迭代
特征值范围相差太大时要特征缩放
当特征数n很大时，能工作很好

正规方程：
不需要选择学习率
不需要多次迭代
不需要特征缩放
当特征数n很大时，运算很慢，因为求逆矩阵的时间复杂度是O(n^3)

何时选择梯度下降，正规方程：
n<10000时，用正规方程，n>=10000时，用梯度下降
一些复杂算法只能用梯度下降
 
 
 
 
'''









