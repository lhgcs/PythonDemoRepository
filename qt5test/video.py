import numpy as np
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

'''
pylab 模块是一款由python提供的可以绘制二维，三维数据的工具模块，其中包括了绘图软件包 matplotlib,其可以生成matab绘图库的图像
'''

#pip install opencv-python

# cap = cv2.VideoCapture("C:\\Users\\Administrator.000\\Desktop\\Android_test\\test.mp4")
#
# while (cap.isOpened()):
#     ret, frame = cap.read()
#     cv2.imshow('frame', frame)
#
#     k = cv2.waitKey(20)
#     # q键退出
#     if k & 0xFF == ord('q'):
#         break
#
# cap.release()
# cv2.destroyAllWindows()


# !/usr/bin/env python3


# -*- coding:utf-8 -*-


import cv2
import threading

class Producer(threading.Thread):
    """docstring for ClassName"""

    def __init__(self, str_rtsp):
        super(Producer, self).__init__()
        self.str_rtsp = str_rtsp
        self.play = True
        # 通过cv2中的类获取视频流操作对象cap
        self.cap = cv2.VideoCapture(self.str_rtsp)
        # 调用cv2方法获取cap的视频帧（帧：每秒多少张图片）
        fps = self.cap.get(cv2.CAP_PROP_FPS)
        print(fps)
        # 获取cap视频流的每帧大小
        size = (int(self.cap.get(cv2.CAP_PROP_FRAME_WIDTH)),
                int(self.cap.get(cv2.CAP_PROP_FRAME_HEIGHT)))
        print(size)
        # 定义编码格式mpge-4
        #fourcc = cv2.VideoWriter_fourcc('M', 'P', '4', '2')
        # 定义视频文件输入对象
        #self.outVideo = cv2.VideoWriter('saveDir.avi', fourcc, fps, size)
        #cv2.namedWindow("cap video", 0);
    def run(self):
        print('in producer')
        while self.cap.isOpened():
            ret, image = self.cap.read()
            # if (ret == True):
            #     if win32gui.FindWindow(None, 'cap video'):
            if image is not None:
                cv2.imshow('cap video', image)
            else:
                break
            #self.outVideo.write(image)
                # else:
                #     self.outVideo.release()
                #     self.cap.release()
                #     cv2.destroyAllWindows()
                #     break

            cv2.waitKey(1)
            if cv2.waitKey(1) & 0xFF == ord('q'):
                self.outVideo.release()
                self.cap.release()
                cv2.destroyAllWindows()
                break
                # continue

if __name__ == '__main__':
    print('run program')
    #rtsp_str = 'rtmp://live.hkstv.hk.lxdns.com/live/hks'
    rtsp_str ="C:\\Users\\Administrator.000\\Desktop\\Android_test\\test.mp4"
    producer = Producer(rtsp_str)
    producer.start()

