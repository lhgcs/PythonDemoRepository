#目录下任意xxoo.py文件，创建template对象 register
from django import template

register = template.Library()

# 第一种方式：simple_tag
#     c. 定义：装饰函数
@register.simple_tag
def func(a1,a2,a3):
    return "asdfasd"
    # d. 配置：settings中注册APP
    # e. 引用：哪里用哪里顶部 {% load xxoo %}
    # f. 使用：{% 函数名 arg1 arg2 %}
    # 缺点： 不能作为if条件
    # 优点： 参数任意

# 第二种方式：filter
#     c.定义：装饰函数
@register.filter
def func(a1,a2):
    return "asdfasd"
    # d. 配置：settings中注册APP
    # e. 引用：部 {% load xxoo %}  # 引用母版extend时，load在extend下面。
    # f. 使用：{ 参数1|函数名:"参数二" }}  # 传多个，把参数二自定义“a,b,c”多个，自己处理
    # 缺点： 最多两个参数，不能加空格
    # 优点： 能作为if条件