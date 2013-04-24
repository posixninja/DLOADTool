//
//  usb.h
//  dloadtool
//
//  Created by Joshua Hill on 1/31/13.
//
//

#ifndef dloadtool_usb_h
#define dloadtool_usb_h

#include <IOKit/usb/IOUSBLib.h>

typedef IOUSBDeviceInterface** USBDevice;
typedef IOUSBInterfaceInterface** USBInterface;

void CloseDevice(USBDevice device);
USBDevice OpenDevice(SInt32 vendorId, SInt32 productId);

UInt8 SetConfiguration(USBDevice device, UInt8 configuration);

USBInterface OpenInterface(USBDevice device, UInt8 interface, UInt8 alt_interface);
void CloseInterface(USBInterface interface);

int WriteBulk(USBInterface iface, UInt8 pipe, void* buf, UInt32 size);
int ReadBulk(USBInterface iface, UInt8 pipe, void* buf, UInt32* size);

#endif
