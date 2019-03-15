'''
手写图片识别
MNIST机器学习入门

数据准备：
kaggle的数据是表格形式的，对图片信息进行处理，把一个28*28的图片信息变成28*28=784的一行数据

模型设计：
使用最简单的单层神经网络进行学习
Softmax作为激活函数（主流的激活函数sigmoid（S型函数，取值范围[0,1]）,tanh（双切正切函数，取值范围[-1,1]）,ReLU（大于0留下，否则一律为0）。
激活函数要处处可微，可微分才能求导，求极值；还要非线性，线性模型的表达能力不够)
交叉熵作为损失函数（损失函数是模型对数据拟合程度的反应，拟合越好损失应越小）
梯度下降作为优化方式
'''

import numpy as np
import tensorflow as tf
import pandas as pd

#加载数据集，把输入和结果分开
train = pd.read_csv('train.csv')
images = train.iloc[:,1:].values
labels_flat = train.iloc[:,0].values

#对输入数据进行处理
images = images.astype(np.float)
type(np.float)
#对应位置乘以1/255，求灰度值
images = np.multiply(images,1.0/255)
#images为1行多列
images_size = images.shape[1]
#开根号获取图片宽高
images_width = images_height = np.ceil(np.sqrt(images).astype(np.uint8))

#分类结果的种类
labels_counts = np.unique(labels_flat).shape[0]
#输入占位符
x = tf.placeholder(tf.float32,shape=[None,images])
y = tf.placeholder(tf.float32,shape=[None,labels_counts])

#独热编码
def dense_to_one_hot(label_dense,num_class):
    num_labels = label_dense.shape[0]
    index_offset = np.arange(num_labels) * num_class
    label_one_hot = np.zeros((num_labels,num_class))
    label_one_hot.flat[index_offset+label_dense.ravel()]
    return label_one_hot

labels = dense_to_one_hot(labels_flat,labels_counts)
labels = labels.astype(np.uint8)

#把输入数据划分为训练集和验证集,4000个作为测试集，2000个作为验证集
VALIDATION_SIZE=2000

validation_images = images[:VALIDATION_SIZE]
validation_labels = labels[:VALIDATION_SIZE]

train_images = images[VALIDATION_SIZE:]
train_labels = labels[VALIDATION_SIZE:]

#对数据集进行分批
batch_size = 100
n_batch = int(len(train_images)/batch_size)


#建立神经网络，设置损失函数，设置梯度下降的优化参数
weights = tf.Variable(tf.zeros[784,10])
biases = tf.Variable(tf.zeros[10])

result = tf.matmul(x,weights)+biases
prediction = tf.nn.softmax(result)

#创建损失函数，以交叉熵的平均值为衡量
loss=tf.reduce_mean(tf.nn.softmax_cross_entropy_with_logits(labels=y,logits=prediction))

#用梯度下降法优化参数
train_step = tf.train.GradientDescentOptimizer(0.1).minimize(loss)

#初始化变量
init = tf.global_variables_initializer()
#计算准确度
corrent_prediction = tf.equal(tf.argmax(y,1),tf.argmax(prediction,1))
accurary = tf.reduce_mean(tf.cast(corrent_prediction,tf.float32))

with tf.Session as sess:
    sess.init(init)
    for epoch in range(50):
        for batch in range(n_batch):
            #分片取出数据
            batch_x = train_images[batch*batch_size:(batch+1)*batch_size]
            batch_y = train_labels[batch*batch_size:(batch+1)*batch_size]
            #训练
            sess.run(train_step,feed_dict={x:batch_x,y:batch_y})
        #每次训练的准确度
        accurary_n = sess.run(accurary,feed_dict={x:validation_images,y:validation_labels})

















