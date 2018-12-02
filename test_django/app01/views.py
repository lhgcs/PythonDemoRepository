from django.shortcuts import render,render_to_response
from app01.models import UserInfo,UserSex
from django.http.response import HttpResponse

# Create your views here.
from django.http import HttpResponse,HttpRequest

def index(request):
    return HttpResponse('1111111')#返回字符串

def index2(request,id):
    return HttpResponse(id)

def index3(request,name):
    return HttpResponse(name)

def add(request,id,name):
    #UserInfo.objects.create(name)#对象
    obj = UserInfo()
    obj.name='yy'
    obj.age=28
    obj.email='111@163.com'
    obj.txt="123"
    #obj.sex=UserSex.objects.filter(type='man')[0]
    obj.save()

    return HttpResponse('add')

def delete(request,id):
    UserInfo.objects.get(id=id).delete()#get查找一条
    return HttpResponse('del')
def upda(request,id,name):
    obj=UserInfo.objects.get(id=id)
    obj.name=name
    obj.save()

    UserInfo.objects.filter(id__gt=10,).update(name='11')#id大于10
    return HttpResponse('update')

def get(request,id):
    list=UserInfo.objects.all()[0:2]#取前两条
    listid=UserInfo.objects.all().values('id','name')#只获取id name    指定映射
    alldate=UserInfo.objects.all().order_by('id')#根据id排序
    alldate2 = UserInfo.objects.all().order_by('-id')  # 根据id排序  反序

    print(alldate.query)#sql语句
    '''
    cursor = connetiom.cursor()
    cursor.execute('''''')
    row = cursor.fetchone
    '''
    #get只能获取一条，不存在报错    返回一个对象
    #filter不会报错，返回None      返回一个集合

    for item in list:
        print(item.id)
    return HttpResponse(alldate)


def get_all_data(request):
    all=UserInfo.objects.all()
    return render_to_response('index.html',{'user':'admin','data':all})#html文件  数据    返回包含数据的HTML字符串

def get_html(request):
    return render_to_response('master/son.html')


def login(request):
    a = request.GET['a']
    b = request.GET['b']
    a = int(a)
    b = int(b)
    return HttpResponse(str(a+b))


from django import forms
# 引入我们创建的表单类
class AddForm(forms.Form):
    a = forms.IntegerField()
    b = forms.IntegerField()


def fff(request):
    if request.method == 'POST':  # 当提交表单时
        form = AddForm(request.POST)  # form 包含提交的数据
        if form.is_valid():  # 如果提交的数据合法
            a = form.cleaned_data['a']
            b = form.cleaned_data['b']
            return HttpResponse(str(int(a) + int(b)))
    else:  # 当正常访问时
        form = AddForm()
    return render(request, 'index.html', {'form': form})



