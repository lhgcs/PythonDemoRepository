# -*- coding: utf-8 -*-
import scrapy
from douban.items import DoubanItem

class DoubanSpiderSpider(scrapy.Spider):
    #爬虫名，和项目名称不能重复
    name = 'douban_spider'
    #运行的域名，不抓取allowed_domains之外的
    allowed_domains = ['movie.douban.com']
    #入口YRL，放入调度器中   调度器拿到URL进行调度，把请求返回给引擎，引擎再给下载中间件下载器，解析下载，下载完后在parse中解析
    start_urls = ['http://movie.douban.com/top250']

    #默认解析方法，对爬取到的数据进行解析，如果是item的数据就保存，是链接就继续爬
    def parse(self, response):
        print(response.text)
        #条目25条
        movie_list=response.xpath("//div[@class='article']//ol[@class='grid_view']/li")
        for i in movie_list:
            print(i)
            item = DoubanItem()

            item['serial_number']=i.xpath(".//div[@class='item']//em/text()").extract_first()
            item['movie_name']=i.xpath(".//div[@class='info']/div[@class='hd']/a/span[1]/text()").extract_first()
            #内容描述
            content = i.xpath(".//div[@class='info']/div[@class='bd']/p[1]/text()").extract_first()
            for i_content in content:
                content_s = "".join(i_content.split())
                item['introduce'] = content_s
            print(item['introduce'])
            item['star'] = i.xpath(".//span[@class='rating_num']/text()").extract_first()
            item['evaluate'] = i.xpath(".//div[@class='star']//span[4]/text()").extract_first()
            item['describe'] = i.xpath(".//p[@class='quote']/span/text()").extract_first()
            yield item  #数据送到pipeline
        #下一页的数据
        next_link = response.xpath("//span[@class='next']/link/@href").extract()
        if next_link:
            next_link = next_link[0]
            yield scrapy.Request("https://movie.douban.com/top250"+next_link,callback=self.parse)#回调
            #调用Request中callback指向的函数



