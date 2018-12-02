from django.db import models

# Create your models here.

class Topic(models.Model):
    text = models.CharField(max_length=100)
    date_added = models.DateTimeField(auto_now_add=True)

    def __str__(self):
        return self.text

class Entry(models.Model):
    topic = models.ForeignKey(Topic,on_delete=models.CASCADE)
    text = models.CharField(max_length=100)
    date_added = models.DateTimeField(auto_now_add=True)

    class Meta:
        verbose_name_plural = 'entry'

    def __str__(self):
        return self.text[:20]+"..."



#python manage.py makemigrations app_logs  生成迁移文件 0001_initial.py  --- > 如何修改数据库
#python manage.py migrate   应用这种迁移

#python manage.py shell  打开终端
#Topic.objects.all()

