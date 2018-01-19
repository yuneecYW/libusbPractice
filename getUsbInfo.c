#include <stdio.h>


#include "libusb.h"


int main()
{
    int ret, cnt;

    libusb_context *ctx;
    libusb_device **devs;
    struct libusb_device_descriptor desc;
    struct libusb_config_descriptor cfg;

    
    ret = libusb_init(&ctx);
    if(ret != 0) printf("libusb init failed\n");

    libusb_set_debug(ctx, 3);

    cnt = libusb_get_device_list(ctx, &devs);
    if(cnt < 0) printf("get device error!\n");

    for(int i=0; i < cnt; i++)
    {
        ret = libusb_get_device_descriptor(devs[i], &desc);
        if(ret != 0) printf("get device descriptor error!\n");

        printf("bLength:%d\n", desc.idVendor);
        printf("bDescriptorType:0x%02x\n", desc.bDescriptorType);
        printf("bcdUSB:%d.%d\n", (desc.bcdUSB>>8&0xff), desc.bcdUSB&0xff);
        printf("bDeviceClass:0x%02x\n", desc.bDeviceClass);
        printf("bDeviceSubClass:0x%02x\n", desc.bDeviceSubClass);
        printf("bMaxPacketSize0:%d\n", desc.bMaxPacketSize0);
        printf("idVendor:0x%04x\n", desc.idVendor);
        printf("idProduct:0x%04x\n", desc.idProduct);
        printf("bNumConfigurations:%d\n", desc.bNumConfigurations);
        printf("----------------------------------------------\n");

        // libusb_get_config_descriptor(devs[i], )

    }

    libusb_free_device_list(devs, 1);
    libusb_exit(ctx);

    return 0;
}