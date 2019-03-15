#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>

#define GPIO_DIR_IN	 0
#define GPIO_DIR_OUT 1

int gpio_export(unsigned gpio)//请求gpio
{
    int fd, len;
    char buf[11];

    fd = open("/sys/class/gpio/export", O_WRONLY);
    if (fd < 0) {
        perror("gpio/export");
        return fd;
    }

    len = snprintf(buf, sizeof(buf), "%d", gpio);
    write(fd, buf, len);
    close(fd);
    return 0;
}

int gpio_unexport(unsigned gpio)//释放gpio
{
    int fd, len;
    char buf[11];

    fd = open("/sys/class/gpio/unexport", O_WRONLY);
    if (fd < 0) {
        perror("gpio/export");
        return fd;
    }

    len = snprintf(buf, sizeof(buf), "%d", gpio);
    write(fd, buf, len);
    close(fd);
    return 0;
}

int gpio_dir(unsigned gpio, unsigned dir)
{
    int fd, len;
    char buf[60];

    len = snprintf(buf, sizeof(buf), "/sys/class/gpio/gpio%d/direction", gpio);

    fd = open(buf, O_WRONLY);
    if (fd < 0) {
        perror("gpio/direction");
        return fd;
    }

    if (dir == GPIO_DIR_OUT)
        write(fd, "out", 4);
    else
        write(fd, "in", 3);

    close(fd);
    return 0;
}

int gpio_dir_out(unsigned gpio)
{
    return gpio_dir(gpio, GPIO_DIR_OUT);
}

int gpio_dir_in(unsigned gpio)
{
    return gpio_dir(gpio, GPIO_DIR_IN);
}

int gpio_value(unsigned gpio, unsigned value)//gpio输出
{
    int fd, len;
    char buf[60];

    len = snprintf(buf, sizeof(buf), "/sys/class/gpio/gpio%d/value", gpio);

    fd = open(buf, O_WRONLY);
    if (fd < 0) {
        perror("gpio/value");
        return fd;
    }

    if (value)
        write(fd, "1", 2);
    else
        write(fd, "0", 2);

    close(fd);
    return 0;
}

int gpio_pwm(int fd, unsigned value)//gpio输出pwm
{
    write(fd, "1", 2);
    sleep(value);
    write(fd, "0", 2);
    sleep(value);
    return 0;
}

 /*
#if 0
int main(int argc, char *argv[])
{
    int i = 20;

    gpio_export(6);
    gpio_dir_out(6);

    while(i--) {
        gpio_value(6, i & 1);
        sleep(1);
    }

    gpio_unexport(6);
}
#endif
*/
