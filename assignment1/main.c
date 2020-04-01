//this is program for kernel module creation
#include<linux/module.h>
#include<linux/version.h>
#include<linux/kernel.h>
#include<linux/types.h>
#include<linux/kdev_t.h>
#include<linux/fs.h>
#include<linux/cdev.h>
#include<linux/device.h>
#include<linux/init.h>
#include<linux/uaccess.h>
#include<linux/random.h>
#include <asm/io.h>
#include"adc8.h"

static dev_t first;

static struct cdev c_dev;

static struct class *cls;

unsigned short rand_v;
unsigned int chno,arg;
unsigned int a_l;
//call back functions

static int my_open(struct inode *i, struct file *f)
	{
		printk(KERN_INFO "ADC8 OPEN()\n");
		return 0;
	}
static int my_close(struct inode *i, struct file *f)

	{
		printk(KERN_INFO "ADC8 CLOSE()\n");
		return 0;
	}

//use reads from kernel space 

//adders modes might be differnt in user space and kerspace therfor follwong funtion is done

static ssize_t my_read(struct file *f, char __user *buf, size_t len, loff_t *off  )

{	int min;
	printk(KERN_INFO "ADC8: READ()\n");
	get_random_bytes(&rand_v, sizeof(unsigned short));
	outb_p(0x02,0x70);
	min=(inb(0x71))*60; //min to seconds 
	rand_v=rand_v+min;
	if(a_l==1) 
	{
		rand_v=rand_v>>6;
	}		
	else if(a_l==2)
	{
		rand_v=rand_v<<6;
	}
	if(copy_to_user(buf,&rand_v,sizeof(rand_v)))
	printk(KERN_INFO "SOME DATA IS MISSING");
	return sizeof(rand_v);

}

/////////////////ioctl def/////////

static long my_ioctl(struct file *f, unsigned int cmd, unsigned long arg)

{
 switch(cmd){

		case SEL_CHANNEL: 
			switch(arg)
			{
				case 0:
				case 1:
				case 2:
				case 3:
				case 4:
				case 5:
				case 6:
				case 7:chno=arg; break;
				default:return -1; 
			}
			break;
		
		case ALIGNMENT_LR: 
							if(arg==1) a_l=1;
							else if(arg==2) a_l=2;
							else return -1;
							break;
		default:-ENOTTY;
}

return 0;

}
//////////////////////////////////////

// instantiate fops

static struct file_operations fops= { 
										.owner= THIS_MODULE,
										.open=my_open,
										.release=my_close,
										.read =my_read,
										.unlocked_ioctl=my_ioctl
									};

static int __init mychar_i(void)

{	//registeing the device FILE

	printk(KERN_INFO "DEVICE FILE NAMED adc8 IS REGISTERED\n");
	if(alloc_chrdev_region(&first,0,3,"adc8")<0) 
	{
		return -1;
	}
	printk(KERN_INFO "MAJOR: %d, MINOR: %d\n",MAJOR(first),MINOR(first));
	//creartion of device

		if((cls=class_create(THIS_MODULE, "adc8"))==NULL) 
		{ 	
			unregister_chrdev_region(first,3);
			return -1;
		}

		if (device_create(cls,NULL, first, NULL, "adc8")==NULL)

			{
			class_destroy(cls);
			unregister_chrdev_region(first,3);
			return -1;
			}

	//link fops and cdev to device

	cdev_init(&c_dev, &fops);   //takes fops and initilize to cdev
		if(cdev_add(&c_dev, first, 3)==-1)
		{ 
			device_destroy(cls,first);
			class_destroy(cls);
			unregister_chrdev_region(first,3);
			return -1;
		}
	return 0;
}



static void __exit mychar_e(void)

{	
	cdev_del(&c_dev); 	
    device_destroy(cls,first);
	class_destroy(cls);
	unregister_chrdev_region(first,3);
	printk(KERN_INFO "adc8 unregistered\n");

}

module_init(mychar_i);
module_exit(mychar_e);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("POOJALAKSHMI");
MODULE_DESCRIPTION("SIMPLE ADC DRIVER");

//NOTE: attached .h file to link kernel and user space for ioctl program as macro name can be differ from one developer to other
//NOTE: git hub user name is: 2019h1400098g (full bits roll number as both pilani and goa has same email with diffent extention)
