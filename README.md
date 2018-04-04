Ultra low cost IR timing gate transponder compatible with i-lap rc timing gates. How low can you go? We've seen builds as cheap as $3 per transponder!
[Now with a detailed build guide!](http://mrickert.com/2016/10/12/building-your-own-coreir-transponder/)


Comes ready for you to input any 7 digit transponder ID you'd like, edit the arduino file to set which transponder ID to use.

Don't want to edit arduino files? Use the new [CoreIR-Uplink desktop app!](https://github.com/slacker87/CoreIR-Uplink)

ID range: 1111111 to 9999999

Required parts:
* Arduino pro mini or pro micro (pro micro preferred)
* 9.1 ohm resistor
* npn transistor (2N222 preferred)
* IR led (TSAL6400 preferred)
* Arduino IDE 1.8.3 or higher

Testing:
You should see the following once you have built your tranceiver to confirm the code/hardware is working properly...
* Once the firmware is flashed, the led on the arduino should blink two times per second
* If the arduino is blinking around 2x per second, then the code is running correctly
* If the arduino is not blinking, or is blinking seemingly at random, the code needs to be reflashed as the upload failed (or there's a hardware issue)
* With the arduino blinking, point your phone camera at the IR led, the led should be emitting a purple hue
* If you see the purple color from the led on your phone camera, congratulations! Everthing is running smoothly.
* If you do not see any purple hue, but the arduino is blinking steadily, there is a hardware wiring issue or the led has been damaged, check your wiring.

Wiring:

For micro and pro micro boards:
![alt tag](https://raw.githubusercontent.com/slacker87/CoreIR/master/micro.png)

For nano and pro mini boards:
![alt tag](https://raw.githubusercontent.com/slacker87/CoreIR/master/mini.png)
