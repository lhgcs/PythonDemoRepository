#!/usr/bin/python
# -*- coding=utf-8 -*-
from __future__ import division  #尽量相除时返回浮点型，而不是0
import pandas as pd
import numpy as np               #数据读成矩阵
import scipy as stats            #特殊计算 众数 导数

'''
数据读取
'''
df=pd.read_csv("./data/train.csv")   #读取csv数据集
label = df['TARGET']                 #特征选取出来  （TARGET目标）
df = df.drop(['ID','TARGET'],axis=1) #删除掉'ID','TARGET'这2列 （不需要处理）  axis=1 轴=1即对列操作

'''
数据诊断
统计指标：均值，最大，最小，中位数；计数类；缺失值，方差；分位点，值的频数
'''
'''
基本描述统计--计数类
'''
#缺失值
#np.nan 缺失
missSet = [np.nan,9999999999999,-99999999999]
#计数
#取第一列
len(df.iloc[:,0].unique())# ：--》所有的行  0 --》第一列  unique特征不同的值     得到第一列特征有多少个不同的值
#前3列
count_un = df.iloc[:,0:3].apply(lambda x:len(x.unique()))# 0:3前3列  apply遍历每一列（对每一列操作）

#零值
#第一列
np.sum(df.iloc[:,0] == 0)  #一列中0的个数   先转为True,False的数组，再相加得到个数
#前3列
count_zero = df.iloc[:,0:3].apply(lambda x:np.sum(x == 0))# 0:3列的数据集操作


'''
基本描述统计--均值，中位数
'''
#均值
np.mean(df.iloc[:,0])
#去除缺失值
df.iloc[:,0][~np.isin(df.iloc[:,0],missSet)]
'''
np.isin(a,b)  a是否满足b   返回true false
df.iloc[:,0][位置]  引用
'''
#去除缺失值再求均值
np.mean(df.iloc[:,0][~np.isin(df.iloc[:,0],missSet)])
df_mean = df.iloc[:,0:3].apply(lambda x:np.mean(x[~np.isin(x,missSet)] ))# 0:3列的数据集操作

#中位数
df_median = df.iloc[:,0:3].apply(lambda x:np.median(x[~np.isin(x,missSet)] ))# 0:3列的数据集操作

'''
基本描述统计--众数
'''
#stats.mode
df_mode = df.iloc[:,0:3].apply(lambda x:stats.mode(x[~np.isin(x,missSet)] )[0][0])       #返回两个值：众数（是什么），频数

#比例
df_mode_count = df.iloc[:,0:3].apply(lambda x:stats.mode(x[~np.isin(x,missSet)] )[1][0]) #频数
df_mode_perct = df_mode_count/df.shape[0]  #shape样本数量   shape[0]行   shape[1]列

#最小值
np.min(df.iloc[:,0])
#最大值
np.max(df.iloc[:,0])

'''
基本描述统计--分位点
无法使用apply
'''
np.percentile(df.iloc[:,0],(1,5,25,50,75,95))  #(1,5,25,50,75,95) 分位点   返回数组
#字典 名字：计数
json_quantile = {}

for i,name in enumerate(df.iloc[:,0:3].columns()):
    print('the {} columns {}').format(i,name)
    json_quantile[name] = np.percentile(df.iloc[:,0],(1,5,25,50,75,95))

'''
基本描述统计--频数
无法使用apply
'''
df.iloc[:,0].value_counts().iloc[0:5,]


'''
基本描述统计--缺失值
无法使用apply
'''
#统计缺失值
np.sum(np.isin(df.iloc[:,0],missSet))#np.isin 返回缺失值位置

'''
工具整合
'''
def fill_fre_top_5(x):
    if(len(x) <=5 ):
        new_array = np.full(5,np.full)
        new_array[0:len(x)] = x
        return new_array

def eda_analysis_vl(missSet = [np.nan,99999999,-9999999],df=None):
    count_un=df.apply(lambda x:len(x.unique()))
    count_un=count_un.to_frame('count')#to_frame转成datafram  count特征名字

    count_zero=df.apply(lambda x:np.sum(x == 0))
    count_zero=count_zero.to_frame('count_zero')

    count_mean=df.apply(lambda x:np.mean(x[~np.isin(x,missSet)]))
    count_mean=count_mean.to_frame('mean')

    count_median=df.apply(lambda x:np.median(x[~np.isin(x,missSet)]))
    count_median=count_mean.to_frame('median')

    count_mode = df.apply(lambda x: stats.mode(x[~np.isin(x, missSet)])[0][0])
    count_mode = count_mode.to_frame('mode')

    count_mode_count = df.apply(lambda x: stats.mode(x[~np.isin(x, missSet)])[0][1])
    count_mode_count = count_mode_count.to_frame('count_mode_count')

    df_mode_perct=df_mode_count/df.shape[0]

    df_min = df.apply(lambda x: np.min(x[~np.isin(x, missSet)]))
    df_min = df_min.to_frame('min')

    df_max = df.apply(lambda x: np.max(x[~np.isin(x, missSet)]))
    df_max = df_max.to_frame('max')

    np.percentile(df.iloc[:,0],(1,5,25,50,75,95))  #(1,5,25,50,75,95) 分位点   返回数组
    #字典 名字：计数
    json_quantile = {}

    for i,name in enumerate(df.columns):
        print('the {} columns {}').format(i,name)
        json_quantile[name] = np.percentile(df[name][~np.isin(df[name],missSet)],(1,5,25,50,75,95))

    df_quantile = pd.DataFrame(json_quantile)[df.columns].T
    df_quantile.columns = ['quan0','quan5','quan25','quan50']


    json_fre_name = {}
    json_fre_count = {}

    for i, name in enumerate(df.columns):
        print('the {} columns {}').format(i, name)
        index_name = df[name][~np.isin(df[name], missSet)].value_counts().iloc[0:5,].index.values
        index_name = fill_fre_top_5(index_name)

        json_fre_name[name] = index_name

        values_count = df[name][~np.isin(df[name], missSet)].value_counts().iloc[0:5,].values
        values_count = fill_fre_top_5(values_count)

        json_fre_name[name] = values_count

    df_fre_name = pd.DataFrame(json_fre_name)[df.columns].T
    df_fre_count = pd.DataFrame(json_fre_count)[df.columns].T

    df_fre = pd.concat([df_fre_name,df_fre_count],axis=1)
    df_fre.columns = ['value1', 'value2', 'value3', 'value4','value5']

    df_miss = df.apply(lambda x:np.sum(np.isin(x,missSet)))
    df_miss = df_miss.to_fram("miss")



    df_eda_summary = pd.concat(
        [count_un,count_zero,df_mean,df_median,df_mode,df_mode_count,df_mode_perct,df_min,df_max,df_fre,df_miss],
        axis=1
    )
    return df_eda_summary

'''
工具测试
'''
start = timeit.default_timer()
df_eda_summary = eda_analysis_vl(missSet = [np.nan,9999999999,-99999999],df=df.iloc[:,0:3])
print("running time: {0:.2f} seconds".format(timeit.default_timer() - start)) #时间差









