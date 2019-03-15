# encoding=utf8

"""
线性回归:y=kx+b  一个输入

y=∑θnXn+θ0 (1-n)= ∑θnXn （0-n)

最小二乘法：
Y=θX
L=1/2(θX-Y)^T(θX-Y）  损失函数：找到θ使得L最小
θ= (X^TX)^(-1)X^-1Y  求导

梯度下降：

"""

import numpy as np
from numpy.linalg import inv #逆
fron numpy import dot  #点乘
from numpy import mat  #矩阵

A = np.mat([1,1])
B = np.mat([1,2],[2,3])
dot(A,B)  #A.B
dot(B,A.T)  #
inv(B) #逆
B[0,:]
B[:,0]
A,reshape(2,1)

#y=2x
X=mat([1,2,3]).reshape(3,1)
Y=2*X

#最小二乘法
#theta = (X^TX)^-1 X^TY
theta = dot(dot(inv(dot(X.T,X)),X.T),Y)

#梯度下降
#theta = theta - alpha*(theta*X-Y)*X   alpha 学习速率，梯度下降的速率不要太快 （0-1）
theta = 1
alpha = 0.1
for i in range(100):
    theta = theta + np.sum(alpha * (Y-dot(X,theta)) *X.reshape(1,3))/3   #/3--->3组数据



import pandas as pd
daraset = pd.read_csv("data,csv")

#序号 Y X1 X2 X3

temp = dataset.iloc[:,2:5]  #X1 X2 X3
temp['x0'] = 1              #截距
X= temp.iloc[:,[3,0,1,2]]   #X0 X1 X2 X3
Y = dataset.iloc[:,1].reshape[150,1]

#最小二乘
theta = dot(dot(inv(dot(X.T,X)),X.T),Y)
#梯度下降
theta = np.array([1,1,1,1]).reshape(4,1)
alpha = 0.1
X0 = X.iloc[:,0].values.reshape(150,1)  #150组数据
X1 = X.iloc[:,1].reshape(150,1)
X2 = X.iloc[:,2].reshape(150,1)
X3 = X.iloc[:,3].reshape(150,1)

temp = theta
for i in range(100):
    temp[0] = theta[0] + np.sum(alpha * (Y-dot(X,theta)) *X0)/150.   #/3--->3组数据
    temp[1] = theta[1] + np.sum(alpha * (Y - dot(X, theta)) * X1) / 150.
    temp[2] = theta[2] + np.sum(alpha * (Y - dot(X, theta)) * X2) / 150.
    temp[3] = theta[3] + np.sum(alpha * (Y - dot(X, theta)) * X3) / 150.
    theta = temp




























