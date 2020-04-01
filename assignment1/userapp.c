#include "adc8.h"
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>              /* open */
#include <unistd.h>             /* exit */
#include <sys/ioctl.h>          /* ioctl */


int channel_no;
int channel_alignment;


int call_open()
{
	return open(DEVICE_FILE_NAME, 0);
}

int call_ioctl_channel(int channel_no,int file_desc)
{
	int ret_val;
	ret_val=ioctl(file_desc,SEL_CHANNEL,channel_no);
	if(ret_val<0) 
	{
		printf("cannot select\n");
		exit(-1);
	}
	return 0;
}

int ioctl_align(int channel_alignment,int file_desc)
{
	int ret_val;
	ret_val=ioctl(file_desc,ALIGNMENT_LR,channel_alignment);
	if(ret_val<0) 
	{
		printf("cannot align\n");
		exit(-1);
	}
	return 0;
}	

int call_read(file_desc)
{
	unsigned short voltage;
	read(file_desc, &voltage, sizeof(unsigned short));
	printf("adc out put is %d\n",voltage);
	printf("from channel number %d\n",channel_no);
	return 0;
}

int call_close(file_desc)
{
	close(file_desc);
	return 0;
}


int main()
{
	int file_desc;
	char yn;
	do{
	file_desc=call_open();
	printf("select one of the channel 0,1,2,3,4,5,6,7,\n");
	scanf("%d",&channel_no);
	call_ioctl_channel(channel_no,file_desc);
	printf("left or right aligned data on 16 bit register right=1,left=2\n");
	scanf("%d",&channel_alignment);
	ioctl_align(channel_alignment,file_desc);
	//call_open();
	call_read(file_desc);
	call_close(file_desc);
	printf("do you want to see next reading y or n");
	scanf(" %c",&yn);
	//if(yn!='y'||yn!='Y') break;
	}while(yn=='y');
}

