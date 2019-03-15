import numpy as np
import matplotlib.pyplot as plt
import matplotlib as mpl
mpl.rcParams['font.size'] = 10

#设置标题
plt.title("title",color='#FF0000')
#显示网线
plt.grid(True)
#坐标轴加标签
plt.xlabel('x',fontsize=20)
plt.ylabel('y')
plt.text(0,0,'0')
#刻度样式
plt.tick_params(size=10,labelsize=15,labelcolor='r')
#y轴刻度
plt.yticks([0,1,2,3,4,5,6,7,8,9,-1,-2,-3,-4,-5,-6,-7,-8,-9])
#使用自带的样式
plt.style.use('ggplot')
#坐标轴范围
#plt.axis([0,5,0,20])
#x轴范围
plt.xlim(-10, 10)
#y轴范围
plt.ylim(-10,10)
#坐标轴隐藏
plt.axes().get_xaxis().set_visible(True)
plt.axes().get_yaxis().set_visible(True)

#将画布分成3*3个区域，plot绘制在第5个块
#plt.subplot(335)

fig = plt.figure()#创建图

#当前的图表和子图可以使用plt.gcf()和plt.gca()获得
#ax = plt.gca()
#ax.spines["right"].set_color("none")#隐藏
#ax.xaxis.set_tick_position("bottom")

#折线图
#x
a = [1,2,3]
#y
b = [2,4,6]

#线条样式
#plt.plot(a,b,'r-.')
#plt.plot(a,b,'r--*',linewidth=5)#plot函数根据数字绘制有意义的图形,linewidth线宽
ax = fig.add_subplot(341)  #创建子图
ax.plot(a,b)

#线条
#输入值，输出值
a = [1,2,3]
b = [1,2,3]
#plt.plot(input_data,output_data)
#plt.plot(input_data,output_data,"y",linewidth=5,linestyle="-")
ax = fig.add_subplot(342)  #创建子图
ax.plot(a,b)

#点
#plt.scatter(5,5) #在（5,5）画一个点
ax = fig.add_subplot(343)  #创建子图
ax.scatter(5,5)

#多个点
x = [1,2,3]
y = [3,6,9]
#plt.scatter(x,y)
#plt.scatter(x,y,edgecolors='none',s=100)          #不显示轮廓
#plt.scatter(x,y,edgecolors='none',s=100,c=(1,0,0)) #红色
#plt.scatter(x,y,edgecolors='none',s=100,c=[x**2 for x in range(50)],cmap=plt.cm.binary)#颜色渐变
ax = fig.add_subplot(344)  #创建子图
ax.plot(x,y)


x = np.linspace(0,6,num=100,endpoint=True) #等差数列，返回[0,6]均匀间隔的数字,元素个数100个，endpoint=true 包含终值
#x = np.arange(0.0,4.0*np.pi,0.01)         #相当于range
c = np.cos(x)
s = np.sin(x)
'''
#正弦曲线
plt.plot(x,s,label="sin",c='blue')
#余弦曲线
plt.plot(x,c,label="cos",c='yellow')
#水平线
plt.plot((x.min(),x.max()),(0,0),c="green")
#函数和坐标轴之间填充颜色
plt.fill_between(x,s,where=(3<x)&(x<6),facecolor="red")
#两个函数之间填充颜色
plt.fill_between(x,c,s,where=c>s,facecolor="green",alpha=0.5)
plt.fill_between(x,c,s,where=c<s,facecolor="yellow")
'''
ax = fig.add_subplot(345)  #创建子图
ax.plot(x,s,label="sin",c='blue')
ax.plot(x,c,label="cos",c='yellow')
ax.fill_between(x,c,s,where=c>s,facecolor="green",alpha=0.5)

#散点图
n = 256
x=np.random.normal(0,1,n)
y=np.random.normal(0,1,n)
t=np.arctan2(y,x)#返回给定的 X 及 Y 坐标值的反正切值
#plt.axis([0.025,0.025,0.95,0.95])
#plt.scatter(x,y,s=10,c=t)#红色
ax = fig.add_subplot(346)  #创建子图
ax.plot(x,y)

#直方图
n = 256
x=np.arange(n)
#y=np.random.normal(0,10)
#y2=np.random.normal(0,10)
y=np.random.uniform(0,10,n)
y2=np.random.uniform(0,10,n)
#plt.bar(x,+y)
#plt.bar(x,-y2)
ax = fig.add_subplot(347)  #创建子图
ax.bar(x,+y)
ax.bar(x,-y2)

#for x,y in zip(x,y):
#    plt.text(x+0.5,y+0.05,"x.2f y.2f",ha="center",va="bottom")

#热图
from matplotlib import cm
data = np.random.rand(1,1)
map = cm.Blues
#m = plt.imshow(data,interpolation='nearest',cmap=map,aspect='auto',vmin=0,vmax=1)#插值最近，自动缩放，颜色最大值1（蓝色
ax = fig.add_subplot(348)  #创建子图
ax.imshow(data,interpolation='nearest',cmap=map,aspect='auto',vmin=0,vmax=1)

#3D图
from mpl_toolkits.mplot3d import Axes3D
#ax = fig.add_subplot(349,projection="3d")
#ax.scatter(1,1,3)

#热力图
X=np.linspace(-3,3,100)
Y=np.linspace(-3,3,100)
x,y = np.meshgrid(X,Y)
#plt.contourf(x,y,f(X,Y),8,cmap = plt.cm.hot)
#ax = fig.add_subplot(348)  #创建子图
def f(x,y):
    return (1 - x/2 + x**5 + y**3) * np.exp(-x**2,-y**2)
#ax.contourf(x,y,f(X,Y),8,cmap = plt.cm.hot)#将f(X,Y)的值对应到color map的暖色组中寻找对应颜色

#饼图
z = np.ones(1)
#plt.pie(z,explode=5,colors=["'%f" % (i/100) for i in range(100)],labels=["'%f" % (i/100) for i in range(100)])
#plt.gca().set_aspeet("equal")
labels = [1,2,3,4,5]
z = [1,2,3,4,5]
explode = (0,0,0,0,0)
ax = fig.add_subplot(3,4,10)  #创建子图,3410不行
#ax.pie(z,labels=labels, radius=1,explode=5)
ax.pie(z,explode=explode,labels=labels,autopct='%1.1f%%',shadow=False,startangle=150)
#ax.title("饼图示例-8月份家庭支出")

'''
labels 是标签，radius 是饼图半径#explodes 为0 代表不偏离圆心， 不为零则代表偏离圆心的距离
autopct控制饼图内百分比设置,
pctdistance 位置刻度
labeldistance到圆心的距离
textprops标签和比例的格式
'''



#调整尺寸适应屏幕
plt.figure(dpi=128,figsize=(10,6))#dpi分辨率
plt.show()


#可视化包pygal 生成可缩放的矢量图形文件 自动缩放
import pygal

#直方图
data = [1,2,3,1,2,3,1,2,3,4,5,6,1,1,1,1,1]
x_value = list(set(data))
#统计
freq = []
for i in x_value:
    res = data.count(i)
    freq.append(res)

#可视化
hist = pygal.Bar(color='g')

#hist = pygal.Histogram()
#hist.add('人数',[(4,1,2),(5,2,3),(6,3,4)])#（y轴高度，x轴开始坐标，x轴结束坐标）

#标题
hist.title = "title"
hist._x_title = 'x'
hist._y_title = 'y'
hist.x_labels = x_value
hist.add('人数',freq)       #添加到图表
hist.render_to_file("a.svg")#图表渲染为svg，用浏览器打开svg文件

#
# import csv
# filename = "1.csv"
# with open(filename) as f:
#     reader = csv.reader(f)
#     header_row = next(reader)#下一行
#     print(header_row)
#     for index,colum in enumerate(header_row):#获取索引和值
#         print(index,colum)
#
#     hight = []
#     for row in reader:
#         hight.append(row[1])
#
#
# #datetime 模块
# from datetime import datetime
# date = datetime.strftime('2018-09-00',"%Y-%m-%d")

#填色 fill_between 填充两个y值之间的数据
#自动保存
#plt.savefig('123.png',bbox_inches='tight')#裁剪空白区域


