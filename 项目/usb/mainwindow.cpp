#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>

//#define USB_VENDOR_ID   0xe851
//#define USB_PRODUCT_ID  0x1000
//VID:PID
//#define VID             0xe851  //生产商ID
//#define PID             0x1000  //设备ID

//#define VID             0x1a2c  //生产商ID
//#define PID             0x2c27  //设备ID


//#define USB_VID         0x1a2c          //USB设备的产商ID
//#define USB_PID         0x2c27         //USB设备的产品ID

//#define USB_VID         0x04f3          //USB设备的产商ID
//#define USB_PID         0x0103         //USB设备的产品ID

#define USB_VID         0xffff          //USB设备的产商ID
#define USB_PID         0x0035         //USB设备的产品ID


#include "usb.h"
#include <unistd.h>
#include <stdio.h>

//#define USB_ENDPOINT_IN         0x80
//#define USB_TYPE_CLASS          (0x01 << 5)
//#define USB_RECIP_INTERFACE     0x01
//#define HID_REPORT_GET          0x01

//#define EP0ADDR         0x01            //Write端口0地址，通道0
//#define EP1ADDR         0x81            //Read 端口1地址，通道1   129
//#define EP2ADDR         0x02            //Write端口2地址，通道2
//#define EP3ADDR         0x82            //Read 端口3地址，通道3    130
//#define USB_TIMEOUT     10000           //传输数据的时间延迟



#include <usb.h>//libusb-0.1.12库的头文件

#define EP0ADDR         0x01            //端口0地址，通道0
#define EP1ADDR         0x81            //端口1地址，通道1
#define EP2ADDR         0x02            //端口2地址，通道2
#define EP3ADDR         0x82            //端口3地址，通道3
#define USB_TIMEOUT     10000           //传输数据的时间延迟

/********** IMAGE ************/
#define IR_ROW 288     //IMAGE Line
#define COL 512*2      //IMAGE Column
#define IR_IMAGE_SIZE       IR_ROW*COL*2        //IMAGE一帧图像的大小

static struct usb_bus *bus = NULL;
static struct usb_device* dev = NULL;
static usb_dev_handle *device_handle = NULL;





//void printdev(libusb_device *dev){
//    libusb_device_descriptor desc;
//    int r = libusb_get_device_descriptor(dev, &desc);
//    if(r <0){
//        printf("failed to get device descriptor\n");
//        return;
//    }

//    printf("Number of possible configurations: %d\n,Device Class: %d\n",(int)desc.bNumConfigurations,(int)desc.bDeviceClass);
//    printf("VendorID: %x\n",desc.idVendor);
//    printf("ProductID: %x\n",desc.idProduct);

//    libusb_config_descriptor *config;
//    libusb_get_config_descriptor(dev, 0, &config);
//    printf("Interfaces: %d\n",(int)config->bNumInterfaces);
//    const libusb_interface *inter;
//    const libusb_interface_descriptor *interdesc;
//    const libusb_endpoint_descriptor *epdesc;

//    for(int i=0; i<(int)config->bNumInterfaces; i++){
//        inter =&config->interface[i];
//        printf("Number of alternate settings: %d\n",inter->num_altsetting);
//        for(int j=0; j<inter->num_altsetting; j++){
//            interdesc =&inter->altsetting[j];
//            printf("Interface Number: %d\n",(int)interdesc->bInterfaceNumber);
//            printf("Number of endpoints: %d\n",(int)interdesc->bNumEndpoints);

//                for(int k=0; k<(int)interdesc->bNumEndpoints; k++){
//                    epdesc =&interdesc->endpoint[k];
//                    printf("Descriptor Type: %d\n",(int)epdesc->bDescriptorType);
//                    printf("EP Address: %d\n",(int)epdesc->bEndpointAddress);

//                    const struct libusb_endpoint_descriptor *endpoint = &config->interface[i].altsetting[j].endpoint[k];
//                    if (endpoint->bEndpointAddress & LIBUSB_ENDPOINT_IN)
//                                        {
//                                            endpoint_in = endpoint->bEndpointAddress;
//                                        }
//                                        else
//                                        {
//                                            endpoint_out = endpoint->bEndpointAddress;
//                                        }
//                }
//        }
//    }
//    libusb_free_config_descriptor(config);
//}





//void print_dev(){
//   usb_device **devs;
//        usb_device *usb_dev;
//        ssize_t num_devs;
//        int res;
//        int d = 0;
//        int good_open = 0;

//       // setlocale(LC_ALL,"");

//        num_devs = libusb_get_device_list(NULL, &devs);
//        while ((usb_dev = devs[d++]) != NULL) {
//            struct libusb_device_descriptor desc;
//            struct libusb_config_descriptor *conf_desc = NULL;
//            int i,j,k;
//            libusb_get_device_descriptor(usb_dev, &desc);

//            if (libusb_get_active_config_descriptor(usb_dev, &conf_desc) < 0)
//                continue;
//            for (j = 0; j < conf_desc->bNumInterfaces; j++) {
//                const struct libusb_interface *intf = &conf_desc->interface[j];
//                for (k = 0; k < intf->num_altsetting; k++) {
//                    const struct libusb_interface_descriptor *intf_desc;
//                    intf_desc = &intf->altsetting[k];
//                    if (intf_desc->bInterfaceClass == LIBUSB_CLASS_HID) {
//                        char *dev_path = make_path(usb_dev, intf_desc->bInterfaceNumber);

//                        printf("%s\n",dev_path);

//                        if (!strcmp(dev_path, path)) {
//                            /* Matched Paths. Open this device */

//                            // OPEN HERE //
//                            res = libusb_open(usb_dev, &dev->device_handle);
//                            if (res < 0) {
//                                LOG("can't open device\n");
//                                printf("cannot open device\n");
//                                free(dev_path);
//                                break;
//                            }
//                            good_open = 1;

//                            /* Detach the kernel driver, but only if the
//                               device is managed by the kernel */


//                            /* Store off the string descriptor indexes */
//                            dev->manufacturer_index = desc.iManufacturer;
//                            dev->product_index      = desc.iProduct;
//                            dev->serial_index       = desc.iSerialNumber;

//                            /* Store off the interface number */
//                            dev->interface = intf_desc->bInterfaceNumber;

//                            /* Find the INPUT and OUTPUT endpoints. An
//                               OUTPUT endpoint is not required. */
//                            for (i = 0; i < intf_desc->bNumEndpoints; i++) {
//                                const struct libusb_endpoint_descriptor *ep
//                                    = &intf_desc->endpoint[i];

//                                /* Determine the type and direction of this
//                                   endpoint. */
//                                int is_interrupt =
//                                    (ep->bmAttributes & LIBUSB_TRANSFER_TYPE_MASK)
//                                      == LIBUSB_TRANSFER_TYPE_INTERRUPT;
//                                int is_output =
//                                    (ep->bEndpointAddress & LIBUSB_ENDPOINT_DIR_MASK)
//                                      == LIBUSB_ENDPOINT_OUT;
//                                int is_input =
//                                    (ep->bEndpointAddress & LIBUSB_ENDPOINT_DIR_MASK)
//                                      == LIBUSB_ENDPOINT_IN;

//                                /* Decide whether to use it for intput or output. */
//                                if (dev->input_endpoint == 0 &&
//                                    is_interrupt && is_input) {
//                                    /* Use this endpoint for INPUT */
//                                    dev->input_endpoint = ep->bEndpointAddress;
//                                    dev->input_ep_max_packet_size = ep->wMaxPacketSize;
//                                }
//                                if (dev->output_endpoint == 0 &&
//                                    is_interrupt && is_output) {
//                                    /* Use this endpoint for OUTPUT */
//                                    dev->output_endpoint = ep->bEndpointAddress;
//                                }
//                            }

//                            pthread_create(&dev->thread, NULL, read_thread, dev);

//                            // Wait here for the read thread to be initialized.
//                            pthread_barrier_wait(&dev->barrier);

//                        }
//                        free(dev_path);
//                    }
//                }
//            }
//            libusb_free_config_descriptor(conf_desc);

//        }

//        libusb_free_device_list(devs, 1);

//        // If we have a good handle, return it.
//        if (good_open) {
//            return dev;
//        }
//        else {
//            // Unable to open any devices.
//            free_hid_device(dev);
//            return NULL;
//        }
//    }
//}





struct usb_device* find_device(int usb_vid, int usb_pid)
{
        /* 1. 初始化相关数据并寻找相关设备，一开始就要调用 */
        usb_init();

        /* 2. 寻找系统上的usb总线，任何usb设备都通过usb总线与主机通讯，返回总线数 */
        usb_find_busses();

        /* 3. 寻找usb总线上的设备*/
        usb_find_devices();

        /* 4. 获得系统总线链表的句柄，modified by Su */
        for (bus = usb_busses; bus; bus = bus->next)
        {
                /* 遍历总线上的设备 */
                for (dev = bus->devices; dev; dev = dev->next)
                {
                        if(dev->descriptor.idVendor == usb_vid && dev->descriptor.idProduct == usb_pid) {
                                printf("Found USB_DEVICE！！！\n");
                                printf("%04x:%04x",usb_vid,usb_pid);
                                return dev;
                        }



                }
        }

        return NULL;
}

usb_dev_handle* open_device(struct usb_device* udev)
{
        if(udev != NULL) {
                device_handle = usb_open(udev);

//                /* 进行设备的初始化
//                 1.设置当前的设备使用的configuration,参数2是要使用配置描述符中的bConfigurationValue */
//                usb_set_configuration(device_handle, 1);

//                /* 2.注册与操作系统通讯的接口,必须被调用，只有注册接口才能做相应的操作，参数2指bInterfaceNumber */
//                usb_claim_interface(device_handle, 0);

//                /* 3.设置当前的设备使用的interface descriptor，参数2是指向接口描述符中的bAlternateSetting */
//                usb_set_altinterface(device_handle, 0);


//                usb_set_configuration(device_handle,1);
//                usb_claim_interface(device_handle,0);

              //   hid_set_nonblocking(handle, 0);//阻塞





//                if (usb_kernel_driver_active(device_handle, intf_desc->bInterfaceNumber) == 1) {
//                    res = libusb_detach_kernel_driver(dev->device_handle, intf_desc->bInterfaceNumber);
//                    if (res < 0) {
//                        libusb_close(dev->device_handle);
//                        LOG("Unable to detach Kernel Driver\n");
//                        printf("cannot kernal\n");
//                        free(dev_path);
//                        good_open = 0;
//                        break;
//                    }
//                }

//                res = libusb_claim_interface(dev->device_handle, intf_desc->bInterfaceNumber);
//                if (res < 0) {
//                    LOG("can't claim interface %d: %d\n", intf_desc->bInterfaceNumber, res);
//                    printf("can't claim interface\n");
//                    free(dev_path);
//                    libusb_close(dev->device_handle);
//                    good_open = 0;
//                    break;
//                }

        }
        return device_handle;
}


//static int return_data(struct usb_device *dev, unsigned char *data, int length)
//{
//    /* Copy the data out of the linked list item (rpt) into the
//       return buffer (data), and delete the liked list item. */
//    struct input_report *rpt = dev->input_reports;
//    size_t len = (length < rpt->len)? length: rpt->len;
//    if (len > 0)
//        memcpy(data, rpt->data, len);
//    dev->input_reports = rpt->next;
//    free(rpt->data);
//    free(rpt);
//    return len;
//}


int bulk_read_data(usb_dev_handle* device_handle, char* data_ir){

        int ret = -1;
        char cmd_ir_start[50] = {0x55, 0xaa, 0x00, 0x00,0x05};    //读Image指令
        char cmd_stop[50] = {0x5e, 0xaa};                         //结束指令
        char cmd_state[64];

        char mybuff[16]={0};

        if(device_handle == NULL){
            printf("device_handle is null\n");
        }else{
            printf("device_handle is ok\n");
        }
        //0x02为bulk类型的OUT端点
        //0x81为bulk类型的IN端点
        int t=usb_bulk_read(device_handle,0x81,mybuff,sizeof(mybuff),1000);
        if (t < 0 )
        {
            printf (" 81: ReadFile failed with error\n");

        }else{
            printf ("81read: %d %s\n",t,mybuff);
        }

        t=usb_bulk_read(device_handle,0x1,mybuff,sizeof(mybuff),1000);
        if (t < 0 )
        {
            printf (" 1Error: ReadFile failed with error\n");

        }else{
            printf ("1read: %d %s\n",t,mybuff);
        }


//        int res = libusb_interrupt_transfer(device_handle, dev->input_endpoint, data, length, &transferred, 5000);



        /* 1. 发送读Image数据指令，使用0号通道 */
        ret = usb_bulk_write(device_handle, EP0ADDR, cmd_ir_start, 5, USB_TIMEOUT);
        if(ret < 0){
                printf("Failed to write the start transfer camand!   %d \n",ret);
        }
        /* 2. 读Image数据, 使用通道3，最小2KB,最大3MB */
        ret = usb_bulk_read(device_handle, EP3ADDR, data_ir, IR_IMAGE_SIZE, USB_TIMEOUT);
        //printf("Have read data length is: %d\n", ret);
        if(ret != IR_IMAGE_SIZE){
                printf("Failed to read data!\n");
        }

        /* 3. 发送结束指令（以0x5e,0xaa开头，使用通道0, 最大64字节）*/
        ret = usb_bulk_write(device_handle, EP0ADDR, cmd_stop, 2, USB_TIMEOUT);
        if(ret < 0){
                printf("Failed to write the stop transfer camand!\n");
        }

        /* 4. 读状态指令（以0x5e,0xaa开头，使用通道1, 最大64字节）*/
        ret = usb_bulk_read(device_handle, EP1ADDR, cmd_state, sizeof(cmd_state), USB_TIMEOUT);
        if(ret < 0){
                printf("Failed to read data!\n");
        }
        return ret;
}

int close_usb_handle(usb_dev_handle* device_handle){
    usb_release_interface(device_handle, 0);
    usb_close(device_handle);
    return 0;
}

int print_data(char* data_ir){

     int i = 0;
     printf("*** Start To Reading Data After Read operation***\n");
     for( i= 1024 * 240;i <1024 * 240 + 100 ;i++){
            printf("0x%x\t  ",data_ir[i]);
            if((i+1) %10 == 0){
                printf("\n");
            }
     }
    printf("***Reading Data Done, ***\n");

    return 0;
}




MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    int ret = 0, i = 0;
    char data_ir[IR_IMAGE_SIZE] = {0};

    struct usb_device* device = NULL;
    usb_dev_handle* device_handle = NULL;;
    //1. 通过USB的VID与PID找到设备
    if((device = find_device(USB_VID, USB_PID)) == NULL) {
          printf("USB_DEVICE not found!\n");
          exit(0);
    }
    //2. 打开找到的设备返回传输数据所要操作的句柄Handle
    if((device_handle=open_device(device)) == NULL) {
          printf("Open USB_DEVICE failed!\n");
          exit(0);
    }

    while(1){
    //3. 数据是实时的，采用while(1)读取数据
        ret = bulk_read_data(device_handle, data_ir);
        if(ret <= 0){
            //printf("Read Data fail!\n");
        }else{
            //打印数据的测试函数
            print_data(data_ir);
        }
        sleep(1);
    }
    //4. 关闭USB设备
    close_usb_handle(device_handle);
}











//static void print_devs(libusb_device **devs){
//    libusb_device *dev;
//    int i = 0;

//    while ((dev = devs[i++]) != NULL) {
//        struct libusb_device_descriptor desc;
//        int r = libusb_get_device_descriptor(dev, &desc);
//        if (r < 0) {
//                fprintf(stderr, "failed to get device descriptor");
//                return;
//        }

//        printf("%04x:%04x (bus %d, device %d)\n",
//            desc.idVendor, desc.idProduct,
//            libusb_get_bus_number(dev),
//            libusb_get_device_address(dev));
//    }
//}

//libusb_device_handle *dev_handle=NULL;
//quint8 endpoint_in, endpoint_out;

//void printdev(libusb_device *dev){
//    libusb_device_descriptor desc;
//    int r = libusb_get_device_descriptor(dev, &desc);
//    if(r <0){
//        printf("failed to get device descriptor\n");
//        return;
//    }

//    printf("Number of possible configurations: %d\n,Device Class: %d\n",(int)desc.bNumConfigurations,(int)desc.bDeviceClass);
//    printf("VendorID: %x\n",desc.idVendor);
//    printf("ProductID: %x\n",desc.idProduct);

//    libusb_config_descriptor *config;
//    libusb_get_config_descriptor(dev, 0, &config);
//    printf("Interfaces: %d\n",(int)config->bNumInterfaces);
//    const libusb_interface *inter;
//    const libusb_interface_descriptor *interdesc;
//    const libusb_endpoint_descriptor *epdesc;

//    for(int i=0; i<(int)config->bNumInterfaces; i++){
//        inter =&config->interface[i];
//        printf("Number of alternate settings: %d\n",inter->num_altsetting);
//        for(int j=0; j<inter->num_altsetting; j++){
//            interdesc =&inter->altsetting[j];
//            printf("Interface Number: %d\n",(int)interdesc->bInterfaceNumber);
//            printf("Number of endpoints: %d\n",(int)interdesc->bNumEndpoints);

//                for(int k=0; k<(int)interdesc->bNumEndpoints; k++){
//                    epdesc =&interdesc->endpoint[k];
//                    printf("Descriptor Type: %d\n",(int)epdesc->bDescriptorType);
//                    printf("EP Address: %d\n",(int)epdesc->bEndpointAddress);

//                    const struct libusb_endpoint_descriptor *endpoint = &config->interface[i].altsetting[j].endpoint[k];
//                    if (endpoint->bEndpointAddress & LIBUSB_ENDPOINT_IN)
//                                        {
//                                            endpoint_in = endpoint->bEndpointAddress;
//                                        }
//                                        else
//                                        {
//                                            endpoint_out = endpoint->bEndpointAddress;
//                                        }
//                }
//        }
//    }
//    libusb_free_config_descriptor(config);
//}

////static int print_f0_data(void)
////{
////    unsigned char data[0x10];
////    int r;
////    unsigned int i;

////    r = libusb_control_transfer(devh, CTRL_IN, USB_RQ, 0xf0, 0, data,
////        sizeof(data), 0);
////    if (r < 0) {
////        fprintf(stderr, "F0 error %d\n", r);
////        return r;
////    }
////    if ((unsigned int) r < sizeof(data)) {
////        fprintf(stderr, "short read (%d)\n", r);
////        return -1;
////    }

////    printf("F0 data:");
////    for (i = 0; i < sizeof(data); i++)
////        printf("%02x ", data[i]);
////    printf("\n");
////    return 0;
////}

////static int get_hwstat(unsigned char *status)
////{
////    int r;

////    r = libusb_control_transfer(devh, CTRL_IN, USB_RQ, 0x07, 0, status, 1, 0);
////    if (r < 0) {
////        fprintf(stderr, "read hwstat error %d\n", r);
////        return r;
////    }
////    if ((unsigned int) r < 1) {
////        fprintf(stderr, "short read (%d)\n", r);
////        return -1;
////    }

////    printf("hwstat reads %02x\n", *status);
////    return 0;
////}

////static int set_hwstat(unsigned char data)
////{
////    int r;

////    printf("set hwstat to %02x\n", data);
////    r = libusb_control_transfer(devh, CTRL_OUT, USB_RQ, 0x07, 0, &data, 1, 0);
////    if (r < 0) {
////        fprintf(stderr, "set hwstat error %d\n", r);
////        return r;
////    }
////    if ((unsigned int) r < 1) {
////        fprintf(stderr, "short write (%d)", r);
////        return -1;
////    }

////    return 0;
////}

////static int set_mode(unsigned char data)
////{
////    int r;
////    printf("set mode %02x\n", data);

////    r = libusb_control_transfer(devh, CTRL_OUT, USB_RQ, 0x4e, 0, &data, 1, 0);
////    if (r < 0) {
////        fprintf(stderr, "set mode error %d\n", r);
////        return r;
////    }
////    if ((unsigned int) r < 1) {
////        fprintf(stderr, "short write (%d)", r);
////        return -1;
////    }

////    return 0;
////}


//static int device_satus(libusb_device_handle *hd)  {
//    int interface = 0;
//    unsigned char byte;
//    libusb_control_transfer(hd, LIBUSB_ENDPOINT_IN | LIBUSB_REQUEST_TYPE_CLASS | LIBUSB_RECIPIENT_INTERFACE,
//            LIBUSB_REQUEST_CLEAR_FEATURE,
//            0,
//            interface,
//            &byte, 1, 5000);
//    printf("status:0x%x\n", byte);
//    if(byte == 0x18){
//        printf("normal\n");
//    }else if(byte == 0x10){
//        printf("other\n");
//    }
//    return 0;
//}

//MainWindow::MainWindow(QWidget *parent) :
//    QMainWindow(parent),
//    ui(new Ui::MainWindow)
//{
//    ui->setupUi(this);
////    libusb的使用非常方便，指定ID后，相应设备被打开，得到设备句柄之后，即可调用各种传输操作API对我们的USB设备进行访问了。

//    libusb_device **devs;             //pointer to pointer of device, used to retrieve a list of devices
//   // libusb_device_handle *dev_handle; //a device handle
//    libusb_context *ctx = NULL;       //a libusb session
//    int r;                            //for return values
//    ssize_t cnt;                      //holding number of devices in list
//    r = libusb_init(&ctx);            //initialize the library for the session we just declared
//    if(r < 0) {
//        perror("Init Error\n");       //there was an error
//        return;
//    }
//    libusb_set_debug(ctx, LIBUSB_LOG_LEVEL_INFO); //set verbosity level to 3, as suggested in the documentation

//    cnt = libusb_get_device_list(ctx, &devs);     //get the list of devices
//    if(cnt < 0) {
//        perror("Get Device Error\n");             //there was an error
//        return;
//    }
//    printf("%d Devices in list.\n", cnt);

//    dev_handle = libusb_open_device_with_vid_pid(ctx, VID, PID); //these are vendorID and productID I found for my usb device
//    if(dev_handle == NULL){
//        perror("Cannot open device\n");
//        libusb_free_device_list(devs, 1);                   //free the list, unref the devices in it
//        libusb_exit(ctx);                                   //close the session
//        return;
//    }
//    else{
//        printf("Device Opened\n");
//    }


//    printf("******************______************\n");
//    for(int i =0; i < cnt; i++){
//        printdev(devs[i]);
//        printf("__________________******_____________\n");
//    }

//    libusb_free_device_list(devs, 1);                       //free the list, unref the devices in it

//    //首先判断是否存在设备驱动：libusb_kernel_driver_active
//    //如果存在，那么移除设备的驱动，libusb_detach_kernel_driver
//    if(libusb_kernel_driver_active(dev_handle, 0) == 1) {   //find out if kernel driver is attached
//        printf("Kernel Driver Active\n");
//        if(libusb_detach_kernel_driver(dev_handle, 0) == 0) //detach it
//            printf("Kernel Driver Detached!\n");
//    }

//    r = libusb_claim_interface(dev_handle, 0);              //claim interface 0 (the first) of device (mine had jsut 1)
//    if(r < 0) {
//        fprintf(stderr, "failed to initialise libusb\n");
//        perror("Cannot Claim Interface\n");
//        return;
//    }
//    printf("Claimed Interface\n");



////    unsigned char dataOut[0x30]={0};
////     dataOut[0]=0x00;
////     dataOut[1]=0x02;
////     dataOut[2]=0x08;
////     dataOut[3]=0x00;
////     dataOut[4]=0x02;
////     dataOut[5]=0x00;
////     dataOut[6]=0xb2;
////     dataOut[7]=0x30;
////     dataOut[8]=dataOut[2]^dataOut[3]^dataOut[4]^dataOut[5]^dataOut[6]^dataOut[7];
////     //r=libusb_interrupt_transfer(dev_handle,LIBUSB_ENDPOINT_OUT, dataOut,9, &actual_length, 0);
////     //r=libusb_bulk_transfer(dev_handle,LIBUSB_ENDPOINT_OUT,dataOut,9,&actual_length,5000);
//       //当你调用libusb_bulk_transfer，如果他一直没有完成，那么你的应用程序会一直睡在那边，直到他完成并返回。
////     //此设备必须用控制传输
////     r=libusb_control_transfer(dev_handle, //设备句柄
////                               0x21,       //对应Setup Packet包中的bmRequestType字段
////                               0x09,       //对应Setup Packet包中的bRequest字段
////                               0x0300,     //对应Setup Packet包中的wValue字段
////                               0x00,       //对应Setup Packet包中的wIndex字段
////                               dataOut+1, //数据
////                               0x20,      //数据大小
////                               1000);     //超时阀值


////    函数原型：int LIBUSB_CALL libusb_control_transfer(libusb_device_handle *dev_handle,
////                                                 uint8_t request_type,
////                                                 uint8_t bRequest,
////                                                 uint16_t wValue,
////                                                 uint16_t wIndex,
////                                                 unsigned char *data,
////                                                 uint16_t wLength,
////                                                 unsigned int timeout);
////    参数说明：
////        dev_handle libusb_device_handle的指针
////        request_type      D7=0主机到设备， =1设备到主机；
////                          D6D5 =00标准请求命令，01 类请求命令，10用户定义的命令，11保留值
////                          D4D3D2D1D0= 0表示接收者为设备，1表示接收者为接口，2表示接收者为端点，3表示接收者为其他，其他值保留
////        这个参数由 enum libusb_request_recipient 、enum libusb_request_type 、enum libusb_endpoint_direction 组合
////        bRequest      命令的序号(其实就是命令)；所有的命令都是以不同编码值的方式传递给设备的，bRequest就表示USB命令的编码值。可以是USB标准命令，也可以用户自定义命令
////        标准请求命令定义在 enum libusb_standard_request
////        Value    2个字节，用来传送当前请求的参数，随请求不同而变。
////        Index    索引字段同样是2个字节，描述的是接口号
////        data     要传输的数据的指针，不需要传输数据，设置为NULL
////        wLength  数据的长度。当命令不需要传输数据时，此字段设为0
////        timeout  设置超时的毫秒数，如果设置0，则永不超时

//    // int usb_get_string_simple(usb_dev_handle *dev, int index, char *buf, size_t buflen);

//    /*
//     unsigned char dataRead[0x30]={0};
//     while (1) {
//         memset(dataRead,0,0x30);
//         r=libusb_control_transfer(dev_handle,
//                                   0xa1, //101 00001
//                                   0x01,
//                                   0x0300,
//                                   0x00,
//                                   dataRead,
//                                   0x30,
//                                   1000);
//         if(r<0){
//             printf("transfer data error");
//         }else{
//             printf("transfer data success\n");
//             for(int i=0;i<r;i++){
//                 printf("%02x ", dataRead[i]);
//             }
//         }
//     }

//    unsigned char   recv_data[72];
//    int             recv_len;
//    memset(recv_data, 0 , sizeof(recv_data));
//    // 接受数据
//    recv_len = libusb_control_msg(dev_handle,
//                                USB_ENDPOINT_IN + USB_TYPE_CLASS + USB_RECIP_INTERFACE,
//                                HID_REPORT_GET,
//                                0x300,
//                                0,
//                                recv_data, 72, 1000);

////    while (1) {

////    recv_len =libusb_get_string_descriptor_ascii(dev_handle,     0x0,
////                                                                recv_data, 72);
////    int size=0;
////    unsigned char datain[1024]="\0";
////    for(int i=0;i<300;i++)  {
////        //中断传输操作
////        int rr = libusb_interrupt_transfer(dev_handle,  //设备句柄
////                                           0x81,        //端点编号  LIBUSB_ENDPOINT_IN,
////                                           datain,      //数据
////                                           0x0020,      //数据块长度
////                                           &size,       //实际传输数据块大小
////                                           1000);       //超时阀值
////        qDebug()<<"libusb_interrupt_transfer rr: "<<rr<<"size: "<<size;
////        printf("data: ");
////        if(rr>=0){
////            for(int j=0; j<size; j++)
////            printf("%02x ", (unsigned char)(datain[j]));
////            printf("\n");
////        }
////    }

////   // printf("Error: %s\n", strerror(errno));


//*/


//    unsigned char intTransData[2] = {0xAA, 0x55};
//    int OutSize = sizeof(intTransData);
////    int intTrans = libusb_interrupt_transfer(dev_handle, endpoint_in,
////                                             intTransData, OutSize, &OutSize, 5000);

//    unsigned char dataOut[0x30]={0};
//    //此设备必须用控制传输
//        //r=libusb_control_transfer(dev_handle, 0x21, 0x09, 0x0300, 0x00, dataOut+1, 0x20, 1000);
//        unsigned char dataRead[0x30]={0};
//        //r=libusb_control_transfer(dev_handle, 0xa1, 0x01, 0x0300, 0x00, dataRead, 0x20, 5000);

//       // int intTrans=libusb_control_transfer(dev_handle, 0x81, 0x0a, 0x0000, 0x00, dataRead, sizeof(dataRead), 20000);

//      // int intTrans =libusb_control_transfer(dev_handle, 0x81, 0x01, 0x05, 0x00, dataRead, 0x20, 5000);

////        int size=0;
////       int intTrans =libusb_bulk_transfer(dev_handle,0x1,dataRead,sizeof(dataRead),&size,20000);//接收数据



////        if (devs->descriptor->iProduct) {

////        int ret = libusb_get_string_descriptor(dev_handle, *devs->descriptor.iProduct, dataRead, sizeof(dataRead));
////        if (ret > 0)
////        printf("- Product : %s\n", dataRead);
////        else
////        printf("- Unable to fetch product string\n");
////        }



//int len=0;
//        int intTrans=libusb_bulk_transfer (dev_handle, 5, dataRead, sizeof(dataRead), &len, 10000);




//      ///  int intTrans = (dev_handle, 0, dataRead, sizeof(dataRead));
////        if (intTrans > 0)
////        printf("- Product : %s\n", dataRead);
////        else
////        printf("- Unable to fetch product string\n");


// //        libusb_bulk_read(dev_handle, 0, dataRead, sizeof(dataRead),10000);

////        //发送控制信息，请求传输
////           usb_control_msg(dev_handle,0x40,0xb5,2,0,buffer,sizeof(buffer),1000);
////           if(dev_handle){
////           //若请求成功，从端点6传输数据到image_buffer中
////               usb_bulk_read(dev_handle,6,&image_buffer[64],64,1000);
////           }


//     //   wValue字段
//                /*
//        控制传输操作
//        libusb_device_handle *dev_handle：  设备句柄。
//        uint8_t  bmRequestType： 对应Setup Packet包中的bmRequestType字段。
//        uint8_t  bRequest： 对应Setup Packet包中的bRequest字段。
//        uint16_t   wValue： 对应Setup Packet包中的wValue字段。
//        uint16_t wIndex： 对应Setup Packet包中的wIndex字段。
//        unsigned char  *data： 数据。
//        uint16_t  wLength： 数据大小。
//        unsigned int  timeout： 超时阀值。
//*/

//    if (intTrans >= 0){
//        printf("read data:%s!\n",dataRead);
//    }else{
//         printf("Failed to read data:%d!\n",intTrans);
//    }



////    /* 1. 发送读Image数据指令，使用0号通道??????????????????*/
////    r = libusb_bulk_transfer(dev_handle, EP0ADDR, cmd_ir_start, 5, &transferred, USB_TIMEOUT);
////    if(r==0 && transferred==5){
////        printf("write Successful!\n");
////    }else{
////        printf("write error!\n");
////    }



//    //release everything
//    r = libusb_release_interface(dev_handle, 0); //release the claimed interface
//    if(r!=0) {
//        qDebug()<<"Cannot Release Interface";
//    }else{
//        printf("Released Interface!\n");
//    }
//    libusb_attach_kernel_driver(dev_handle,0);
//    device_satus(dev_handle);
//    libusb_close(dev_handle);        //close the device we opened
//    libusb_exit(ctx);                //needs to be called to end the
//}



//evdevtouch: Invalid ABS limits, behavior unspecified
//6 Devices in list.
//Device Opened
//Kernel Driver Active
//evdevtouch: Could not read from input device (No such device)
//Kernel Driver Detached!
//Claimed Interface
//status:0x0


MainWindow::~MainWindow()
{
    delete ui;
}
