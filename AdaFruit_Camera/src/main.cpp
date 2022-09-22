#include <SparkFunBQ27441.h>
#include <Adafruit_VC0706.h>
#include"TFT_eSPI.h"
#include <SPI.h>
#include <Seeed_FS.h>
#include "SD/Seeed_SD.h"
#include <JPEGDEC.h> 
#include "RTC_SAMD51.h"
#include "DateTime.h"
#include <Adafruit_SleepyDog.h>
 
RTC_SAMD51 rtc;
TFT_eSPI tft;
File myFile;

#define cameraconnection Serial1

Adafruit_VC0706 cam = Adafruit_VC0706(&cameraconnection);

bool motionOn = false;// set motion off by default
bool displayOn = true;//display is on by default
int imageCounter = 0;
int8_t cnt = 0;

JPEGDEC jpeg;
char sdImages[][17] {"","","","","","","","","","","","","","","","","","","","","",
"","","","","","","","","","","","","","","","","",
"","","","","","","","","","","","","","","","","",
"","","","","","","","","","","","","","","","","",
"","","","","","","","","","","","","","","","","",
"","","","","","","","","","","","","","","","","",
"","","","","","","","","","","","","","","","","",
"","","","","","","","","","","","","","","","","",
"","","","","","","","","","","","","","","","","",
"","","","","","","","","","","","","","","","","",
"","","","","","","","","","","","","","","","","",
"","","","","","","","",""};
//char mytext[][10] = 

int showImageCounter = 0;

// Functions to access a file on the SD card
File myfile;

void * myOpen(const char *filename, int32_t *size) {
  myfile = SD.open(filename);
  *size = myfile.size();

  return &myfile;
}
void myClose(void *handle) {
  if (myfile) myfile.close();
}
int32_t myRead(JPEGFILE *handle, uint8_t *buffer, int32_t length) {
  if (!myfile) return 0;
  return myfile.read(buffer, length);
}
int32_t mySeek(JPEGFILE *handle, int32_t position) {
  if (!myfile) return 0;
  return myfile.seek(position);
}

// Function to draw pixels to the display
int JPEGDraw(JPEGDRAW *pDraw) {
 // Serial.printf("jpeg draw: x,y=%d,%d, cx,cy = %d,%d\n",
  //   pDraw->x, pDraw->y, pDraw->iWidth, pDraw->iHeight);
//tft.pushRect()
  tft.pushRect(pDraw->x, pDraw->y, pDraw->iWidth, pDraw->iHeight, pDraw->pPixels);
  return 1;
}

int cnter = 0;
DateTime now = DateTime(F(__DATE__), F(__TIME__));
    
void loadFilesFromSD(){
  cnter = 0;
  File dir=SD.open("/");
  dir.rewindDirectory();
  
  while(true) {
     File entry =  dir.openNextFile();
     if (! entry) {
       // no more files
      Serial.println("**nomorefiles**");
       break;
     }
     // Print the 8.3 name
    // tft.print(entry.name());
     // Recurse for directories, otherwise print the file size
     if (entry.isDirectory()) {
       Serial.println("/");
     } 
	 else{
       // files have sizes, directories do not
       Serial.print("\t\t");
       Serial.println(entry.size(), DEC);
       String fname = String(entry.name());
if (fname.indexOf("IMG_") > 0) {
//.. found.
//tft.print(fname);
//tft.print(":");
Serial.println(String(entry.name()));
//strcpy(sdImages[cnter],entry.name());
//sdImages[cnter] = entry.name();
strcpy(sdImages[cnter], entry.name());

cnter++;
} 
	   //if(bmpDraw(entry.name(),0,0))delay(5000);
     }
     entry.close();
   }
showImageCounter = cnter;
}

String currentImage = "";//holds current image

void showImage(const char* filename){

   tft.setCursor(20, 0);
  //const char *name = "parrot_o.jpg";
        Serial.print("File: ");
        Serial.println(filename);
       // tft.print("File: ");
        //tft.println(name);
        
       const char* cfilename = filename;// filename.toCharArray(cbuff,filename.length()+1);

        jpeg.open(cfilename, myOpen, myClose, myRead, mySeek, JPEGDraw);
        
        if(jpeg.getWidth()>0){
        currentImage = filename;//set for delete
        jpeg.setPixelType(RGB565_BIG_ENDIAN);
        jpeg.decode(0, 0, JPEG_SCALE_HALF);
        jpeg.close();
          }else{
          tft.println("Image not available.");
          }
}

bool left_flag = false;
void button_handler_left() {
  Serial.print(String(showImageCounter));
  if(showImageCounter > 0){
    showImageCounter--;
   String filen = "";
  /*if (showImageCounter<10){
    filen = sdImages[];//"IMAGE0"+String(showImageCounter)+".JPG";
  }else{
    filen = "IMAGE"+String(showImageCounter)+".JPG";
  }

for (int cnt = 0; cnt < sizeof(sdImages) / sizeof(sdImages[0]); cnt++)
{ 
  String x =  String(sdImages[cnt]);
    x.trim();
  
  if(x.length()>0){
  
    Serial.println(x);
   Serial.println(x.length()); 
    }else{
    Serial.println("Zero");
    }
}*/

filen = sdImages[showImageCounter];

    showImage(filen.c_str());
  }
  left_flag = true;
}
 
bool right_flag = false;
void button_handler_right() {
  
  Serial.print(String(showImageCounter));
    if(showImageCounter < cnter){
    showImageCounter++;
    
  Serial.print("SHOWIMAGE");
  String filen = "";
/*  if (showImageCounter<10){
    filen = "IMAGE0"+String(showImageCounter)+".JPG";
  }else{
    filen = "IMAGE"+String(showImageCounter)+".JPG";
  }
*/
    filen = sdImages[showImageCounter];

    showImage(filen.c_str());
  }
  right_flag = true;
}


 
const unsigned int BATTERY_CAPACITY = 650; // Set Wio Terminal Battery's Capacity 
 
void setupBQ27441(void)
{
  // Use lipo.begin() to initialize the BQ27441-G1A and confirm that it's
  // connected and communicating.
  if (!lipo.begin()) // begin() will return true if communication is successful
  {
  // If communication fails, print an error message and loop forever.
    tft.setCursor((320 - tft.textWidth("Battery Not Initialised!"))/2, 120);
    tft.print("Battery Not Initialised!");
   // while (1) ;
  }else{
  Serial.println("Connected to BQ27441!");
  }
  // Uset lipo.setCapacity(BATTERY_CAPACITY) to set the design capacity
  // of your battery.
  lipo.setCapacity(BATTERY_CAPACITY);
}

bool sleepMode = false;

void letsGoToSleep(){

sleepMode = true;
Serial.println("Going to Sleep in 1 second");



}
 
void printBatteryStats()
{
  // Read battery stats from the BQ27441-G1A
  unsigned int soc = lipo.soc();  // Read state-of-charge (%)
  unsigned int volts = lipo.voltage(); // Read battery voltage (mV)
  int current = lipo.current(AVG); // Read average current (mA)
  unsigned int fullCapacity = lipo.capacity(FULL); // Read full capacity (mAh)
  unsigned int capacity = lipo.capacity(REMAIN); // Read remaining capacity (mAh)
  int power = lipo.power(); // Read average power draw (mW)
  int health = lipo.soh(); // Read state-of-health (%)
 
  // Now print out those values:
  String toPrint = String(soc) + "% | ";
  toPrint += String(volts) + " mV | ";
  toPrint += String(current) + " mA | ";
  toPrint += String(capacity) + " / ";
  toPrint += String(fullCapacity) + " mAh | ";
  toPrint += String(power) + " mW | ";
  toPrint += String(health) + "%";
 
  Serial.println(toPrint);
 
  // LCD Graphics
  //tft.setTextColor(TFT_BLUE);
  //tft.drawRoundRect(10, 10, 300, 220, 10, TFT_BLUE);
  //tft.setTextColor(TFT_MAGENTA);
  tft.println("Battery State of Charge:");

  // Data
tft.print(String(soc)+" % ");
}

void setup() {
//ADD SOME TIME


    rtc.begin(); 
    Serial.println("adjust time!");

    rtc.adjust(now);

//turn on the tft
  tft.begin();
    tft.setRotation(3);
    tft.setTextSize(1); //set text size 
    tft.setCursor(10,10);
    digitalWrite(LCD_BACKLIGHT, HIGH); // turn on the backlight
  
  tft.println("Setup...");//prints string at (70,80)
  //char buf1[20];
//Updated now.day to now.date
//sprintf(buf1, "%02d:%02d:%02d %02d/%02d/%02d",  now.hour(), now.minute(), now.second(), now.day(), now.month(), now.year()); 

//tft.print(F("Date/Time: "));
//tft.println(now.timestamp());
Serial1.begin(57600);
  Serial.begin(57600);
  delay(500);
  while (!Serial1) {
  }

  tft.println("VC0706 Camera snapshot test");//prints string at (70,80)
  if (Serial1.available()) {
    tft.println("Serial 1 is available:");
  }else{
    tft.println("Serial 1 is NOT available:");
  }

  //need delay to initi serial
if (!SD.begin(SDCARD_SS_PIN, SDCARD_SPI)) {
    tft.println("SD initialization failed!");
   // while (1);
  }
  tft.println("SD initialization done.");

  // Try to locate the camera
  if (cam.begin()) {
    tft.println("Camera Found:");
   } else {
    tft.println("No camera found?");
   // return;
  }

  cam.TVoff();
  cam.setBaud115200();
  // Print out the camera version information (optional)
  char *reply = cam.getVersion();
  if (reply == 0) {
    tft.println("Failed to get version");
  } else {
   // tft.println("-----------------");
    tft.println(reply);
  }

  // Set the picture size - you can choose one of 640x480, 320x240 or 160x120 
  // Remember that bigger pictures take longer to transmit!
  
  cam.setImageSize(VC0706_640x480);        // biggest
  //cam.setImageSize(VC0706_320x240);        // medium
  //cam.setImageSize(VC0706_160x120);          // small

  // You can read the size back from the camera (optional, but maybe useful?)
 /* uint8_t imgsize = cam.getImageSize();
  Serial1.print("Image size: ");
  if (imgsize == VC0706_640x480) tft.println("640x480");
  if (imgsize == VC0706_320x240) tft.println("320x240");
  if (imgsize == VC0706_160x120) tft.println("160x120");
*/

  pinMode(WIO_KEY_A, INPUT_PULLUP); //rhs
  pinMode(WIO_KEY_B, INPUT_PULLUP);//middle
  pinMode(WIO_KEY_C, INPUT_PULLUP); //lhs

  pinMode(WIO_5S_UP, INPUT_PULLUP);
  pinMode(WIO_5S_DOWN, INPUT_PULLUP);
  pinMode(WIO_5S_LEFT, INPUT_PULLUP);
  pinMode(WIO_5S_RIGHT, INPUT_PULLUP);
  pinMode(WIO_5S_PRESS, INPUT_PULLUP);

delay(500);

loadFilesFromSD();//load from SD

 setupBQ27441();
 // tft.setTextColor(TFT_GREEN);
  //tft.setCursor((320 - tft.textWidth("Battery Initialised!"))/2, 120);
  tft.println("Battery Initialised!");
//pinMode(WIO_LIGHT, INPUT);
}

void deleteImage(const char* filename){
if (! SD.exists(filename)) {
       tft.println("File "+String(filename)+" doesn't exist - delete failed");
      return;
      }

SD.remove(filename);
if (!SD.exists(filename)) {
       tft.println("Delete succeeded");
}

}

void takePicture(){


//  tft.println("Taking Picture ...");
  
  if (! cam.takePicture()) 
    tft.println("Failed to snap!");
  else 
    tft.println("Picture taken!");

// Create an image with the name IMAGExx.JPG
  char filename[17];
  const char * f = "IMG_DDHHMMSS.JPG";
  //rtc.begin();
  //rtc.adjust(now);
  now = rtc.now();
  //rtc.
  Serial.println(now.timestamp());
 // const char * fa = now.timestamp().c_str();
 strcpy(filename, f);

   char  DD[3];
   int intDD = now.day();
   itoa(intDD, DD, 10);
char  HH[3];
   int intHH = now.hour();
   itoa(intHH, HH, 10);
char  MM[3];
   int intMM = now.minute();
   itoa(intMM, MM, 10);
char  SS[3];
   int intSS = now.second();
   itoa(intSS, SS, 10);
  //const char * suf = ".JPG";
if (intDD < 10) {
    filename[4] = '0';
    filename[5] = DD[0];
}else{    
  filename[4] = DD[0];
    filename[5] = DD[1];
}
if (intHH < 10) {
    filename[6] = '0';
    filename[7] = HH[0];
}else{    
  filename[6] = HH[0];
    filename[7] = HH[1];
}
if (intMM < 10) {
    filename[8] = '0';
    filename[9] = MM[0];
}else{    
  filename[8] = MM[0];
    filename[9] = MM[1];
}
if (intSS < 10) {
    filename[10] = '0';
    filename[11] = SS[0];
}else{    
  filename[10] = SS[0];
    filename[11] = SS[1];
}

  /*for (int i = 0; i < 200; i++) {
//    filename[5] = '0' + i/10;
  //  filename[6] = '0' + i%10;

    // create if does not exist, do not open existing, write, sync after write
    //tft.println(filename);
    if (! SD.exists(filename)) {
       tft.println("File doesn't exist - continue: "+String(filename));
      break;
    }
  }*/

tft.println(filename);
 // Open the file for writing
  //File imgFile = SD.open(filename, FILE_WRITE);
  myFile = SD.open(filename, FILE_WRITE);

  // Get the size of the image (frame) taken  
  uint16_t jpglen = cam.frameLength();
  //tft.println("Storing ");
  //tft.println(jpglen, DEC);
  //tft.println(" byte image.");

  //int32_t time = millis();
  //pinMode(8, OUTPUT);
  // Read all the data up to # bytes!
  byte wCount = 0; // For counting # of writes
  while (jpglen > 0) {
    // read 32 bytes at a time;
    uint8_t *buffer;
    uint8_t bytesToRead = min((uint16_t)32, jpglen); // change 32 to 64 for a speedup but may not work with all setups!
    buffer = cam.readPicture(bytesToRead);
    myFile.write(buffer, bytesToRead);
    if(++wCount >= 64) { // Every 2K, give a little feedback so it doesn't appear locked up
      tft.print('.');
      wCount = 0;
    }
    //Serial.print("Read ");  Serial.print(bytesToRead, DEC); tft.println(" bytes");
    jpglen -= bytesToRead;
  }
  myFile.close();
  
 //tft.println("...Done!");
  if(motionOn){
  cam.resumeVideo();
  cam.setMotionDetect(true);
  }
  //time = millis() - time;
//  tft.println("done!");
 // tft.println(time); tft.println(" ms elapsed");
imageCounter++;
cnter++;
tft.setCursor(10,10);
tft.fillScreen(TFT_LIGHTGREY);
tft.println("Images:");
tft.print(String(imageCounter));
tft.println("Last Image Name:");
tft.print(filename);

loadFilesFromSD();//load from SD
}

int sleepMS =1;

void loop() {


if (digitalRead(WIO_KEY_A) == LOW) {//right
    
    //turn off display
    if(displayOn){
      displayOn = false;
    digitalWrite(LCD_BACKLIGHT, LOW); // turn off the backlight
    }else{
      displayOn = true;
    digitalWrite(LCD_BACKLIGHT, HIGH); // turn on the backlight
    }

    tft.println("Toggle Display:");
    tft.print(String(displayOn));
    delay(500);
   }
   else if (digitalRead(WIO_KEY_B) == LOW) {//middle
    Serial.println("Take Picture");
    takePicture();
   }
   else if (digitalRead(WIO_KEY_C) == LOW) {//left
    Serial.println("Toggle Motion On");
    //takePicture();
tft.println("Motion detection is: ");
    
    if(motionOn){
      motionOn = false;
      //  Motion detection system can alert you when the camera 'sees' motion!
  cam.setMotionDetect(false);           // turn it on
  //cam.setMotionDetect(false);        // turn it off   (default)
tft.print("OFF");
  
    }else{
      motionOn = true;
      cam.setMotionDetect(true);   
    
      tft.print("ON");
    }

//delay(500);


}

//file system
if (digitalRead(WIO_5S_UP) == LOW) {
    Serial.println("5 Way Up");
    tft.println(sleepMS,DEC);
    tft.println(sleepMode);
    printBatteryStats();
   }
   else if (digitalRead(WIO_5S_DOWN) == LOW) {
    Serial.println("5 Way Down - Sleep");
    //letsGoToSleep();
   delay(1000);
sleepMode = true;   
 digitalWrite(LCD_BACKLIGHT, LOW); // turn off the backlight
    displayOn = false;
    sleepMS = Watchdog.sleep(50000);
    //Watchdog.
    Serial.println("I am asleep");
    delay(1000); // minimum of 600 ms after the attach to receive something in the monitor

   }
   else if (digitalRead(WIO_5S_LEFT) == LOW) {
    Serial.println("5 Way Left");
    button_handler_left();
   }
   else if (digitalRead(WIO_5S_RIGHT) == LOW) {
    Serial.println("5 Way Right");
    button_handler_right();
   }
   else if (digitalRead(WIO_5S_PRESS) == LOW) {
    Serial.println("5 Way Press");
    deleteImage(currentImage.c_str());
   }

if(sleepMode){
if(!Serial){
  Serial.begin(57600);
}

  tft.print(F("I'm awake now! I slept for "));
  Serial.print(sleepMS, DEC);
  Serial.print(F(" milliseconds.\n"));
  Serial.println("");
sleepMode = false;

}   
   
delay(500);
   
if(motionOn){
if (cam.motionDetected()) {
   tft.println("Motion!");   
   delay(100);//tiny delay to let subject get in front of cam??
   cam.setMotionDetect(false);
   
   
   takePicture();
}
}

//int light = analogRead(WIO_LIGHT);
  // Serial.print("Light value: ");
  // Serial.println(light);

}

bool isItNight(){

int light = analogRead(WIO_LIGHT);

if(light < 10){
return true;//it is quite dark
}

  return false;
}

