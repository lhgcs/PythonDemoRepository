'''
机器学习：多领域交叉学科（概率论/统计学/逼近论），研究计算机怎么模拟或实现人类的学习行为，以获得知识或技能
模型/数据

机器学习：监督（输入的数据输出预测的结果（数据有标记））   分类 ---〉 逻辑回归（二分类）   预测归属的类别
                                                        预测
         无监督（数据没有标记                            聚类

评价指标：
准确率：accuracy = （TP+TN）/（TP+TN+FP+FN)   预测正确的比例
精度：  precision = TP/(TP+FP)                预测结果中感兴趣的类别的预测正确的比例
召回率：recall = TP/(TP+FN)                   数据的数据中感兴趣的类别的预测正确的比例
F1 score：F1=2pr/(p+r)

逻辑回归（LR），广义的线性回归分析模型   -》分离

特征向量x,线性变换，sigmoid,      预测结果
样本特征     z    y=1/(1+e^(-z))  样本属于0/1
z=w^Tx+b=[w^T b][x
                 1]

逻辑回归模型就是每个特征的回归系数w^T
性质：线性分类器，无法解决非线性问题
      通过训练数据集，计算出最合适的系数向量
      最合适：错误概率最低

极大似然估计：利用已知的样本结果，反推最有可能导致这样结果的参数值
P（A|θ）=1/9   A：结果   θ样本

利用实验结果D={x1,……xn},得到参数值θ，使样本出现的概率最大
L（θ）=P（D|θ）=∏P(xi|θ)  似然函数
求解θ，θ=argmax(L(θ))


假设样本服从正太分布N（μ,σ^2），用似然法估计参数
(1)建立对数似然函数
㏑L(μ,σ^2)
(2)求解使似然函数最大的参数
∂㏑L(μ,σ^2)/∂μ=0
∂㏑L(μ,σ^2)/∂σ^2=0


用极大似然建立损失函数

输入特征向量，输出0-1的概率值，表示其为正例的概率
y=1/(1+e^(-z)) = e^z/(1+e^z)  （给定输入x，输出标签为1的概率）

该样本属于真实标签的概率
p(y=1|x)=Φ(z)= e^z/(1+e^z)
p(y=0|x)=1-Φ(z)
最大化样本属于真实标签的概率，则采用极大似然估计。采用梯度上升法最大化似然函数

梯度grad(x,y)：方向向量，表示某一函数在某一点沿改方向变化最大
grad(x,y)=▽f(x,y)={∂f(x,y)/∂x,∂f(x,y)/∂y}

f(x,y)=x^2+2xy+2y^2
grad(x,y)={2x+2y,2x+4y}
在（1,2）点的梯度grad(x,y) = {2x+2y,2x+4y} = (6,10)


梯度上升：
1.初始化回归系数向量x
2.重复迭代max_loop次
  计算本次迭代的梯度∂l(w)/∂w = x*error
  计算w+α*梯度 = w+α*x*error
  更新w

逻辑回归伪代码：
[X,Y] = load_data()
w = rang（m，1）
α= 0.3  学习率（每次的步长），越大可能错过w最佳值（导致震荡），越小收敛越慢,一般0.01-0.05

正则项系数在于解决过拟合，所以观察测试误差和系数变化曲线
正则项系数，影响损失函数，决定模型好坏。
系数为0，即不设置正则
系数无穷大，即不考虑经济损失

for i in range(max_loop):#越大越好
  y_pre = log_reg(X)   X的逻辑回归输出值
  error = Y - y_pre
  grad = X * error
  wtemp = w+α*grad
  w=wtemp

最大熵原理：在所有可能的概率模型中，熵最大的模型最好。
假设离散随机变量X的概率分布为P（X），则其熵为H(P)=-∑P(x)*logP(x)  0<=H(P)<=log|P|   等概率时熵最大


数据集：特征+标签
二分类：标签值取0/1，包含2个特征x1,x2,x1与标签成正比，x2成反比
采样误差：数据采集有误差，服从正太分布N（0,1）
数据集：x1,x2在【-5,5】均匀分布，随机采样500个样本点作为数据集

'''

from numpy import *

#加载数据
def loadData(f):
    dataMat = [[1,1,0.5], [1,2,0.2], [1,3,2], [1,1,2],  [1,2,5], [1,3,3]]#6个样本，每个样本3个特征
    labelMat = [0,0,0,1,1,1]                                             #标签
    # fi = open("./"+f)
    # for line in fi:
    #     fd = line.strip().split('\t')
    #     dataMat.append(1.0,float(fd[0]),float(fd[1]))
    #     labelMat.append(fd[2])
    return dataMat,labelMat
#训练W
def getw(x,y):
    dataMatrix = mat(x)               #矩阵       400 * 3
    labelMatrix = mat(y).transpose()  #真正的Y值  400 * 1
    m,n = shape(dataMatrix)           #400 * 3
    w = ones((n,1))                   #3 * 1
    alpha = 0.01                      #步长
    max_loop = 200                    #训练次数
    lam = 0.2
    for i in range(max_loop):
        y_pre = sigmoid(dataMatrix * w)  #400 *1
        error = labelMatrix - y_pre      #
        grand = dataMatrix.transpose() * error  # 3*400 * 400*1 -- > 3*1
        # w = w + (grand -lam*w)* alpha
        w = w + grand * alpha
    return w
#激活函数
def sigmoid(z):
    return 1.0/(1+exp(-z))

def test():
    '''
    决策：
    -4*1 + 2* x1 -x2 = [-4,2,-1] * [1,x1,x2].T
    x(400,3)  3项
    y(400,1)
    w(3,1)
    '''

    x,y = loadData("train.txt")
    w = getw(x,y)
    print(w)

    tx,ty = loadData("train.txt")
    y_pre = sigmoid(mat(tx) * w)           # 400*3 * 3*1  --> 400*1    --->[0,1]
    y_pre_label = (sign(y_pre - 0.5) +1)/2 #-->[0,1]
    tp =0
    tn =0
    fp =0
    fn =0

    for i in range(len(ty)):
        if y_pre_label[i] == 1 and ty[i] == 1:   #预测值为1，标签为1
            tp +=1
        if y_pre_label[i] == 0 and ty[i] == 0:
            tn +=1
        if y_pre_label[i] == 1 and ty[i] == 0:
            fp +=1
        if y_pre_label[i] == 0 and ty[i] == 1:
            fn +=1

    #查准率   P=tp/(tp+fp)
    #查全率   R=tp/(tp+fn)
    print('查准率:',tp/(tp+fp))
    print('查全率:',tp/(tp+fn))

    '''
    matlab:theta = glmfit(a(;,1:2),a(:,3),'binomial','link','logit')
    sklearn:LogisticRegression().fit(x,y)
    
    线性回归拟合的本质：用一条线，尽可能的把所有的点串起来
    逻辑回归拟合的本质：二分类两个类别样本的决策超平面
    
    欠拟合：训练误差大，上线误差大
    过拟合：训练误差小，上线误差大
    过拟合危险大
    解决：
    数据：扩充数据集
    特征：特征选择
    算法：正则化
    模型：交叉验证  （数据集，一部分训练，一部分验证
    
    正则化：L1范数，所有系数的绝对值之和     
    正则化：L2范数，所有系数的平方之和 
    使用方法：将范数添加至损失函数后，再求解梯度
    l(w) = loss(w) + ||w||
    l(w) = loss(w) + 1/2*w^2    
    '''
'''
简单神经网络：一层

分类算法：感知器、适应性线性神经元
机器学习本质：模拟人的神经元对信息的处理方法
神经元：带有二进制输出的逻辑电路门

神经元数字表示：
z=w1x1+……+wmxm
w=[w1,……，wm] 对输入进行弱化
x=[x1,……,xm]  神经元输入

综合处理后的电信号z进一步处理：激活函数

Φ(z)=1 if z>=θ        阈值
     -1 otherwise

相当于分类



感知器数据分类步骤：
权重向量w，训练样本X
1.把权重向量初始化为0，或把每个分量初始化为【0,1】的任意小数
2.把训练样本输入感知器，得到分类结果（-1/1）
3.根据分类结果更新权重向量
----》不断更新，获取W

步调函数（激活函数）与阈值


权重更新算法
▽w(j)=η*(y-y')*X(j)    ----》如果计算出的分类结果和正确的结果一样，▽w(j) = 0
w(j)=w(j)+▽w(j)
η学习率，【0,1】间的小数
y输入样本的正确分类，y'感知器计算出来的分类

感知器算法只能线性分割

'''
#
# class Perceptron(object):
#     '''
#     eta 学习率
#     n_iter 权重向量训练次数
#     w_ 神经分叉权重向量
#     errors_ 记录神经元判断出错次数
#     '''
#     def __init__(self,eta = 0.01,n_iter = 10):
#         self.eta = eta
#         self.n_iter = n_iter
#
#     '''
#     训练
#     x 电信号
#     y 对应的分类
#     '''
#     def fit(self,x,y):
#         '''
#         输入训练样本，培训神经元
#         x输入样本向量
#         y对应样本分类
#         x:shape(n_samples,n_features)  n_samples输入样本量，n_features输入电信号数量
#         X:[[1,2,3],[4,5,6]]
#         y:[1,-1]  ---> [1,2,3]输出1,[4,5,6]输出-1
#         X.shape(）---> n_samples=2，n_features=3
#         '''
#
#         #初始化权重向量0，加1是因为w0,也就是步调函数阈值
#         self.w_ = np.zero(1+x.shape[1])
#         self.errors_ = []
#
#         for i in range(self.n_iter):#出现错误时反复训练，如果训练n_iter次
#             errors = 0
#             '''
#             X:[[1,2,3],[4,5,6]]
#             y:[1,-1]
#             zip(x,y) ---> [[1,2,3,1],[4,5,6,-1]]
#             '''
#             for xi,target in zip(x,y):
#                 #▽w(j)=η*(y-y')*X(j)
#                 update = self.eta * (target - self.predict(xi))
#                 #▽w(1)=x[1]*update  ▽w(2)=x[2]*update  ▽w(3)=x[3]*update
#                 self.w_[1:] +=update * xi
#                 self.w_[0] += update
#                 errors += int(update !=0)
#                 self.errors_.append(errors)
#
#     #输入*权重
#     def net_input(self,x):
#         #z=w0*1+w1*x1+.....+wm*xm
#         return np.dot(x,self.w_[1:])+self.w_[0]
#
#     #预测
#     def predict(self,x):
#         print('predict:',x)
#         return np.where(self.net_input(x) >= 0.0 ,1,-1)
#
#     '''
#     大量数据 --》 模型 --》 总结出规律 --》 不断调节神经元的权重数组 --》 对新数据进行预测
#     '''



import pandas as pd
import matplotlib.pyplot as plt
import numpy as np
import matplotlib
from matplotlib.colors import ListedColormap

#训练
def plot_decision_regions(x,y,classifier,resolution=0.02):
    marker = ('s','x','o','v')
    colors = ('red','blue','lightgreen','gray','cyan')
    cmap = ListedColormap(colors[:len(np.unique(y))])#np.unique 去除重复的数据

    #获取花萼长度 花瓣长度的最小值和最大值
    x1_min,x1_max = x[:,0].min()-1,x[:,0].max()
    x2_min,x2_max = x[:,1].min()-1,x[:,1].max()

    #向量转二维矩阵                矩阵行                              矩阵列
    # 输入的x，y，就是网格点的横纵坐标列向量（非矩阵）   -----》 x,y组合成网格点
    # 输出的X，Y，就是坐标矩阵
    xx1,xx2 = np.meshgrid(np.arange(x1_min,x1_max,resolution),np.arange(x2_min,x2_max,resolution))#返回list,有两个元素,第一个元素是X轴的取值,第二个元素是Y轴的取值
    z = classifier.predict(np.array([xx1.ravel(),xx2.ravel()]).T) #ravel()矩阵转数组

    z = z.reshape(xx1.shape)
    plt.contourf(xx1,xx2,z,alpha=0.4,cmap=cmap)    #等高图

    print(len(np.arange(x1_min,x1_max,resolution)))#185
    print(len(np.arange(x2_min,x2_max,resolution)))#255
    print('xx1shape:',xx1.shape)                   #(255, 185)
    print('xx2shape:',xx2.shape)                   #(255, 185)
    print('zshape:', z.shape)                      #(255, 185)    [(x10,x20),(x11,x20),(x13,x20)...]
    print(xx2)
    print('z:',z)
    plt.xlim(xx1.min(),xx1.max())
    plt.ylim(xx2.min(),xx2.max())

    print(np.unique(y))            #[-1,1]
    #绘制训练数据的散点图
    for idx,label in enumerate(np.unique(y)):
        print(idx,label)
        print(x)
        plt.scatter(x=x[y == label,0],y = x[y == label,1],alpha=0.8,c=cmap(idx),marker=marker[idx],label=label)#x:特征1  y:特征2

'''
自适应线性神经元：自动把算出的结果与正确的结果比较

激活函数不是步调函数而是数据和神经参数相乘后结果当做最终结果输出

渐进下降法：调整计算结果与正确结果的距离
距离 ---》 和方差：J(w)=∑(yi-Φ(zi))^2 /2   和方差越来越小，误差越来越小，网对数据的预测越高
yi:第i组输入数据的正确结果
Φ(zi)：神经网络计算后得到的结果
z = w^Tx

调整w使得J最小：求和方差J(w)的偏导数  ∂J/∂w = -∑（yi-Φ(zi))xi

神经元参数更新
w = w + ▽w
▽w = -η∂J/∂w
'''

class AdalineGD(object):
    '''
    eta     float  学习效率 0-1   越小越精确
    n_iter  int    对训练数据进行学习改进次数
    w_      一维向量 权重数值
    error_  每次迭代改进时，网络对数据进行错误判断的次数
    '''

    def __init__(self,eta=0.01,n_iter=50):
        self.eta = eta
        self.n_iter = n_iter
    def fit(self,X,y):
        '''
        X 二维数组 【n_sample,n_features】
        n_sample  X 中含有训练数据的条目
        features  含有4个数据的一维向量，表示一条训练条目
        y:一维向量 用于存贮每一训练条目对应的正确分类
        y = w1*x1+w2*x2+W0*1
        '''
        self.w_ = np.zeros(1+X.shape[1])
        self.const = []   #训练的次数
        self.cost_ = []   #损失
        for i in range(self.n_iter):
            output = self.net_input(X)  #输入数据加权 ， 加w0
            #output = w0 + w1*x1 + ... +wmxm
            # print(output.shape)
            # print(y.shape)
            errors = (y - output)          #预测值与标签的误差
            self.w_[1:] += self.eta *X.T.dot(errors)
            self.w_[0] += self.eta * errors.sum()
            cost = (errors ** 2).sum()/2.0 #损失函数：误差平方和的均值
            self.cost_.append(cost)
            if cost > 5:                   #误差平方和的均值 >5，累计加一
                if len(self.const) == 0:
                    self.const.append(1)
                else:
                    self.const.append(self.const[-1]+1)
            else:
                if len(self.const) == 0:
                    self.const.append(1)
                else:
                    self.const.append(self.const[-1])

    #加权求和
    def net_input(self,x):
        return np.dot(x,self.w_[1:])+self.w_[0]
    #激活函数
    def activation(self,x):
        return self.net_input(x)
    #预测
    def predict(self,x):
        print('predict:',x)
        return np.where(self.activation(x) >= 0 ,1,-1)

if __name__ == '__main__':
    # 数据集  http://archive.ics.uci.edu/ml/machine-learning-databases/iris/iris.data
    # 花萼长度 花萼宽度 花瓣长度 花瓣宽度 类别（Iris Setosa(山鸢尾)、Iris Versicolour(杂色鸢尾)、Iris Virginica(维吉尼亚鸢尾)）

    #加载数据
    file = './datasets/iris.data.txt'
    df = pd.read_csv(file, header=None)      # 第一行不是数据说明，是真实数据
    df.head()

    #提取特征数据，标签
    y = df.loc[0:99, 4].values               # 前100行的第5列
    print('y:',y.shape)
    print(y)
    y = np.where(y == 'Iris-setosa', 1, -1)  # np.where(条件, x, y)  条件为True，输出x，不满足输出y
    x = df.iloc[0:100, [0, 2]].values        # 获取花萼长度 花瓣长度特征
    plt.scatter(x[:100, 0], x[:100, 1], color='red', marker='o', label='setosa')  # marker形状  花萼长度为横轴，花瓣长度为纵轴
    plt.xlabel('x1')
    plt.ylabel('x2')
    plt.xticks(rotation=45)#倾斜45
    plt.title('训练数据')
    plt.legend(loc='upper left')
    #系统字体
    # matplotlib.matplotlib_fname()  # 将会获得matplotlib包所在文件夹
    # 指定默认字体
    matplotlib.rcParams['font.sans-serif'] = ['SimHei']
    matplotlib.rcParams['font.family'] = 'sans-serif'
    # 解决负号'-'显示为方块的问题
    matplotlib.rcParams['axes.unicode_minus'] = False
    plt.show()

    #训练
    ada = AdalineGD(eta=0.0001,n_iter=1000)
    ada.fit(x,y)                              #训练
    plot_decision_regions(x,y,classifier=ada) #classifier 分类器
    plt.show()

    # 错误的次数
    plt.title('错误的次数')
    plt.plot(range(1,len(ada.const)+1),ada.const,marker='o')
    plt.show()

    #误差平方和
    plt.title('误差')
    plt.plot(range(1,len(ada.cost_)+1),ada.cost_,marker='o')
    plt.show()
