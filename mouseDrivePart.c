#include <stdio.h>
#include <signal.h>

#include <libusb.h>
// #include "libusbi.h"


int main()
{
    int ret;
    ssize_t cnt;

    libusb_device **devs;
    libusb_device_handle *dev_hd = NULL;
    libusb_context *ctx = NULL;

    
    ret = libusb_init(&ctx);
    if(ret < 0)
    {
        printf("init failed!\n");
        return -1;
    }
    libusb_set_debug(ctx, 3);

    cnt = libusb_get_device_list(ctx, &devs);
    if(cnt < 0)
    {
        printf("get device list failed!\n");
        return -1;
    }
    else
        printf("dev total:%d\n", cnt);


    dev_hd = libusb_open_device_with_vid_pid(ctx, 0x10c4, 0x8108);

    if(dev_hd == NULL)
    {
        printf("yw:open failed!\n");
        libusb_free_device_list(devs, 1);
        return -1;        
    }
    else
    {
        printf("yw:open successed!\n");
        libusb_free_device_list(devs, 1);
        if(libusb_kernel_driver_active(dev_hd, 0) == 1)
        {
            printf("kernel driver is active\n");
            if(libusb_detach_kernel_driver(dev_hd, 0) == 0)
            {
                printf("detach driver is success!\n");
            }
        }
        ret = libusb_claim_interface(dev_hd, 0);
        if(ret != 0)
        {
            printf("claim interface failed\n");
            libusb_close(dev_hd);
            return -1;
        }
        else
            printf("claim interface success\n");

        int size;
        unsigned char datain[1024] ="\0";
        for(int i=0; i<300; i++)
        {
            int rr = libusb_interrupt_transfer(dev_hd,
                                                0x81,
                                                datain,
                                                0x0004,
                                                &size,
                                                1000);
            printf("rr:%d;\tsize:%d;\ndata:\n", rr, size);
            for(int j=0; j<size; j++)
            {
                printf("%02x  ", (unsigned char)(datain[j]));
            }
            printf("\n");
        }
        ret = libusb_release_interface(dev_hd, 0);
        if(ret != 0)
        {
            printf("release interface faile\n");
            libusb_close(dev_hd);  
            libusb_exit(ctx); 
            return -1;
        }

        printf("release interface success!\n");
        ret = libusb_attach_kernel_driver(dev_hd, 0);   
        printf("attach:%d\n", ret);

   
  
    }


    libusb_close(dev_hd);  
    libusb_exit(ctx); 

    return 0;
}