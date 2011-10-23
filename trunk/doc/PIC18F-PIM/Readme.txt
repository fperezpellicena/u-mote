www.microchipDIRECT.com part number, Applicable Documentation

MA180021, "PIC18F87J50 FS USB Demo Board (PIM) Users Guide (51678a).pdf"
MA180024, "PIC18F46J50 FS USB Demo Board (PIM) Users Guide (51806a).pdf"
MA240014, "PIC24FJ256GB110 PIM Info Sheet (39908a).pdf"
MA320002, "PIC32MX460F512L PIM Info Sheet (51775a).pdf"
AC164131, "USB PICTail Plus Info Sheet (39909a).pdf"
DM163025, "PICDEM FS USB Demo Board Users Guide (51526b).pdf"
DM164127 and DV164126, "Low Pin Count Development Kit Users Guide (41356b).pdf" and "Getting Started with Low Pin Count Dev Kit.pdf"

Microchip also produces a variety of additional USB related demo boards
not directly listed above.  The above list includes the boards that this CD-ROM
disc (may) get shipped with.  

Check the USB design center (www.microchip.com/usb) for more information
regarding the available USB related demo boards, as well as to obtain the latest 
MCHPFSUSB package and related documents.


Note: The PICDEM FS USB Demo Board User's Guide 51526, revision B, was written with the intent
that the demo board would be used with an older version of the MCHPFSUSB framework package.  In
this newer release of the MCHPFSUSB framework package provided on this CD, the directory 
names/location of the files referred to in the user's guide are somewhat different, but 
the files are still included in this new version.

If using the PICDEM FS USB Demo Board, it should come pre-programmed from
the factory with some default USB firmware.  Upon plugging in the device, the
operating system should detect the device and request a driver.  To use the default
factory pre-programmed demo in the PICDEM FS USB Demo Board, first install the MCHPFSUSB
framework, and then point the new hardware wizard to the mchpusb.inf file in the directory:

[Base MCHPFSUSB installation directory]\USB Tools\MCHPUSB Custom Driver\MCHPUSB Driver\Release

