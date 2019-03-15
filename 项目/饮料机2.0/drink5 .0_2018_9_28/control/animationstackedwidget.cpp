#include "animationstackedwidget.h"
#include <QPainter>
#include <QDebug>
AnimationStackedWidget::AnimationStackedWidget(QWidget *parent):QStackedWidget(parent)
{
    isAnimation = false;
    //设置默认的时间间隔
    duration = 400;
    //初始化animation框架、并连接信号和槽  指定要动画的属性
    animation = new QPropertyAnimation(this,QByteArray());//QByteArray()

    thread=new QThread();
    this->moveToThread(thread);
    connect(animation, SIGNAL(valueChanged(QVariant)), this, SLOT(valueChanged_slot(QVariant)));
    connect(animation, SIGNAL(finished()), this, SLOT(animationFinished()));
}

/*
 * 切换下一页
 */
void AnimationStackedWidget::next(){
    //如果正在动画，那么return
    if( isAnimation ){
        return;
    }
    thread->start();
    isAnimation = true;
    widgetCount = count();
    int c = currentIndex();
    //计算下一页的索引
    nextIndex = (c + 1) % widgetCount;
    //隐藏当前的widget
    widget(c)->hide();
    //开始动画并设置间隔和开始、结束值
    QRect g = geometry();
    //int x = g.x();
    int width = g.width();
    //设定初始值
    animation->setStartValue(width);
    //animation->setStartValue(QRect(0, 0, 0, 0));
    animation->setEndValue(0);
    animation->setDuration(duration);//设定持续时间，单位为毫秒
    //animation->setEasingCurve(QEasingCurve::OutBounce); //使用弹跳曲线
 //   animation->setEasingCurve(QEasingCurve::CosineCurve);  //余弦曲线
//    animation->setEasingCurve(QEasingCurve::SineCurve);   //正弦曲线
//    animation->setEasingCurve(QEasingCurve::InBack);   //回撤效果
//    animation->setEasingCurve(QEasingCurve::InBounce); //回弹效果
   // animation->setEasingCurve(QEasingCurve::OutQuad);  // 速度减慢

  //  animation->setEasingCurve(QEasingCurve::InOutQuad);  // 慢快慢
//animation->setEasingCurve(QEasingCurve::OutInQuad);//快慢快
animation->setEasingCurve(QEasingCurve::InCubic);//慢快
//animation->setEasingCurve(QEasingCurve::InQuart);//慢快
   // animation->setEasingCurve(QEasingCurve::InQuint);//慢快
  //  animation->setEasingCurve(QEasingCurve::InSine);//慢快  11111111111111111
   // animation->setEasingCurve(QEasingCurve::InExpo);//111111

//    animation->setEasingCurve(QEasingCurve::InCirc);

   // animation->setEasingCurve(QEasingCurve::InElastic);//左右
// animation->setEasingCurve(QEasingCurve::InBack);//左右11111111111111

 /*
    Linear,
    InQuad, OutQuad, InOutQuad, OutInQuad,
    InCubic, OutCubic, InOutCubic, OutInCubic,
    InQuart, OutQuart, InOutQuart, OutInQuart,
    InQuint, OutQuint, InOutQuint, OutInQuint,
    InSine, OutSine, InOutSine, OutInSine,
    InExpo, OutExpo, InOutExpo, OutInExpo,
    InCirc, OutCirc, InOutCirc, OutInCirc,
    InElastic, OutElastic, InOutElastic, OutInElastic,
    InBack, OutBack, InOutBack, OutInBack,
    InBounce, OutBounce, InOutBounce, OutInBounce,
    InCurve, OutCurve, SineCurve, CosineCurve,
    BezierSpline, TCBSpline, Custom, NCurveTypes
    */

    //开启动画
    animation->start();
}
//void AnimationStackedWidget::forward(){
//    //如果正在动画，那么return
//    if( isAnimation ){
//        return;
//    }
//    isAnimation = true;
//    widgetCount = count();
//    int c = currentIndex();
//    //计算下一页的索引
//    nextIndex = (c + 1) % widgetCount;
//    //隐藏当前的widget
//    widget(c)->hide();
//    //开始动画并设置间隔和开始、结束值
//    QRect g = geometry();
//    //int x = g.x();
//    int width = g.width();
//    animation->setStartValue(width);
//    animation->setEndValue(0);
//    animation->setDuration(duration);
//    //开启动画
//    animation->start();
//}

/*
 * 因为QPropertyAnimation::start()方法调用之后会发射valueChanged()信号，所以我们可以接收到改变了的值：
*/
void AnimationStackedWidget::valueChanged_slot(QVariant value){
    currentValue = value;
    update();
}

/*
 * update又会触发paintEvent函数,所以我们在paintEvent里面可以将过渡的效果绘制出来:
*/
void AnimationStackedWidget::paintEvent(QPaintEvent *){
    if( isAnimation ){
        QPainter paint(this);
        //绘制当前Widget
        paintPrevious(paint, currentIndex());
        //绘制下一个widget
        paintNext(paint, nextIndex);
    }
}

void AnimationStackedWidget::paintPrevious(QPainter &paint, int currentIndex){
    //获得当前页面的Widget
    QWidget *w = widget(currentIndex);
    QPixmap pixmap(w->size());
    //将Widget的内容渲染到QPixmap对象中，即将Widget变成一张图片
    w->render(&pixmap);
    QRect r = w->geometry();
    //绘制当前的Widget
    double value = currentValue.toDouble();
    QRectF r1(0.0, 0.0, value, r.height());
    QRectF r2(r.width() - value, 0, value, r.height());
    paint.drawPixmap(r1, pixmap, r2);
}

void AnimationStackedWidget::paintNext(QPainter &paint, int nextIndex){
    QWidget *nextWidget = widget(nextIndex);
    QRect r = geometry();
    //这行代码不加会有bug，第一次切换的时候，QStackedWidget并没有为child分配大小
    nextWidget->resize(r.width(), r.height());
    QPixmap nextPixmap(nextWidget->size());
    nextWidget->render(&nextPixmap);
    double value = currentValue.toDouble();
    QRectF r1(value, 0.0, r.width() - value, r.height());
    QRectF r2(0.0, 0.0, r.width() - value, r.height());
    paint.drawPixmap(r1, nextPixmap, r2);
}


/*
 * 动画完成，真正开始切换界面：
 */
void AnimationStackedWidget::animationFinished(){
    isAnimation = false;
    widget(currentIndex())->show();
    setCurrentIndex(nextIndex);
    thread->quit();
    thread->wait();
}
