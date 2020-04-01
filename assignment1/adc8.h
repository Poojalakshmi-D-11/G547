#ifndef ADC8_H
#define ADC8_H
#include <linux/ioctl.h>

#define MAJOR_NUM 250


#define SEL_CHANNEL _IOW(MAJOR_NUM, 0, unsigned int)
#define ALIGNMENT_LR _IOW(MAJOR_NUM, 1, unsigned int)
#define DEVICE_FILE_NAME "/dev/adc8"

#endif


