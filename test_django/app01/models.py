from django.db import models

# Create your models here.

'''
由于mysqldb不支持python3，所以django连接mysql就不能再使用mysqldb了
故而选择了mysqlclient，然而两者之间并没有太大的使用上的差异。 
pip install mysqlclient

#pip install mysql-python
解决方法：
通过提示我们可以得知是由于c++依赖库不存在，所以在对包进行编译的时候出错，故我们可以去直接下载二进制安装包进行安装。


还是使用Django自带的sqlite3吧

python3 manage.py makemigrations
python3 manage.py migrate


 raise AppRegistryNotReady("Apps aren't loaded yet.")
django.core.exceptions.AppRegistryNotReady: Apps aren't loaded yet.


'''

class UserSex(models.Model):
    nid = models.AutoField(primary_key=True)
    type = models.TextField()

class UserInfo(models.Model):
    GENDER_CHOICE = (#不在数据库，在程序的内存
        (u'1',u'普通用户'),
        (u'2', u'超级用户'),
    )

    name = models.CharField(max_length=10,default=None,null=True)
    age = models.IntegerField(error_messages={'invalid':"error"})
    email = models.EmailField(max_length=32,unique=True,help_text='emild')
    #date = models.DateField()#创建时  ,editable=False  auto_now_add=True
    time = models.DateTimeField(auto_now=True)#修改时
    txt = models.TextField(null=True)
    #sex = models.ForeignKey('UserSex',on_delete=models.CASCADE)







#TypeError: __init__() missing 1 required positional argument: 'on_delete'
#django.db.utils.OperationalError: (2003, "Can't connect to MySQL server on 'localhost' (10061)")


