/* Name: text2Controler.c
 * Project: 128pixels Laufschrift based on PowerSwitch based on AVR USB driver
 * Author: iThomas Gfuellner, Christian Starkjohann
 * Creation Date: 2010-10-23, 2005-01-16
 * Tabsize: 4
 * Copyright: (c) 2005 by OBJECTIVE DEVELOPMENT Software GmbH
 * License: GNU GPL v2 (see License.txt) or proprietary (CommercialLicense.txt)
 * This Revision: $Id: powerSwitch.c 472 2008-01-21 18:21:59Z cs $
 */

/*
General Description:
This Program writes a Text into the Controler.
The Controler shows a running Text on a 8x8 Dot Display.

USB AVR Lab with bitbanging firmware is used to program the controler.
Pin 1 is of AVR Lab is used.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iconv.h>
#include <assert.h>
#include <usb.h>    /* this is libusb, see http://libusb.sourceforge.net/ */

#include "font_5x8.h"

#define MAX_TEXT_LENGTH 64
#define ESCAPE 0xff

#define USBDEV_SHARED_VENDOR    0x16C0  /* VOTI */
#define USBDEV_SHARED_PRODUCT   0x05DC  /* Obdev's free shared PID */
/* Use obdev's generic shared VID/PID pair and follow the rules outlined
 * in firmware/usbdrv/USBID-License.txt.
 */

#define FUNC_WRITE_BUFFER   1
/* These are the vendor specific SETUP commands implemented by our USB device */

#define iMANUFACTURER "runga.tumblr.com"
#define iPRODUCT "128-PIX"

static void usage(char *name)
{
    fprintf(stderr, "usage:\n");
    fprintf(stderr, "  %s [-v] [-p] <your Text Message>\n", name);
    fprintf(stderr, "  -p is programming Mode\n");
    fprintf(stderr, "  -v is verbose output to console\n");
}


static int  usbGetStringAscii(usb_dev_handle *dev, int index, int langid, char *buf, int buflen)
{
char    buffer[256];
int     rval, i;

    if((rval = usb_control_msg(dev, USB_ENDPOINT_IN, USB_REQ_GET_DESCRIPTOR,
                 (USB_DT_STRING << 8) + index, langid, buffer, sizeof(buffer), 1000)) < 0)
        return rval;
    if(buffer[1] != USB_DT_STRING)
        return 0;
    if((unsigned char)buffer[0] < rval)
        rval = (unsigned char)buffer[0];
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


/* PowerSwitch uses the free shared default VID/PID. If you want to see an
 * example device lookup where an individually reserved PID is used, see our
 * RemoteSensor reference implementation.
 */

#define USB_ERROR_NOTFOUND  1
#define USB_ERROR_ACCESS    2
#define USB_ERROR_IO        3

static int usbOpenDevice(usb_dev_handle **device, int vendor, char *vendorName, int product, char *productName)
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
                 /* we need to open the device in order to query strings */
                handle = usb_open(dev);
                if(!handle){
                    errorCode = USB_ERROR_ACCESS;
                    fprintf(stderr,"Warning: cannot open USB device: %s\n", usb_strerror());
                    continue;
                }
                if(vendorName == NULL && productName == NULL){  /* name does not matter */
                    break;
                }
                /* now check whether the names match: */
                len = usbGetStringAscii(handle, dev->descriptor.iManufacturer, 0x0409,
                        string, sizeof(string));
                if(len < 0){
                    errorCode = USB_ERROR_IO;
                    fprintf(stderr, "Warning: cannot query manufacturer for device: %s\n",
                            usb_strerror());
                }else{
                    errorCode = USB_ERROR_NOTFOUND;
                    /* fprintf(stderr, "seen device from vendor ->%s<-\n", string); */
                    if(strcmp(string, vendorName) == 0){
                        len = usbGetStringAscii(handle, dev->descriptor.iProduct, 0x0409,
                                string, sizeof(string));
                        if(len < 0){
                            errorCode = USB_ERROR_IO;
                            fprintf(stderr,"Warning: cannot query product for device: %s\n"
                                    , usb_strerror());
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

static unsigned char ReverseByteBits(unsigned char value) {
    unsigned char ret = 0;
    int i;

    for (i = 0; i < 8; i++)
        if ((value & (unsigned char)(1 << i)) != 0) ret += (unsigned char)(1 << (7 - i));

    return ret;
}

void outByte(usb_dev_handle *handle, unsigned char byte) {
    unsigned char       buffer[8];
    int                 nBytes;


    nBytes = usb_control_msg(handle, 
            USB_TYPE_VENDOR|USB_RECIP_DEVICE|USB_ENDPOINT_IN, FUNC_WRITE_BUFFER,
            ReverseByteBits(byte), 0, (char *)buffer, sizeof(buffer), 5000);

    if(nBytes < 0) {
       fprintf(stderr, "USB error: %s\n", usb_strerror());
       exit(1);
    }
}


void sendBitPatterns(usb_dev_handle *handle, unsigned char theCharToSend, int verbose) {
    int charIndex;
    if (verbose) {
        printf(" %c: ", theCharToSend);
    }
    for (charIndex=0; charIndex < sizeof(font[0]); charIndex++) {
        unsigned char col = font[(theCharToSend)-CHAR_OFFSET][charIndex]; 

        // Skip space at start and end of char.
        if (charIndex == 0 && col == 0) 
            continue;
        if (charIndex == sizeof(font[0])-1 && col == 0) 
            continue;

        outByte(handle, col);
        usleep(100000);
        if (verbose) {
            printf(" 0x%x %d ",  col, col);
        }
    }
    outByte(handle, 0);
    usleep(100000);

    if (verbose) {
        printf(" 0\n");
    }
}

void changeToMyCoding(char *source, char *dest) {
    // Convert from UTF-8 encoding to single byte
    iconv_t cDescriptor = iconv_open ("ISO−8859−9", "UTF-8");;
    assert(cDescriptor != (iconv_t)-1);

    size_t rc, inBytesLeft=strlen(source), outBytesLeft=MAX_TEXT_LENGTH;
    char *destEnd = dest;

    rc = iconv(cDescriptor, &source, &inBytesLeft, &destEnd, &outBytesLeft);
    assert(rc != -1);
    *destEnd = 0;
    iconv_close(cDescriptor);

    // äöüß
    while (*dest) {
        // printf("(%c) %hhu -> ", *dest, *dest);
        if ((unsigned char)*dest == 228) *dest = 127;
        if ((unsigned char)*dest == 246) *dest = 128;
        if ((unsigned char)*dest == 252) *dest = 129;
        if ((unsigned char)*dest == 223) *dest = 130;
        // printf("%hhu\n", *dest);

        dest++;
    }
}


int main(int argc, char **argv)
{
    usb_dev_handle      *handle = NULL;

    int progMode = 0;
    int verbose = 0;
    int c;
    while ((c = getopt(argc, argv, "pv")) != -1) {
        if (c == 'p') {
            progMode = 1;
        } else if (c == 'v') {
            verbose = 1;
        } else {
            return 1;
        }
    }

    if(argc != optind+1){
        usage(argv[0]);
        return 1;
    }

    unsigned char *textForControler = (unsigned char *)argv[optind];

    if (strlen(argv[optind]) > MAX_TEXT_LENGTH) {
        fprintf(stderr, "Text is to long, Max is %d chars\n", MAX_TEXT_LENGTH);
        return 1;
    }

    unsigned char textBuffer[MAX_TEXT_LENGTH+1];
    
    changeToMyCoding((char *)textForControler, (char *)textBuffer);
    textForControler = textBuffer;

    usb_init();
    if(usbOpenDevice(&handle, USBDEV_SHARED_VENDOR, iMANUFACTURER,
                USBDEV_SHARED_PRODUCT, iPRODUCT) != 0){
        fprintf(stderr, "Could not find USB device \"%s\" with vid=0x%x pid=0x%x\n",
                iPRODUCT, USBDEV_SHARED_VENDOR, USBDEV_SHARED_PRODUCT);
        exit(1);
    }

    if (progMode) {
        outByte(handle, ESCAPE);   // ESCAPE
        usleep(200000);
        while (*textForControler) {
            if (*textForControler == '\\') {
                textForControler++;
                if (*textForControler >= '1' && *textForControler <= '9') {
                    unsigned char out = *textForControler - '1' + 1;
                    outByte(handle, ReverseByteBits(out));
                    if (verbose) {
                        printf("eeprom delay cmd: %c 0x%x\n", *textForControler, out);
                    }
                }
            } else {
                outByte(handle, ReverseByteBits(*textForControler));
                if (verbose) {
                    printf("eeprom: %c 0x%x\n", *textForControler, *textForControler);
                }
            }
            usleep(100000);
            textForControler++;
        }
        outByte(handle, ESCAPE);   // ESCAPE
        usleep(100000);

    } else {

        while (1) {
            while (*textForControler) {
                if (*textForControler == '\\') {
                    textForControler++;
                    if (*textForControler >= '1' && *textForControler <= '9') {
                        int secondsToWait = *textForControler++ - '1' + 1;
                        sleep(secondsToWait);
                        continue;
                    }
                }
                sendBitPatterns(handle, *textForControler++, verbose);
            }
            textForControler = textBuffer;
        }
    }

    usb_close(handle);
    return 0;
}
