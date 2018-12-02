# -*- coding: utf-8 -*-

# Scrapy settings for douban project
#
# For simplicity, this file contains only settings considered important or
# commonly used. You can find more settings consulting the documentation:
#
#     https://doc.scrapy.org/en/latest/topics/settings.html
#     https://doc.scrapy.org/en/latest/topics/downloader-middleware.html
#     https://doc.scrapy.org/en/latest/topics/spider-middleware.html


'''
1.新建项目
scrapy startproject douban

cfg 项目配置文件
item 数据结构
settting 设置文件


cd douban
cd spiders

scrapy genspider douban_spider movie.douban.com
爬虫名 域名

2.明确目标
3.爬虫文件编写

爬虫文件名和爬虫名称不能相同


scrapy crawl douban_spider

sqlite*

数据保存
scrapy crawl douban_spider -o 1.csv


伪装
设置代理IP或者随机user agent

pip install pypiwin32
'''

mongo_host = '127.0.0.1'
mongo_port = 27017
mongo_db_name = 'root'
#表
mongo_db_collection = 'douban_movie'


BOT_NAME = 'douban'

SPIDER_MODULES = ['douban.spiders']
NEWSPIDER_MODULE = 'douban.spiders'


# Crawl responsibly by identifying yourself (and your website) on the user-agent
#USER_AGENT = 'douban (+http://www.yourdomain.com)'
USER_AGENT ="Mozilla/5.0 (Windows NT 6.2; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/65.0.3325.181 Safari/537.36"

#robot协议
# Obey robots.txt rules
ROBOTSTXT_OBEY = "False"  #不是False

#并发量
# Configure maximum concurrent requests performed by Scrapy (default: 16)
#CONCURRENT_REQUESTS = 32

#下载延迟3秒
# Configure a delay for requests for the same website (default: 0)
# See https://doc.scrapy.org/en/latest/topics/settings.html#download-delay
# See also autothrottle settings and docs
#DOWNLOAD_DELAY = 3
# The download delay setting will honor only one of:
CONCURRENT_REQUESTS_PER_DOMAIN = 16   #域名并发量
CONCURRENT_REQUESTS_PER_IP = 16       #IP并发量

# Disable cookies (enabled by default)
#COOKIES_ENABLED = False

# Disable Telnet Console (enabled by default)
#TELNETCONSOLE_ENABLED = False

#请求头
# Override the default request headers:
# DEFAULT_REQUEST_HEADERS = {
#   'Accept': 'text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8',
#   'Accept-Language': 'en',
# # 'Accept': 'text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,image/apng,*/*;q=0.8',
# # 'Accept-Language': 'zh-CN,zh;q=0.9',
# }

'''
Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,image/apng,*/*;q=0.8
Accept-Encoding: gzip, deflate, br
Accept-Language: zh-CN,zh;q=0.9
Cache-Control: max-age=0
Connection: keep-alive
Cookie: bid=_bSlvHAOfkA; __utmz=30149280.1523321394.3.3.utmcsr=baidu|utmccn=(organic)|utmcmd=organic; __utmz=223695111.1523321394.1.1.utmcsr=baidu|utmccn=(organic)|utmcmd=organic; __yadk_uid=tvCGhBaxHBMFJtViEItrA5SarD1xqKSy; Hm_lvt_407473d433e871de861cf818aa1405a1=1523321400; __utma=30149280.482153053.1509510531.1523321394.1533557492.4; __utmb=30149280.0.10.1533557492; __utmc=30149280; __utma=223695111.1263639213.1523321394.1523321394.1533557492.2; __utmb=223695111.0.10.1533557492; __utmc=223695111; _pk_ref.100001.4cf6=%5B%22%22%2C%22%22%2C1533557493%2C%22http%3A%2F%2Fwww.baidu.com%2Flink%3Furl%3DsVbP8mLpg7lM7oQCDGitmCWm6vaNfGOyZeGlNR9S3GonAFABOgA_6w09y-BzCWWC5ThFxW5jpQTQGuLJEniAMq%26wd%3D%26eqid%3D9c2ce24a0003511e000000065acc0a24%22%5D; _pk_id.100001.4cf6=ccdbe9186ff7ec45.1523321395.2.1533557493.1523321395.; _pk_ses.100001.4cf6=*
Host: movie.douban.com
Upgrade-Insecure-Requests: 1
User-Agent: Mozilla/5.0 (Windows NT 6.2; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/65.0.3325.181 Safari/537.36
'''

#中间件
# Enable or disable spider middlewares
# See https://doc.scrapy.org/en/latest/topics/spider-middleware.html
# SPIDER_MIDDLEWARES = {
#    'douban.middlewares.DoubanSpiderMiddleware': 543,
# }

#启用中间件
# Enable or disable downloader middlewares
# See https://doc.scrapy.org/en/latest/topics/downloader-middleware.html
# DOWNLOADER_MIDDLEWARES = {
#    # 'douban.middlewares.DoubanDownloaderMiddleware': 543,
#     'douban.middlewares.my_proxy': 543,#优先级
#     'douban.middlewares.my_useragent': 544,#优先级
# }

# Enable or disable extensions
# See https://doc.scrapy.org/en/latest/topics/extensions.html
#EXTENSIONS = {
#    'scrapy.extensions.telnet.TelnetConsole': None,
#}

# Configure item pipelines
#See https://doc.scrapy.org/en/latest/topics/item-pipeline.html
ITEM_PIPELINES = {
   'douban.pipelines.DoubanPipeline': 300,
}

# Enable and configure the AutoThrottle extension (disabled by default)
# See https://doc.scrapy.org/en/latest/topics/autothrottle.html
#AUTOTHROTTLE_ENABLED = True
# The initial download delay
#AUTOTHROTTLE_START_DELAY = 5
# The maximum download delay to be set in case of high latencies
#AUTOTHROTTLE_MAX_DELAY = 60
# The average number of requests Scrapy should be sending in parallel to
# each remote server
#AUTOTHROTTLE_TARGET_CONCURRENCY = 1.0
# Enable showing throttling stats for every response received:
#AUTOTHROTTLE_DEBUG = False

# Enable and configure HTTP caching (disabled by default)
# See https://doc.scrapy.org/en/latest/topics/downloader-middleware.html#httpcache-middleware-settings
#HTTPCACHE_ENABLED = True
#HTTPCACHE_EXPIRATION_SECS = 0
#HTTPCACHE_DIR = 'httpcache'
#HTTPCACHE_IGNORE_HTTP_CODES = []
#HTTPCACHE_STORAGE = 'scrapy.extensions.httpcache.FilesystemCacheStorage'
