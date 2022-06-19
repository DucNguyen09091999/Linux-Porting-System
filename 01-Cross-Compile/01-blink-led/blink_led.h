#ifndef __BLINK_LED_H__
#define __BLINK_LED_H_

#define GPIO_0_ADDR_BASE     0x44E07000
#define GPIO_0_ADDR_SIZE     (0x44E07FFF - 0x44E07000)

#define GPIO_OE_OFFSET                0x134
#define GPIO_CLEARDATAOUT_OFFSET      0x190
#define GPIO_SETDATAOUT_OFFSET        0x194

#define LED (1 << 31) //P9_13 or GPIO_0_31 


#endif