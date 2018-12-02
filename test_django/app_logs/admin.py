from django.contrib import admin

# Register your models here.
#向管理网站注册模型
from app_logs.models import Topic,Entry
admin.site.register(Topic)
admin.site.register(Entry)

#http://127.0.0.1:8000/admin  可以登录
