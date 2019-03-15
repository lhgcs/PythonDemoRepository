'''
逻辑回归解决多分类：手写数字识别
0-9:10个类 --》10个分类器：判断数字的概率

读入mat文件
scipy是高级科学计算库，一般操控numpy数组进行科学计算，有很多子模块对应不同的应用。插值，优化，图像处理，数字统计等。
scipy.io数据输入输出
loadmat

scipy.optimize.minimize(fun,x0,args=(),method=None,jac=none)
fun 要优化的函数
method 优化方法
jac 梯度向量
x0  参数初始值
args x,y,正则化项


矩阵相乘：
np.dot(A,B),np.matmul(A,B),A@B  对于二维矩阵，矩阵乘积；对于一维矩阵，内积 --《 一个数值
np.multiply(A,B)或者A*B  对应元素相乘，数量积
'''

import numpy as np
import scipy.io as sio
import matplotlib.pyplot as plt

data = sio.loadmat('data.mat')

data.keys()
raw_X = data["X"]
raw_Y = data["Y"]

def plot_on_image(X):
    '显示1张图片'
    one = np.random.randint(5000)
    image = X[one,:]
    fig,ax = plt.subplots(figsize=(1,1))#横轴1，纵轴1
    ax.imshow(image.reshape(20,20),cmap='gray_r')

plot_on_image(raw_X)


def plot_100_image(X):
    '显示100张图片'
    index = np.random.choice(5000,100)#选出100个
    image = X[index,:]                  #取100行
    fig,ax = plt.subplots(nrows=10,ncols=10,figsize=(8,8),sharex=True,sharey=True)#10行10列，共享xy
    for i in range(10):
        for j in range(10):
            ax[i,j].imshow(image[10*i+j].reshare(20,20).T,cmap='gray_r')

    plt.xticks([])

def signal(z):
    return 1/(1+np.exp(-z))

def costFumtion(theta,X,y,lamda):
    '损失函数'
    A = signal(X@theta)
    #交叉熵
    first = y*np.log(A)
    second = (1-y)*np.log(1-y)
    #正则化
    #reg = np.sum(np.power(theta[1:],2) * lamda/(2*len(X)))
    reg = theta[1:] @ theta [1:]  #theta一维数组
    return -np.sum(first+second)/len(X) + reg

def gradientDescent(X,y,theta,alpha,iters,lamda):
    '梯度下降'
    costs =[]
    for i in range(iters):
        reg = theta[1:]*(lamda/len(X))
        reg = np.insert(reg,0,values=0,axis=0)

        theta = theta - (X.T@(signal(X@theta)-y)) *alpha/len(X) -reg
        cost = costFumtion(theta,X,y,lamda)
        costs.append(cost)
    return theta,costs


def gradient_reg(theta,X, y, lamda):
    '梯度向量'
    reg = theta[1:] * (lamda / len(X))
    reg = np.insert(reg, 0, values=0, axis=0)

    theta = (X.T @ (signal(X @ theta) - y)) / len(X) + reg
    return theta

X = np.insert(raw_X,0,values=1,axis=1)
Y = raw_Y.flatten()

#优化10个分类器
from scipy.optimize import minimize

def one_vs_all(X,y,lamda,K):#[K,401]  k个类
    n = X.shape[1]          #特征个数
    theta_all = np.zeros((K,n))

    for i in range(1,K+1):
        theta_i = np.zeros(n,)#一维数组
        reg = minimize(fun=costFumtion,x0=theta_i,args=(X,y==i,lamda),method='TNC',jac=gradient_reg)
        theta_all[i-1,:] = reg.x

    return theta_all

lamda = 1
K=10
theta_all = one_vs_all(X,y,lamda,K)

def predict(X,theta_final):
    h = signal(X@theta_final.T)
    h_arg = np.argmax(h,axis=1)#沿列取最大值，返回索引
    return h_arg+1

y_pre = predict(X,theta_all)

#准确率
acc = np.mean(y==y_pre)


'''
神经网络实现，前向传播
输入层  a1 = x      输入特征
隐藏层  z2 = θ1a1  a2 = g(z2)   θ权重参数，g激活函数
输出层  z3 = θ2a2  a3 = g(z3) = h(x) 
'''
#添加常数项即偏置
X = np.insert(raw_X,0,values=1,axis=1)#原矩阵，索引，值，方向
#降为一维数组
Y = raw_Y.flatten()

theta = []
theta1 = theta['Theta1']
theta2 = theta['Theta2']

def sigmoid(z):
    return 1/(1+np.exp(-z))

a1 = X
z2 = X@theta.T
a2 = sigmoid(z2)
a2.insert(a2,0,1,axis=1)
z3 = a2 @ theta2.T
a3 = sigmoid(z3)





'''
神经网络实现，反向传播
输入层  a1 = x      输入特征
隐藏层  z2 = θ1a1  a2 = g(z2)   θ权重参数，g激活函数
输出层  z3 = θ2a2  a3 = g(z3) = h(x)


one-hot编码：每个样本的单特征只有一位处于状态1，其它都是0
对y做变换：
y=[1,0,0],[0,1,0],[0,0,1]


损失函数
无正则化:
J=(-1/m)np.sum(y*np.log(h)+(1-y)*np.log(1-h))   有多个y，需要独热编码扩展
带正则化：
reg = (lamda/2m)*np.sum(theta1[:,1:]**2)+np.sum(theta2[:,1:]**2)]
'''

#添加常数项即偏置
X = np.insert(raw_X,0,values=1,axis=1)#原矩阵，索引，值，方向
#降为一维数组
Y = raw_Y.flatten()

def one_hot_encoder(raw_y):
    '对y进行独热编码处理'
    result=[]
    for i in raw_y:
        y_temp = np.zeros(10)
        y_temp[i-1] = 1
        result.append(y_temp)
    return np.array(result)

#对y进行独热编码处理
Y = one_hot_encoder(Y)


#序列化权重参数   因为scipy接受的是一维数组
def serialize(a,b):
    return np.append(a.flatten(),b.flatten())

theta = []
theta1 = theta['Theta1']
theta2 = theta['Theta2']
theta_serial = serialize(theta1,theta2)

#解序列化权重参数
def deserialize(theta_serial):
    theta1 = theta_serial[:25*401].reshape[25,401]
    theta2 = theta_serial[25*401:].reshape[10,26]
    return theta1,theta2

theta1,theta2 = deserialize(theta_serial)


#前向传播
def feed_forward(theta_serial,X):
    theta1, theta2 = deserialize(theta_serial)
    a1 = X
    z2 = X @ theta1.T
    a2 = sigmoid(z2)
    a2.insert(a2, 0, 1, axis=1)
    z3 = a2 @ theta2.T
    a3 = sigmoid(z3)
    return a1,z2,a2,z3,a3

#不带正则化的损失函数
def cost(theta_serial,X,y):
    a1, z2, a2, z3, a3= feed_forward(theta_serial,X)
    J=-np.sum(y*np.log(a3)+(1-y)*np.log(1-a3)) / len(X)
    return J

#带正则化的损失函数
def reg_cost(theta_serial,X,y,lamda):
    D = gradient(theta_serial)
    D1,D2 = deserialize(D)
    theta1,theta2 = deserialize(theta_serial)
    D1[:,1:] = D1[:,1:] + theta1[:,1:] *lamda /len(X)
    D2[:,1:] = D2[:,1:] + theta2[:,1:] *lamda /len(X)
    return serialize(D1,D2)

#反向传播
#求导后的激活函数
def sigmoid_gradient(z):
    return sigmoid(z)*(1-sigmoid(z))
'''
J对θ1求导=（h-y）a2=d3a2
J对θ2求导=（h-y）θ2g'(z2)a1=d2a1

正则化：
reg1=(lamda/m)*theta1[:,1:]
reg2=(lamda/m)*theta2[:,1:]
'''

#无正则化的梯度
def gradient(theta_serial,X,y):
    theta1,theta2 = deserialize(theta_serial)
    a1, z2, a2, z3, a3 = feed_forward(theta_serial, X)
    d3 = a3 - y
    d2 = d3 @ theta2[:,1:]*sigmoid_gradient(z2)
    D2 = (d3.T @ a2)/len(X)
    D1 = (d2.T @ a1)/len(X)
    return serialize(D1,D2)

#正则化的梯度
def reg_gradient(theta_serial,X,y,lamda):

    theta1,theta2 = deserialize(theta_serial)
    a1, z2, a2, z3, a3 = feed_forward(theta_serial, X)
    d3 = a3 - y
    d2 = d3 @ theta2[:,1:]*sigmoid_gradient(z2)
    D2 = (d3.T @ a2)/len(X)
    D1 = (d2.T @ a1)/len(X)
    return serialize(D1,D2)

#神经网络优化
def nn_train(X,y):
    init_theta = np.random.uniform(-0.5,0.5,10285)#-0.5~0.5之间，均匀分布，取10285个
    res = minimize(fun=cost,x0=init_theta,args=(X,y,lamda),method='TNC',jac=reg_gradient,options={'maxiter':300} )
    return res

res = nn_train(X,y)
raw_y = data['y'].reshape(5000,)

_,_,_,_,h=feed_forward(res.x,X)
y_pre = np.argmax (h,axis=1)+1
acc = np.mean(y_pre==y)


#可视化隐藏层
def plot_hidden_layer(theta):
    theta1,_ = deserialize(theta)
    hidden_layer = theta1[:,1:]
    fig,ax = plt.subplots(nrows=5,ncols=5,figsize=(8,8),sharex=True,sharey=True)

    for i in range(5):
        for j in range(5):
            ax[i,j].imshow(images[10*i+j].reshape(20,20).T,cmap='gray_r')
    plt.show()

plot_hidden_layer(res.x)

'''
偏差和方差
偏差：预测值和真实值的差距，表示是算法本身的拟合能力
方差：预测值的变化范围，表示数据扰动造成的影响

训练集：训练模型
验证集：模型选择。模型的最终优化
测试集：利用训练好的模型测试其泛化能力

案例：利用水库水位变化预测大坝的出水量
'''

#读取
from scipy.io import loadmat  #matlab文件
data = loadmat('1.mat')
data.keys()

#训练集
X_train,y_train = data['X'],data['y']
X_train.shape,y_train.shape  #行：样本格式，列：特征个数

#验证集
X_val,y_val=data['Xval'],data['yval']
X_val.shape,y_val.shape

#测试集
X_test,y_test = data['Xtest'],data['ytest']

X_train = np.insert(X_train,0,values=1,axis=1)
X_val = np.insert(X_val,0,values=1,axis=1)
X_test = np.insert(X_test,0,values=1,axis=1)

def plot_data():
    fig,ax = plt.sublots()
    ax.scatter(X_train[:,1:],y_train[:,1:])
    ax.set(xlabel='x',ylabel='y')

plot_data()

'''
线性回归：获取损失函数，梯度，用scipy的优化函数优化
损失函数：J(θ)=(1/2m)sum((Xθ-y)^2 + (λ/2m)θ^2
梯度（即对θ求导）：
reg=（1/m)(Xθ-y)X + (λ/m)θ    θ的第一个参数不参与正则化
'''

def reg_cost(theta,X,y,lamda):
    cost = np.sum(np.power(X@theta-y.flatten(),2))
    reg = theta[1:] @ theta[1:] *lamda
    return (cost+reg)/(2*len(X))

theta = np.ones(X_train.shape[1])
lamda = 1
reg_cost(theta,X_train,y_train,lamda)

#梯度
def reg_gradient(theta,X,y,lamda):
    grad = (X@theta - y.flatten()) @ X
    reg = lamda * theta
    reg[0] = 0
    return (grad+reg)/len(X)

reg_gradient(theta,X_train,y_train,lamda)

def train_model(X,y,lamda):
    theta = np.ones(X.shape[1])
    res = minimize(fun=reg_cost,x0=theta,args=(X,y,lamda),method='TNC',jac=reg_gradient)
    return res.x

theta_final = train_model(X_train,y_train,lamda=0)

#样本
plot_data()
#拟合
plt.plot(X_train[:,1],X_train@theta_final)
plt.show()


def plot_learning_curve(X_train,y_train,X_val,y_val,lamda):
    x = range(1,len(X_train)+1)
    train_cost = []   #训练误差
    cv_cost = []      #验证误差

    for i in x:
        res = train_model(X_train[:i,:],y_train[:i,:],lamda)
        train_cost_i = reg_cost(res,X_train[:i],y_train[:i,:],lamda)
        cv_cost_i = reg_cost(res,X_val,y_val,lamda)
        train_cost.append(train_cost_i)
        cv_cost.append(cv_cost_i)
    plt.plot(x,train_cost,label='train cost')
    plt.plot(x,cv_cost,label='cv cost')
    plt.legend()
    plt.xlabel('train num')
    plt.ylabel('cost')
    plt.show()

#训练集误差逐渐增大，验证集误差逐渐减小，最终稳定在一定值。高偏差则欠拟合。


'''
简单的线性模型造成欠拟合
Jtrain和Jcv同时很大，则高偏差问题。Jcv比Jtrain大很多，则高方差
高方差：采集更多的数据样本；减少特征数量，去除非主要特征；增加正则化参数
高偏差：引入更多的相关特征；采用多项式特征；减小正则化参数

解决方法：
构造多项式特征，进行多项式回归
h=θ0+θ1*x+θ2*x^2+θ3*x^3

'''
def poly_feature(X,power):
    for i in range(2,power+1):
        X = np.insert(X,X.shape[1],np,power(X[:,1]),axis=1)
        return X

#获取均值，方差
def get_mean_std(X):
    mean = np.mean(X,axis=0)
    std = np.std(X,axis=0)
    return mean,std

#归一化
def feature_normalize(X,mean,std):
    X[:,1:] = (X[:,1:]-mean[1:])/std[1:]
    return X

power =6

X_train_poly = poly_feature(X_train,power)
X_val_poly = poly_feature(X_val,power)
X_test_poly = poly_feature(X_test,power)

train_mean,train_std = get_mean_std(X_train_poly)

#归一化
X_train_normal = feature_normalize(X_train_poly,train_mean,train_std)
X_val_normal = feature_normalize(X_val_poly,train_mean,train_std)
X_test_normal = feature_normalize(X_test_poly,train_mean,train_std)


#模型训练，获取最优theta
theta_fit=train_model(X_train_normal,y_train,lamda)

def plot_poly_fit():
    plot_data()
    x=np.linsapce(-60,60,100)
    xx = x.reshape(100,1)
    XX = np.insert(xx,0,1,axis=1)
    xx = poly_feature(xx,power)
    xx = feature_normalize(xx,train_mean,train_std)
    plt.plot(x,xx@theta_fit,'r--')

plot_poly_fit()
plot_learning_curve(X_train_normal,y_train,X_val_normal,y_val,lamda)

#正则化参数的选择

lamdas = [0,0.001,0.01,0.1]
train_cost = []
cv_cost = []
for lamda in lamdas：
    res = train_model(X_train_normal,y_train,lamda)
    tc = reg_cost(res,X_train_normal,y_train,lamda,lamda=0)
    tv = reg_cost(res,X_val_normal,y_val,lamda=0)
    train_cost.append(tc)
    cv_cost.append(tv)

plt.plot(lamdas,train_cost,label='train cost')
plt.plot(lamdas,cv_cost,label='cv cost')
plt.legend()
plt.show()

lamdas[np.argmin(cv_cost)]#验证误差最小时的lamda

#测试集
res = train_model(X_train_normal,y_train,lamda = 3)
test_coset = reg_cost(res,X_test_normal,y_test,lamda=0)




































