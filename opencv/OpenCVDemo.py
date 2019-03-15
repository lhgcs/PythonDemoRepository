# -*- coding:utf-8 -*-

import os
import threading
import cv2
import numpy as np

'''
pip install opencv-python  opencv视觉库
'''

class Producer(threading.Thread):
    """线程播放视频"""
    def __init__(self, str_rtsp):
        super(Producer, self).__init__()
        self.setDaemon(True)  # 主线程结束，子线程也结束
        #join()：join函数会使得主调线程阻塞，直到被调用线程运行结束或超时，才执行主线程。
        self.str_rtsp = str_rtsp
        self.play = True
        # 通过cv2中的类获取视频流操作对象cap
        self.cap = cv2.VideoCapture(self.str_rtsp)
        # 调用cv2方法获取cap的视频帧（帧：每秒多少张图片）
        fps = self.cap.get(cv2.CAP_PROP_FPS)
        print(fps)
        # 获取cap视频流的每帧大小
        size = (int(self.cap.get(cv2.CAP_PROP_FRAME_WIDTH)),int(self.cap.get(cv2.CAP_PROP_FRAME_HEIGHT)))
        print(size)
    def run(self):
        i = 0
        # 循环显示每一帧
        while self.cap.isOpened():#成功打开
            ret, image = self.cap.read()
            if (ret == True and image is not None):
                cv2.imshow('video windows', image)
                name = "%d.png" %i
                i +=1
                cv2.imwrite(name, image)#保存图片
            else:
                break
            cv2.waitKey(1)#延时1ms
            #按下q退出
            if cv2.waitKey(1) & 0xFF == ord('q'):
                break
        self.cap.release()#释放已打开的视频
        cv2.destroyAllWindows()

class SaverVideo(threading.Thread):
    """图片合成视频"""
    def __init__(self):
        super(SaverVideo, self).__init__()
        self.setDaemon(False) #主线程等待子线程结束
        fps = 25
        size = (672,378)#图片的大小（一定要对否则合成视频失败）
        # 定义编码格式
        # fourcc = cv2.VideoWriter_fourcc('I', '4', '2', '0')
        fourcc = cv2.VideoWriter_fourcc('M', 'P', '4', '2')
        # 定义视频文件输入对象
        self.outVideo = cv2.VideoWriter('test.mp4', fourcc, fps, size)
    def run(self):
        #遍历文件夹获取图片列表
        path = './'
        list = os.listdir(path)
        print(list)
        list2 = []
        for image in list:
            if image[-3:] == 'png':
                list2.append(int(image[:-4]))
        list2.sort()#排序
        print(list2)
        if self.outVideo.isOpened() == False or not list:
            return
        for i in list2:
            name = '%s%d.png' %(path,i)
            print(name)
            image = cv2.imread(name)
            self.outVideo.write(image)#图片写入视频
            #按下q退出
            if cv2.waitKey(1) & 0xFF == ord('q'):
                break
        self.outVideo.release()
        cv2.destroyAllWindows()

#进度条回调
def on_track_callback(value,*list):
    #print(value)#当前位置
    v = cv2.getTrackbarPos('t','w')
    print(v)

def test():
    img = cv2.imread("./1.jpg",-1)#-1：原图的通道，0：单通道，1：3通道，2：原图是16/32位深度的图像则返回对应深度的图像，否则转换为8位
    print(type(img))#numpy.ndarray
    if img.size > 0:
        cv2.imshow('src', img)
        pass
    else:
        print("null")

    dstImage = np.array(0)
    element = np.ones((5, 5), np.uint8)                          # 结构元素，值越大，膨胀或腐蚀的区域越大   值要大于1
    element = cv2.getStructuringElement(cv2.MORPH_RECT, (5, 5))  # 生成矩形的的结果元素

    #进行腐蚀操作
    dstImage = cv2.erode(img, element)                  # 腐蚀，取局部最小值
    cv2.dilate(dstImage,element)                        # 膨胀，取局部最大值
    cv2.morphologyEx(img, cv2.MORPH_OPEN    , element)  # 开运算：先腐蚀再膨胀，分类物体，消除小区域   --》 目标物体外面有无关的小东西
    cv2.morphologyEx(img, cv2.MORPH_CLOSE   , element)  # 闭运算：消除物体的小黑洞   ---》   目标物体内部有很多小黑洞
    cv2.morphologyEx(img, cv2.MORPH_GRADIENT, element)  # 形态学梯度：膨胀图减去腐蚀图   --》物体轮廓
    cv2.morphologyEx(img, cv2.MORPH_TOPHAT  , element)  # 顶帽：原图减去开运算 --》消除轮廓，几乎都是黑色
    cv2.morphologyEx(img, cv2.MORPH_BLACKHAT, element)  # 黑帽：原图减去闭运算 --》轮廓白色，其它黑色
    print(type(dstImage))
    cv2.imshow("腐蚀操作", dstImage)

    # 进行均值滤波操作  ---》  去除噪声，模糊
    dstImage = cv2.boxFilter(img, -1,(7, 7))#原图的深度
    cv2.imshow("均值滤波" ,dstImage)

    #进行均值滤波操作  ---》  去除噪声，模糊
    dstImage = cv2.blur(img,(7,7))
    cv2.imshow("均值滤波" ,dstImage)

    #将原图像转换为灰度图像   ---》  减少计算
    grayImage = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY )#转换颜色空间
    cv2.imshow(r"灰度图", grayImage)
    #先用使用 3x3内核来降噪
    edge = cv2.blur(grayImage, (3,3) )
    #运行Canny算子  ---》 边缘检测
    edge = cv2.Canny( edge, 3, 9,3 )

    start = cv2.getTickCount()#电脑启动以来的时钟周期数
    cv2.imshow("Canny边缘检测", edge)
    end = cv2.getTickCount()
    print((end - start) / cv2.getTickFrequency())#getTickFrequency电脑的主频   单位秒  使用了0.4秒

    import time
    start = time.clock()
    # 创建纯色
    imgg = np.zeros(500 * 1000 * 4, np.uint8).reshape(500, 1000, 4)
    for i in range(imgg.shape[0]):
        for j in range(imgg.shape[1]):
            imgg[i][j][0] = 0  # B
            imgg[i][j][1] = 0  # G
            imgg[i][j][2] = 255  # R
            imgg[i][j][3] = 0
    end = time.clock()
    print(end - start)   # ----》 创建数组用了2.8秒，Q^Q
    start = end
    cv2.line(imgg, (0, 0), (100, 100), (0, 255, 0), 5, 0)  # 左上角（0,0）  线宽5  颜色（b,g,r)
    cv2.circle(imgg, (50, 50), 50, (0, 255, 0), 5, 0)      # 左上角（0,0）  线宽5  颜色（b,g,r)
    cv2.imshow('red', imgg)
    end = time.clock()
    print(end - start)   # ----》 0.秒

    global x_old, y_old, is_draw
    # 鼠标回调
    def on_mouse_callback(event, x, y, flag, params):
        if event == cv2.EVENT_MOUSEMOVE:
            # print('move')
            global x_old, y_old, is_draw
            if is_draw == True:
                cv2.rectangle(imgg, (x_old, y_old), (x, y), (255, 255, 255), -1)  # (img,左上角坐标，右下角坐标，颜色，线宽（-1表示填充）)
        elif event == cv2.EVENT_LBUTTONUP:
            print('left up')
            is_draw = False
        elif event == cv2.EVENT_LBUTTONDOWN:
            print('left down')
            is_draw = True
        x_old, y_old = x, y
    v = 0
    cv2.createTrackbar('t', 'red', v, 100, on_track_callback)  # 创建滑动条   进度条仅在red窗口有限
    cv2.setMouseCallback('red', on_mouse_callback, img)        # 鼠标监听     鼠标监听仅在red窗口有限
    #刷新red图
    while True:
        cv2.imshow('red', imgg)
        cv2.waitKey(100)
    #播放视频
    #rtsp_str = 'rtmp://live.hkstv.hk.lxdns.com/live/hks'
    # rtsp_str ="C:\\Users\\Administrator.000\\Desktop\\1.avi"
    # producer = Producer(rtsp_str)
    # producer.start()

    # 图片合成视频
    # saver = SaverVideo()
    # saver.start()

    # 从摄像头读入视频
    # capture = cv2.VideoCapture(0)
    # frame = np.mat(None,None)
    # 循环显示每一帧
    # while capture.isOpened():
    #     #capture >> frame;  #读取当前帧
    #     ret, image = capture.read()
    #     if (ret == True and image is not None):
    #         cv2.imshow("capture",img);  #显示当前帧
    #     cv2.waitKey(30);  #延时30ms

if __name__ == '__main__':
    test()



def dd():
    # m = cv2.getOptimalDFTSize(img2.shape[0])
    # n = cv2.getOptimalDFTSize(img2.shape[1])

    #cv2.copyMakeBorder(img2, padded, 0, m - srcImage.rows, 0, n - srcImage.cols, BORDER_CONSTANT, Scalar::all(0));

    # // 【4】进行就地离散傅里叶变换
    # dft(complexI, complexI);
    #
    # // 【5】将复数转换为幅值，即 = > log(1 + sqrt(Re(DFT(I)) ^ 2 + Im(DFT(I)) ^ 2))

    # fs = cv2.FileStorage("test.yaml", cv2.WRITE)
    # // 第一种方法，对FileNode操作
    # frameCount = fs2["frameCount"];
    #img2 = cv2.addWeighted(img, 0.5, img2, 0.3, 0.,img2);


    #cv2.ellipse(img2,(50,50),(50,20),(255,0,0),5)#左上角（0,0）  线宽5  颜色（b,g,r)
    #cv2.fillPoly(img2,((0,0),(100,100),(200,0)),100,80,(255,0,0),0)#左上角（0,0）  线宽5  颜色（b,g,r)

    cv2.imshow('w', img2)

    g_srcImage = cv2.imread("1.jpg", 1)
    g_templateImage = cv2.imread("2.jpg", 1)

    def on_Matching():
        srcImage = g_srcImage.copy()
        resultImage_cols = g_srcImage.shape[1] - g_templateImage.shape[1] + 1
        resultImage_rows = g_srcImage.shape[0] - g_templateImage.shape[0] + 1
        print(type(resultImage_cols))
        print(type(resultImage_rows))
        g_resultImage = np.array(resultImage_rows, resultImage_cols)

        g_nMatchMethod = None
        #进行匹配和标准化
        cv2.matchTemplate(g_srcImage, g_templateImage, g_resultImage, g_nMatchMethod)
        cv2.normalize(g_resultImage, g_resultImage, 0, 1, cv2.NORM_MINMAX, -1, None)

        #通过函数minMaxLoc定位最匹配的位置
        minValue=0.
        maxValue=0
        minLocation=(0,0)
        maxLocation=(0,0)
        matchLocation=(0,0)
        cv2.minMaxLoc(g_resultImage, minValue, maxValue, minLocation, maxLocation, Mat() );

        #对于方法SQDIFF和SQDIFF_NORMED, 越小的数值有着更高的匹配结果.而其余的方法, 数值越大匹配效果越好
        if (g_nMatchMethod == cv2.CV_TM_SQDIFF or g_nMatchMethod == cv2.CV_TM_SQDIFF_NORMED):
            matchLocation = minLocation
        else:
            matchLocation = maxLocation

        #绘制出矩形，并显示最终结果
        cv2.ectangle(srcImage, matchLocation,(matchLocation.x + g_templateImage.cols, matchLocation.y + g_templateImage.rows), (0, 0, 255),2, 8, 0)
        cv2.rectangle(g_resultImage, matchLocation,(matchLocation.x + g_templateImage.cols, matchLocation.y + g_templateImage.rows), (0, 0, 255),2, 8, 0)
        cv2.imshow('1', srcImage);
        cv2.imshow('2', g_resultImage);

    on_Matching()
    cv2.waitKey(999999)
#cv2.rectangle(edge, (0, 0), (100, 100), (0, 255, 0), 20)

def test2():
    cv2.namedWindow("w", 0)  # 创建窗口
    img = cv2.imread("./1.jpg", -1)
    img2 = cv2.imread("./1.png", -1)
    print(type(img))  # numpy.ndarray
    if img.size > 0:
        #img3 = cv2.addWeighted(img,img2)
        cv2.imshow('w', img2)
        pass
    else:
        print("null")


    #img3 = cv2.clone()
    #img2.copyTo(img)
    #m = cv2.mat(3,3,cv2.CV_8UC3)

    #s = cv2.Scalar(0,0,0)
    #cv2.line(img,10,10,(0,0,0),2,8)





# import pylab
# #import matplotlib
# #python-numpy python-scipy python-matplotlib
# import sys
# print(sys.path)
#
# #from matplotlib import pylab
# import imageio
# #视频的绝对路径
# filename = '/path/to/your/video.mp4'
# #可以选择解码工具
# vid = imageio.get_reader(filename,  'ffmpeg')
# for im in enumerate(vid):
#     #image的类型是mageio.core.util.Image可用下面这一注释行转换为arrary
#     #image = skimage.img_as_float(im).astype(np.float32)
#     fig = pylab.figure()
#     #fig.suptitle('image #{}'.format(num), fontsize=20)
#     pylab.imshow(im)
# pylab.show()

# '''
# pylab 模块是一款由python提供的可以绘制二维，三维数据的工具模块，其中包括了绘图软件包 matplotlib,其可以生成matab绘图库的图像
# '''






