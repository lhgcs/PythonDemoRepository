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
#将path改成url ???
from django.contrib import admin
from django.urls import path, include
from django import urls
from app01 import views

from django.conf.urls import url

urlpatterns = [
    url(r'index/(?P<id>\d*)/$',views.index2),
    url(r'add/(?P<id>\d*)/(?P<name>\d*)',views.add),
    url(r'del/(?P<id>\d*)/$',views.delete),
    url(r'update/(?P<id>\d*)/(?P<name>\d*)',views.upda),
    url(r'get/(?P<id>\d*)/$',views.get),
    url(r'getall/$',views.get_all_data),

    url(r'temp/$',views.get_html),
    url(r'login/$',views.login),



    #path(r'index/',app01.views.index),
    #如果要给获取的参数起名，URL写法'index/?P<名字>表达式' 但是在视图函数接受的时候，必须使用起的名字接受

    #path(r'index/?P<name>(\d*)/',app01.views.index3),
    #path(r'^index/(?P<year>[0-9]{4})/(?P<month>[0-9]{2})/$',views.testapp01.views.index3),
]
