#!/user/bin/env python
# coding=utf-8

def outer(fun):
    def ff(arg):
        print 222
        fun(arg)
        print 333
        return "xxxxxxx"
    return ff

@outer
def fun(arg):
    print 111111,arg


#print fun('ffff')

class person:
    a = 1  # 静态字段

    def __init__(self, name):  # 初始化
        self.name = name  # 动态字段

    #销毁时做些事情,最后的一件事
    def __del__(self):#析构函数，不销毁内存，是虚拟机解释器销毁
        pass

    def __call__(self, *args, **kwargs):
        print 24243543
    def fun(self):# 动态方法
        print self.name

    @staticmethod  #装饰器
    def Fun(self):# 静态方法
        print person.a

    @property  #装饰器
    def Fun1(self):# 特性
        print self.name
        return 999999
    #可写  推荐访问私有字段用这个
#    @Thailand.setter  #装饰器  方法名.setter    去掉装饰器就是只读的
    def Thailand(self, value): # 特性
        self.__si=value
        return 999999

    def foo(self, name): # 私有字段
        self.__name = name
    def __sha (self): #私有方法
        pass

p1 = person(13)
print p1.name
print person.a
print p1.a
p1.Fun1   #不用加括号

p1()  #执行call 方法

print p1.__dict__

class work(person):
    def fun(self):
        person.fun(self)
        #super(work,self).fun()   另一种方法    super（类名，self）
        #父类需要继承 object（调用C接口）
        #  新式类（2.2kais)，经典类 区别在于继承时深度优先和广度优先,
        # 经典类是深度优先，在多重继承（同时继承两个类），不断向父类，父类的父类，一直往上，而不是先在同一级的另一个父类中查找
        #class A
        #class B(A)
        #class C(A)
        #class D(B,C)  先B后C
        #继承时：
        #新式类  B--〉C--->A
        #经典类  B---〉A--->C

#经典类的特性都是可读可写
        #新式类默认只读，需要可写时@xxx.setter

#特性在私有字段时设置权限时用


#Java C#不允许多继承

        print 11111


# 抽象类  interface  不能写具体的实现，只定义规则

from abc import abstractmethod,ABCMeta

class aa:
    __metaclass__ = ABCMeta
    @abstractmethod
    def send(self):pass
class bb(aa):
    def send (self):
        print 435435435

#字符串导入模块，字符串调用函数
try:
    user=__import__("11");
    model = getattr(user,"11")
    func = getattr(model,"33")
    func()
except (ImportError,AttributeError),e:
    print e
except IOError,e:
    print e
except Exception,e:
    print e
else:
    print "no error"
finally:
    print "都会执行"
#一旦出错就不再往下，直接跳到eccept
#IOError#输入输出异常，打开文件
#ImportError #倒入模块

class MyException(Exception):
    def __init__(self, msg):
        self.error=msg

    def __str__(self):
        return "错误"

obj=MyException("cuo")
print obj

#raise MyException("77777")





