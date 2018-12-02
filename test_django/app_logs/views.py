from django.contrib.auth.decorators import login_required
from django.shortcuts import render
from app_logs.models import Topic
# Create your views here.

from django.http import HttpResponse

def index(request):
    #return HttpResponse('1111111')  # 返回字符串
    topic = Topic.objects.order_by("date_added")
    context = {}
    context['topic'] = topic
    return render(request,'master/son.html',context)
    #return render(request, 'app_logs\\templates\\master\\son.html', context)

def index_id(request,id):
    count = Topic.objects.all().count()
    if int(id) > count:
        return HttpResponse('id')  # 返回字符串
    topic = Topic.objects.get(id=id)
    context = {}
    context['topic'] = topic
    #return render(request,'master/son.html',context)
    return HttpResponse('1111111')  # 返回字符串

from django.http import HttpResponseRedirect
#from  django.core import reverse
from .forms import TopicForm
@login_required
def forms(request):
    if request.method != 'Post':
        form = TopicForm()
    else:
        form = TopicForm(request.POST)
        # if form.is_valid():
        #     form.save()
            #return HttpResponseRedirect(reverse('app_logs:topics'))

    context = {'form':form}
    return render(request,'form.html',context)


def mypost(request):
    return HttpResponse('1111111')  # 返回字符串


