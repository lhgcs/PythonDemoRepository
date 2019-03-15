#ifndef KEYPRESS_H
#define KEYPRESS_H

#include <QObject>
#include <QThread>

#include <linux/input.h>

#include <sys/types.h>//open
#include <sys/stat.h>
#include <fcntl.h>

#include <unistd.h>//close read write

typedef enum{
    SUPPLY_SHOW_BUTTON_KEY_VALUE	= 28,
    SUPPLY_HIDE_BUTTON_KEY_VALUE	= 139,
}SupplyButtonKey;

class KeyPress : public QThread
{
    Q_OBJECT
public:
    KeyPress(const QString &device, QObject *parent=0);
    ~KeyPress(){
        if(fd>=0)
         close(fd);
    }
    void run();

signals:
    void SupplyButtonPress(int);

private:
    int fd;
    struct input_event t;
    void KeypadFilter();

//    关键结构体input_event信息：
//    struct input_event {
//    struct timeval time;
//    __u16 type;
//    __u16 code;
//    __s32 value;
//    };
};

#endif // KEYPRESS_H
