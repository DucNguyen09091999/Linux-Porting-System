#ifndef __BUTTON_H__
#define __BUTTON_H__


#define GPIO_Number(port,bit) (32*port+bit)


/* GPIO address - size */
#define GPIO_0_ADDR_BASE    0x44E07000
#define GPIO_0_ADDR_SIZE	(0x44E07FFF - 0x44E07000)

/* Registers */
#define GPIO_OE_OFFSET			    0x134
#define GPIO_SETDATAOUT_OFFSET		0x194
#define GPIO_CLEARDATAOUT_OFFSET	0x190

/* GPIO Pin */
#define BUTTON_PIN     GPIO_Number(0,31)// P9_13 <=> GPIO_0_26 BUTTON
#define LED_PIN        GPIO_Number(0,30)// P9_11  <=> GPIO_0_31 LED


#endif  /*__BUTTON_H__ */
