
# Wio Terminal TTL Camera Jpeg for a cheap wildlife or security camera.
This repo shows how to use an Adafruit TTL weatherprrof camera with Wio Terminal and saving jpg's to SD Card.

This was inspired by this project https://www.instructables.com/Arduino-Wildlife-Night-Camera/

My plan was/is to use this for a AI model driven camera as a predator wildlife incursion detection.  I built a model on Edge Impulse deployed as an Arduino library and started writing the code, but have not completed it yet - mainly because I found it pretty hard to edit the images suitable for the inference. I probably won't finish it as Seeed has released a Grove AI Camera (https://wiki.seeedstudio.com/K1100-Vision-AI-Module-Grove-LoRa-E5/) for the Wio Terminal and an industrial version, so I will use that instead.

The main value of this repo is using the Adafruit camera with Wio Terminal and saving the images to SD - it really is a great camera, but the jpg saving is a bit slow.  The IR and motion detection are excellent - better than my commercial (though cheap) Trail Camera's.

NB: It is a bit of a work in progress and has lots of bugs.

**Hardware used:**
Seeed Wio Terminal - https://www.seeedstudio.com/Wio-Terminal-p-4509.html
Adafruit Weatherproof TTL Serial JPEG Camera with NTSC Video and IR LEDs - https://www.adafruit.com/product/613
Seeed Wio Terminal Chassis - Battery (650mAh) https://www.seeedstudio.com/Wio-Terminal-Chassis-Battery-650mAh-p-4756.html

**Functionality:**
Wio Terminal Button Functionality - 
Button 1 - Top Left - Toggle Motion Detection
Button 2 - Top Middle - Take Photo
Button 3 - Top Right - Toggle Screen on/off

5 Way Button - Up - Show Battery Stats; Wake from Sleep (doesn't really work) 
5 Way Button - Down - Sleep - uses Adafruit SleepyDog - doesn't seem really work that great with Wio Terminal
5 Way Button - Left - Previous Photo - bug with Array that stores images
5 Way Button - Right - Next Photo
5 Way Button - Centre - Delete Current Photo

**Wiring**
Pretty straight forward with Wio.  Just plug in the wires as below.
![Wiring the TTL Camera to Wio Terminal](https://forum.seeedstudio.com/uploads/default/original/2X/c/cc33014fa102a072a099e36ea93114e7cfaa9638.jpeg)
