// Uses the Grove - Rotary Angle Sensor to control the position of the Grove - Servo.
// Connect the Grove - Servo to the socket marked D3
// Connect the Grove - Rotary Angle Sensor to A1

// Write a config file to SD card:
// /media/sdcard/test.txt
/*
 * SD card attached to SPI bus as follows:
 ** MOSI - pin 11
 ** MISO - pin 12
 ** CLK - pin 13
 ** CS - pin 4
*/
#include <Servo.h>

#include <SPI.h>
#include <SD.h>

File myFile;

// Define the pins to which the servo and sensor are connected.
const int pinServo = 3;
const int potentiometer = 1;

// Use a Servo object to represent and control the servo.
Servo groveServo;

void setup()
{
    // Set up the motor
    // Tell the Servo object which pin to use to control the servo.
    groveServo.attach(pinServo);
    // Configure the angle sensor's pin for input signals.
    pinMode(potentiometer, INPUT);
    
    
// Open serial communications and wait for port to open:
  Serial.begin(9600);
   while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo only
  }
  Serial.print("Initializing SD card...");
  // On the Ethernet Shield, CS is pin 4. It's set as an output by default.
  // Note that even if it's not used as the CS pin, the hardware SS pin 
  // (10 on most Arduino boards, 53 on the Mega) must be left as an output 
  // or the SD library functions will not work. 
   pinMode(10, OUTPUT);
   
  if (!SD.begin(4)) {
    Serial.println("initialization failed!");
    return;
  }
  Serial.println("initialization done.");
  
  // open the file. note that only one file can be open at a time,
  // so you have to close this one before opening another.
  myFile = SD.open("squirrelConfig.txt", FILE_WRITE);
  
  // if the file opened okay, write to it:
  if (myFile) {
    Serial.print("Writing to test.txt...");
    
    myFile.println("servo:min=0");
    myFile.println("servo:max=1023");
    
    myFile.println("LED1:R=255");
    myFile.println("LED1:G=0");
    myFile.println("LED1:B=100");
    
    myFile.println("LED2:R=0");
    myFile.println("LED2:G=180");
    myFile.println("LED2:B=100");
	// close the file:
    myFile.close();
    Serial.println("done.");
  } else {
    // if the file didn't open, print an error:
    Serial.println("error opening test.txt");
  }
  
  // re-open the file for reading:
  myFile = SD.open("test.txt");
  if (myFile) {
    Serial.println("test.txt:");
    
    // read from the file until there's nothing else in it:
    while (myFile.available()) {
    	Serial.write(myFile.read());
    }
    // close the file:
    myFile.close();
  } else {
  	// if the file didn't open, print an error:
    Serial.println("error opening test.txt");
  }
    
}

void loop()
{
    // Read the value of the angle sensor.
    int sensorPosition = analogRead(potentiometer);

    // The analog value from the angle sensor is between 0 and 1023, but
    // the servo only accepts values between 0 and 179; use the map()
    // function as a linear conversion between the two ranges.
    int shaftPosition = map(sensorPosition, 0, 1023, 0, 179);

    // Use the Servo object to move the servo.
    groveServo.write(shaftPosition);
   

    delay(15);
}
