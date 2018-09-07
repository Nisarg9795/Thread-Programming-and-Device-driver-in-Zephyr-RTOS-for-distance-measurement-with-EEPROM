#include<device.h>
#include<kernel.h>
#include<gpio.h>
#include<i2c.h>
#include<misc/util.h>
#include<zephyr.h>
#include<flash.h>
#include<board.h>
#include <zephyr/types.h>
#include <misc/byteorder.h>
#include <misc/__assert.h>
#include <misc/printk.h>
#include <pinmux.h>


#define FC256_I2C_ADDR 0x50


struct fc256_data
{
	struct device *i2c;
	struct device *gpio_wp;
} fc256_data;

static int write_protection(struct device *dev, bool en)
{
	printk("In memory WP\n");
	
	struct fc256_data *data = dev->driver_data;
	if(en == false)
		gpio_pin_write(data->gpio_wp,3,0);
	else if(en == true)
		gpio_pin_write(data->gpio_wp,3,1);
	return 0;
}


static int write(struct device *dev, off_t offset, const void *data,size_t len)
{

	printk("Inside write\n");
	struct fc256_data *mem_write_data = dev->driver_data;
	u8_t wr_addr[2];
	struct i2c_msg msgs[2];

	wr_addr[0] = (offset >> 8) & 0xFF;
	wr_addr[1] = offset & 0xFF;

	msgs[0].buf = wr_addr;
	msgs[0].len = 2;
	msgs[0].flags = I2C_MSG_WRITE;

	msgs[1].buf = data;
	msgs[1].len = len;
	msgs[1].flags = I2C_MSG_WRITE | I2C_MSG_STOP;

	return i2c_transfer(mem_write_data->i2c, &messages[0], 2, FC256_I2C_ADDR);
	return 0;
}

static int read(struct device *dev, off_t offset, void *data,size_t len)
{
	
	printk("Inside read\n");
	struct fc256_data *mem_read_data = dev->driver_data;
	
	u8_t addr[2];
	struct i2c_msg message[2];

	/* FRAM address */
	rd_addr[0] = (offset >> 8) & 0xFF;
	rd_addr[1] = offset & 0xFF;

	// Send the address to read from 
	messages[0].buf = addr;
	messages[0].len = 2;
	messages[0].flags = I2C_MSG_WRITE;

	/* Read from device. STOP after this. */
	messages[1].buf = data;
	messages[1].len = len;
	messages[1].flags = I2C_MSG_READ | I2C_MSG_STOP;

	return i2c_transfer(mem_read_data->i2c, &messages[0], 2, FC256_I2C_ADDR);

	return 0;
}

static int erase(struct device *dev, off_t offset, size_t size)
{
	printk("Memory erase\n");
	return 0;
}

static int init(struct device *dev)
{
	int ret=1;
	struct fc256_data *data = dev->driver_data;
	struct device *dev_gpio = device_get_binding("EXP1");
	if(dev_gpio==NULL)
		printk("Error\n");
	gpio_pin_configure(dev_gpio,0,GPIO_DIR_OUT);
	ret = gpio_pin_write(dev_gpio,0,0);
	gpio_pin_configure(dev_gpio,1,GPIO_DIR_OUT);
	ret = gpio_pin_write(dev_gpio,1,0);
	data->gpio_wp = device_get_binding("GPIO_0");
	gpio_pin_configure(data->gpio_wp,3,GPIO_DIR_OUT);
	
	struct fc256_data *drv_data = dev->driver_data;
	printk("Trying to bind\n");
	drv_data->i2c = device_get_binding(CONFIG_FC256_I2C_MASTER);

	if (drv_data->i2c == NULL) {
		printk("Failed to get pointer to %s device!",CONFIG_FC256_I2C_MASTER);
		return -EINVAL;
	}
	return 0;

}


static const struct flash_driver_api fc256_api = {
	.read = read,
	.write = write,
	.erase = erase,
	.write_protection = write_protection
};

DEVICE_AND_API_INIT(FC256_NAME,CONFIG_FC256_DRV_NAME,mem_init,&fc256_data, NULL,POST_KERNEL, 90, &fc256_api);
