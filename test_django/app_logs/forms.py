from django import forms
from .models import Topic

class TopicForm(forms.ModelForm):
    class Meta:
        model = Topic      #根据模型Topic创建表单
        fields = ['text']  #
        labels = {'text':''}