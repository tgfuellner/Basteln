/* Name: avrdebug.c
 * Project: AVR Programmer
 * Author: Christian Starkjohann
 * Creation Date: 2006-07-10
 * Tabsize: 4
 * Copyright: (c) 2006 by OBJECTIVE DEVELOPMENT Software GmbH
 * License: GNU GPL v2 (see License.txt) or proprietary (CommercialLicense.txt)
 * This Revision: $Id: avrdebug.c 493 2008-02-05 18:29:04Z cs $
 */

/*
General Descripotion:
This program connects the the programmer via libusb and uses vendor specific
requests to read data from the programmer's UART. Since the UART RxD is
connected to the programming socket, this is a way to display debug information
from the target.
Sending information to the target is currently not implemented in this tool.

Sinnce we use Vendor class requests which can be sent to the default Control
endpoint, we don't need to claim an interface or set a configuration. We can
therefore run although the device is opened by another application such as e.g.
the CDC-ACM driver or the HID driver.
*/

/* Disable the following define if you have problems compiling the Unix
 * specific non-blocking I/O for keyboard input. This may be necessary on
 * Windows.
 */
#define HAVE_KEYBOARD_INPUT 1

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <usb.h>    /* this is libusb, see http://libusb.sourceforge.net/ */

#if HAVE_KEYBOARD_INPUT
#   ifdef WIN32
#       include <conio.h>
#       include <windows.h>
#       define  usleep(t)   Sleep((t)/1000) /* Sleep() is in ms */
#   else
#       include <sys/time.h>
#       include <fcntl.h>
#       include <sys/file.h>
#       include <termios.h>
#   endif  /* _WINDOWS */
#endif  /* HAVE_KEYBOARD_INPUT */

#define USBDEV_SHARED_VENDOR    0x16c0  /* VOTI */
#define USBDEV_SHARED_CDCPID    0x05e1  /* Obdev's free shared PID for CDC-ACM devices */
#define USBDEV_SHARED_HIDPID    0x05df  /* Obdev's free shared PID for HID devices */
/* Use obdev's generic shared VID/PID pair and follow the rules outlined
 * in firmware/usbdrv/USBID-License.txt.
 */
#define USB_VENDOR_STRING       "obdev.at"
#define USB_PRODUCT_STRING      "AVR-Doper"

#define USB_VENDOR_RQ_WRITE     1
#define USB_VENDOR_RQ_READ      2

#ifndef uchar
#define uchar unsigned char
#endif

/* ------------------------------------------------------------------------- */

static int  usbGetStringAscii(usb_dev_handle *dev, int index, int langid, char *buf, int buflen)
{
char    buffer[256];
int     rval, i;

    if((rval = usb_control_msg(dev, USB_ENDPOINT_IN, USB_REQ_GET_DESCRIPTOR, (USB_DT_STRING << 8) + index, langid, buffer, sizeof(buffer), 1000)) < 0)
        return rval;
    if(buffer[1] != USB_DT_STRING)
        return 0;
    if((uchar)buffer[0] < rval)
        rval = (uchar)buffer[0];
    rval /= 2;
    /* lossy conversion to ISO Latin1 */
    for(i=1;i<rval;i++){
        if(i > buflen)  /* destination buffer overflow */
            break;
        buf[i-1] = buffer[2 * i];
        if(buffer[2 * i + 1] != 0)  /* outside of ISO Latin1 range */
            buf[i-1] = '?';
    }
    buf[i-1] = 0;
    return i-1;
}

#define USB_ERROR_NOTFOUND  1
#define USB_ERROR_ACCESS    2
#define USB_ERROR_IO        3

int usbOpenDevice(usb_dev_handle **device, int vendor, char *vendorName, int product, char *productName)
{
struct usb_bus      *bus;
struct usb_device   *dev;
usb_dev_handle      *handle = NULL;
int                 errorCode = USB_ERROR_NOTFOUND;
static int          didUsbInit = 0;

    if(!didUsbInit){
        didUsbInit = 1;
        usb_init();
    }
    usb_find_busses();
    usb_find_devices();
    for(bus=usb_get_busses(); bus; bus=bus->next){
        for(dev=bus->devices; dev; dev=dev->next){
            if(dev->descriptor.idVendor == vendor && dev->descriptor.idProduct == product){
                char    string[256];
                int     len;
                handle = usb_open(dev); /* we need to open the device in order to query strings */
                if(!handle){
                    errorCode = USB_ERROR_ACCESS;
                    fprintf(stderr, "Warning: cannot open USB device: %s\n", usb_strerror());
                    continue;
                }
                if(vendorName == NULL && productName == NULL){  /* name does not matter */
                    break;
                }
                /* now check whether the names match: */
                len = usbGetStringAscii(handle, dev->descriptor.iManufacturer, 0x0409, string, sizeof(string));
                if(len < 0){
                    errorCode = USB_ERROR_IO;
                    fprintf(stderr, "Warning: cannot query manufacturer for device: %s\n", usb_strerror());
                }else{
                    errorCode = USB_ERROR_NOTFOUND;
                    /* fprintf(stderr, "seen device from vendor ->%s<-\n", string); */
                    if(strcmp(string, vendorName) == 0){
                        len = usbGetStringAscii(handle, dev->descriptor.iProduct, 0x0409, string, sizeof(string));
                        if(len < 0){
                            errorCode = USB_ERROR_IO;
                            fprintf(stderr, "Warning: cannot query product for device: %s\n", usb_strerror());
                        }else{
                            errorCode = USB_ERROR_NOTFOUND;
                            /* fprintf(stderr, "seen product ->%s<-\n", string); */
                            if(strcmp(string, productName) == 0)
                                break;
                        }
                    }
                }
                usb_close(handle);
                handle = NULL;
            }
        }
        if(handle)
            break;
    }
    if(handle != NULL){
        errorCode = 0;
        *device = handle;
    }
    return errorCode;
}

/* ------------------------------------------------------------------------- */

int openAvrDoper(usb_dev_handle **device)
{
    if(usbOpenDevice(device, USBDEV_SHARED_VENDOR, USB_VENDOR_STRING, USBDEV_SHARED_CDCPID, USB_PRODUCT_STRING) == 0)
        return 0;
    return usbOpenDevice(device, USBDEV_SHARED_VENDOR, USB_VENDOR_STRING, USBDEV_SHARED_HIDPID, USB_PRODUCT_STRING);
}

void    avrDoperReconnect(usb_dev_handle **device)
{
    fprintf(stderr, "\nUSB error: %s\nTrying to reconnect...\n", usb_strerror());
    usb_close(*device);
    while(openAvrDoper(device) != 0){
        usleep(1000 * 1000);  /* 1 second */
    }
    fprintf(stderr, "Reconnected.\n\n");
}

/* ------------------------------------------------------------------------- */

/* <c> <LF> <c>         -> <c> LF <c>
 * <c> CR LF <c>        -> <c> LF <c>
 * <c> LF CR <c>        -> <c> LF <c>
 * <c> CR <c>           -> <c> LF <c>
 * <c> CR CR <c>        -> <c> LF LF <c>
 * <c> CR LF CR LF <c>  -> <c> LF LF <c>
 */

static int isCrOrLf(uchar c)
{
    return c == '\r' || c == '\n';
}

static void printChar(uchar c, int needFlush)
{
static char     isStartOfLine = 1;
static uchar    lastChar = 0;
uchar           c1;

    c1 = lastChar;
    lastChar = c;
    if(isCrOrLf(c)){
        if(isCrOrLf(c1)){
            if(c != c1)     /* must be second char of CR LF or LF CR */
                return;
        }
        c = '\n';           /* map all newline chars to LF */
    }
#ifndef WIN32   /* anybody wants to port this functionality to Windows? */
    if(isStartOfLine){
        struct timeval  now;
        struct timezone tz[1];
        struct tm       *tm;
        gettimeofday(&now, tz);
        tm = localtime(&now.tv_sec);
        printf("%02d:%02d:%02d.%03d: ", tm->tm_hour, tm->tm_min, tm->tm_sec, now.tv_usec / 1000);
    }
#endif
    putc(c, stdout);
    isStartOfLine = c == '\n';
    if(needFlush && c != '\n'){
        fflush(stdout);
    }
}

static void normalizeAndPrintString(uchar *s, int len)
{
int i;

    for(i = 0; i < len; i++){
        printChar(s[i], (i + 1) >= len);
    }
}

/* ------------------------------------------------------------------------- */
/* ------------------------ handling keyboard input ------------------------ */
/* ------------------------------------------------------------------------- */

/* Thanks to V. Bosch for the keyboard input routines! */

#if HAVE_KEYBOARD_INPUT

#ifdef WIN32

#define inputInit()
#define inputExit()

/* Thanks to Kai Morich for this routine! */
static int inputPoll(usb_dev_handle **device)
{
int nBytes, c;

    while(_kbhit()){
        c = getch();
        if(c == '\r')
            c='\n';
        putch(c);
        if(c == 'X' - 'A' + 1)
            return 1;
        nBytes = usb_control_msg(*device, USB_TYPE_VENDOR | USB_RECIP_DEVICE | USB_ENDPOINT_OUT, USB_VENDOR_RQ_WRITE, c, 0, NULL, 0, 100);
        if(nBytes < 0)
            avrDoperReconnect(device);
    }
    return 0;
}

#else /* WIN32 */
static struct termios savedStdinTermios;

static void inputInit(void)
{
struct termios  t;

    fcntl(STDIN_FILENO, F_SETFL, FNDELAY);
    tcgetattr(STDIN_FILENO, &savedStdinTermios);
    t = savedStdinTermios;
    t.c_iflag = t.c_lflag = 0;
    t.c_cc[VMIN]  = 0;    /* Minimum number of characters for non-canonical read */
    t.c_cc[VTIME] = 0;    /* Timeout in deciseconds for non-canonical read */
    tcsetattr(STDIN_FILENO, TCSANOW, &t);
}

static void inputExit(void)
{
    tcsetattr(STDIN_FILENO, TCSANOW, &savedStdinTermios);
}

static int inputPoll(usb_dev_handle **device)
{
int             nBytes, cnt, i;
unsigned char   buffer[16];

    cnt = read(STDIN_FILENO, buffer, sizeof(buffer));  /* non-blocking read */
    if(cnt > 0){
        if(memchr(buffer, 'X' - 'A' + 1, cnt))
            return 1;
        for(i = 0; i < cnt; i++){
            nBytes = usb_control_msg(*device, USB_TYPE_VENDOR | USB_RECIP_DEVICE | USB_ENDPOINT_OUT, USB_VENDOR_RQ_WRITE, buffer[i], 0, NULL, 0, 100);
            if(nBytes < 0){
                avrDoperReconnect(device);
            }
        }
    }
    return 0;
}
#endif /* WIN32 */
#else /* HAVE_KEYBOARD_INPUT */

#define inputInit()
#define inputExit()
#define inputPoll(handle) 0

#endif /* HAVE_KEYBOARD_INPUT */

/* ------------------------------------------------------------------------- */

int main(int argc, char **argv)
{
usb_dev_handle      *handle = NULL;
unsigned char       buffer[254];
int                 nBytes;

    if(openAvrDoper(&handle) != 0){
        fprintf(stderr, "Could not find USB device \"%s\" with vid=0x%x pid=0x%x or 0x%x\n", USB_PRODUCT_STRING, USBDEV_SHARED_VENDOR, USBDEV_SHARED_CDCPID, USBDEV_SHARED_HIDPID);
        exit(1);
    }
    /* We need not set a configuration and claim an interface to use endpoint 0 */
    inputInit();
    /* The following loop does busy polling with 100ms sleeps when no data was
     * received. We can't use an interrupt endpoint because the CDC device class
     * has already consumed them all.
     */
    for(;;){
        nBytes = usb_control_msg(handle, USB_TYPE_VENDOR | USB_RECIP_DEVICE | USB_ENDPOINT_IN, USB_VENDOR_RQ_READ, 0, 0, (char *)buffer, sizeof(buffer), 5000);
        if(nBytes < 0){
            avrDoperReconnect(&handle);
            continue;
        }
        if(nBytes > 0){
            normalizeAndPrintString(buffer, nBytes);
        }else{
            usleep(50000); /* 50ms */
        }
        if(inputPoll(&handle))
            break;
    }
    usb_close(handle);
    inputExit();
    return 0;
}

/* ------------------------------------------------------------------------- */
