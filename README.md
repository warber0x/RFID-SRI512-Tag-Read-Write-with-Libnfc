# RFID SRI512 Tag Read/Write with Libnfc

This little project will show you how to read/write on SRx RFID tags using libnfc.

I have an Adafruit PN532 shield that I use with Libnfc. I compiled the library to communicate through UART with arduino and the later as a SPI gateway to PN532. 

I was struggling at first to find a way on how to read SRx Tags using libnfc as the library couldn't go deep into these type of RFID tags but just showing you the UID and the type of protocol it uses.

I decided then to understand how libnfc works and try to find a solution on how to add a custom function that can extend the ability of this great opensource library that was forgotten a little bit.

Requirements:

- Arduino UNO of course.
- PN532 shield (SPI pins shorted).
- Working version of libnfc (That can communicate with Arduino). Compiled to use UART.
- SRx Tag (Mine is SRI512).
- Your time.

If you don't know of to compile libnfc for your needs. Go to this link: http://nfc-tools.org/index.php/Libnfc:Arduino
The SR512 documentation and how to communicate with them: https://media.digikey.com/pdf/Data%20Sheets/ST%20Microelectronics%20PDFS/SRI512.pdf

Let's begin:

1- You have to add a custom functions to libnfc. I have a tutorial on how to do this in my repositories. There is two methods I will use the second as it's clean and easy. Here is the link to the second method: [Method 2]https://github.com/warber0x/Libnfc_custom_function/blob/master/Custom_function_method_2.md


