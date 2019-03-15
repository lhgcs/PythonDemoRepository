#ifndef PLAYSOUND_H
#define PLAYSOUND_H

#include "playmp3.h"
#include <QThread>

class PlaySound
{
public:
    PlaySound();
    ~PlaySound(){
        thread->terminate();
        thread->wait(1000);
    }

    static PlaySound *mPlaySound;
    static PlaySound *getInstance(){
        if(mPlaySound == NULL){
            mPlaySound=new PlaySound();
        }
        return mPlaySound;
    }

    int play(char *fileName){
        //读取块的大小不要太大,否则等待线程释放太久
        p->setPlayFlag(false);
        if(thread->isRunning() == true){
            //建议：一般情况下，都在run函数里面设置一个标识符，可以控制循环停止。然后才调用quit函数，退出线程。
            //thread->quit();            //告诉线程事件循环退出
            thread->terminate();         //终止线程，线程可能会立即被终止也可能不会,线程可以在代码执行的任何点被终止。线程可能在更新数据时被终止，从而没有机会来清理自己，解锁等等
            bool res=thread->wait(500);  //线程等待  线程将会被阻塞，等待time毫秒
            if (res == false){
                return -1;
            }
            if(thread->isFinished() == false){
                return -1;
            }
        }
        //只支持WAV,不支持MP3
        p->setFileName(fileName);
        p->setPlayFlag(true);
        thread->start();
        return 0;
    }

private:
    QThread *thread;
    PlayMp3 *p;
};

#endif // PLAYSOUND_H
