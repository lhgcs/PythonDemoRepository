#ifndef GPIO_H
#define GPIO_H

int gpio_export(unsigned gpio);
int gpio_unexport(unsigned gpio);
int gpio_dir_out(unsigned gpio);
int gpio_dir_in(unsigned gpio);
int gpio_value(unsigned gpio, unsigned value);

#endif // GPIO_H
