import multiprocessing
import re
import os
import requests
import time

from multiprocessing import Process, Queue, cpu_count

#使用PyInstaller模块把python转exe文件
#pip install PyInstaller

#jpeg文件有固定的文件头，其文件头的格式如下：
#Start Marker | JFIF Marker | Header Length | Identifier
#0xff, 0xd8 | 0xff, 0xe0 | 2 - bytes | "JFIF\0"

#判断JPG是否完整：方法一
def is_jpg(filename):
    data = open(filename,'rb').read(11)
    if data[:4] != '\xff\xd8\xff\xe0' and data[:4]!='\xff\xd8\xff\xe1':   #前4个字节不等于固定的文件头
        return False
    if data[6:] != 'JFIF\0' and data[6:] != 'Exif\0':                     #最后6个字节
        return False
    return True
#判断JPG是否完整：方法二
def is_valid_jpg(jpg_file):
    if jpg_file.split('.')[-1].lower() == 'jpg':
        with open(jpg_file, 'rb') as f:
            f.seek(-2, 2)
            return f.read() == '\xff\xd9' #判定jpg是否包含结束字段
    else:
        return True
#判断JPG是否完整：方法三
from PIL import Image
#pip install Pillow
#通过PIL类库来做判断
def is_jpg(filename):
    try:
        img=Image.open(filename)
        if img.format =='JPEG': #利用PIL库进行jpeg格式判定
            #img.show()
            return True
    except IOError:
        return False

#判断图片是否完整：方法四
import io
from PIL import Image
#判断文件是否为有效（完整）的图片
#输入参数为文件路径
def IsValidImage(pathfile):
  isValid = True
  try:
      Image.open(pathfile).verify()
  except:
      isValid = False
  return isValid
#判断文件是否为有效（完整）的图片
#输入参数为bytes，如网络请求返回的二进制数据
def IsValidImage4Bytes(buf):
    bValid = True
    try:
        Image.open(io.BytesIO(buf)).verify()
    except:
        bValid = False
    return bValid

#图片数目，全局变量
global PhotoNum
PhotoNum = 0
#存储路径
PWD ="D:/work/python/pic/huaban"
#伪装成浏览器
head = {'User-Agent' :'Mozilla/5.0 (Windows; U; Windows NT 6.1; en-US; rv:1.9.1.6) Gecko/20091201 Firefox/3.5.6'}
TimeOut = 30

#花瓣网请求路径（此时HTML源文件中并没有图片的URL，需要AJAX请求才能获取图片的URL）
url = "http://huaban.com/favorite/beauty/"
url_image = "http://hbimg.b0.upaiyun.com/"
#花瓣网下一页的AJAX请求路径
urlNext = "http://huaban.com/favorite/beauty/?iqkxaeyv&limit=20&wfl=1&max="

'''
花瓣网分析
AJAX请求：
Request URL: http://huaban.com/favorite/beauty/?fetch&jk2qu4kp&since=1779015196&limit=100&wfl=1

请求头：
X-Request: JSON
X-Requested-With: XMLHttpRequest

请求参数：
fetch: 
jk2qu4kp: 
since: 1779015196
limit: 100
wfl: 1

向下翻页：发出多个ajax请求，参数没找到规律
http://huaban.com/favorite/beauty/?fetch&jk2r5o54&since=1779015196&limit=100&wfl=1
http://huaban.com/favorite/beauty/?fetch&jk2r5o55&since=1779015196&limit=100&wfl=1
http://huaban.com/favorite/beauty/?fetch&jk2r5o56&since=1779015196&limit=100&wfl=1
http://huaban.com/favorite/beauty/?fetch&jk2r5o57&since=1779015196&limit=100&wfl=1

有时间用Selenium爬取吧
'''

'''
当把get函数的stream参数设置成False时，它会立即开始下载文件并放到内存中，如果文件过大，有可能导致内存不足。
当把get函数的stream参数设置成True时，它不会立即开始下载，当你使用iter_content或iter_lines遍历内容或访问内容属性时才开始下载。需要注意一点：文件没有下载之前，它也需要保持连接。
•iter_content：一块一块的遍历要下载的内容
•iter_lines：一行一行的遍历要下载的内容
使用上面两个函数下载大文件可以防止占用过多的内存，因为每次只下载小部分数据
'''
#下载文件
def downfile(file ,url):
    print("开始下载：" ,file ,url)
    try:
        r = requests.get(url ,stream=True)
        with open(file, 'wb') as fd:
            for chunk in r.iter_content():
                fd.write(chunk)
    except Exception as e:
        print("下载失败了" ,e)

from threading import Thread
import threading
from bs4 import BeautifulSoup

#联网失败重试次数
request_count=5
#获取HTML
def requestpageText(url):
    global request_count
    try:
        request_count -= 1
        Page = requests.session().get(url ,headers=head ,timeout=TimeOut)
        Page.encoding = "utf-8"
        return Page.text
    except Exception as e:
        print("联网失败了...重试中" ,e)
        time.sleep(5)
        print("暂停结束")
        if request_count > 0:
            requestpageText(url)

#获取HTML，解析，下载图片
def requestUrl(url):
    global PhotoNum
    print("*******************************************************************")
    print("请求网址：" ,url)
    text = requestpageText(url)

    if not os.path.exists(PWD):
        os.makedirs(PWD)

    max_pin_id = 0
    pattern = re.compile('{"pin_id":(\d*?),.*?"key":"(.*?)",.*?"like_count":(\d*?),.*?"repin_count":(\d*?),.*?}' ,re.S)#re.S 把\n当工作普通字符
    items = re.findall(pattern ,text)
    print(items)
    for item in items:
        max_pin_id = item[0]
        x_key = item[1]
        x_like_count = int(item[2])
        x_repin_count = int(item[3])
        if (x_repin_count >10 and x_like_count > 10) or x_repin_count > 100 or x_like_count > 20:
            url_item = url_image + x_key
            filename = PWD + str(max_pin_id) + ".jpg"
            if os.path.isfile(filename):
                print("文件存在：", filename)
                if IsValidImage(filename):
                    continue
                else:
                    os.remove(filename)

            print("开始下载第{0}张图片".format(PhotoNum))
            PhotoNum += 1
            # 使用多线程下载
            t1 = Thread(target=downfile, args=(filename, url_item))
            threading.BoundedSemaphore(5)  # 同一时刻可以有四个线程
            t1.start()
            # 使用多进程下载
            # t = multiprocessing.Process(target=downfile, args=(filename, url_item))
            # t.daemon = False  # 将daemon设置为True，则主线程不等待子进程，主线程结束则所有结束
            # t.start()
            #t1.join(10)
            #downfile(filename, url_item)
    requestUrl(urlNext + max_pin_id)


# if __name__ == "__main__":
#     requestUrl(url)

#os.chdir(directory)  #切换到directory目录
#cwd = os.getcwd()  #获取当前目录即dir目录下


'''
获取百度图片
'''
def get_baidu_image():
    url="https://image.baidu.com/search/index?tn=baiduimage&ct=201326592&lm=-1&cl=2&ie=gb18030&word=%C3%C0%C5%AE&fr=ala&ala=1&alatpl=adress&pos=0&hs=2&xthttps=111111"
    print("请求网址：", url)
    text = requestpageText(url)

    soup = BeautifulSoup(text, 'lxml')
    # soup = BeautifulSoup(open('index.html'))
    print(soup.prettify())  # 格式化输出 soup 对象的内容
    '''
    四大对象种类
    Tag   标签
    NavigableString  获取标签内部的文字
    BeautifulSoup    一个文档的全部内容
    Comment          一个特殊类型的 NavigableString 对象
    '''
    print(soup.title)
    print(soup.title.name)  # 签本身的名称
    print(soup.title.attrs)  # 签的所有属性
    print(soup.head)

    imglist = soup.find_all("[class='imgbox],a[href],img")# class=imgbox  所有的a节点，有href属性  带img标签  的所有节点
    lenth = len(imglist)  # 计算集合的个数
    print("#######################################")
    for i in range(lenth):
        print(imglist[i].attrs['src'])  # 抓取img中属性为src的信息,例如<img src="123456" xxxxxxxxxxxxxxxx,则输出为123456

#百度图片获取的方式是采用ajax （怪不得使用开发者工具定位到了图片链接，但使用beatifulsoub解析结果为空）
#请求头中包含：X-Requested-With: XMLHttpRequest  ---》  是AJAX请求

'''
发出的AJAX请求：
Request URL: https://image.baidu.com/search/acjson?tn=resultjson_com&ipn=rj&ct=201326592&is=&fp=result&queryWord=%E7%BE%8E%E5%A5%B3&cl=2&lm=-1&ie=utf-8&oe=utf-8&adpicid=&st=&z=&ic=&word=%E7%BE%8E%E5%A5%B3&s=&se=&tab=&width=&height=&face=&istype=&qc=&nc=&fr=&cg=girl&pn=30&rn=30&gsm=1e&1532580786933=

请求参数：网页向下滑动，发出更多的AJAX请求，参数大部分一样只有pn(应该是图片数量），gsm(pn的16进制）不一样
tn: resultjson_com
ipn: rj
ct: 201326592
is: 
fp: result
queryWord: 美女
cl: 2
lm: -1
ie: utf-8
oe: utf-8
adpicid: 
st: 
z: 
ic: 
word: 美女
s: 
se: 
tab: 
width: 
height: 
face: 
istype: 
qc: 
nc: 
fr: 
cg: girl
pn: 30
rn: 30
gsm: 1e
1532580786933: 

'''
from urllib.parse import urlencode
import json
def getManyPages(keyword,pages):
    params=[]
    for i in range(30,30*pages+30,30):
        params.append({
            'tn': 'resultjson_com',
            'ipn': 'rj',
            'ct': '201326592',
            'is': '',
            'fp': 'result',
            'queryWord': '美女',
            'cl': '2',
            'lm': '-1',
            'ie': 'utf-8',
            'oe': 'utf-8',
            'adpicid': '',
            'st': '',
            'z': '',
            'ic': '',
            'word': '美女',
            's': '',
            'se': '',
            'tab': '',
            'width': '',
            'height': '',
            'face': '',
            'istype': '',
            'qc': '',
            'nc': '',
            'fr': '',
            'cg': 'girl',
            'pn': str(i),
            'rn': '30',
            'gsm': str(hex(i)),
            '1532580786933': ''
        })
    url = 'https://image.baidu.com/search/acjson'
    urls = []
    for i in params:
        # urls.append(requests.get(url,params=i).json().get('data'))#json()转为json格式
        #print(url+'?'+urlencode(i))
        #txt=requests.get(url+urlencode(i)).text
        respone = requests.get(url,params=i)
        print(respone.url)
        '''
        https://image.baidu.com/search/acjson?tn=resultjson_com&ipn=rj&ct=201326592&is=&fp=result&queryWord=%E7%BE%8E%E5%A5%B3&cl=2&lm=-1&ie=utf-8&oe=utf-8&adpicid=&st=&z=&ic=&word=%E7%BE%8E%E5%A5%B3&s=&se=&tab=&width=&height=&face=&istype=&qc=&nc=&fr=&cg=girl&pn=30&rn=30&gsm=1e&1532756515209=
         '''
        txt = respone.text.encode('utf-8')
        print(txt)
        myjson=json.loads(txt)
        #方法一
        cnt=0
        for it in myjson['data']:
            if 'thumbURL' in it:#python3不用has_key
                img_url=it['thumbURL']
                print(img_url)
                filename = PWD + img_url.split('/')[-1]
                if os.path.isfile(filename):
                    print("文件存在：", filename)
                    if IsValidImage(filename):
                        continue
                    else:
                        os.remove(filename)
                cnt +=1
                print("开始下载第{0}张图片".format(cnt))
                # 使用多线程下载
                t1 = Thread(target=downfile, args=(filename, img_url))
                threading.BoundedSemaphore(5)  # 同一时刻可以有四个线程
                t1.start()
            else:
                print('no key:thumbURL')
        #方法二
        # for it in range(len(myjson['data'])):
        #     print(myjson['data'][it]['thumbURL'])

    return urls

if __name__ == "__main__":
    #下载百度图片
    #getManyPages("美女",1)
    #下载花瓣图片
    requestUrl(url)

    '''
    AJAX请求的返回json结果：data数组有30个元素，应该是对应30个图片，每个元素中有好几个图片链接
    bdstatic.com是属于baidu的域名
    thumbURL/middleURL/hoverURL的链接是一样，用浏览器打开就是一张图片，但是json中"width": 800, "height": 1027, 下载下来的图片只有
    171px*220px
    继续分析，
    
    
    {
    "queryEnc": "%E7%BE%8E%E5%A5%B3", 
    "queryExt": "美女", 
    "listNum": 891, 
    "displayNum": 16925, 
    "gsm": "3c", 
    "bdFmtDispNum": "约16,900", 
    "bdSearchTime": "", 
    "isNeedAsyncRequest": 0, 
    "bdIsClustered": "1", 
    "data": [
        {
            "adType": "0", 
            "hasAspData": "0", 
            "thumbURL": "https://ss0.bdstatic.com/70cFvHSh_Q1YnxGkpoWK1HF6hhy/it/u=2747088918,3789947110&fm=15&gp=0.jpg", 
            "middleURL": "https://ss0.bdstatic.com/70cFvHSh_Q1YnxGkpoWK1HF6hhy/it/u=2747088918,3789947110&fm=15&gp=0.jpg", 
            "largeTnImageUrl": "", 
            "hasLarge": 0, 
            "hoverURL": "https://ss0.bdstatic.com/70cFvHSh_Q1YnxGkpoWK1HF6hhy/it/u=2747088918,3789947110&fm=15&gp=0.jpg", 
            "pageNum": 30, 
            "objURL": "ippr_z2C$qAzdH3FAzdH3Ft42_z&e3B77p7o_z&e3Bv54AzdH3F77p7oAzdH3Fkt2AzdH3F77AzdH3Fa9AzdH3FllAzdH3Fb980_z&e3B3r2", 
            "fromURL": "ippr_z2C$qAzdH3FAzdH3Fooo_z&e3B77p7o_z&e3Bv54AzdH3Ft4w2jetjovuwmjcn0_z&e3Bip4s", 
            "fromURLHost": "www.uutuw.com", 
            "currentIndex": "", 
            "width": 800, 
            "height": 1027, 
            "type": "jpg", 
            "is_gif": 0, 
            "filesize": "", 
            "bdSrcType": "15", 
            "di": "52979456762", 
            "pi": "0", 
            "is": "0,0", 
            "imgCollectionWord": "", 
            "replaceUrl": [
                {
                    "ObjURL": "http://img3.imgtn.bdimg.com/it/u=2747088918,3789947110&fm=214&gp=0.jpg", 
                    "FromURL": "http://news.163.com/11/0512/11/73rmp20500014jb6_3.html"
                }, 
                {
                    "ObjURL": "http://image58.360doc.com/downloadimg/2013/01/2118/29751043_6.jpg", 
                    "FromURL": "http://www.360doc.com/content/13/0121/06/276037_261607301.shtml"
                }
            ], 
            "hasThumbData": "0", 
            "bdSetImgNum": 0, 
            "partnerId": 0, 
            "spn": 0, 
            "bdImgnewsDate": "1970-01-01 08:00", 
            "fromPageTitle": "董璇<strong>写真</strong> - uu美女图网 www.uutuw.com", 
            "fromPageTitleEnc": "董璇写真 - uu美女图网 www.uutuw.com", 
            "bdSourceName": "", 
            "bdFromPageTitlePrefix": "", 
            "isAspDianjing": 0, 
            "token": "", 
            "imgType": "", 
            "cs": "2747088918,3789947110", 
            "os": "1691283298,3872427903", 
            "simid": "10029312,1058436041", 
            "personalized": "0", 
            "simid_info": null, 
            "face_info": null, 
            "xiangshi_info": null, 
            "adPicId": "0", 
            "source_type": ""
        }, 
        {
            "adType": "0", 
            "hasAspData": "0", 
            "thumbURL": "https://ss1.bdstatic.com/70cFvXSh_Q1YnxGkpoWK1HF6hhy/it/u=2319449996,2409855939&fm=15&gp=0.jpg", 
            "middleURL": "https://ss1.bdstatic.com/70cFvXSh_Q1YnxGkpoWK1HF6hhy/it/u=2319449996,2409855939&fm=15&gp=0.jpg", 
            "largeTnImageUrl": "", 
            "hasLarge": 0, 
            "hoverURL": "https://ss1.bdstatic.com/70cFvXSh_Q1YnxGkpoWK1HF6hhy/it/u=2319449996,2409855939&fm=15&gp=0.jpg", 
            "pageNum": 31, 
            "objURL": "ippr_z2C$qAzdH3FAzdH3Ft42_z&e3B77p7o_z&e3Bv54AzdH3F77p7oAzdH3Fkt2AzdH3F77p7o8aAzdH3FaaAzdH3F89AzdH3FalAzdH3Fc89_z&e3B3r2", 
            "fromURL": "ippr_z2C$qAzdH3FAzdH3Fooo_z&e3B77p7o_z&e3Bv54AzdH3Ft4w2jetjow1ljbmd8_z&e3Bip4s", 
            "fromURLHost": "www.uutuw.com", 
            "currentIndex": "", 
            "width": 800, 
            "height": 1150, 
            "type": "jpg", 
            "is_gif": 0, 
            "filesize": "", 
            "bdSrcType": "15", 
            "di": "293353746962", 
            "pi": "0", 
            "is": "0,0", 
            "imgCollectionWord": "", 
            "replaceUrl": [
                {
                    "ObjURL": "http://img1.imgtn.bdimg.com/it/u=2319449996,2409855939&fm=214&gp=0.jpg", 
                    "FromURL": "http://yuedu.163.com/sns/feed?uid=umgi4"
                }, 
                {
                    "ObjURL": "http://img2.ph.126.net/min15uwrzziw7kswr_cgog==/1258193145996684392.jpg", 
                    "FromURL": "http://pp.163.com/tyrantz/pp/11704002.html"
                }
            ], 
            "hasThumbData": "0", 
            "bdSetImgNum": 0, 
            "partnerId": 0, 
            "spn": 0, 
            "bdImgnewsDate": "1970-01-01 08:00", 
            "fromPageTitle": "学生妹拍<strong>写真</strong> - uu美女图网 www.uutuw.com", 
            "fromPageTitleEnc": "学生妹拍写真 - uu美女图网 www.uutuw.com", 
            "bdSourceName": "", 
            "bdFromPageTitlePrefix": "", 
            "isAspDianjing": 0, 
            "token": "", 
            "imgType": "", 
            "cs": "2319449996,2409855939", 
            "os": "1903368392,2409688898", 
            "simid": "4153476169,418956316", 
            "personalized": "0", 
            "simid_info": null, 
            "face_info": null, 
            "xiangshi_info": null, 
            "adPicId": "0", 
            "source_type": ""
        }, 
        {
            "adType": "0", 
            "hasAspData": "0", 
            "thumbURL": "https://ss1.bdstatic.com/70cFvXSh_Q1YnxGkpoWK1HF6hhy/it/u=2116705811,3146691174&fm=27&gp=0.jpg", 
            "middleURL": "https://ss1.bdstatic.com/70cFvXSh_Q1YnxGkpoWK1HF6hhy/it/u=2116705811,3146691174&fm=27&gp=0.jpg", 
            "largeTnImageUrl": "", 
            "hasLarge": 0, 
            "hoverURL": "https://ss1.bdstatic.com/70cFvXSh_Q1YnxGkpoWK1HF6hhy/it/u=2116705811,3146691174&fm=27&gp=0.jpg", 
            "pageNum": 32, 
            "objURL": "ippr_z2C$qAzdH3FAzdH3Ffn_z&e3Bftgwt42_z&e3BvgAzdH3F4t11sjAzdH3Fbjjnjawvxka808km0wd1d&mla", 
            "fromURL": "ippr_z2C$qAzdH3FAzdH3Fks52_z&e3Bftgw_z&e3Bv54_z&e3BvgAzdH3FfAzdH3Fks52_bjjnjawva8aaetmg_z&e3Bip4s", 
            "fromURLHost": "blog.sina.com.cn", 
            "currentIndex": "", 
            "width": 494, 
            "height": 690, 
            "type": "jpg", 
            "is_gif": 0, 
            "filesize": "", 
            "bdSrcType": "0", 
            "di": "114124179071", 
            "pi": "0", 
            "is": "0,0", 
            "imgCollectionWord": "", 
            "replaceUrl": [
                {
                    "ObjURL": "http://img1.imgtn.bdimg.com/it/u=2116705811,3146691174&fm=214&gp=0.jpg", 
                    "FromURL": "http://www.mafengwo.cn/i/1030080.html"
                }, 
                {
                    "ObjURL": "http://file20.mafengwo.net/m00/15/59/wkgb3fb9btbfemddaakihpq-ps073.groupinfo.w600.jpeg", 
                    "FromURL": "http://www.mafengwo.cn/i/1030080.html"
                }
            ], 
            "hasThumbData": "0", 
            "bdSetImgNum": 0, 
            "partnerId": 0, 
            "spn": 0, 
            "bdImgnewsDate": "1970-01-01 08:00", 
            "fromPageTitle": "90后气质<strong>美女</strong>郭梦瑶私房写真", 
            "fromPageTitleEnc": "90后气质美女郭梦瑶私房写真", 
            "bdSourceName": "", 
            "bdFromPageTitlePrefix": "", 
            "isAspDianjing": 0, 
            "token": "", 
            "imgType": "", 
            "cs": "2116705811,3146691174", 
            "os": "1790093731,1184106442", 
            "simid": "4132130946,671686393", 
            "personalized": "0", 
            "simid_info": null, 
            "face_info": null, 
            "xiangshi_info": null, 
            "adPicId": "0", 
            "source_type": ""
        }, 
        {
            "adType": "0", 
            "hasAspData": "0", 
            "thumbURL": "https://ss2.bdstatic.com/70cFvnSh_Q1YnxGkpoWK1HF6hhy/it/u=3813142697,522369589&fm=27&gp=0.jpg", 
            "middleURL": "https://ss2.bdstatic.com/70cFvnSh_Q1YnxGkpoWK1HF6hhy/it/u=3813142697,522369589&fm=27&gp=0.jpg", 
            "largeTnImageUrl": "", 
            "hasLarge": 0, 
            "hoverURL": "https://ss2.bdstatic.com/70cFvnSh_Q1YnxGkpoWK1HF6hhy/it/u=3813142697,522369589&fm=27&gp=0.jpg", 
            "pageNum": 33, 
            "objURL": "ippr_z2C$qAzdH3FAzdH3Ff8m_z&e3Bftgwt42_z&e3BvgAzdH3F4omlaAzdH3FaamLD5UHzy0w7Xp6pnFmu&mla", 
            "fromURL": "ippr_z2C$qAzdH3FAzdH3Fks52_z&e3Bftgw_z&e3Bv54_z&e3BvgAzdH3FfAzdH3Fks52_808lmvv8na8adxcgr_z&e3Bip4s", 
            "fromURLHost": "blog.sina.com.cn", 
            "currentIndex": "", 
            "width": 690, 
            "height": 1013, 
            "type": "jpg", 
            "is_gif": 0, 
            "filesize": "", 
            "bdSrcType": "0", 
            "di": "196389140421", 
            "pi": "0", 
            "is": "0,0", 
            "imgCollectionWord": "", 
            "replaceUrl": [
                {
                    "ObjURL": "http://img2.imgtn.bdimg.com/it/u=3813142697,522369589&fm=214&gp=0.jpg", 
                    "FromURL": "http://blog.sina.com.cn/s/blog_17a5b84e30102x20v.html"
                }, 
                {
                    "ObjURL": "http://img.xinwenge.net/data/attachment/63d6037fe9bdf26b48714a3787391022.jpg", 
                    "FromURL": "http://www.xinwenge.net/ntes/yulebigbang/b2868a3b.html"
                }
            ], 
            "hasThumbData": "0", 
            "bdSetImgNum": 0, 
            "partnerId": 0, 
            "spn": 0, 
            "bdImgnewsDate": "1970-01-01 08:00", 
            "fromPageTitle": "前一篇:性感学妹小甜心cc海边风情内衣诱惑迷人<strong>写真</strong> 后一篇:性感美女", 
            "fromPageTitleEnc": "前一篇:性感学妹小甜心cc海边风情内衣诱惑迷人写真 后一篇:性感美女", 
            "bdSourceName": "", 
            "bdFromPageTitlePrefix": "", 
            "isAspDianjing": 0, 
            "token": "", 
            "imgType": "", 
            "cs": "3813142697,522369589", 
            "os": "4069147977,3305375524", 
            "simid": "3365118876,13356578", 
            "personalized": "0", 
            "simid_info": null, 
            "face_info": null, 
            "xiangshi_info": null, 
            "adPicId": "0", 
            "source_type": ""
        }, 
        {
            "adType": "0", 
            "hasAspData": "0", 
            "thumbURL": "https://ss3.bdstatic.com/70cFv8Sh_Q1YnxGkpoWK1HF6hhy/it/u=3721367840,547918981&fm=15&gp=0.jpg", 
            "middleURL": "https://ss3.bdstatic.com/70cFv8Sh_Q1YnxGkpoWK1HF6hhy/it/u=3721367840,547918981&fm=15&gp=0.jpg", 
            "largeTnImageUrl": "", 
            "hasLarge": 0, 
            "hoverURL": "https://ss3.bdstatic.com/70cFv8Sh_Q1YnxGkpoWK1HF6hhy/it/u=3721367840,547918981&fm=15&gp=0.jpg", 
            "pageNum": 34, 
            "objURL": "ippr_z2C$qAzdH3FAzdH3Ft42_z&e3B77p7o_z&e3Bv54AzdH3F77p7oAzdH3Fkt2AzdH3F77AzdH3F88AzdH3FcmAzdH3Flllc_z&e3B3r2", 
            "fromURL": "ippr_z2C$qAzdH3FAzdH3Fooo_z&e3B77p7o_z&e3Bv54AzdH3Ft4w2jetjokavdul90_z&e3Bip4s", 
            "fromURLHost": "www.uutuw.com", 
            "currentIndex": "", 
            "width": 800, 
            "height": 1196, 
            "type": "jpg", 
            "is_gif": 0, 
            "filesize": "", 
            "bdSrcType": "15", 
            "di": "386531116852", 
            "pi": "0", 
            "is": "0,0", 
            "imgCollectionWord": "", 
            "replaceUrl": [
                {
                    "ObjURL": "http://img0.imgtn.bdimg.com/it/u=3721367840,547918981&fm=214&gp=0.jpg", 
                    "FromURL": "http://roll.sohu.com/20111216/n329260604.shtml"
                }, 
                {
                    "ObjURL": "http://photocdn.sohu.com/20111216/img329257454.jpg", 
                    "FromURL": "http://roll.sohu.com/20111216/n329257453.shtml"
                }
            ], 
            "hasThumbData": "0", 
            "bdSetImgNum": 0, 
            "partnerId": 0, 
            "spn": 0, 
            "bdImgnewsDate": "1970-01-01 08:00", 
            "fromPageTitle": "四川在线徐冬冬拍黑色蕾丝内衣<strong>写真</strong>秀性感唯 - uu美女", 
            "fromPageTitleEnc": "四川在线徐冬冬拍黑色蕾丝内衣写真秀性感唯 - uu美女", 
            "bdSourceName": "", 
            "bdFromPageTitlePrefix": "", 
            "isAspDianjing": 0, 
            "token": "", 
            "imgType": "", 
            "cs": "3721367840,547918981", 
            "os": "1791791647,3851090721", 
            "simid": "3504093980,350195221", 
            "personalized": "0", 
            "simid_info": null, 
            "face_info": null, 
            "xiangshi_info": null, 
            "adPicId": "0", 
            "source_type": ""
        }, 
        {
            "adType": "0", 
            "hasAspData": "0", 
            "thumbURL": "https://ss1.bdstatic.com/70cFvXSh_Q1YnxGkpoWK1HF6hhy/it/u=1149051579,715530148&fm=27&gp=0.jpg", 
            "middleURL": "https://ss1.bdstatic.com/70cFvXSh_Q1YnxGkpoWK1HF6hhy/it/u=1149051579,715530148&fm=27&gp=0.jpg", 
            "largeTnImageUrl": "", 
            "hasLarge": 0, 
            "hoverURL": "https://ss1.bdstatic.com/70cFvXSh_Q1YnxGkpoWK1HF6hhy/it/u=1149051579,715530148&fm=27&gp=0.jpg", 
            "pageNum": 35, 
            "objURL": "ippr_z2C$qAzdH3FAzdH3Ft42_z&e3B8l8lm_z&e3Bv54AzdH3F7rs5w1fAzdH3F8c8dalAzdH3Fl-8c8dal88ddnmdS_z&e3B3r2", 
            "fromURL": "ippr_z2C$qAzdH3FAzdH3Fooo_z&e3B8l8lm_z&e3Bv54AzdH3F4AzdH3Fetjo_z&e3Brir?wt1=nlb9m", 
            "fromURLHost": "www.19196.com", 
            "currentIndex": "", 
            "width": 440, 
            "height": 632, 
            "type": "jpg", 
            "is_gif": 0, 
            "filesize": "", 
            "bdSrcType": "0", 
            "di": "41597699002", 
            "pi": "0", 
            "is": "0,0", 
            "imgCollectionWord": "", 
            "replaceUrl": [
                {
                    "ObjURL": "http://img4.imgtn.bdimg.com/it/u=1149051579,715530148&fm=214&gp=0.jpg", 
                    "FromURL": "http://www.19196.com/m/view.php?aid=39846"
                }
            ], 
            "hasThumbData": "0", 
            "bdSetImgNum": 0, 
            "partnerId": 0, 
            "spn": 0, 
            "bdImgnewsDate": "1970-01-01 08:00", 
            "fromPageTitle": "甜美<strong>美女</strong>milk楚楚户外写真", 
            "fromPageTitleEnc": "甜美美女milk楚楚户外写真", 
            "bdSourceName": "", 
            "bdFromPageTitlePrefix": "", 
            "isAspDianjing": 0, 
            "token": "", 
            "imgType": "", 
            "cs": "1149051579,715530148", 
            "os": "2816947110,112700409", 
            "simid": "0,0", 
            "personalized": "0", 
            "simid_info": null, 
            "face_info": null, 
            "xiangshi_info": null, 
            "adPicId": "0", 
            "source_type": ""
        }, 
        {
            "adType": "0", 
            "hasAspData": "0", 
            "thumbURL": "https://ss1.bdstatic.com/70cFvXSh_Q1YnxGkpoWK1HF6hhy/it/u=362699704,1351130835&fm=15&gp=0.jpg", 
            "middleURL": "https://ss1.bdstatic.com/70cFvXSh_Q1YnxGkpoWK1HF6hhy/it/u=362699704,1351130835&fm=15&gp=0.jpg", 
            "largeTnImageUrl": "", 
            "hasLarge": 0, 
            "hoverURL": "https://ss1.bdstatic.com/70cFvXSh_Q1YnxGkpoWK1HF6hhy/it/u=362699704,1351130835&fm=15&gp=0.jpg", 
            "pageNum": 36, 
            "objURL": "ippr_z2C$qAzdH3FAzdH3Ft42_z&e3B77p7o_z&e3Bv54AzdH3F77p7oAzdH3Fkt2AzdH3F77AzdH3FacAzdH3Fl9AzdH3Fnbn8_z&e3B3r2", 
            "fromURL": "ippr_z2C$qAzdH3FAzdH3Fooo_z&e3B77p7o_z&e3Bv54AzdH3Ft4w2jetjollm90ww8_z&e3Bip4s", 
            "fromURLHost": "www.uutuw.com", 
            "currentIndex": "", 
            "width": 800, 
            "height": 1070, 
            "type": "jpg", 
            "is_gif": 0, 
            "filesize": "", 
            "bdSrcType": "15", 
            "di": "135549114592", 
            "pi": "0", 
            "is": "0,0", 
            "imgCollectionWord": "", 
            "replaceUrl": [
                {
                    "ObjURL": "http://img4.imgtn.bdimg.com/it/u=362699704,1351130835&fm=214&gp=0.jpg", 
                    "FromURL": "http://blog.sina.com.cn/s/blog_721e4aa20100yeuv.html"
                }, 
                {
                    "ObjURL": "http://s11.sinaimg.cn/mw690/002sr99hgy6e3wzyjkc5a&690", 
                    "FromURL": "http://blog.sina.com.cn/s/blog_9d2834ab0101owyj.html"
                }
            ], 
            "hasThumbData": "0", 
            "bdSetImgNum": 0, 
            "partnerId": 0, 
            "spn": 0, 
            "bdImgnewsDate": "1970-01-01 08:00", 
            "fromPageTitle": "韩最美女球迷<strong>写真</strong>遭曝光原是著名车模 - uu美女图网 .", 
            "fromPageTitleEnc": "韩最美女球迷写真遭曝光原是著名车模 - uu美女图网 .", 
            "bdSourceName": "", 
            "bdFromPageTitlePrefix": "", 
            "isAspDianjing": 0, 
            "token": "", 
            "imgType": "", 
            "cs": "362699704,1351130835", 
            "os": "1618787141,3770151683", 
            "simid": "4125601393,693327876", 
            "personalized": "0", 
            "simid_info": null, 
            "face_info": null, 
            "xiangshi_info": null, 
            "adPicId": "0", 
            "source_type": ""
        }, 
        {
            "adType": "0", 
            "hasAspData": "0", 
            "thumbURL": "https://ss1.bdstatic.com/70cFvXSh_Q1YnxGkpoWK1HF6hhy/it/u=3064380408,1343813255&fm=15&gp=0.jpg", 
            "middleURL": "https://ss1.bdstatic.com/70cFvXSh_Q1YnxGkpoWK1HF6hhy/it/u=3064380408,1343813255&fm=15&gp=0.jpg", 
            "largeTnImageUrl": "", 
            "hasLarge": 0, 
            "hoverURL": "https://ss1.bdstatic.com/70cFvXSh_Q1YnxGkpoWK1HF6hhy/it/u=3064380408,1343813255&fm=15&gp=0.jpg", 
            "pageNum": 37, 
            "objURL": "ippr_z2C$qAzdH3FAzdH3Ft42_z&e3B77p7o_z&e3Bv54AzdH3F77p7oAzdH3Fkt2AzdH3F77AzdH3F88AzdH3F9bAzdH3Fabld_z&e3B3r2", 
            "fromURL": "ippr_z2C$qAzdH3FAzdH3Fooo_z&e3B77p7o_z&e3Bv54AzdH3Ft4w2jetjo0kjujn8u_z&e3Bip4s", 
            "fromURLHost": "www.uutuw.com", 
            "currentIndex": "", 
            "width": 800, 
            "height": 508, 
            "type": "jpg", 
            "is_gif": 0, 
            "filesize": "", 
            "bdSrcType": "15", 
            "di": "190598458942", 
            "pi": "0", 
            "is": "0,0", 
            "imgCollectionWord": "", 
            "replaceUrl": [
                {
                    "ObjURL": "http://img3.imgtn.bdimg.com/it/u=3064380408,1343813255&fm=214&gp=0.jpg", 
                    "FromURL": "http://www.meituan.com/ztbjbysytg"
                }, 
                {
                    "ObjURL": "http://f4.lashouimg.com/cms/m00/b1/1a/cqgbhltk-emaauqaaagfmgai_vw570.jpg", 
                    "FromURL": "http://www.lashou.com/cate/sheying/sanshuiqu-all/page4"
                }
            ], 
            "hasThumbData": "0", 
            "bdSetImgNum": 0, 
            "partnerId": 0, 
            "spn": 0, 
            "bdImgnewsDate": "1970-01-01 08:00", 
            "fromPageTitle": "八月海岸298元个人<strong>写真</strong>照团购个人写真北京 - uu美女", 
            "fromPageTitleEnc": "八月海岸298元个人写真照团购个人写真北京 - uu美女", 
            "bdSourceName": "", 
            "bdFromPageTitlePrefix": "", 
            "isAspDianjing": 0, 
            "token": "", 
            "imgType": "", 
            "cs": "3064380408,1343813255", 
            "os": "1625818215,3705249651", 
            "simid": "3220527946,2883175014", 
            "personalized": "0", 
            "simid_info": null, 
            "face_info": null, 
            "xiangshi_info": null, 
            "adPicId": "0", 
            "source_type": ""
        }, 
        {
            "adType": "0", 
            "hasAspData": "0", 
            "thumbURL": "https://ss1.bdstatic.com/70cFvXSh_Q1YnxGkpoWK1HF6hhy/it/u=2437058269,824365392&fm=15&gp=0.jpg", 
            "middleURL": "https://ss1.bdstatic.com/70cFvXSh_Q1YnxGkpoWK1HF6hhy/it/u=2437058269,824365392&fm=15&gp=0.jpg", 
            "largeTnImageUrl": "", 
            "hasLarge": 0, 
            "hoverURL": "https://ss1.bdstatic.com/70cFvXSh_Q1YnxGkpoWK1HF6hhy/it/u=2437058269,824365392&fm=15&gp=0.jpg", 
            "pageNum": 38, 
            "objURL": "ippr_z2C$qAzdH3FAzdH3Ft42_z&e3B77p7o_z&e3Bv54AzdH3F77p7oAzdH3Fkt2AzdH3F8AzdH3FmmAzdH3FmcnlAzdH3Fmcnbal_z&e3B3r2", 
            "fromURL": "ippr_z2C$qAzdH3FAzdH3Fooo_z&e3B77p7o_z&e3Bv54AzdH3Ft4w2jetjoa8knmbcc_z&e3Bip4s", 
            "fromURLHost": "www.uutuw.com", 
            "currentIndex": "", 
            "width": 800, 
            "height": 1155, 
            "type": "jpg", 
            "is_gif": 0, 
            "filesize": "", 
            "bdSrcType": "15", 
            "di": "125879801422", 
            "pi": "0", 
            "is": "0,0", 
            "imgCollectionWord": "", 
            "replaceUrl": [
                {
                    "ObjURL": "http://img4.imgtn.bdimg.com/it/u=2437058269,824365392&fm=214&gp=0.jpg", 
                    "FromURL": "http://www.sc115.com/mm/609.html"
                }, 
                {
                    "ObjURL": "http://img0.ph.126.net/ee882q6ma9xic0hizcsusw==/6632043032514711105.jpg", 
                    "FromURL": "http://m13918321859.blog.163.com/blog/static/22903806720171122203712"
                }
            ], 
            "hasThumbData": "0", 
            "bdSetImgNum": 0, 
            "partnerId": 0, 
            "spn": 0, 
            "bdImgnewsDate": "1970-01-01 08:00", 
            "fromPageTitle": "<strong>美女</strong>图片 | 美女 美女写真", 
            "fromPageTitleEnc": "美女图片 | 美女 美女写真", 
            "bdSourceName": "", 
            "bdFromPageTitlePrefix": "", 
            "isAspDianjing": 0, 
            "token": "", 
            "imgType": "", 
            "cs": "2437058269,824365392", 
            "os": "2538441667,2351408832", 
            "simid": "3382460767,254256648", 
            "personalized": "0", 
            "simid_info": null, 
            "face_info": null, 
            "xiangshi_info": null, 
            "adPicId": "0", 
            "source_type": ""
        }, 
        {
            "adType": "0", 
            "hasAspData": "0", 
            "thumbURL": "https://ss0.bdstatic.com/70cFuHSh_Q1YnxGkpoWK1HF6hhy/it/u=1420852939,2285362443&fm=15&gp=0.jpg", 
            "middleURL": "https://ss0.bdstatic.com/70cFuHSh_Q1YnxGkpoWK1HF6hhy/it/u=1420852939,2285362443&fm=15&gp=0.jpg", 
            "largeTnImageUrl": "", 
            "hasLarge": 0, 
            "hoverURL": "https://ss0.bdstatic.com/70cFuHSh_Q1YnxGkpoWK1HF6hhy/it/u=1420852939,2285362443&fm=15&gp=0.jpg", 
            "pageNum": 39, 
            "objURL": "ippr_z2C$qAzdH3FAzdH3Ft42_z&e3B77p7o_z&e3Bv54AzdH3F77p7oAzdH3Fkt2AzdH3F8AzdH3F8mAzdH3F8c0dAzdH3F8c08m9_z&e3B3r2", 
            "fromURL": "ippr_z2C$qAzdH3FAzdH3Fooo_z&e3B77p7o_z&e3Bv54AzdH3Ft4w2jetjo1011jaac_z&e3Bip4s", 
            "fromURLHost": "www.uutuw.com", 
            "currentIndex": "", 
            "width": 800, 
            "height": 1132, 
            "type": "jpg", 
            "is_gif": 0, 
            "filesize": "", 
            "bdSrcType": "15", 
            "di": "264706932062", 
            "pi": "0", 
            "is": "0,0", 
            "imgCollectionWord": "", 
            "replaceUrl": [
                {
                    "ObjURL": "http://img4.imgtn.bdimg.com/it/u=1420852939,2285362443&fm=214&gp=0.jpg", 
                    "FromURL": "http://pic.yesky.com/144/36896644_2.shtml"
                }, 
                {
                    "ObjURL": "http://image.xcar.com.cn/attachments/a/day_141227/2014122713_e981ed69444a51573404pxeuvnmtlw85.jpg", 
                    "FromURL": "http://www.xcar.com.cn/bbs/viewthread.php?tid=21353833"
                }
            ], 
            "hasThumbData": "0", 
            "bdSetImgNum": 0, 
            "partnerId": 0, 
            "spn": 0, 
            "bdImgnewsDate": "1970-01-01 08:00", 
            "fromPageTitle": "沙发 <strong>写真</strong>摄影图吉利大学美女", 
            "fromPageTitleEnc": "沙发 写真摄影图吉利大学美女", 
            "bdSourceName": "", 
            "bdFromPageTitlePrefix": "", 
            "isAspDianjing": 0, 
            "token": "", 
            "imgType": "", 
            "cs": "1420852939,2285362443", 
            "os": "2405242401,2328757113", 
            "simid": "3328138489,232471219", 
            "personalized": "0", 
            "simid_info": null, 
            "face_info": null, 
            "xiangshi_info": null, 
            "adPicId": "0", 
            "source_type": ""
        }, 
        {
            "adType": "0", 
            "hasAspData": "0", 
            "thumbURL": "https://ss1.bdstatic.com/70cFuXSh_Q1YnxGkpoWK1HF6hhy/it/u=4252342285,3039395162&fm=15&gp=0.jpg", 
            "middleURL": "https://ss1.bdstatic.com/70cFuXSh_Q1YnxGkpoWK1HF6hhy/it/u=4252342285,3039395162&fm=15&gp=0.jpg", 
            "largeTnImageUrl": "", 
            "hasLarge": 0, 
            "hoverURL": "https://ss1.bdstatic.com/70cFuXSh_Q1YnxGkpoWK1HF6hhy/it/u=4252342285,3039395162&fm=15&gp=0.jpg", 
            "pageNum": 40, 
            "objURL": "ippr_z2C$qAzdH3FAzdH3Ft42_z&e3B77p7o_z&e3Bv54AzdH3F77p7oAzdH3Fkt2AzdH3F77AzdH3F88AzdH3F09AzdH3F9n8m_z&e3B3r2", 
            "fromURL": "ippr_z2C$qAzdH3FAzdH3Fooo_z&e3B77p7o_z&e3Bv54AzdH3Ft4w2jetjo0mdcmnv1_z&e3Bip4s", 
            "fromURLHost": "www.uutuw.com", 
            "currentIndex": "", 
            "width": 800, 
            "height": 1198, 
            "type": "jpg", 
            "is_gif": 0, 
            "filesize": "", 
            "bdSrcType": "15", 
            "di": "191177668782", 
            "pi": "0", 
            "is": "0,0", 
            "imgCollectionWord": "", 
            "replaceUrl": [
                {
                    "ObjURL": "http://img0.imgtn.bdimg.com/it/u=4252342285,3039395162&fm=214&gp=0.jpg", 
                    "FromURL": "http://www.topit.me/album/1121484/item/13872489"
                }, 
                {
                    "ObjURL": "http://dynamic-image.yesky.com/320x480/uploadimages/2014/307/42/46j9go6o9887.jpg", 
                    "FromURL": "http://wap.yesky.com/pics/309/39858809.shtml"
                }
            ], 
            "hasThumbData": "0", 
            "bdSetImgNum": 0, 
            "partnerId": 0, 
            "spn": 0, 
            "bdImgnewsDate": "1970-01-01 08:00", 
            "fromPageTitle": "长腿<strong>美女</strong>宝贝拍写真力挺德意志战车南通网中", 
            "fromPageTitleEnc": "长腿美女宝贝拍写真力挺德意志战车南通网中", 
            "bdSourceName": "", 
            "bdFromPageTitlePrefix": "", 
            "isAspDianjing": 0, 
            "token": "", 
            "imgType": "", 
            "cs": "4252342285,3039395162", 
            "os": "1598412185,3725378371", 
            "simid": "3451033160,169367025", 
            "personalized": "0", 
            "simid_info": null, 
            "face_info": null, 
            "xiangshi_info": null, 
            "adPicId": "0", 
            "source_type": ""
        }, 
        {
            "adType": "0", 
            "hasAspData": "0", 
            "thumbURL": "https://ss0.bdstatic.com/70cFvHSh_Q1YnxGkpoWK1HF6hhy/it/u=4127080476,1632892528&fm=15&gp=0.jpg", 
            "middleURL": "https://ss0.bdstatic.com/70cFvHSh_Q1YnxGkpoWK1HF6hhy/it/u=4127080476,1632892528&fm=15&gp=0.jpg", 
            "largeTnImageUrl": "", 
            "hasLarge": 0, 
            "hoverURL": "https://ss0.bdstatic.com/70cFvHSh_Q1YnxGkpoWK1HF6hhy/it/u=4127080476,1632892528&fm=15&gp=0.jpg", 
            "pageNum": 41, 
            "objURL": "ippr_z2C$qAzdH3FAzdH3Ft42_z&e3B77p7o_z&e3Bv54AzdH3F77p7oAzdH3Fkt2AzdH3F77AzdH3FamAzdH3F98AzdH3Fm899_z&e3B3r2", 
            "fromURL": "ippr_z2C$qAzdH3FAzdH3Fooo_z&e3B77p7o_z&e3Bv54AzdH3Ft4w2jetjo881uvul8_z&e3Bip4s", 
            "fromURLHost": "www.uutuw.com", 
            "currentIndex": "", 
            "width": 800, 
            "height": 1188, 
            "type": "jpg", 
            "is_gif": 0, 
            "filesize": "", 
            "bdSrcType": "15", 
            "di": "23787315872", 
            "pi": "0", 
            "is": "0,0", 
            "imgCollectionWord": "", 
            "replaceUrl": [
                {
                    "ObjURL": "http://img1.imgtn.bdimg.com/it/u=4127080476,1632892528&fm=214&gp=0.jpg", 
                    "FromURL": "http://m.sohu.com/n/452269960/"
                }, 
                {
                    "ObjURL": "http://img.mp.itc.cn/upload/20160601/cc7dec8f6c954686b924995467f16097_th.jpg", 
                    "FromURL": "http://mt.sohu.com/20160601/n452269960.shtml"
                }
            ], 
            "hasThumbData": "0", 
            "bdSetImgNum": 0, 
            "partnerId": 0, 
            "spn": 0, 
            "bdImgnewsDate": "1970-01-01 08:00", 
            "fromPageTitle": "漂亮<strong>美女</strong>文员写真", 
            "fromPageTitleEnc": "漂亮美女文员写真", 
            "bdSourceName": "", 
            "bdFromPageTitlePrefix": "", 
            "isAspDianjing": 0, 
            "token": "", 
            "imgType": "", 
            "cs": "4127080476,1632892528", 
            "os": "1584031599,3660854406", 
            "simid": "4214672663,669035388", 
            "personalized": "0", 
            "simid_info": null, 
            "face_info": null, 
            "xiangshi_info": null, 
            "adPicId": "0", 
            "source_type": ""
        }, 
        {
            "adType": "0", 
            "hasAspData": "0", 
            "thumbURL": "https://ss0.bdstatic.com/70cFuHSh_Q1YnxGkpoWK1HF6hhy/it/u=1200297111,2595091645&fm=15&gp=0.jpg", 
            "middleURL": "https://ss0.bdstatic.com/70cFuHSh_Q1YnxGkpoWK1HF6hhy/it/u=1200297111,2595091645&fm=15&gp=0.jpg", 
            "largeTnImageUrl": "", 
            "hasLarge": 0, 
            "hoverURL": "https://ss0.bdstatic.com/70cFuHSh_Q1YnxGkpoWK1HF6hhy/it/u=1200297111,2595091645&fm=15&gp=0.jpg", 
            "pageNum": 42, 
            "objURL": "ippr_z2C$qAzdH3FAzdH3Ft42_z&e3B77p7o_z&e3Bv54AzdH3F77p7oAzdH3Fkt2AzdH3F77AzdH3F88AzdH3F09AzdH3Fl0dd_z&e3B3r2", 
            "fromURL": "ippr_z2C$qAzdH3FAzdH3Fooo_z&e3B77p7o_z&e3Bv54AzdH3Ft4w2jetjodclkdmub_z&e3Bip4s", 
            "fromURLHost": "www.uutuw.com", 
            "currentIndex": "", 
            "width": 800, 
            "height": 1198, 
            "type": "jpg", 
            "is_gif": 0, 
            "filesize": "", 
            "bdSrcType": "15", 
            "di": "108143605472", 
            "pi": "0", 
            "is": "0,0", 
            "imgCollectionWord": "", 
            "replaceUrl": [
                {
                    "ObjURL": "http://img1.imgtn.bdimg.com/it/u=1200297111,2595091645&fm=214&gp=0.jpg", 
                    "FromURL": "http://pp.163.com/zanebaby/pp/12905032.html"
                }, 
                {
                    "ObjURL": "http://img.pconline.com.cn/images/upload/upc/tx/photoblog/1406/30/c0/35812155_1404095296388_mthumb.jpg", 
                    "FromURL": "http://dp.pconline.com.cn/photo/list_3360813.html"
                }
            ], 
            "hasThumbData": "0", 
            "bdSetImgNum": 0, 
            "partnerId": 0, 
            "spn": 0, 
            "bdImgnewsDate": "1970-01-01 08:00", 
            "fromPageTitle": "夜色朦胧气质养眼<strong>美女</strong>牛仔裙甜美小清新写真", 
            "fromPageTitleEnc": "夜色朦胧气质养眼美女牛仔裙甜美小清新写真", 
            "bdSourceName": "", 
            "bdFromPageTitlePrefix": "", 
            "isAspDianjing": 0, 
            "token": "", 
            "imgType": "", 
            "cs": "1200297111,2595091645", 
            "os": "1671315054,3780841734", 
            "simid": "4165991404,838566692", 
            "personalized": "0", 
            "simid_info": null, 
            "face_info": null, 
            "xiangshi_info": null, 
            "adPicId": "0", 
            "source_type": ""
        }, 
        {
            "adType": "0", 
            "hasAspData": "0", 
            "thumbURL": "https://ss3.bdstatic.com/70cFv8Sh_Q1YnxGkpoWK1HF6hhy/it/u=3226292998,4174234203&fm=27&gp=0.jpg", 
            "middleURL": "https://ss3.bdstatic.com/70cFv8Sh_Q1YnxGkpoWK1HF6hhy/it/u=3226292998,4174234203&fm=27&gp=0.jpg", 
            "largeTnImageUrl": "", 
            "hasLarge": 0, 
            "hoverURL": "https://ss3.bdstatic.com/70cFv8Sh_Q1YnxGkpoWK1HF6hhy/it/u=3226292998,4174234203&fm=27&gp=0.jpg", 
            "pageNum": 43, 
            "objURL": "ippr_z2C$qAzdH3FAzdH3Fp-8_z&e3Bp7ziwg_z&e3Bv54AzdH3Fvalmdu9nwkwvAzdH3Fr-dAzdH3FsAzdH3Fda8nAzdH3Fa9AzdH3FabAzdH3FddAzdH3Fnuu0j9wauunl9nc0bdvuavv8cwwklaj9_z&e3B3r2", 
            "fromURL": "ippr_z2C$qAzdH3FAzdH3Fooo_z&e3Bp7ziwg_z&e3Bv54AzdH3FffiuAzdH3Fri5p5AzdH3Fwwmk01m0jwnuawlw_z&e3Bip4s", 
            "fromURLHost": "www.tuzhan.com", 
            "currentIndex": "", 
            "width": 600, 
            "height": 903, 
            "type": "jpg", 
            "is_gif": 0, 
            "filesize": "", 
            "bdSrcType": "0", 
            "di": "132691893841", 
            "pi": "0", 
            "is": "0,0", 
            "imgCollectionWord": "", 
            "replaceUrl": [
                {
                    "ObjURL": "http://img3.imgtn.bdimg.com/it/u=3226292998,4174234203&fm=214&gp=0.jpg", 
                    "FromURL": "http://www.tuzhan.com/sshf/photo/aa6b7d67ea3f0a9a.html"
                }, 
                {
                    "ObjURL": "http://t-1.tuzhan.com/31622b2d9dfa/p-2/l/2013/04/08/22/3ff7e4a0ff39435782cf0cc15aab90e4.jpg", 
                    "FromURL": "http://www.tuzhan.com/sshf/aa6b7d67ea3f0a9a.html"
                }
            ], 
            "hasThumbData": "0", 
            "bdSetImgNum": 0, 
            "partnerId": 0, 
            "spn": 0, 
            "bdImgnewsDate": "1970-01-01 08:00", 
            "fromPageTitle": "uu-4-7 (22)_副本.jpg - 人像|<strong>写真</strong>|自拍|美女 - 小u", 
            "fromPageTitleEnc": "uu-4-7 (22)_副本.jpg - 人像|写真|自拍|美女 - 小u", 
            "bdSourceName": "", 
            "bdFromPageTitlePrefix": "", 
            "isAspDianjing": 0, 
            "token": "", 
            "imgType": "", 
            "cs": "3226292998,4174234203", 
            "os": "3003879501,1911543348", 
            "simid": "4234890889,687622536", 
            "personalized": "0", 
            "simid_info": null, 
            "face_info": null, 
            "xiangshi_info": null, 
            "adPicId": "0", 
            "source_type": ""
        }, 
        {
            "adType": "0", 
            "hasAspData": "0", 
            "thumbURL": "https://ss0.bdstatic.com/70cFvHSh_Q1YnxGkpoWK1HF6hhy/it/u=860443285,3985579293&fm=15&gp=0.jpg", 
            "middleURL": "https://ss0.bdstatic.com/70cFvHSh_Q1YnxGkpoWK1HF6hhy/it/u=860443285,3985579293&fm=15&gp=0.jpg", 
            "largeTnImageUrl": "", 
            "hasLarge": 0, 
            "hoverURL": "https://ss0.bdstatic.com/70cFvHSh_Q1YnxGkpoWK1HF6hhy/it/u=860443285,3985579293&fm=15&gp=0.jpg", 
            "pageNum": 44, 
            "objURL": "ippr_z2C$qAzdH3FAzdH3Ft42_z&e3B77p7o_z&e3Bv54AzdH3F77p7oAzdH3Fkt2AzdH3F8AzdH3FnaAzdH3FdladAzdH3Fdla8dn_z&e3B3r2", 
            "fromURL": "ippr_z2C$qAzdH3FAzdH3Fooo_z&e3B77p7o_z&e3Bv54AzdH3Ft4w2jetjowd0wbvm9_z&e3Bip4s", 
            "fromURLHost": "www.uutuw.com", 
            "currentIndex": "", 
            "width": 800, 
            "height": 1055, 
            "type": "jpg", 
            "is_gif": 0, 
            "filesize": "", 
            "bdSrcType": "15", 
            "di": "69014319782", 
            "pi": "0", 
            "is": "0,0", 
            "imgCollectionWord": "", 
            "replaceUrl": [
                {
                    "ObjURL": "http://img0.imgtn.bdimg.com/it/u=860443285,3985579293&fm=214&gp=0.jpg", 
                    "FromURL": "http://www.360doc.com/content/17/0610/06/10791158_661520601.shtml"
                }, 
                {
                    "ObjURL": "http://s3.sinaimg.cn/mw690/0046675bzy7cwcbd5zie2&690", 
                    "FromURL": "http://blog.sina.com.cn/s/blog_dfcb5c270102x6lc.html"
                }
            ], 
            "hasThumbData": "0", 
            "bdSetImgNum": 0, 
            "partnerId": 0, 
            "spn": 0, 
            "bdImgnewsDate": "1970-01-01 08:00", 
            "fromPageTitle": "里香- 模特 - 3k<strong>美女写真</strong>图片网", 
            "fromPageTitleEnc": "里香- 模特 - 3k美女写真图片网", 
            "bdSourceName": "", 
            "bdFromPageTitlePrefix": "", 
            "isAspDianjing": 0, 
            "token": "", 
            "imgType": "", 
            "cs": "860443285,3985579293", 
            "os": "2328068706,2143831646", 
            "simid": "4110164868,380611783", 
            "personalized": "0", 
            "simid_info": null, 
            "face_info": null, 
            "xiangshi_info": null, 
            "adPicId": "0", 
            "source_type": ""
        }, 
        {
            "adType": "0", 
            "hasAspData": "0", 
            "thumbURL": "https://ss1.bdstatic.com/70cFuXSh_Q1YnxGkpoWK1HF6hhy/it/u=3198816309,3761849172&fm=27&gp=0.jpg", 
            "middleURL": "https://ss1.bdstatic.com/70cFuXSh_Q1YnxGkpoWK1HF6hhy/it/u=3198816309,3761849172&fm=27&gp=0.jpg", 
            "largeTnImageUrl": "", 
            "hasLarge": 0, 
            "hoverURL": "https://ss1.bdstatic.com/70cFuXSh_Q1YnxGkpoWK1HF6hhy/it/u=3198816309,3761849172&fm=27&gp=0.jpg", 
            "pageNum": 45, 
            "objURL": "ippr_z2C$qAzdH3FAzdH3Ff88_z&e3Bftgwt42_z&e3BvgAzdH3F4omlaAzdH3FaamrSjB22y0a4OOs7Uwvw&mla", 
            "fromURL": "ippr_z2C$qAzdH3FAzdH3Fks52_z&e3Bftgw_z&e3Bv54_z&e3BvgAzdH3FfAzdH3Fks52_8cjmvmvcja8ado0v5_z&e3Bip4s", 
            "fromURLHost": "blog.sina.com.cn", 
            "currentIndex": "", 
            "width": 690, 
            "height": 1035, 
            "type": "jpg", 
            "is_gif": 0, 
            "filesize": "", 
            "bdSrcType": "0", 
            "di": "144903692012", 
            "pi": "0", 
            "is": "0,0", 
            "imgCollectionWord": "", 
            "replaceUrl": [
                {
                    "ObjURL": "http://img4.imgtn.bdimg.com/it/u=3198816309,3761849172&fm=214&gp=0.jpg", 
                    "FromURL": "http://blog.163.com/zlxy63/blog/static/2257291172016879365389?ignoreua"
                }, 
                {
                    "ObjURL": "http://pic.feizl.com/upload/allimg/170615/1229514510-0.jpg", 
                    "FromURL": "http://www.feizl.com/html/95576.htm"
                }
            ], 
            "hasThumbData": "0", 
            "bdSetImgNum": 0, 
            "partnerId": 0, 
            "spn": 0, 
            "bdImgnewsDate": "1970-01-01 08:00", 
            "fromPageTitle": "高清<strong>美女</strong>私房写真", 
            "fromPageTitleEnc": "高清美女私房写真", 
            "bdSourceName": "", 
            "bdFromPageTitlePrefix": "", 
            "isAspDianjing": 0, 
            "token": "", 
            "imgType": "", 
            "cs": "3198816309,3761849172", 
            "os": "4045972619,461786879", 
            "simid": "3427426778,352277581", 
            "personalized": "0", 
            "simid_info": null, 
            "face_info": null, 
            "xiangshi_info": null, 
            "adPicId": "0", 
            "source_type": ""
        }, 
        {
            "adType": "0", 
            "hasAspData": "0", 
            "thumbURL": "https://ss3.bdstatic.com/70cFv8Sh_Q1YnxGkpoWK1HF6hhy/it/u=3742877786,1804650041&fm=27&gp=0.jpg", 
            "middleURL": "https://ss3.bdstatic.com/70cFv8Sh_Q1YnxGkpoWK1HF6hhy/it/u=3742877786,1804650041&fm=27&gp=0.jpg", 
            "largeTnImageUrl": "", 
            "hasLarge": 0, 
            "hoverURL": "https://ss3.bdstatic.com/70cFv8Sh_Q1YnxGkpoWK1HF6hhy/it/u=3742877786,1804650041&fm=27&gp=0.jpg", 
            "pageNum": 46, 
            "objURL": "ippr_z2C$qAzdH3FAzdH3Ft42_z&e3Bzv55s_z&e3BvgAzdH3Fv5447gtpyAzdH3Fa8d8kkcmablca1ndub0cw8ndm88vj0_z&e3B3r2@8dbao_8s_d5_8aafi_z&e3B3r2", 
            "fromURL": "ippr_z2C$qAzdH3FAzdH3Fooo_z&e3Bzv55s_z&e3Bv54_z&e3BvgAzdH3Fo56hAzdH3FZMTM8MD2oODQ=_z&e3Bip4s", 
            "fromURLHost": "www.zcool.com.cn", 
            "currentIndex": "", 
            "width": 1280, 
            "height": 853, 
            "type": "jpg", 
            "is_gif": 0, 
            "filesize": "", 
            "bdSrcType": "0", 
            "di": "99386396121", 
            "pi": "0", 
            "is": "0,0", 
            "imgCollectionWord": "", 
            "replaceUrl": [
                {
                    "ObjURL": "http://img1.imgtn.bdimg.com/it/u=3742877786,1804650041&fm=214&gp=0.jpg", 
                    "FromURL": "http://dev-yp.yueus.com/mall/user/test/goods/service_detail.php?goods_id=2133709"
                }
            ], 
            "hasThumbData": "0", 
            "bdSetImgNum": 0, 
            "partnerId": 0, 
            "spn": 0, 
            "bdImgnewsDate": "1970-01-01 08:00", 
            "fromPageTitle": "个人<strong>写真</strong>", 
            "fromPageTitleEnc": "个人写真", 
            "bdSourceName": "", 
            "bdFromPageTitlePrefix": "", 
            "isAspDianjing": 0, 
            "token": "", 
            "imgType": "", 
            "cs": "3742877786,1804650041", 
            "os": "1121342431,621354996", 
            "simid": "3494669277,370166653", 
            "personalized": "0", 
            "simid_info": null, 
            "face_info": null, 
            "xiangshi_info": null, 
            "adPicId": "0", 
            "source_type": ""
        }, 
        {
            "adType": "0", 
            "hasAspData": "0", 
            "thumbURL": "https://ss0.bdstatic.com/70cFvHSh_Q1YnxGkpoWK1HF6hhy/it/u=3213274608,2555296276&fm=15&gp=0.jpg", 
            "middleURL": "https://ss0.bdstatic.com/70cFvHSh_Q1YnxGkpoWK1HF6hhy/it/u=3213274608,2555296276&fm=15&gp=0.jpg", 
            "largeTnImageUrl": "", 
            "hasLarge": 0, 
            "hoverURL": "https://ss0.bdstatic.com/70cFvHSh_Q1YnxGkpoWK1HF6hhy/it/u=3213274608,2555296276&fm=15&gp=0.jpg", 
            "pageNum": 47, 
            "objURL": "ippr_z2C$qAzdH3FAzdH3Ft42_z&e3B77p7o_z&e3Bv54AzdH3F77p7oAzdH3Fkt2AzdH3F77AzdH3FacAzdH3FbnAzdH3Faan0_z&e3B3r2", 
            "fromURL": "ippr_z2C$qAzdH3FAzdH3Fooo_z&e3B77p7o_z&e3Bv54AzdH3Ft4w2jetjo8090cuan_z&e3Bip4s", 
            "fromURLHost": "www.uutuw.com", 
            "currentIndex": "", 
            "width": 800, 
            "height": 1280, 
            "type": "jpg", 
            "is_gif": 0, 
            "filesize": "", 
            "bdSrcType": "15", 
            "di": "162834820072", 
            "pi": "0", 
            "is": "0,0", 
            "imgCollectionWord": "", 
            "replaceUrl": [
                {
                    "ObjURL": "http://img3.imgtn.bdimg.com/it/u=3213274608,2555296276&fm=214&gp=0.jpg", 
                    "FromURL": "http://www.tuzhan.com/wallpaper-h-7-4-2.html"
                }, 
                {
                    "ObjURL": "http://t-1.tuzhan.com/31e853dca1e9/c-2/w/2014/02/11/19/0caa4304e1b94def9f990eacdc36627d.jpg", 
                    "FromURL": "http://www.tuzhan.com/wallpaper-h-7-4-2.html"
                }
            ], 
            "hasThumbData": "0", 
            "bdSetImgNum": 0, 
            "partnerId": 0, 
            "spn": 0, 
            "bdImgnewsDate": "1970-01-01 08:00", 
            "fromPageTitle": "<strong>美女</strong>校花清新写真高清安手机壁纸美女校花", 
            "fromPageTitleEnc": "美女校花清新写真高清安手机壁纸美女校花", 
            "bdSourceName": "", 
            "bdFromPageTitlePrefix": "", 
            "isAspDianjing": 0, 
            "token": "", 
            "imgType": "", 
            "cs": "3213274608,2555296276", 
            "os": "1569503764,3692146651", 
            "simid": "3492525933,312203351", 
            "personalized": "0", 
            "simid_info": null, 
            "face_info": null, 
            "xiangshi_info": null, 
            "adPicId": "0", 
            "source_type": ""
        }, 
        {
            "adType": "0", 
            "hasAspData": "0", 
            "thumbURL": "https://ss1.bdstatic.com/70cFvXSh_Q1YnxGkpoWK1HF6hhy/it/u=3135668943,2714399354&fm=15&gp=0.jpg", 
            "middleURL": "https://ss1.bdstatic.com/70cFvXSh_Q1YnxGkpoWK1HF6hhy/it/u=3135668943,2714399354&fm=15&gp=0.jpg", 
            "largeTnImageUrl": "", 
            "hasLarge": 0, 
            "hoverURL": "https://ss1.bdstatic.com/70cFvXSh_Q1YnxGkpoWK1HF6hhy/it/u=3135668943,2714399354&fm=15&gp=0.jpg", 
            "pageNum": 48, 
            "objURL": "ippr_z2C$qAzdH3FAzdH3Ft42_z&e3B77p7o_z&e3Bv54AzdH3F77p7oAzdH3Fkt2AzdH3F77AzdH3Fa0AzdH3Fm9AzdH3Fbacd_z&e3B3r2", 
            "fromURL": "ippr_z2C$qAzdH3FAzdH3Fooo_z&e3B77p7o_z&e3Bv54AzdH3Ft4w2jetjonlbvbjc0_z&e3Bip4s", 
            "fromURLHost": "www.uutuw.com", 
            "currentIndex": "", 
            "width": 800, 
            "height": 1279, 
            "type": "jpg", 
            "is_gif": 0, 
            "filesize": "", 
            "bdSrcType": "15", 
            "di": "246518021762", 
            "pi": "0", 
            "is": "0,0", 
            "imgCollectionWord": "", 
            "replaceUrl": [
                {
                    "ObjURL": "http://img3.imgtn.bdimg.com/it/u=3135668943,2714399354&fm=214&gp=0.jpg", 
                    "FromURL": "http://www.uutuw.com/imageview398c8e57.html"
                }
            ], 
            "hasThumbData": "0", 
            "bdSetImgNum": 0, 
            "partnerId": 0, 
            "spn": 0, 
            "bdImgnewsDate": "1970-01-01 08:00", 
            "fromPageTitle": "阳光<strong>美女</strong>校园可爱写真安卓高清平板壁纸推", 
            "fromPageTitleEnc": "阳光美女校园可爱写真安卓高清平板壁纸推", 
            "bdSourceName": "", 
            "bdFromPageTitlePrefix": "", 
            "isAspDianjing": 0, 
            "token": "", 
            "imgType": "", 
            "cs": "3135668943,2714399354", 
            "os": "1644366314,3711997944", 
            "simid": "0,0", 
            "personalized": "0", 
            "simid_info": null, 
            "face_info": null, 
            "xiangshi_info": null, 
            "adPicId": "0", 
            "source_type": ""
        }, 
        {
            "adType": "0", 
            "hasAspData": "0", 
            "thumbURL": "https://ss0.bdstatic.com/70cFuHSh_Q1YnxGkpoWK1HF6hhy/it/u=985288225,4264113757&fm=15&gp=0.jpg", 
            "middleURL": "https://ss0.bdstatic.com/70cFuHSh_Q1YnxGkpoWK1HF6hhy/it/u=985288225,4264113757&fm=15&gp=0.jpg", 
            "largeTnImageUrl": "", 
            "hasLarge": 0, 
            "hoverURL": "https://ss0.bdstatic.com/70cFuHSh_Q1YnxGkpoWK1HF6hhy/it/u=985288225,4264113757&fm=15&gp=0.jpg", 
            "pageNum": 49, 
            "objURL": "ippr_z2C$qAzdH3FAzdH3Ft42_z&e3B77p7o_z&e3Bv54AzdH3F77p7oAzdH3Fkt2AzdH3F77AzdH3Fa0AzdH3F9aAzdH3F9cl9_z&e3B3r2", 
            "fromURL": "ippr_z2C$qAzdH3FAzdH3Fooo_z&e3B77p7o_z&e3Bv54AzdH3Ft4w2jetjoaudan9kk_z&e3Bip4s", 
            "fromURLHost": "www.uutuw.com", 
            "currentIndex": "", 
            "width": 800, 
            "height": 986, 
            "type": "jpg", 
            "is_gif": 0, 
            "filesize": "", 
            "bdSrcType": "15", 
            "di": "107541511892", 
            "pi": "0", 
            "is": "0,0", 
            "imgCollectionWord": "", 
            "replaceUrl": [
                {
                    "ObjURL": "http://img0.imgtn.bdimg.com/it/u=985288225,4264113757&fm=214&gp=0.jpg", 
                    "FromURL": "http://www.360doc.com/content/11/1206/22/7814681_170239225.shtml"
                }, 
                {
                    "ObjURL": "http://s14.sinaimg.cn/middle/61f00ecchcbae73ae5f5d&690", 
                    "FromURL": "http://blog.sina.com.cn/s/blog_61f00ecc0102dy93.html"
                }
            ], 
            "hasThumbData": "0", 
            "bdSetImgNum": 0, 
            "partnerId": 0, 
            "spn": 0, 
            "bdImgnewsDate": "1970-01-01 08:00", 
            "fromPageTitle": "祖海高清<strong>写真</strong> - uu美女图网 www.uutuw.com", 
            "fromPageTitleEnc": "祖海高清写真 - uu美女图网 www.uutuw.com", 
            "bdSourceName": "", 
            "bdFromPageTitlePrefix": "", 
            "isAspDianjing": 0, 
            "token": "", 
            "imgType": "", 
            "cs": "985288225,4264113757", 
            "os": "1656810679,3706608989", 
            "simid": "3457630518,400792708", 
            "personalized": "0", 
            "simid_info": null, 
            "face_info": null, 
            "xiangshi_info": null, 
            "adPicId": "0", 
            "source_type": ""
        }, 
        {
            "adType": "0", 
            "hasAspData": "0", 
            "thumbURL": "https://ss3.bdstatic.com/70cFv8Sh_Q1YnxGkpoWK1HF6hhy/it/u=2700911463,2355264976&fm=27&gp=0.jpg", 
            "middleURL": "https://ss3.bdstatic.com/70cFv8Sh_Q1YnxGkpoWK1HF6hhy/it/u=2700911463,2355264976&fm=27&gp=0.jpg", 
            "largeTnImageUrl": "", 
            "hasLarge": 0, 
            "hoverURL": "https://ss3.bdstatic.com/70cFv8Sh_Q1YnxGkpoWK1HF6hhy/it/u=2700911463,2355264976&fm=27&gp=0.jpg", 
            "pageNum": 50, 
            "objURL": "ippr_z2C$qAzdH3FAzdH3F6jf_z&e3Bhwtu7_z&e3Bv54AzdH3FtfyAzdH3F7rs5w1AzdH3F7j1tp56AzdH3Ft4w2jAzdH3Fda89an80AzdH3F3vnhm7boyd6ben1s_z&e3B3r2", 
            "fromURL": "ippr_z2C$qAzdH3FAzdH3Fooo_z&e3Bhwtu7_z&e3Bv54AzdH3Fw6ptvsjv5gpjgp-bnam9-a_z&e3Bip4s", 
            "fromURLHost": "www.kaifu.com", 
            "currentIndex": "", 
            "width": 500, 
            "height": 712, 
            "type": "jpg", 
            "is_gif": 0, 
            "filesize": "", 
            "bdSrcType": "0", 
            "di": "278117511", 
            "pi": "0", 
            "is": "0,0", 
            "imgCollectionWord": "", 
            "replaceUrl": [
                {
                    "ObjURL": "http://img3.imgtn.bdimg.com/it/u=2700911463,2355264976&fm=214&gp=0.jpg", 
                    "FromURL": "http://news.hexun.com/2013-07-25/156490335.html"
                }, 
                {
                    "ObjURL": "http://img1.d.cn/be/image/1403/5t2d5hsv447v3.jpg", 
                    "FromURL": "http://www.d.cn/news/321177.html"
                }
            ], 
            "hasThumbData": "0", 
            "bdSetImgNum": 0, 
            "partnerId": 0, 
            "spn": 0, 
            "bdImgnewsDate": "1970-01-01 08:00", 
            "fromPageTitle": "《我叫mt》广告拍摄<strong>美女</strong>主角写真", 
            "fromPageTitleEnc": "《我叫mt》广告拍摄美女主角写真", 
            "bdSourceName": "", 
            "bdFromPageTitlePrefix": "", 
            "isAspDianjing": 0, 
            "token": "", 
            "imgType": "", 
            "cs": "2700911463,2355264976", 
            "os": "660743670,82392450", 
            "simid": "3157302318,3830877133", 
            "personalized": "0", 
            "simid_info": null, 
            "face_info": null, 
            "xiangshi_info": null, 
            "adPicId": "0", 
            "source_type": ""
        }, 
        {
            "adType": "0", 
            "hasAspData": "0", 
            "thumbURL": "https://ss0.bdstatic.com/70cFvHSh_Q1YnxGkpoWK1HF6hhy/it/u=1422508525,4263818637&fm=15&gp=0.jpg", 
            "middleURL": "https://ss0.bdstatic.com/70cFvHSh_Q1YnxGkpoWK1HF6hhy/it/u=1422508525,4263818637&fm=15&gp=0.jpg", 
            "largeTnImageUrl": "", 
            "hasLarge": 0, 
            "hoverURL": "https://ss0.bdstatic.com/70cFvHSh_Q1YnxGkpoWK1HF6hhy/it/u=1422508525,4263818637&fm=15&gp=0.jpg", 
            "pageNum": 51, 
            "objURL": "ippr_z2C$qAzdH3FAzdH3Ft42_z&e3B77p7o_z&e3Bv54AzdH3F77p7oAzdH3Fkt2AzdH3F77AzdH3F88AzdH3F9bAzdH3Fbl9d_z&e3B3r2", 
            "fromURL": "ippr_z2C$qAzdH3FAzdH3Fooo_z&e3B77p7o_z&e3Bv54AzdH3Ft4w2jetjouvbmdkwm_z&e3Bip4s", 
            "fromURLHost": "www.uutuw.com", 
            "currentIndex": "", 
            "width": 800, 
            "height": 1198, 
            "type": "jpg", 
            "is_gif": 0, 
            "filesize": "", 
            "bdSrcType": "15", 
            "di": "24520695222", 
            "pi": "0", 
            "is": "0,0", 
            "imgCollectionWord": "", 
            "replaceUrl": [
                {
                    "ObjURL": "http://img0.imgtn.bdimg.com/it/u=1422508525,4263818637&fm=214&gp=0.jpg", 
                    "FromURL": "http://www.tplm123.com/picinfo-839132.html"
                }, 
                {
                    "ObjURL": "http://img2.tplm123.com/2009/03/11/770/low/1693811409054.jpg", 
                    "FromURL": "http://www.tplm123.com/picinfo-839129-0.html"
                }
            ], 
            "hasThumbData": "0", 
            "bdSetImgNum": 0, 
            "partnerId": 0, 
            "spn": 0, 
            "bdImgnewsDate": "1970-01-01 08:00", 
            "fromPageTitle": "青春<strong>美女</strong>个性写真", 
            "fromPageTitleEnc": "青春美女个性写真", 
            "bdSourceName": "", 
            "bdFromPageTitlePrefix": "", 
            "isAspDianjing": 0, 
            "token": "", 
            "imgType": "", 
            "cs": "1422508525,4263818637", 
            "os": "1658971760,3783190660", 
            "simid": "3511079478,256971936", 
            "personalized": "0", 
            "simid_info": null, 
            "face_info": null, 
            "xiangshi_info": null, 
            "adPicId": "0", 
            "source_type": ""
        }, 
        {
            "adType": "0", 
            "hasAspData": "0", 
            "thumbURL": "https://ss0.bdstatic.com/70cFuHSh_Q1YnxGkpoWK1HF6hhy/it/u=2828664989,3886589235&fm=15&gp=0.jpg", 
            "middleURL": "https://ss0.bdstatic.com/70cFuHSh_Q1YnxGkpoWK1HF6hhy/it/u=2828664989,3886589235&fm=15&gp=0.jpg", 
            "largeTnImageUrl": "", 
            "hasLarge": 0, 
            "hoverURL": "https://ss0.bdstatic.com/70cFuHSh_Q1YnxGkpoWK1HF6hhy/it/u=2828664989,3886589235&fm=15&gp=0.jpg", 
            "pageNum": 52, 
            "objURL": "ippr_z2C$qAzdH3FAzdH3Ft42_z&e3B77p7o_z&e3Bv54AzdH3F77p7oAzdH3Fkt2AzdH3F77p7o8aAzdH3FaaAzdH3F8aAzdH3FnnAzdH3Fda8_z&e3B3r2", 
            "fromURL": "ippr_z2C$qAzdH3FAzdH3Fooo_z&e3B77p7o_z&e3Bv54AzdH3Ft4w2jetjow8bddvk0_z&e3Bip4s", 
            "fromURLHost": "www.uutuw.com", 
            "currentIndex": "", 
            "width": 800, 
            "height": 1200, 
            "type": "jpg", 
            "is_gif": 0, 
            "filesize": "", 
            "bdSrcType": "15", 
            "di": "264721209952", 
            "pi": "0", 
            "is": "0,0", 
            "imgCollectionWord": "", 
            "replaceUrl": [
                {
                    "ObjURL": "http://img4.imgtn.bdimg.com/it/u=2828664989,3886589235&fm=214&gp=0.jpg", 
                    "FromURL": "http://miniimg.eastday.com/detail/170411134914853789974.html"
                }, 
                {
                    "ObjURL": "http://00.minipic.eastday.com/20170307/20170307175622_62ed696abeea995b9088555b074dcfea_6.jpeg", 
                    "FromURL": "http://miniimg.eastday.com/detail/170307175622402045408.html"
                }
            ], 
            "hasThumbData": "0", 
            "bdSetImgNum": 0, 
            "partnerId": 0, 
            "spn": 0, 
            "bdImgnewsDate": "1970-01-01 08:00", 
            "fromPageTitle": "清纯可爱吊带牛仔裤<strong>写真</strong> - uu美女图网 www.uutuw.com", 
            "fromPageTitleEnc": "清纯可爱吊带牛仔裤写真 - uu美女图网 www.uutuw.com", 
            "bdSourceName": "", 
            "bdFromPageTitlePrefix": "", 
            "isAspDianjing": 0, 
            "token": "", 
            "imgType": "", 
            "cs": "2828664989,3886589235", 
            "os": "1793815181,2297832568", 
            "simid": "3540487057,322569098", 
            "personalized": "0", 
            "simid_info": null, 
            "face_info": null, 
            "xiangshi_info": null, 
            "adPicId": "0", 
            "source_type": ""
        }, 
        {
            "adType": "0", 
            "hasAspData": "0", 
            "thumbURL": "https://ss2.bdstatic.com/70cFvnSh_Q1YnxGkpoWK1HF6hhy/it/u=1806997913,3133657909&fm=15&gp=0.jpg", 
            "middleURL": "https://ss2.bdstatic.com/70cFvnSh_Q1YnxGkpoWK1HF6hhy/it/u=1806997913,3133657909&fm=15&gp=0.jpg", 
            "largeTnImageUrl": "", 
            "hasLarge": 0, 
            "hoverURL": "https://ss2.bdstatic.com/70cFvnSh_Q1YnxGkpoWK1HF6hhy/it/u=1806997913,3133657909&fm=15&gp=0.jpg", 
            "pageNum": 53, 
            "objURL": "ippr_z2C$qAzdH3FAzdH3Ft42_z&e3B77p7o_z&e3Bv54AzdH3F77p7oAzdH3Fkt2AzdH3F8AzdH3FcbAzdH3Fc0daAzdH3Fc08l8b_z&e3B3r2", 
            "fromURL": "ippr_z2C$qAzdH3FAzdH3Fooo_z&e3B77p7o_z&e3Bv54AzdH3Ft4w2jetjolnw1u8jv_z&e3Bip4s", 
            "fromURLHost": "www.uutuw.com", 
            "currentIndex": "", 
            "width": 800, 
            "height": 1204, 
            "type": "jpg", 
            "is_gif": 0, 
            "filesize": "", 
            "bdSrcType": "15", 
            "di": "14768950362", 
            "pi": "0", 
            "is": "0,0", 
            "imgCollectionWord": "", 
            "replaceUrl": [
                {
                    "ObjURL": "http://img3.imgtn.bdimg.com/it/u=1806997913,3133657909&fm=214&gp=0.jpg", 
                    "FromURL": "http://www.uutuw.com/imageview93adf1ec.html"
                }
            ], 
            "hasThumbData": "0", 
            "bdSetImgNum": 0, 
            "partnerId": 0, 
            "spn": 0, 
            "bdImgnewsDate": "1970-01-01 08:00", 
            "fromPageTitle": "<strong>写真</strong> 美女 图片-明星图片网", 
            "fromPageTitleEnc": "写真 美女 图片-明星图片网", 
            "bdSourceName": "", 
            "bdFromPageTitlePrefix": "", 
            "isAspDianjing": 0, 
            "token": "", 
            "imgType": "", 
            "cs": "1806997913,3133657909", 
            "os": "2573545622,2339371318", 
            "simid": "0,0", 
            "personalized": "0", 
            "simid_info": null, 
            "face_info": null, 
            "xiangshi_info": null, 
            "adPicId": "0", 
            "source_type": ""
        }, 
        {
            "adType": "0", 
            "hasAspData": "0", 
            "thumbURL": "https://ss2.bdstatic.com/70cFvnSh_Q1YnxGkpoWK1HF6hhy/it/u=1274513138,2526641544&fm=15&gp=0.jpg", 
            "middleURL": "https://ss2.bdstatic.com/70cFvnSh_Q1YnxGkpoWK1HF6hhy/it/u=1274513138,2526641544&fm=15&gp=0.jpg", 
            "largeTnImageUrl": "", 
            "hasLarge": 0, 
            "hoverURL": "https://ss2.bdstatic.com/70cFvnSh_Q1YnxGkpoWK1HF6hhy/it/u=1274513138,2526641544&fm=15&gp=0.jpg", 
            "pageNum": 54, 
            "objURL": "ippr_z2C$qAzdH3FAzdH3Ft42_z&e3B77p7o_z&e3Bv54AzdH3F77p7oAzdH3Fkt2AzdH3F77AzdH3F88AzdH3FcaAzdH3F0l00_z&e3B3r2", 
            "fromURL": "ippr_z2C$qAzdH3FAzdH3Fooo_z&e3B77p7o_z&e3Bv54AzdH3Ft4w2jetjoa9w1cjad_z&e3Bip4s", 
            "fromURLHost": "www.uutuw.com", 
            "currentIndex": "", 
            "width": 800, 
            "height": 1201, 
            "type": "jpg", 
            "is_gif": 0, 
            "filesize": "", 
            "bdSrcType": "15", 
            "di": "357778665102", 
            "pi": "0", 
            "is": "0,0", 
            "imgCollectionWord": "", 
            "replaceUrl": [
                {
                    "ObjURL": "http://img3.imgtn.bdimg.com/it/u=1274513138,2526641544&fm=214&gp=0.jpg", 
                    "FromURL": "http://www.sohu.com/a/59976273_129080"
                }, 
                {
                    "ObjURL": "http://img.mp.itc.cn/upload/20161215/1f976a3883bb477ea51c6d459b160a64_th.jpeg", 
                    "FromURL": "http://www.sohu.com/a/121680632_390656"
                }
            ], 
            "hasThumbData": "0", 
            "bdSetImgNum": 0, 
            "partnerId": 0, 
            "spn": 0, 
            "bdImgnewsDate": "1970-01-01 08:00", 
            "fromPageTitle": "刘雨欣曝初春粉嫩<strong>写真</strong>锁骨性感迷人 - uu美女图网 www", 
            "fromPageTitleEnc": "刘雨欣曝初春粉嫩写真锁骨性感迷人 - uu美女图网 www", 
            "bdSourceName": "", 
            "bdFromPageTitlePrefix": "", 
            "isAspDianjing": 0, 
            "token": "", 
            "imgType": "", 
            "cs": "1274513138,2526641544", 
            "os": "1737542509,3801418313", 
            "simid": "4206426847,577530350", 
            "personalized": "0", 
            "simid_info": null, 
            "face_info": null, 
            "xiangshi_info": null, 
            "adPicId": "0", 
            "source_type": ""
        }, 
        {
            "adType": "0", 
            "hasAspData": "0", 
            "thumbURL": "https://ss3.bdstatic.com/70cFv8Sh_Q1YnxGkpoWK1HF6hhy/it/u=897150686,3534570125&fm=15&gp=0.jpg", 
            "middleURL": "https://ss3.bdstatic.com/70cFv8Sh_Q1YnxGkpoWK1HF6hhy/it/u=897150686,3534570125&fm=15&gp=0.jpg", 
            "largeTnImageUrl": "", 
            "hasLarge": 0, 
            "hoverURL": "https://ss3.bdstatic.com/70cFv8Sh_Q1YnxGkpoWK1HF6hhy/it/u=897150686,3534570125&fm=15&gp=0.jpg", 
            "pageNum": 55, 
            "objURL": "ippr_z2C$qAzdH3FAzdH3Ft42_z&e3B77p7o_z&e3Bv54AzdH3F77p7oAzdH3Fkt2AzdH3F77AzdH3F88AzdH3Fb8AzdH3Flmm9_z&e3B3r2", 
            "fromURL": "ippr_z2C$qAzdH3FAzdH3Fooo_z&e3B77p7o_z&e3Bv54AzdH3Ft4w2jetjow0kwlnn1_z&e3Bip4s", 
            "fromURLHost": "www.uutuw.com", 
            "currentIndex": "", 
            "width": 800, 
            "height": 950, 
            "type": "jpg", 
            "is_gif": 0, 
            "filesize": "", 
            "bdSrcType": "15", 
            "di": "302838445152", 
            "pi": "0", 
            "is": "0,0", 
            "imgCollectionWord": "", 
            "replaceUrl": [
                {
                    "ObjURL": "http://img1.imgtn.bdimg.com/it/u=897150686,3534570125&fm=214&gp=0.jpg", 
                    "FromURL": "http://m.yoka.com/information_1697900.html"
                }, 
                {
                    "ObjURL": "http://p1.yokacdn.com/pic/test/astrology/2012/u242p1t117d707123f2577dt20120929095949.jpg", 
                    "FromURL": "http://m.yoka.com/information_1697906.html"
                }
            ], 
            "hasThumbData": "0", 
            "bdSetImgNum": 0, 
            "partnerId": 0, 
            "spn": 0, 
            "bdImgnewsDate": "1970-01-01 08:00", 
            "fromPageTitle": "为艺术大尺度献身<strong>美女</strong>模特写真3星座指南", 
            "fromPageTitleEnc": "为艺术大尺度献身美女模特写真3星座指南", 
            "bdSourceName": "", 
            "bdFromPageTitlePrefix": "", 
            "isAspDianjing": 0, 
            "token": "", 
            "imgType": "", 
            "cs": "897150686,3534570125", 
            "os": "1759444239,3802265896", 
            "simid": "4220863272,598589665", 
            "personalized": "0", 
            "simid_info": null, 
            "face_info": null, 
            "xiangshi_info": null, 
            "adPicId": "0", 
            "source_type": ""
        }, 
        {
            "adType": "0", 
            "hasAspData": "0", 
            "thumbURL": "https://ss1.bdstatic.com/70cFvXSh_Q1YnxGkpoWK1HF6hhy/it/u=1965616112,1270744157&fm=15&gp=0.jpg", 
            "middleURL": "https://ss1.bdstatic.com/70cFvXSh_Q1YnxGkpoWK1HF6hhy/it/u=1965616112,1270744157&fm=15&gp=0.jpg", 
            "largeTnImageUrl": "", 
            "hasLarge": 0, 
            "hoverURL": "https://ss1.bdstatic.com/70cFvXSh_Q1YnxGkpoWK1HF6hhy/it/u=1965616112,1270744157&fm=15&gp=0.jpg", 
            "pageNum": 56, 
            "objURL": "ippr_z2C$qAzdH3FAzdH3Ft42_z&e3B77p7o_z&e3Bv54AzdH3F77p7oAzdH3Fkt2AzdH3F77p7o8aAzdH3FaaAzdH3F8nAzdH3F9dAzdH3Fcn8_z&e3B3r2", 
            "fromURL": "ippr_z2C$qAzdH3FAzdH3Fooo_z&e3B77p7o_z&e3Bv54AzdH3Ft4w2jetjonw91vlka_z&e3Bip4s", 
            "fromURLHost": "www.uutuw.com", 
            "currentIndex": "", 
            "width": 800, 
            "height": 1116, 
            "type": "jpg", 
            "is_gif": 0, 
            "filesize": "", 
            "bdSrcType": "15", 
            "di": "237773711132", 
            "pi": "0", 
            "is": "0,0", 
            "imgCollectionWord": "", 
            "replaceUrl": [
                {
                    "ObjURL": "http://img2.imgtn.bdimg.com/it/u=1965616112,1270744157&fm=214&gp=0.jpg", 
                    "FromURL": "http://www.sohu.com/a/77542516_419510"
                }, 
                {
                    "ObjURL": "http://n1.itc.cn/img8/wb/recom/2016/05/26/146426970575620045.jpeg", 
                    "FromURL": "http://www.sohu.com/a/77542516_419510"
                }
            ], 
            "hasThumbData": "0", 
            "bdSetImgNum": 0, 
            "partnerId": 0, 
            "spn": 0, 
            "bdImgnewsDate": "1970-01-01 08:00", 
            "fromPageTitle": "打扮的<strong>美女</strong>宾馆写真", 
            "fromPageTitleEnc": "打扮的美女宾馆写真", 
            "bdSourceName": "", 
            "bdFromPageTitlePrefix": "", 
            "isAspDianjing": 0, 
            "token": "", 
            "imgType": "", 
            "cs": "1965616112,1270744157", 
            "os": "1883099575,2380264798", 
            "simid": "4183227795,499634634", 
            "personalized": "0", 
            "simid_info": null, 
            "face_info": null, 
            "xiangshi_info": null, 
            "adPicId": "0", 
            "source_type": ""
        }, 
        {
            "adType": "0", 
            "hasAspData": "0", 
            "thumbURL": "https://ss3.bdstatic.com/70cFv8Sh_Q1YnxGkpoWK1HF6hhy/it/u=664831409,972048586&fm=15&gp=0.jpg", 
            "middleURL": "https://ss3.bdstatic.com/70cFv8Sh_Q1YnxGkpoWK1HF6hhy/it/u=664831409,972048586&fm=15&gp=0.jpg", 
            "largeTnImageUrl": "", 
            "hasLarge": 0, 
            "hoverURL": "https://ss3.bdstatic.com/70cFv8Sh_Q1YnxGkpoWK1HF6hhy/it/u=664831409,972048586&fm=15&gp=0.jpg", 
            "pageNum": 57, 
            "objURL": "ippr_z2C$qAzdH3FAzdH3Ft42_z&e3B77p7o_z&e3Bv54AzdH3F77p7oAzdH3Fkt2AzdH3F8AzdH3FdmAzdH3Fdc0lAzdH3Fdc0ba8_z&e3B3r2", 
            "fromURL": "ippr_z2C$qAzdH3FAzdH3Fooo_z&e3B77p7o_z&e3Bv54AzdH3Ft4w2jetjob9v8cj90_z&e3Bip4s", 
            "fromURLHost": "www.uutuw.com", 
            "currentIndex": "", 
            "width": 800, 
            "height": 1200, 
            "type": "jpg", 
            "is_gif": 0, 
            "filesize": "", 
            "bdSrcType": "15", 
            "di": "375553267772", 
            "pi": "0", 
            "is": "0,0", 
            "imgCollectionWord": "", 
            "replaceUrl": [
                {
                    "ObjURL": "http://img4.imgtn.bdimg.com/it/u=664831409,972048586&fm=214&gp=0.jpg", 
                    "FromURL": "http://www.uutuw.com/imageview84c15e47.html"
                }
            ], 
            "hasThumbData": "0", 
            "bdSetImgNum": 0, 
            "partnerId": 0, 
            "spn": 0, 
            "bdImgnewsDate": "1970-01-01 08:00", 
            "fromPageTitle": "长腿小<strong>美女</strong> 最新性感 私房写真", 
            "fromPageTitleEnc": "长腿小美女 最新性感 私房写真", 
            "bdSourceName": "", 
            "bdFromPageTitlePrefix": "", 
            "isAspDianjing": 0, 
            "token": "", 
            "imgType": "", 
            "cs": "664831409,972048586", 
            "os": "2445681095,2289429728", 
            "simid": "0,0", 
            "personalized": "0", 
            "simid_info": null, 
            "face_info": null, 
            "xiangshi_info": null, 
            "adPicId": "0", 
            "source_type": ""
        }, 
        {
            "adType": "0", 
            "hasAspData": "0", 
            "thumbURL": "https://ss2.bdstatic.com/70cFvnSh_Q1YnxGkpoWK1HF6hhy/it/u=1142654161,1693444081&fm=15&gp=0.jpg", 
            "middleURL": "https://ss2.bdstatic.com/70cFvnSh_Q1YnxGkpoWK1HF6hhy/it/u=1142654161,1693444081&fm=15&gp=0.jpg", 
            "largeTnImageUrl": "", 
            "hasLarge": 0, 
            "hoverURL": "https://ss2.bdstatic.com/70cFvnSh_Q1YnxGkpoWK1HF6hhy/it/u=1142654161,1693444081&fm=15&gp=0.jpg", 
            "pageNum": 58, 
            "objURL": "ippr_z2C$qAzdH3FAzdH3Ff8n_z&e3Bftgwt42_z&e3BvgAzdH3Fk4t11sjAzdH3F0j9bll8lpw1ucajvj0luv&mla", 
            "fromURL": "ippr_z2C$qAzdH3FAzdH3Fks52_z&e3Bftgw_z&e3BvgAzdH3F1r55sAzdH3Fks52AzdH3FfAzdH3Fks52_m1a8lcwaa8aazp5e_z&e3Bip4s", 
            "fromURLHost": "blog.sina.cn", 
            "currentIndex": "", 
            "width": 490, 
            "height": 561, 
            "type": "jpg", 
            "is_gif": 0, 
            "filesize": "", 
            "bdSrcType": "15", 
            "di": "17652485082", 
            "pi": "0", 
            "is": "0,0", 
            "imgCollectionWord": "", 
            "replaceUrl": [
                {
                    "ObjURL": "http://img1.imgtn.bdimg.com/it/u=1142654161,1693444081&fm=214&gp=0.jpg", 
                    "FromURL": "http://tupian.baike.com/11854/9.html?prd=zutu_thumbs"
                }, 
                {
                    "ObjURL": "http://image69.360doc.com/downloadimg/2014/03/0607/39654750_1.jpg", 
                    "FromURL": "http://www.360doc.com/content/14/0615/20/18005671_386874583.shtml"
                }
            ], 
            "hasThumbData": "0", 
            "bdSetImgNum": 0, 
            "partnerId": 0, 
            "spn": 0, 
            "bdImgnewsDate": "1970-01-01 08:00", 
            "fromPageTitle": "[转载]<strong>美女</strong>倪歆柔写真性感版", 
            "fromPageTitleEnc": "[转载]美女倪歆柔写真性感版", 
            "bdSourceName": "", 
            "bdFromPageTitlePrefix": "", 
            "isAspDianjing": 0, 
            "token": "", 
            "imgType": "", 
            "cs": "1142654161,1693444081", 
            "os": "3436409816,2755898209", 
            "simid": "3373841178,292783644", 
            "personalized": "0", 
            "simid_info": null, 
            "face_info": null, 
            "xiangshi_info": null, 
            "adPicId": "0", 
            "source_type": ""
        }, 
        {
            "adType": "0", 
            "hasAspData": "0", 
            "thumbURL": "https://ss3.bdstatic.com/70cFv8Sh_Q1YnxGkpoWK1HF6hhy/it/u=2362770036,4004148829&fm=15&gp=0.jpg", 
            "middleURL": "https://ss3.bdstatic.com/70cFv8Sh_Q1YnxGkpoWK1HF6hhy/it/u=2362770036,4004148829&fm=15&gp=0.jpg", 
            "largeTnImageUrl": "", 
            "hasLarge": 0, 
            "hoverURL": "https://ss3.bdstatic.com/70cFv8Sh_Q1YnxGkpoWK1HF6hhy/it/u=2362770036,4004148829&fm=15&gp=0.jpg", 
            "pageNum": 59, 
            "objURL": "ippr_z2C$qAzdH3FAzdH3Ft42_z&e3B77p7o_z&e3Bv54AzdH3F77p7oAzdH3Fkt2AzdH3F77AzdH3F8dAzdH3F8bAzdH3Flca0_z&e3B3r2", 
            "fromURL": "ippr_z2C$qAzdH3FAzdH3Fooo_z&e3B77p7o_z&e3Bv54AzdH3Ft4w2jetjobajmdmj0_z&e3Bip4s", 
            "fromURLHost": "www.uutuw.com", 
            "currentIndex": "", 
            "width": 800, 
            "height": 1200, 
            "type": "jpg", 
            "is_gif": 0, 
            "filesize": "", 
            "bdSrcType": "15", 
            "di": "385921544922", 
            "pi": "0", 
            "is": "0,0", 
            "imgCollectionWord": "", 
            "replaceUrl": [
                {
                    "ObjURL": "http://img1.imgtn.bdimg.com/it/u=2362770036,4004148829&fm=214&gp=0.jpg", 
                    "FromURL": "http://www.uutuw.com/imageview80e626e7.html"
                }
            ], 
            "hasThumbData": "0", 
            "bdSetImgNum": 0, 
            "partnerId": 0, 
            "spn": 0, 
            "bdImgnewsDate": "1970-01-01 08:00", 
            "fromPageTitle": "韩国甜美型的<strong>美女</strong>车模高贵写真图137160美", 
            "fromPageTitleEnc": "韩国甜美型的美女车模高贵写真图137160美", 
            "bdSourceName": "", 
            "bdFromPageTitlePrefix": "", 
            "isAspDianjing": 0, 
            "token": "", 
            "imgType": "", 
            "cs": "2362770036,4004148829", 
            "os": "1580823512,3750482786", 
            "simid": "0,0", 
            "personalized": "0", 
            "simid_info": null, 
            "face_info": null, 
            "xiangshi_info": null, 
            "adPicId": "0", 
            "source_type": ""
        }, 
        { }
    ]
}S
    '''


'''
  # imglist = soup.select(".image > a > img")
    # imglist = soup.select(".image")

    # css_soup.find_all("div", class_="body")  # 多值class,指定其中一个即可
    # css_soup.find_all("a", class_="strikeout")
    # css_soup.find_all("p", class_="body strikeout")  # 精确匹配
    # # text 参数可以是字符串，列表、方法、True
    # soup.find_all("a", text="Elsie")  # text="Elsie"的a标签

    #    print(soup.p.string)
    #
    #     print(type(soup.name))
    #     # <type 'unicode'>
    #     print(soup.name)
    #     # [document]
    #     print(soup.attrs)

    # {} 空字典
    #
    # #直接子节点
    #     print(soup.head.contents)#list
    #     print(soup.head.children)# list 生成器对象
    #
    # #所有子孙节点
    #     for child in soup.descendants:
    #         print(child)
    #
    # #节点内容
    #     print(soup.head.string)
    #
    # #父节点
    #     p = soup.p
    #     print(p.parent.name)

    # for link in soup.find_all('a'):
    #     print(link.get('href'))
    # print(soup.get_text())
    #
    # print(soup.find_all(attrs={'id': 'list-1'}))
    # print(soup.find_all(text='Foo'))
    #
    # print(soup.select('#list-2 .element'))
    # print(type(soup.select('ul')[0]))

    
    # find_all( name , attrs , recursive , text , **kwargs )
#  name :要查找的标签名（字符串、正则、方法、True）
#  attrs: 标签的属性
#  recursive: 递归
#  text: 查找文本
# **kwargs :其它 键值参数
soup = BeautifulSoup(html_doc, 'html.parser')
print(soup.find_all('p', 'title')) # p标签且class="title"
soup.find_all('title')  # 以列表形式返回 所有title标签a
soup.find_all(attrs={"class":"sister"})  # 以列表形式返回 所有class属性==sister的标签
soup.find_all(id='link2')  # 返回所有id属性==link2的标签
soup.find_all(href=re.compile("elsie")) # 返回所有href属性包含elsie的标签
soup.find_all(id=True)  # 返回 所有包含id属性的标签
soup.find_all(id="link1", href=re.compile('elsie'))  #  id=link1且href包含elsie


    # 类名前加点，id名前加 #

    # img_ = soup.find_all(name='img')
    # for each in img_:
    #     print(each.get('src'))
    # print('end')
    #
    # href_ = soup.find_all(name='a')
    # for each in href_:
    #     if str(each.get('href'))[:4] == 'http':
    #         print(each.get('href'))
    # print('end')

    # for item in imglist:
    #     img_url =item.attrs['src']
    #     name=img_url.split('/')[-1]
    #     filename = PWD + name + ".jpg"
    #     if os.path.isfile(filename):
    #         print("文件存在：", filename)
    #         if IsValidImage(filename):
    #             continue
    #         else:
    #             os.remove(filename)
    #
    #     print("开始下载第{0}张图片".format(PhotoNum))
    #     PhotoNum += 1
    #     #使用多线程下载
    #     t1 = Thread(target=downfile, args=(filename, img_url))
    #     threading.BoundedSemaphore(5)  # 同一时刻可以有四个线程
    #     t1.start()
    #     #使用多进程下载
    #     # t = multiprocessing.Process(target=downfile, args=(filename, img_url))
    #     # t.daemon = False  # 将daemon设置为True，则主线程不等待子进程，主线程结束则所有结束
    #     # t.start()

'''



