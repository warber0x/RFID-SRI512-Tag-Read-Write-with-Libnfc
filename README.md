#  Read/Write/Get UID from RFID SRI512 Tag with Libnfc

This little project will show you how to read/write SRx RFID tags using libnfc.

I have an Adafruit PN532 shield that I use with Libnfc. I compiled the library to communicate through UART with arduino and the later as a SPI gateway to PN532. 

I was struggling at first to find a way on how to read SRx Tags using libnfc, as the library couldn't go deep into these type of RFID tags but in the contrary it just showing you the UID and the type of protocol it uses.

I decided then to understand how libnfc works and try to find a solution on how to add a custom function that can extend the ability of this great opensource library that was forgotten a little bit.

Requirements:

- Arduino UNO of course.
- PN532 shield (SPI pins shorted).
- Working version of libnfc (That can communicate with Arduino). Compiled to use UART.
- SRx Tag (Mine is SRI512).
- Your time.

If you don't know how to compile libnfc for your needs. Go to this link: http://nfc-tools.org/index.php/Libnfc:Arduino

The SR512 documentation and how to communicate with them: https://media.digikey.com/pdf/Data%20Sheets/ST%20Microelectronics%20PDFS/SRI512.pdf

Let's begin:

1- You have to add a custom functions to libnfc. I have a tutorial on how to do this in my repositories. There is two methods, I will use the second as it's clean and easy. Here is the link to the second method: [Method 2](https://github.com/warber0x/Libnfc_custom_function/blob/master/Custom_function_method_2.md)

2- If you read this, it means that you were able to make the second method work without any problem. Now, we have to make some modifications on your custom function. I programmed three functions (Read, Write, ReadUID) and one main that contains all the three functions. See the "srx.c" and the "main.c" file. You have to take the part that you're intrested in and put it in your code.

N.B Thus far, I weren't able to make all the three functions work one after another. I think this is due to the PN532's registers that must be resetted after each call. For my case, I create three seperate programs, each one did one thing (Read/Write/UID).

3- I suggest that you begin with readUID as it's much easier to implement. Then, step by step till you complete all the three functions.

The code is straight forward, if you got your hands dirty on the second method what it will remain here is just to change some lines of code especially the SRI512 commands. This is not a complete tutorial, but it can help to move forward if you're stuck.

A demo: https://www.youtube.com/watch?v=HZDPleJ0gAw

More to come especially in System_OTP_bits feature ...

Stay tuned.



