#include <cstdio>
#include <cstring>
#include <cerrno>
#include <unistd.h>
#include <termios.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "gpio.h"


//:-1: error: gpio.o: undefined reference to symbol 'clock_gettime@@GLIBC_2.4'



#define DBG_EN 	0
#if (DBG_EN == 1)
#define printf_dbg(x,arg...) printf("[gpio_debug]"x,##arg)
#else
#define printf_dbg(x,arg...)
#endif

#define printf_info(x,arg...) printf("[gpio_info]"x,##arg)
#define printf_warn(x,arg...) printf("[gpio_warnning]"x,##arg)
#define printf_err(x,arg...)  printf("[gpio_error]"x,##arg)


GPIO::GPIO(){
	num[0] = '\0';
    fd_gpio=-1;
}

/**
  根据引脚名获取编号
 * @brief GPIO::sunxi_name_to_gpio
 * @param name
 * @param gpio
 * @return
 */
int GPIO::sunxi_name_to_gpio(char const *name, char *gpio){
	char bank;
	int num;

	if (!name || !gpio) {
		return -EINVAL;
	}

    sscanf(name,"P%c%d", &bank, &num);//从一个字符串中读进与指定格式相符的数据
	if(num >= 32 || num < 0) {
		return -EINVAL;
	}

	if (bank >= 'a' && bank <= 'z') {
        num = num + SUNXI_BANK_SIZE * (bank - 'a');printf("%d",num);
	} else if (bank >= 'A' && bank <= 'Z') {
        num = num + SUNXI_BANK_SIZE * (bank - 'A');printf("%d",num);
	} else {
		return -EINVAL;
	}

	return sprintf(gpio, "%d",  num);
}
/**
  导出设备节点
 * @brief GPIO::init
 * @param pin_name
 * @return
 */
int GPIO::init(char const *pinName){
	int ret;

    if(!pinName) {
		return -EINVAL;
	}
	if(num[0] != '\0') {
        memset(num,0,sizeof(num));
	}
    ret = sunxi_name_to_gpio(pinName, num);//引脚名称转换为数字编号
	if(ret < 0) {
		printf_err("get name fail,%d\n",ret);
		return -ret;
	}
	direction = GPIO_INVAL;

    get_gpio_path(num);
    get_gpio_direct_level_path(num);
	return 0;
}
/**
  导出GPIO管脚
 * @brief get_gpio_path
 * @param pinNum
 * @return
 */
int GPIO::get_gpio_path(char *pinNum){
    int ret=-1;
    int fd = open("/sys/class/gpio/export",O_WRONLY);//导出
    if(fd < 0){
        printf_err("Export gpio fail, %s(%d)\n", strerror(errno), errno);
        return fd;
    }
    printf_dbg("init num:%s\n", pinNum);
    ret = write(fd, pinNum, NUM_MAXLEN);//写入GPIO编号
    if(ret < 0) {
        //printf_err("gpio write init error.(%d)\n",ret);
        int fdd = open("/sys/class/gpio/unexport", O_WRONLY);//取消导出
        if(fdd == -1){
            printf_err("Open and try unexport gpio error\n");
            return -1;
        } else {
            printf_dbg("try unexport num:%s\n",pinNum);
            ret = write(fdd, pinNum, NUM_MAXLEN);
            if(ret < 0) {
                printf_err("ERR: gpio write unexport error.\n");
            }
            close(fdd);
        }

        ret = write(fd, pinNum, NUM_MAXLEN);
        if(ret < 0) {
            printf_err("Try again gpio write init error.\n");
            return ret;
        }
    }
    close(fd);
    return ret;
}
/**
  获取设置方向和设置电平的路径
 * @brief get_gpio_direct_level_path
 * @param pinNum
 */
void GPIO::get_gpio_direct_level_path(char *pinNum){
    char gpio_path[50] = "/sys/class/gpio/gpio";
    memset(directionPath,0,sizeof(directionPath));
    strcpy(directionPath,gpio_path);
    strcat(directionPath,pinNum);
    strcat(directionPath,"/direction");
    printf_dbg("gpio direction path is : %s\n", directionPath);
    ///sys/class/gpio/gpio100/direction

    memset(valuePath,0,sizeof(valuePath));
    strcpy(valuePath, gpio_path);
    strcat(valuePath, pinNum);
    strcat(valuePath,"/value");
    printf_dbg("gpio value path is : %s\n", valuePath);
    ///sys/class/gpio/gpio100/value
}

int GPIO::exit(){
    int fd;
	int ret = 0;

	if(num[0] == '\0') {
		return -EINVAL;
	}

    fd = open("/sys/class/gpio/unexport", O_WRONLY);
    if(fd == -1){
		printf_err("open unexport file fail.\n");
	}
    printf_dbg("exit num:%s\n",num);

    ret = write(fd, num, NUM_MAXLEN);
	if(ret < 0) {
		printf_err("write exit error.\n");
	}
    close(fd);

	direction = GPIO_INVAL;
	num[0] = '\0';
	return ret;
}
/**
  设置方向
 * @brief GPIO::set_direction
 * @param flags
 * @return
 */
int GPIO::set_direction(int flags){
    int fd;
	int ret = 0;

    fd = open(directionPath, O_WRONLY);
    if(fd == -1){
        printf_err("open %s fail\n", directionPath);
		return -EIO;
	}

	//设置GPIO方向
    switch (flags) {
        case GPIO_OUT:{
            ret = write(fd,"out",sizeof("out"));
            if(ret > 0) {
                direction = GPIO_OUT;
            }
        }
        break;
        case GPIO_IN:{
            ret= write(fd,"in",sizeof("in"));
            if(ret > 0) {
                direction = GPIO_IN;
            }
        }
        break;
        default:{
            ret=-1;
            return -EINVAL;
        }
        break;
    }
	if(ret < 0) {
		printf_err("ERR: gpio direction write error.\n");
        direction = GPIO_INVAL;
	}
    close(fd);
	return ret;
}
/**
  输出
 * @brief GPIO::gpio_out_open
 * @return
 */
int GPIO::gpio_open(){
    int fd;
    fd = open(valuePath, O_WRONLY);
    if(fd == -1){
        printf_err("open %s\n",valuePath);
		return -EIO;
    }
    return fd;
}
int GPIO::gpio_close(int fd){
    if(fd < 0) {
        return -EINVAL;
	}
    close(fd);
	return 0;
}
int GPIO::set_gpio_out_level(int fd,int level){
    int res=-1;
    if(fd>0){
        if(level > 0){
            res=write(fd,"1",1);
        }else{
            res=write(fd,"0",1);
        }
    }
    return res;
}
int GPIO::get_gpio_in_level(int fd){
    int a;
    if(fd>0&&read(fd,&a,1)>0)
        return a;
    else
        return -1;
}

int GPIO::gpio_out_init(char *gpioName,int level){
    GPIO gpio;
    int fd=gpio.init(gpioName);
    if(fd < 0) {
        printf("init gpio fail,%d\n",fd);
        return -1;
    }
    fd = gpio.set_direction(GPIO_OUT);
    if(fd < 0) {
        printf("set gpio output direction fail, %d\n",fd);
        return -1;
    }
    fd = gpio.gpio_open();
    if(fd < 0) {
        printf("gpio output open fail\n");
        return -1;
    }
    return set_gpio_out_level(fd,level);
}
int GPIO::gpio_in_init(char gpioName[]){
    GPIO gpio;
    int fd=gpio.init(gpioName);
    if(fd < 0) {
        printf("init gpio fail,%d\n",fd);
        return -1;
    }
    fd = gpio.set_direction(GPIO_IN);
    if(fd < 0) {
        printf("set gpio input direction fail, %d\n",fd);
        return -1;
    }
    return fd;
}


#define _POSIX_C_SOURCE 199309L // enable interface for struct timespec ...

#include <time.h>               // struct timespec, clock_gettime
#include <stdio.h>              // FILE, fscanf, stdout, printf

const unsigned int N = 1000;

typedef struct
   {struct timespec t;
    unsigned        temp;}
RecT;

int get_cpu_temperature(){
    RecT r [N];
    for (unsigned i = 0; i < N; ++i){
        (void) clock_gettime (CLOCK_REALTIME, &r [i].t);
        FILE *fp = fopen ("/sys/devices/virtual/thermal/thermal_zone0/temp", "r");
        (void) fscanf (fp, "%u", &r[i].temp);
        (void) fclose (fp);
    }
    for (unsigned i = 0; i < N; ++i){
        printf ("%lu.%06llu %u\n", r [i].t.tv_sec, (r [i].t.tv_nsec + 500ull) / 1000ull, r [i].temp);
    }
    return 0;
}


