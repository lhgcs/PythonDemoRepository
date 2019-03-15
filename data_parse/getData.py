#!/usr/bin/python
import pandas as pd
import os

#爬取
import pandas_datareader.data as web
data = web.DataReader("1","yahoo",datetime.datetime(2018,12,1),datatime.data.taday())
data.to_csv("./temp.csv",columns=data.columns)#指定列索引

#从CSV中获取
if os.path.exists("./temp.csv"):
    data = pd.read_csv("./temp.csv",index_colum=0,nrows=10,encoding='gb2312')#指定第0列作为索引，读100行
    data = data.sort_index(ascending=False)

#tushare
import tushare as ts
#获取历史数据
#ts.get_hist_data(code='hs300',start="2018-12-10",end="2018-12-10",ktype="30")#获取30分钟k线数据
#实时数据
data2 = ts.get_total_all()
data.append(data2)
data["index"] = data.index
data.drop_duplicates(subset='index',keep='last',inplace=True)

if data.isnull():
    data[data.isnull().T.any]#显示有缺少值的行
    #data.dropna(axis=0)#删除有缺失值的行
    #data.dropna(axis=1)#删除有缺失值的列
    #data.dropna(axis=0,how='any')#删除有缺失值的行
    data.dropna(axis=0,how="all")#删除全部是缺失值的行

data.round(2)#保留2位小数
data.ix[data.loc[:,"Height"] == 0,"Height"] = data.Height.mean









