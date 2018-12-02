# -*- coding: utf-8 -*-

# Define your item pipelines here
#
# Don't forget to add your pipeline to the ITEM_PIPELINES setting
# See: https://doc.scrapy.org/en/latest/topics/item-pipeline.html

import pymongo
#pip install pymongo
from douban.settings import mongo_host,mongo_port,mongo_db_name,mongo_db_collection

#保存爬取得到的items数据
class DoubanPipeline(object):
    def __init__(self):
        host = mongo_host
        port = mongo_port
        dbname = mongo_db_name
        sheet_name=mongo_db_collection
        #连接
        client = pymongo.MongoClient(host=host,port=port)
        #数据库
        mydb = client[dbname]
        self.post = mydb[sheet_name]

    # 插入数据库
    def process_item(self, item, spider):#item是spider传过来的yield
        data = dict(item)
        self.post.insert(data)
        return item

    def save_data_to_txt(self, data):
        with open('data.txt', 'w', encoding='utf-8') as f: #写入本地文件名csdn 类型汉字(W) 编码方式utf-8
            f.write(data)
            f.write('\n')   #换行
