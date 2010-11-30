/**
   Temperaturlogger
   Thomas Gfuellner
   thomas.gfuellner@gmx.de

   Works with TemperaturLogger from Thomas Gfuellner
   build closly after: http://www.obdev.at/products/vusb/easylogger.html
   This tool is derived from: http://thread.gmane.org/gmane.comp.lib.libhid.general/56
   Requires libhid; under Ubuntu do a: aptitude install libhid-dev

   VendorID 0x4242 and ProductID 0xe131.
   Requires libhid.

   This Temperaturlogger acts like a USB keyboard.
   On an interrupt read request, the scanner returns 2 bytes: 
   - The first byte corresponds to the control keys pressed. It is
   equal to 2 if SHIFT is pressed, i.e. upper case character.
   - The second byte is part of the tempreture.

   To get a full temperature we need to send requests until we read a
   carriage return character (code 0x28);
*/

#include <hid.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <time.h>

// #define PACKET_LEN 8
#define PACKET_LEN 2

#define VENDOR 0x4242
#define PRODUCT 0xe131


char getTheChar(char usbCoding) {
    switch (usbCoding ) {
        case 30: return '1';
        case 31: return '2';
        case 32: return '3';
        case 33: return '4';
        case 34: return '5';
        case 35: return '6';
        case 36: return '7';
        case 37: return '8';
        case 38: return '9';
        case 39: return '0';
        case 40: return '\n';
        case 55: return '.';
        case 45: return '-';
    }
    return '?';
}



void printPacket(const char* packet, unsigned int size){
  int i;
  printf("received packet: 0x ");
  for( i=0; i<size; printf("%02x ",packet[i++]) );
  printf(" | ");
  putchar(getTheChar(packet[1]));
  putchar('\n');
}

char *getTime() {
    time_t     now;
    struct tm  *ts;
    static char       buf[80];

    /* Get the current time */
    now = time(NULL);

    ts = localtime(&now);
    strftime(buf, sizeof(buf), "%Y-%m-%d_%H:%M:%S_%Z", ts);

    return buf;
}

void showTemperature(const char* packet) {
  if (packet[1] == 0) {
      return;
  }
  const int BUFFERSIZE = 10;
  static char buffer[BUFFERSIZE+1];
  static int i=0;

  char c = getTheChar(packet[1]);
  buffer[i++] = c;
  assert(i<BUFFERSIZE);

  if (c == '\n') {
      buffer[i] = 0;
      fprintf(stdout, "%s %s", getTime(), buffer);
      fflush(stdout);
      i=0;
  }

}


int main(void){
  HIDInterface* hid;
  hid_return ret;
  HIDInterfaceMatcher matcher = { VENDOR, PRODUCT, NULL, NULL, 0 };
  char packet[PACKET_LEN];

  hid_set_debug(HID_DEBUG_NONE);
  hid_set_debug_stream(stderr);
  hid_set_usb_debug(0);
  
  hid_init();
  hid = hid_new_HIDInterface();
  ret = hid_force_open(hid, 0, &matcher, 3);

  if (ret == HID_RET_FAIL_DETACH_DRIVER) {
      printf("Failed to detach Driver for Device %04x:%04x; Permission?\n", VENDOR, PRODUCT);
      return 1;
  }

  if (ret != HID_RET_SUCCESS) {
      printf("Could not open HID device %04x:%04x (ret was %d)\n", VENDOR, PRODUCT, ret);
      return 1;
  }

  // printf("hid_force_open ret=%d\n", ret);

  // hid_set_idle(hid,0,0);

  // Discard till first '\n'
  do {
    memset(packet,0,sizeof(packet));
    ret = hid_interrupt_read(hid,0x81,packet,PACKET_LEN,1000);
  } while (getTheChar(packet[1]) != '\n');

  while (1) {
    memset(packet,0,sizeof(packet));

    ret = hid_interrupt_read(hid,0x81,packet,PACKET_LEN,1000);

    /*
    if (ret == HID_RET_FAIL_INT_READ) {
        printf("Fail hid_interrupt_read\n");
    } else {
        printf("hid_interrupt_read ret=%d\n", ret);
    }
    */

    if( ret == HID_RET_SUCCESS ) {
      //printPacket(packet,PACKET_LEN);
      showTemperature(packet);
    }
  }

  hid_close(hid);
  hid_delete_HIDInterface(&hid);
  hid_cleanup();
  return 0;
}
