#ifndef PLAYMP3_H
#define PLAYMP3_H

#include <QObject>

class PlayMp3 : public QObject
{
public:
    PlayMp3();
    ~PlayMp3(){

    }

    void play();
    void setFileName(char *value);
    void setPlayFlag(volatile bool value);
    volatile bool getPlayFlag() const;

private:
    char *fileName;
    volatile bool playFlag;
    int set_pcm_play(FILE *fp);
};

#endif // PLAYMP3_H
