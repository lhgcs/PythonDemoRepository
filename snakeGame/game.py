import pygame
from pygame.sprite import Group,Sprite
import pygame.sysfont
import sys
import random
import time

def run_game():
    pygame.init()
    screen = pygame.display.set_mode((500,500))
    pygame.display.set_caption("snake")

    #背景色
    bg = (80,80,80)
    screen.fill(bg)

    #图片
    img = pygame.image.load("QR.png")

    img_rect = img.get_rect()      #获取外接矩形
    screen_rect = screen.get_rect()

    img_rect.centerx = screen_rect.centerx  #中心坐标
    #img_rect.bottom = screen_rect.bottom     #下边缘


    #移动步长
    step_len = 10
    #前进方向
    left = 1
    top = 2
    right = 3
    bottom = 4
    direction = left

    class Button():
        def __init__(self,screen,msg):
            self.screen = screen
            self.screen_rect = screen.get_rect
            self.width = 100
            self.height = 50
            self.btn_color = (255,0,0)
            self.text_color = (255,255,255)
            self.font = pygame.font.SysFont(None,30)
            self.rect = pygame.Rect(0,0,self.width,self.height)
            self.rect.center = screen_rect.center
            self.msg_img = self.font.render(msg,True,self.text_color,self.btn_color)#将文本渲染为图片

        def draw_btn(self):
            self.screen.fill(self.btn_color,self.rect)
            self.msg_img_rect = self.msg_img.get_rect()
            self.msg_img_rect.center = self.rect.center  # 居中
            self.screen.blit(self.msg_img,self.msg_img_rect)


    class Point():
        def __init__(self,screen):
            self.screen = screen
            self.screen_rect = screen.get_rect()
            self.btn_color = (70,70,70)
            self.text_color = (255,255,255)
            self.font = pygame.font.SysFont(None,30)
            self.point = 0

        def draw_point(self):
            msg = str(self.point)
            self.msg_img = self.font.render(msg, True, self.text_color, self.btn_color)  # 将文本渲染为图片
            self.msg_img_rect = self.msg_img.get_rect()
            self.msg_img_rect.top = 0
            self.msg_img_rect.left = self.screen_rect.left
            self.screen.blit(self.msg_img,self.msg_img_rect)


    class SnakeItem(Sprite):
        len = 0
        def __init__(self,x,y,color):
            super().__init__()
            self.x = x
            self.y = y
            self.color = color

    #蛇
    item = SnakeItem(screen_rect.centerx,screen_rect.centery,(255,255,255))
    snake = []
    snake.append(item)

    #食物
    def set_food():
        while True:
            x = 0
            y = 0
            while True:
                x = random.randrange(0,screen_rect.width)
                if x % 10 == 0:
                    break
            while True:
                y = random.randrange(0,screen_rect.height)
                if y % 10 == 0:
                    break

            for i in snake:
                if i.x == x and i.y == y:
                    continue

            return SnakeItem(x,y,(255,0,0))

    temp_rect = set_food()


    btn_start = Button(screen,"start")
    start_flag = False
    points = Point(screen)

    while True:
        #方向
        for event in pygame.event.get():#键盘 鼠标 事件
            if event.type == pygame.QUIT:
                sys.exit()
            elif event.type == pygame.KEYDOWN:
                if event.key == pygame.K_RIGHT:
                    direction = right
                elif event.key == pygame.K_LEFT:
                    direction = left
                elif event.key == pygame.K_UP:
                    direction = top
                elif event.key == pygame.K_DOWN:
                    direction = bottom
            elif event.type == pygame.MOUSEBUTTONDOWN:
                mouse_x,mouse_y = pygame.mouse.get_pos()
                if btn_start.rect.collidepoint(mouse_x,mouse_y):
                    start_flag = False if start_flag else True

        #screen.blit(img,img_rect)#在指定位置绘制
        #背景
        screen.fill(bg)

        if not start_flag :
            btn_start.draw_btn()
            points.draw_point()
            pygame.display.flip()
            continue

        #食物
        rect = pygame.Rect(temp_rect.x,temp_rect.y,10,10)
        pygame.draw.rect(screen,temp_rect.color,rect)
        #蛇
        item_head = snake[0]
        xx = snake[0].x
        yy = snake[0].y

        if direction == left:
            xx -= step_len
            if xx < 0:
                continue
        elif direction == top:
            yy -= step_len
            if yy < 0:
                continue
        elif direction == right:
            xx += step_len
            if xx >= screen_rect.right:
                continue
        elif direction == bottom:
            yy += step_len
            if yy >= screen_rect.bottom:
                continue
        #snake.append(item_head)   #不知为何snake中的坐标是一样的？
        nn = SnakeItem(xx,yy,(255,255,255))
        snake.insert(0,nn)

        if item_head.x == temp_rect.x and item_head.y == temp_rect.y:
            temp_rect = set_food()
            points.point +=1
            for i in snake:
                r = pygame.Rect(i.x, i.y, 10, 10)
                pygame.draw.rect(screen, i.color, r)
            points.draw_point()
            pygame.display.flip()  # 最近绘制的屏幕可见
        else:
            snake.pop()
            for i in snake:
                r = pygame.Rect(i.x, i.y, 10, 10)
                pygame.draw.rect(screen, i.color, r)
            points.draw_point()
            pygame.display.flip()  # 最近绘制的屏幕可见
            time.sleep(0.3)


        # print("--------------")
        # for i in snake:
        #     print(i.x,i.y)

run_game()