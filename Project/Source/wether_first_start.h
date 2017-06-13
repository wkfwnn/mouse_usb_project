#ifndef __WETHER_FIRST_START_H_
#define __WETHER_FIRST_START_H_

typedef enum   {
	NUM_SPEAKER = 0x00,
	NUM_HID
}usb_type_t;


extern unsigned char usb_type;

#define CURRENT_USB_DEVICE_ENUM    usb_type

#define USB_HID_MARK      0xA5A5
#define USB_SPEAKER_MARK  0x5A5A

extern unsigned char is_first_start;

void whether_first_start(void );
void set_bkp_data(unsigned int data);


#endif

