iOS Baseband Tools

https://github.com/posixninja/BBTool
https://github.com/posixninja/DLOADTool
https://github.com/posixninja/DBLTool
https://github.com/posixninja/iOSUSBEnum

Description:

These are just some little tools I made for interfacing with the Qualcomm baseband in iOS. They should work on iPhone4s and iPhone5 (although I've only tested them on iPhone4s CDMA).

Setup:

To build these tools you need Xcode with the iOS SDK, and iOSOpenDev installed and setup (http://www.iosopendev.com/). In the terminal switch the each directory and type "xcodebuild" to build the tool.

Usage: 

Before you can use any of the tools (other than iOSUSBEnum) you should first upload CommCenter so it won't interfere. First SSH onto the device and type the following command to unload CommCenter.

launchctl unload /System/Library/LaunchDaemons/com.apple.CommCenter.plist

Once CommCenter is uploaded you can type "iosusbenum" to verify that the Qualcomm baseband is no longer showing up in the device list. Since the baseband no longer has any internal flash to store the baseband firmware, all the firmware is located on the device filesystem. To access these firmware files you first need to unzip them. To do this type

(for 6.x)
cd /usr/local/standalone/firmware/Baseband/Trek
unzip Trek-personalized.zip

(for 5.x)
cd /usr/standalone/firmware/Trek
unzip Trek-personalized.zip

After the firmware has been unzipped you'll see a few different files. The important ones are bbticket.der, dbl.mbn, osbl.mbn, and amss.mbn. Next you need to use bbtool to tell the device to enter DLOAD mode (basically baseband's bootrom or DFU mode). Type

bbtool enter-dload

You can then verify the device has entered DLOAD mode by using "iosusbenum" once again, you should see something like this.

Device Name: QHSUSB_DLOAD
Vendor ID: 0x5c6
Product ID: 0x9008
Version: 0x0
Location: 0x1200000
Configuration: 0
	Length: 0x9
	Descriptor Type: 0x2
	Total Length: 0x20
	Num Interfaces: 0x1
	Configuration Value: 0x1
	Configuration: 0x0
	Attributes: 0x80
	Max Power: 0x1
	Interface
		Length: 0x9
		Descriptor Type: 0x4
		Interface Number: 0x0
		Alternate Setting: 0x0
		Num Endpoints: 0x2
		Interface Class: Vendor Specific
		Interface SubClass: 0xff
		Interface Protocol: 0xff
		Endpoint
			Length: 0x7
			Descriptor Type: 0x5
			Endpoint Address: 0x81
			Attributes: 0x2
				Transfer Type: Bulk
			Max Packet Size: 0x200
			Interval: 0x0
		Endpoint
			Length: 0x7
			Descriptor Type: 0x5
			Endpoint Address: 0x1
			Attributes: 0x2
				Transfer Type: Bulk
			Max Packet Size: 0x200
			Interval: 0x0

Once you've verified the baseband is in DLOAD mode you can then just DLOADTool to boot the device into DBL (or SAH) mode. Depending on where your baseband firmware files are at just send the following command.

dloadtool -f /usr/local/standalone/firmware/Baseband/Trek/dbl.mbn

You should see a bunch of send and recv messages followed by "Closing Interface". Now the device should be in DBL mode. You can verify this by using "iosusbenum" once again. You should see output that looks similar to

Device Name: Qualcomm CDMA Technologies MSM
Vendor ID: 0x5c6
Product ID: 0x900e
Version: 0x0
Location: 0x1200000
Configuration: 0
	Length: 0x9
	Descriptor Type: 0x2
	Total Length: 0x20
	Num Interfaces: 0x1
	Configuration Value: 0x1
	Configuration: 0x1
	Attributes: 0xe0
	Max Power: 0x32
	Interface
		Length: 0x9
		Descriptor Type: 0x4
		Interface Number: 0x0
		Alternate Setting: 0x0
		Num Endpoints: 0x2
		Interface Class: Vendor Specific
		Interface SubClass: 0xff
		Interface Protocol: 0xff
		Endpoint
			Length: 0x7
			Descriptor Type: 0x5
			Endpoint Address: 0x81
			Attributes: 0x2
				Transfer Type: Bulk
			Max Packet Size: 0x200
			Interval: 0x20
		Endpoint
			Length: 0x7
			Descriptor Type: 0x5
			Endpoint Address: 0x1
			Attributes: 0x2
				Transfer Type: Bulk
			Max Packet Size: 0x200
			Interval: 0x20

Now that the device is in DBL mode you can use DBLTool to continue to boot the baseband up to it's normal operating mode. To do this you must pass the path to bbticket, osbl, and amss into DBLTool using the following command.

dbltool -b /usr/local/standalone/firmware/Baseband/Trek/bbticket.der -o /usr/local/standalone/firmware/Baseband/Trek/osbl.mbn -a /usr/local/standalone/firmware/Baseband/Trek/amss.mbn

You should see a bunch of send and recvs once again followed by "Closing Interface". AMSS usually takes a little while to load, but after about 10-20 seconds, you can verify the baseband has fully booted up without the help of CommCenter by using "iosusbenum" once again. There are a bunch of interfaces in AMSS mode, but you should get output similar to the following.

Device Name: Qualcomm CDMA Technologies MSM
Vendor ID: 0x5c6
Product ID: 0x9001
Version: 0x0
Location: 0x1200000
Configuration: 0
	Length: 0x9
	Descriptor Type: 0x2
	Total Length: 0x118
	Num Interfaces: 0xd
	Configuration Value: 0x1
	Configuration: 0x1
	Attributes: 0xe0
	Max Power: 0x32
	Interface
		Length: 0x9
		Descriptor Type: 0x4
		Interface Number: 0x0
		Alternate Setting: 0x0
		Num Endpoints: 0x2
		Interface Class: Vendor Specific
		Interface SubClass: 0xff
		Interface Protocol: 0xff
		Endpoint
			Length: 0x7
			Descriptor Type: 0x5
			Endpoint Address: 0x81
			Attributes: 0x2
				Transfer Type: Bulk
			Max Packet Size: 0x200
			Interval: 0x20
		Endpoint
			Length: 0x7
			Descriptor Type: 0x5
			Endpoint Address: 0x1
			Attributes: 0x2
				Transfer Type: Bulk
			Max Packet Size: 0x200
			Interval: 0x20
	Interface
		Length: 0x9
		Descriptor Type: 0x4
		Interface Number: 0x1
		Alternate Setting: 0x0
		Num Endpoints: 0x2
		Interface Class: Vendor Specific
		Interface SubClass: 0xff
		Interface Protocol: 0xff
		Endpoint
			Length: 0x7
			Descriptor Type: 0x5
			Endpoint Address: 0x82
			Attributes: 0x2
				Transfer Type: Bulk
			Max Packet Size: 0x200
			Interval: 0x20
		Endpoint
			Length: 0x7
			Descriptor Type: 0x5
			Endpoint Address: 0x2
			Attributes: 0x2
				Transfer Type: Bulk
			Max Packet Size: 0x200
			Interval: 0x20
	Interface
		Length: 0x9
		Descriptor Type: 0x4
		Interface Number: 0x2
		Alternate Setting: 0x0
		Num Endpoints: 0x3
		Interface Class: Vendor Specific
		Interface SubClass: 0xff
		Interface Protocol: 0xff
		Endpoint
			Length: 0x7
			Descriptor Type: 0x5
			Endpoint Address: 0x83
			Attributes: 0x3
				Transfer Type: Interrupt
			Max Packet Size: 0x40
			Interval: 0x5
		Endpoint
			Length: 0x7
			Descriptor Type: 0x5
			Endpoint Address: 0x84
			Attributes: 0x2
				Transfer Type: Bulk
			Max Packet Size: 0x200
			Interval: 0x20
		Endpoint
			Length: 0x7
			Descriptor Type: 0x5
			Endpoint Address: 0x3
			Attributes: 0x2
				Transfer Type: Bulk
			Max Packet Size: 0x200
			Interval: 0x20
	Interface
		Length: 0x9
		Descriptor Type: 0x4
		Interface Number: 0x3
		Alternate Setting: 0x0
		Num Endpoints: 0x1
		Interface Class: Vendor Specific
		Interface SubClass: 0xff
		Interface Protocol: 0xff
		Endpoint
			Length: 0x7
			Descriptor Type: 0x5
			Endpoint Address: 0x85
			Attributes: 0x3
				Transfer Type: Interrupt
			Max Packet Size: 0x40
			Interval: 0x5
	Interface
		Length: 0x9
		Descriptor Type: 0x4
		Interface Number: 0x4
		Alternate Setting: 0x0
		Num Endpoints: 0x2
		Interface Class: Communication Data
		Interface SubClass: 0x0
		Interface Protocol: 0x0
		Endpoint
			Length: 0x7
			Descriptor Type: 0x5
			Endpoint Address: 0x86
			Attributes: 0x2
				Transfer Type: Bulk
			Max Packet Size: 0x200
			Interval: 0x20
		Endpoint
			Length: 0x7
			Descriptor Type: 0x5
			Endpoint Address: 0x4
			Attributes: 0x2
				Transfer Type: Bulk
			Max Packet Size: 0x200
			Interval: 0x20
	Interface
		Length: 0x9
		Descriptor Type: 0x4
		Interface Number: 0x5
		Alternate Setting: 0x0
		Num Endpoints: 0x1
		Interface Class: Vendor Specific
		Interface SubClass: 0xff
		Interface Protocol: 0xff
		Endpoint
			Length: 0x7
			Descriptor Type: 0x5
			Endpoint Address: 0x87
			Attributes: 0x3
				Transfer Type: Interrupt
			Max Packet Size: 0x40
			Interval: 0x5
	Interface
		Length: 0x9
		Descriptor Type: 0x4
		Interface Number: 0x6
		Alternate Setting: 0x0
		Num Endpoints: 0x2
		Interface Class: Communication Data
		Interface SubClass: 0x0
		Interface Protocol: 0x0
		Endpoint
			Length: 0x7
			Descriptor Type: 0x5
			Endpoint Address: 0x88
			Attributes: 0x2
				Transfer Type: Bulk
			Max Packet Size: 0x200
			Interval: 0x20
		Endpoint
			Length: 0x7
			Descriptor Type: 0x5
			Endpoint Address: 0x5
			Attributes: 0x2
				Transfer Type: Bulk
			Max Packet Size: 0x200
			Interval: 0x20
	Interface
		Length: 0x9
		Descriptor Type: 0x4
		Interface Number: 0x7
		Alternate Setting: 0x0
		Num Endpoints: 0x1
		Interface Class: Vendor Specific
		Interface SubClass: 0xff
		Interface Protocol: 0xff
		Endpoint
			Length: 0x7
			Descriptor Type: 0x5
			Endpoint Address: 0x89
			Attributes: 0x3
				Transfer Type: Interrupt
			Max Packet Size: 0x40
			Interval: 0x5
	Interface
		Length: 0x9
		Descriptor Type: 0x4
		Interface Number: 0x8
		Alternate Setting: 0x0
		Num Endpoints: 0x2
		Interface Class: Communication Data
		Interface SubClass: 0x0
		Interface Protocol: 0x0
		Endpoint
			Length: 0x7
			Descriptor Type: 0x5
			Endpoint Address: 0x8a
			Attributes: 0x2
				Transfer Type: Bulk
			Max Packet Size: 0x200
			Interval: 0x20
		Endpoint
			Length: 0x7
			Descriptor Type: 0x5
			Endpoint Address: 0x6
			Attributes: 0x2
				Transfer Type: Bulk
			Max Packet Size: 0x200
			Interval: 0x20
	Interface
		Length: 0x9
		Descriptor Type: 0x4
		Interface Number: 0x9
		Alternate Setting: 0x0
		Num Endpoints: 0x1
		Interface Class: Vendor Specific
		Interface SubClass: 0xff
		Interface Protocol: 0xff
		Endpoint
			Length: 0x7
			Descriptor Type: 0x5
			Endpoint Address: 0x8b
			Attributes: 0x3
				Transfer Type: Interrupt
			Max Packet Size: 0x40
			Interval: 0x5
	Interface
		Length: 0x9
		Descriptor Type: 0x4
		Interface Number: 0xa
		Alternate Setting: 0x0
		Num Endpoints: 0x2
		Interface Class: Communication Data
		Interface SubClass: 0x0
		Interface Protocol: 0x0
		Endpoint
			Length: 0x7
			Descriptor Type: 0x5
			Endpoint Address: 0x8c
			Attributes: 0x2
				Transfer Type: Bulk
			Max Packet Size: 0x200
			Interval: 0x20
		Endpoint
			Length: 0x7
			Descriptor Type: 0x5
			Endpoint Address: 0x7
			Attributes: 0x2
				Transfer Type: Bulk
			Max Packet Size: 0x200
			Interval: 0x20
	Interface
		Length: 0x9
		Descriptor Type: 0x4
		Interface Number: 0xb
		Alternate Setting: 0x0
		Num Endpoints: 0x1
		Interface Class: Vendor Specific
		Interface SubClass: 0xff
		Interface Protocol: 0xff
		Endpoint
			Length: 0x7
			Descriptor Type: 0x5
			Endpoint Address: 0x8d
			Attributes: 0x3
				Transfer Type: Interrupt
			Max Packet Size: 0x40
			Interval: 0x5
	Interface
		Length: 0x9
		Descriptor Type: 0x4
		Interface Number: 0xc
		Alternate Setting: 0x0
		Num Endpoints: 0x2
		Interface Class: Communication Data
		Interface SubClass: 0x0
		Interface Protocol: 0x0
		Endpoint
			Length: 0x7
			Descriptor Type: 0x5
			Endpoint Address: 0x8e
			Attributes: 0x2
				Transfer Type: Bulk
			Max Packet Size: 0x200
			Interval: 0x20
		Endpoint
			Length: 0x7
			Descriptor Type: 0x5
			Endpoint Address: 0x8
			Attributes: 0x2
				Transfer Type: Bulk
			Max Packet Size: 0x200
			Interval: 0x20

There you have it. You've successfully used the tools to boot up the baseband without any help from CommCenter. If you want to shutdown the baseband again or if you had problems during any of the steps just use "bbtool reset" to reset the baseband and try again. I will try to finish up my QMITool and DIAGTool for sending commands to the baseband shortly as well as documenting what each interface exposed by AMSS is used for.

Thanks for the interest!!
@p0sixninja

