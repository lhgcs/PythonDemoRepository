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
from django.conf.urls import url

#根据不同的APP进行路由分发


#path没有r，url正则表达式才有
urlpatterns = [
    path('admin/', admin.site.urls),
    path('app1/',include('app01.urls')),#斜杠千万不能少
    url(r'app_logs/',include('app_logs.urls',namespace='app_logs')),
    #url(r'',include('app_logs.urls',namespace="app_logs")),#实参
]
