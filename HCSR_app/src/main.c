#include <device.h>
#include <zephyr.h>
#include <gpio.h>
#include <board.h>
#include <misc/util.h>
#include <misc/printk.h>
#include <stdio.h>
#include <stdlib.h>
#include <shell/shell.h>
#include <pinmux.h>
#include <flash.h>
#include <kernel.h>




#define MUX CONFIG_PINMUX_NAME
#define MY_SHELL_MODULE "shell_mod"

struct device *pinmux;

int main()
{       

//defining variables
	struct device *dev,*fc_dev;
	int ret;
	u8_t cmp_data[64];
	u8_t data[64];
	int val,rc1,i=0;
	struct sensor_value dist;
	printk("in main\n");
	pinmux = device_get_binding(MUX);      //device binding
    
	if(pinmux==NULL)
     	printk("Error in pinmux\n");
	
//configuring pins
	pinmux_pin_set(pinmux, 18, PINMUX_FUNC_C);
	pinmux_pin_set(pinmux, 19, PINMUX_FUNC_C);

	printk("exp2 bind\n");
	dev = device_get_binding("EXP2");
	if(dev==NULL)
     	printk("Error in EXP2\n");
	ret=gpio_pin_write(dev,9,1);
    if(ret<0)
           printk("\nError in pin write of EXP2");
    ret=gpio_pin_write(dev,11,1);
      if(ret<0)
           printk("\nError in pin write of EXP2");

       printk("fc256 bind\n");

    fc_dev = device_get_binding("FC256_NAME");   //device binding
    if(fc_dev==NULL)
    	printk("driver not found\n");

       
    for(int j=0;j<5;j++)
    {
		k_sleep(100);
		printk("Inside for loop \n");
		
		ret=flash_write_protection_set(fc_dev, false);
		if(ret<0)
		printk("\nError in write protection");
		k_sleep(100);
		printk("trying to write\n");

		for (int i = 0; i < sizeof(cmp_data); i++) 
		{
			cmp_data[i] = k_cycle_get_32() & 0XFF;
			data[i] = 0x00;
		}

		/* write them to the FRAM */
		ret = flash_write(fc_dev, 0x00+(j*64), cmp_data,sizeof(cmp_data));
		if (ret) 
		{
			printk("Error writing to FC256! error code (%d)\n", ret);
			return 0;
		} 

		else 
		{
			printk("Wrote %zu bytes to address 0x00.\n", sizeof(cmp_data));
		}



		ret=flash_write_protection_set(fc_dev, true);//calling flash write protection
		if(ret<0)
			printk("\nError in WP");
			k_sleep(500);


		ret = flash_read(fc_dev, 0x00+(j*64),data,sizeof(data));//reading from flash memory
		if (ret) 
		{
			printk("Error reading from FC256!(%d)\n", ret);
			return 0;
		} 

		else 
		{
			printk("Read %zu bytes from address 0x00.\n", sizeof(data));
		}

			ret = 0;
		for (int i = 0; i < sizeof(cmp_data); i++) 
		{
			
			 printk("0x%X ?= 0x%X\n", cmp_data[i], data[i]); 
			if (cmp_data[i] != data[i]) //comparing data
			{
				printk("Comparison failed @ %d.\n", i);
				ret = -EIO;
			}	
		}
		if (ret == 0) 
		{
			printk("Comparison successful.\n");
		}

	}
	
	hcsr04=device_get_binding(CONFIG_HCSR04_NAME_0);
	if(!hcsr04)
		printf("not binding");
	else
		printf("%s\n",hcsr04->config-name);
	while(i<10)
	{
		rc1 =hcsr_write(hcsr04,CONFIG_GPIO_TRIGGER_PIN_6);
		k_sleep(20);
        }

   // int flash_erase(fc_dev, off_t offset, size_t size);
  return 0;
}
