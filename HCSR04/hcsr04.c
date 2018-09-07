#include <device.h>
#include <zephyr/types.h>
#include <stdlib.h>
#include <kernel.h>
#include <string.h>
#include <board.h>
#include <sensor.h>
#include <gpio.h>
#include <errno.h>
#include <misc/byteorder.h>
#include <misc/__assert.h>
#include <asm_inline_gcc.h>
#include "hcsr04.h"


struct device *dev1;
struct device *dev2;
struct hcsr_data driver_0, driver_1;
struct sensor_value res;

u32_t stop_data_log;


static int sensor_channel(struct device *dev,
			       enum sensor_channel chan,
			       struct sensor_value *val)
{
	val-> val1 = res.val1;
	val-> val2 =k_cycle_get_32();
	return 0;	
}

static const struct sensor_driver_api hcsr_driver_api = 
{
	.sample_fetch = fetching,
	.channel_get = sensor_channel,
};


static int initialize(struct device *dev)
{

	printk("Initialize\n");
	struct hcsr_data *drv_data = dev->driver_data;
	pinmux = device_get_binding(CONFIG_PINMUX_NAME);

	int flag;

	printk("Gpio setting\n");

	dev1 = device_get_binding("GPIO_0");

	printk("setting end");

	drv_data-> function = handler;
	dev -> driver_api = &hcsr_driver_api;

	return 0;

}

static int fetching(struct device *dev, enum sensor_channel chan)
{
	int f;
	
	f=gpio_pin_write(dev1,6,1); 
	
	if(f<0)
		printk("Error");

	k_sleep(80);

	flag=gpio_pin_write(dev1,6,0); 
	if(flag<0)
		printk("Error");


	printk("pin write\n");
	
	k_sleep(500);

	return 0;
}

void handle(struct device *dev1, struct gpio_callback *call, u32_t pin)
	{
		long long str=0,stp=0, d=0;
		long long temp = _tsc_read();
		int value;
		gpio_pin_read(dev1,7,&value);
		

		if(value == 0)
		{
			str=temp;
			gpio_pin_configure(dev1,7,(GPIO_DIR_IN|GPIO_INT|GPIO_INT_EDGE|GPIO_INT_ACTIVE_HIGH));
		}

		else if(value == 1)
		{
			stp = temp;
			gpio_pin_configure(dev1,7,(GPIO_DIR_IN|GPIO_INT|GPIO_INT_EDGE|GPIO_INT_ACTIVE_LOW));
		}

		else if(0)
		{
			d = stp-str;
			res.val1=(d/400*58);
			printk("Difference %lld\n Distance= %u \n\n",d, result.val1);		
		}
	}

DEVICE_INIT(HCSR0, CONFIG_HCSR_NAME_0, initialize, &driver_0, NULL, POST_KERNEL, CONFIG_SENSOR_INIT_PRIORITY);
DEVICE_INIT(HCSR1, CONFIG_HCSR_NAME, initialize, &driver_1, NULL, POST_KERNEL, CONFIG_SENSOR_INIT_PRIORITY);
