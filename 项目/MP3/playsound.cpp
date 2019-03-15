#include "playsound.h"
#include "QObject"

PlaySound *PlaySound::mPlaySound=NULL;

PlaySound::PlaySound()
{
    thread = new QThread();
    p=new PlayMp3();
    p->moveToThread(thread);
    QObject::connect(thread, &QThread::started, p, &PlayMp3::play);//在run()函数执行前会发射信号started()
    //connect(thread,SIGNAL(finished()),thread,SLOT(deleteLater()),Qt::QueuedConnection);
}

