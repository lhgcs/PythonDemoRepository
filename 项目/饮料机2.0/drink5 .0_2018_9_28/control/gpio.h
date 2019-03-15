/*
 * gpio.h
 *
 *  Created on: 4 Jul 2016
 *      Author: xiao
 */

#ifndef GPIO_H_
#define GPIO_H_

#include <errno.h>

/* pin group base number name space,
 * the max pin number : 26*32=832.
 */
#define SUNXI_BANK_SIZE 32
#define SUNXI_PA_BASE	0
#define SUNXI_PB_BASE	32
#define SUNXI_PC_BASE	64
#define SUNXI_PD_BASE	96
#define SUNXI_PE_BASE	128
#define SUNXI_PF_BASE	160
#define SUNXI_PG_BASE	192
#define SUNXI_PH_BASE	224
#define SUNXI_PI_BASE	256
#define SUNXI_PJ_BASE	288
#define SUNXI_PK_BASE	320
#define SUNXI_PL_BASE	352
#define SUNXI_PM_BASE	384
#define SUNXI_PN_BASE	416
#define AXP_PIN_BASE	1024

#define SUNXI_PIN_NAME_MAX_LEN	8

#define GPIO_IN    0x01
#define GPIO_OUT   0X02
#define GPIO_INVAL 0X03

#define NUM_MAXLEN  10
#define GPIO_MAXLEN 50

class GPIO{
private:
    int fd_gpio;
    char direction;                 //gpio方向
    char directionPath[GPIO_MAXLEN];
    char valuePath[GPIO_MAXLEN];
    char num[NUM_MAXLEN];
	int sunxi_name_to_gpio(const char *name, char *gpio);

public:
	GPIO();
    int init(char const *pinName);
	int exit();

    int get_gpio_path(char *pinNum);
    void get_gpio_direct_level_path(char *pinNum);
    int set_direction(int flags);

    int gpio_open();
    int gpio_close(int fd);

    static int set_gpio_out_level(int fd,int level);
    static int get_gpio_in_level(int fd);

    static int gpio_out_init(char *gpioName,int level);
    static int gpio_in_init(char gpioName[]);

    /*
    ARM 平台可能是这样：
    # cat /sys/devices/virtual/thermal/thermal_zone0/temp
    # cat /sys/class/thermal/thermal_zone0/temp
    x86 平台可能是这样：
    # cat /proc/acpi/thermal_zone/THRM/temperature
    */
    static int get_cpu_temperature();
};


#endif /* GPIO_H_ */
