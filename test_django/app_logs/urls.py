"""test_django URL Configuration

The `urlpatterns` list routes URLs to views. For more information please see:
    https://docs.djangoproject.com/en/2.0/topics/http/urls/
Examples:
Function views
    1. Add an import:  from my_app import views
    2. Add a URL to urlpatterns:  path('', views.home, name='home')
Class-based views
    1. Add an import:  from other_app.views import Home
    2. Add a URL to urlpatterns:  path('', Home.as_view(), name='home')
Including another URLconf
    1. Import the include() function: from django.urls import include, path
    2. Add a URL to urlpatterns:  path('blog/', include('blog.urls'))
"""
from django.contrib import admin
from django.urls import path, include
from app_logs import views
from django.conf.urls import url
#路由分发，指向视图

urlpatterns = [
    url('admin/', admin.site.urls),
    #url(r'index/\d*/$',views.index_id,name='app_logs_index_id'),
    url(r'index/(?P<id>\d*)/',views.index_id,name='app_logs_index_id'),
    #url(r'index/',views.index,name='app_logs_index'),#斜杠千万不能少
    #url(r'forms/(?P<id>\d*)/',views.forms,name='forms'),
    url(r'forms/',views.forms,name='forms'),
    url(r'mypost/',views.mypost,name='mypost123'),
]

app_name = 'app_logs'
