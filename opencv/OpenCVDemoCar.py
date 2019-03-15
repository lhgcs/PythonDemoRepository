# -*- coding:utf-8 -*-
import numpy as np
import cv2

def car():
    img = cv2.imread("C:\\Users\\Administrator.000\\Desktop\\a.png")
    grayImage = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY )     #转换颜色空间，转为灰度图
    #img2 = copy.copy(grayImage)
    img2 = grayImage.copy()
    cv2.threshold(grayImage,200,255,cv2.THRESH_BINARY,img2)#二值化  阈值是100，大于100为白色
    cv2.imshow("w", grayImage)
    cv2.waitKey(1000)
    cv2.imshow("w",img2)
    cv2.waitKey(1000)

    black = []#每一列黑点的数量
    white = []#每一行黑点的数量
    row = img2.shape[0]
    col = img2.shape[1]
    for i in range(col):
        blackCnt = 0
        whiteCnt = 0
        for j in range(row):
            if img2[j][i] == 0:
                blackCnt +=1
            else:
                whiteCnt +=1
        black.append(blackCnt)
        white.append(whiteCnt)

    blackMax = max(black)#黑点最大数量
    whiteMax = max(white)#白点最大数量
    #黑底
    is_black_backgroud = True if whiteMax < blackMax else False

    if (sum(black) if is_black_backgroud else sum(white)) == row * col:#都是底色，没有字符
        print('no char')

    #去除字符顶部和底部的空白
    def find_end(row,col):
        black = []
        for i in range(row):
            blackCnt = 0
            for j in range(col):
                if img2[i][j] == 0:
                    blackCnt += 1
            black.append(blackCnt)
        start_row = 0 #字符开始行
        end_row = row #字符结束行

        for i in range(row):
            # 这一行全是底色
            if (black[i] if is_black_backgroud else (col-black[i]) == col):
                continue
            else:
                start_row = i
                break
        for i in range(start_row+1,row):
            # 这一行全是底色
            if (black[i] if is_black_backgroud else (col-black[i]) == col):
                end_row = i
                break
        return start_row,end_row

    #分割字符   缺陷：字符不能重合
    def find_end2(start_col,col):
        end_col = start_col
        #找到字符开始的列
        temp = -1
        for i in range(start_col, col-1):
            if (white[i] if is_black_backgroud else black[i]) > (0.1 * (whiteMax if is_black_backgroud else blackMax)):
                temp = i
                print('start:',i)
                break
        if temp == -1:
            return start_col,end_col
        else:
            start_col = temp

        # 找到字符结束的列
        for i in range(start_col + 1, col - 1):
            # 底部的色彩 占这一列的 色彩的 95%   ---》  分割字符结束
            if (black[i] if is_black_backgroud else white[i]) > (0.9 * (blackMax if is_black_backgroud else whiteMax)):
                #下一列还有字体颜色
                if (white[i+1] if is_black_backgroud else black[i+1]) > (0.1 * (whiteMax if is_black_backgroud else blackMax)):
                    continue
                end_col = i
                print('end:',i)
                break
        return start_col,end_col

    start = 0
    end = 0
    top, botton = find_end(row, col)
    i = 0
    while i < col-1:
        start,end = find_end2(start,col)
        i = end
        if start == end:#没找到字符
            break
        if end - start > 10:
            print("ok")
            imggg = img2[top:botton,start:end]
            cv2.imshow('1',imggg)
            cv2.waitKey(2000)
        else:
            print("error")
        start = end + 1
    cv2.waitKey(999999)

if __name__ == '__main__':
    car()















def f():
    # cv2.namedWindow("w",1)
    #capture = cv2.CaptureFromCAM(0)#0本地 -1随机
    # capture = cv2.VideoCapture(0)
    # while True:
    #     r,img = capture.read()
    #     #img = cv2.QueryFrame(capture)
    #     #cv2.ShowImage('y',img)
    #     cv2.imshow('i',img)
    #     cv2.waitKey(1000)
    # cv2.setCaptureProperty(capture,3,360)
    #img = cv2.imread("1.png")


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
        #cv2.minMaxLoc(g_resultImage, minValue, maxValue, minLocation, maxLocation, Mat() );

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






def test():
    img = cv2.imread("./1.jpg",-1)#-1：原图的通道，0：单通道，1：3通道，2：原图是16/32位深度的图像则返回对应深度的图像，否则转换为8位
    print(type(img))#numpy.ndarray
    if img.size >0:
        #cv2.imshow('1.jpg',img)
        pass
    else:
        print("null")

    dstImage = np.array(0)
    element = np.ones((5, 5), np.uint8)  # 结构元素，值越大，膨胀或腐蚀的区域越大   值要大于1
    element = cv2.getStructuringElement(cv2.MORPH_RECT, (5, 5))  # 生成矩形的的结果元素

    #进行腐蚀操作
    dstImage = cv2.erode(img, element)    #腐蚀，取局部最小值
    cv2.dilate(dstImage,element)          #膨胀，取局部最大值
    cv2.morphologyEx(img, cv2.MORPH_OPEN    , element)  # 开运算：先腐蚀再膨胀，分类物体，消除小区域   --》 目标物体外面有无关的小东西
    cv2.morphologyEx(img, cv2.MORPH_CLOSE   , element)  # 闭运算：消除物体的小黑洞   ---》   目标物体内部有很多小黑洞
    cv2.morphologyEx(img, cv2.MORPH_GRADIENT, element)  # 形态学梯度：膨胀图减去腐蚀图   --》物体轮廓
    cv2.morphologyEx(img, cv2.MORPH_TOPHAT  , element)  # 顶帽：原图减去开运算 --》消除轮廓，几乎都是黑色
    cv2.morphologyEx(img, cv2.MORPH_BLACKHAT, element)  # 黑帽：原图减去闭运算 --》轮廓白色，其它黑色
    print(type(dstImage))
    #cv2.imshow("腐蚀操作", dstImage)

    # 进行均值滤波操作  ---》  去除噪声，模糊
    dstImage = cv2.boxFilter(img, -1,(7, 7))#原图的深度
    # cv2.imshow("均值滤波" ,dstImage)

    #进行均值滤波操作  ---》  去除噪声，模糊
    dstImage = cv2.blur(img,(7,7))
    #cv2.imshow("均值滤波" ,dstImage)

    #将原图像转换为灰度图像   ---》  减少计算
    grayImage = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY )#转换颜色空间
    #先用使用 3x3内核来降噪
    edge = cv2.blur(grayImage, (3,3) )
    #运行Canny算子  ---》 边缘检测
    edge = cv2.Canny( edge, 3, 9,3 )
    cv2.imshow("【效果图】Canny边缘检测", edge)



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

'''
pip install opencv-python  opencv视觉库

'''




