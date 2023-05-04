# ArduinoCLIControl
This goal of this repo is to provide an example of how you would program and communicate with an Arduino dev board from an Ubunutu CLI instead of using the Arduino IDE.

For this example I made a library for communicating with arduino nano, specifically for communicating with the ATmega328P microprocessor with UART, and then toggling the on board LED via minicom package.


# How To
1. Install required packages:
```
sudo apt install minicom\
sudo apt-get install gcc-avr avr-libc avrdude
```
2. (Optional) Create udev rules so you can always find your device when connected.
    - This a good guide for that [Udev rules](https://steve.fi/hardware/arduino-basics/) 
4.  Make a new project folder. If you want to try out this repo you could just do:
```
git clone https://github.com/jc-cr/ArduinoCLIControl.git
```
4. Pump out some code for your microcontroller. 
    - In my repo I created a `main.cc` in which I included my library for UART comm and LED toggling, `ledController.h`.

5. Create a Makefile.
    - Check out my for an example, but also check this out for how to make your own:
    - [AVR Makefile Example](https://gist.github.com/SammysHP/8a595ef498ac903be35fcbcf61804c38)
6. Compile and upload (Assuming your device is connected):
```
make 
make upload
```
7. Connect and commincate. With minicom you caa do:
    - Make sure to update with your device symlink name or USB port (found with `lsusb`)
```
minicom -D /dev/yourDevice -b 9600
```
You can exit minicom by doing the following keystrokes:
```
Ctrl + A
X
Enter
```
