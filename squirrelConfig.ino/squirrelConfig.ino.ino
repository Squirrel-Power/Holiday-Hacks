// Uses the Grove - Rotary Angle Sensor to control the position of the Grove - Servo.
// Connect the Grove - Servo to the socket marked D3
// Connect the Grove - Rotary Angle Sensor to A1
// Connect the Grove - single LED to D5

// Write a config file to SD card:
// /media/sdcard/squirrelConfig.txt
/*
 * SD card attached to SPI bus as follows:
 ** MOSI - pin 11
 ** MISO - pin 12
 ** CLK - pin 13
 ** CS - pin 4
*/
#include <Servo.h>  //For the groove servo
#include <WiFi.h>   //For the Web Service
#include <SPI.h>
#include <SD.h>

//File Input/Output Settings
File myFile;  //Global file handle

//Wifi Settings - 
//Todo: should all go into a configuration 
//Dustin's house
//char ssid[] = "CyberGuest";      // your network SSID (name) 
//char pass[] = "Belize38";         // your network password
//char ssid[] = "CyberNet";      // your network SSID (name) 
//char pass[] = "12Belize16";         // your network password
//My House - Make this input from config
char ssid[] = "TheDarkSide";      // your network SSID (name) 
char pass[] = "Shorty2y";         // your network password
int keyIndex = 0;                 // your network key Index number (needed only for WEP)

int status = WL_IDLE_STATUS;

WiFiServer server(8080);  //to get to web page use IP:8080
String readString; //Global string to keep WEB requests - debug

// Define the pins to which the servo (D3) and sensor(A1) are connected.
const int pinServo = 3;
const int potentiometer = 1;
// Use a Servo object to represent and control the servo.
Servo groveServo;
int pos = 0;  //position of servo


// Grove - LED connect to D8 (using the pin9)
// the following pin which support PWM can be used:
// 3, 5, 6, 9, 10, 11
const int pinLed = 5; //  and the single LED

//button at D4
// tmp sensor = A0
// LCD screen at I2C (the one closest to A0...)

/******************************************************
 * readWiFiSettings - Read WiFi settings
 ******************************************************/
bool readWiFiSettings()
{
  // Open the file for reading:
  myFile = SD.open("squirrelConfig.txt");
  if (myFile) {
    Serial.println("squirrelConfig.txt");
    Serial.println("Open WIFI conf.\n");
   // read from the file until there's nothing else in it:
    while (myFile.available()) 
    {
    	Serial.write(myFile.read());
    }
    // close the file:
    myFile.close();
  } else {
  	// if the file didn't open, print an error:
    Serial.println("error opening squirrelConfig.txt\n\n");
    return false;
  }
  
  return true;
}

/******************************************************
 * writeConfig - write osme default valeus for now.
 ******************************************************/
bool writeConfig()
{
   // open the file. note that only one file can be open at a time,
  // so you have to close this one before opening another.
  myFile = SD.open("squirrelConfig.txt", FILE_WRITE);
  
  // if the file opened okay, write to it:
  if (myFile) 
  {
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
    Serial.println("error opening squirrelConfig.txt");
    return false;
  }
    return true;
}

/******************************************************
 * ServoSetup - 
 * ToDo: groveServo is Still Global, fix this.
 ******************************************************/
bool ServoSetup()
{
  // Set up the servo motor
  // Tell the Servo object which pin to use to control the servo.
  groveServo.attach(pinServo);
  // Configure the angle sensor's pin for input signals.
  pinMode(potentiometer, INPUT);
  ////////////////////////////////////////////////////////
  // On the Ethernet Shield, CS is pin 4. It's set as an output by default.
  // Note that even if it's not used as the CS pin, the hardware SS pin 
  // (10 on most Arduino boards, 53 on the Mega) must be left as an output 
  // or the SD library functions will not work. 
   pinMode(10, OUTPUT);
}

/******************************************************
 * WiFiSetup - 
 ******************************************************/
bool WiFiSetup()
{
     // check for the presence of the shield:
   Serial.print("check for the presence of the shield \n");
   if (WiFi.status() == WL_NO_SHIELD) 
   {
     Serial.println("ERROR: WiFi shield not present! \n"); 
     // don't continue:
     while(true);
   } 
  
  Serial.print("\t checking WiFi...\n");
  String fv = WiFi.firmwareVersion();
  if( fv != "1.1.0" )
  {
    Serial.println("Please upgrade the firmware!  \n\n");
    return false;
  }
  
  // attempt to connect to Wifi network:
  Serial.print("\t connecting WiFi...\n");
  int connection_count = 0;
  while ( status != WL_CONNECTED) 
  { 
    //check for timeout
    if(connection_count > 1000)
    {
      Serial.print("ERROR: Attempting to connect to SSID: FAILED, to many attemps\n\n");
      connection_count = 0;
      return false;
    }
    
    Serial.print("\t Attempting to connect to SSID: ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network. Change this line if using open or WEP network:    
    status = WiFi.begin(ssid, pass);

    // wait 10 seconds for connection:
    delay(10000);
    connection_count++;
  } 
  
  connection_count = 0;
  server.begin();
  // you're connected now, so print out the status:
  printWifiStatus();
}

/******************************************************
 * WiFiSetup - 
 ******************************************************/
bool SerialSetup()
{
  /////////////////////////////////////////////////////////
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
   while (!Serial) 
   { ; } // wait for serial port to connect. Needed for Leonardo only
}

/******************************************************
 * SinglePinLedSetup - 
 * Grove - LED connect to D8 (using the pin9)
 * the following pin which support PWM can be used:
 * 3, 5, 6, 9, 10, 11
 ******************************************************/
bool SinglePinLedSetup()
{
   pinMode(pinLed, OUTPUT);  // set led OUTPUT
}
/******************************************************
 * setup - This is run only once!
 ******************************************************/
void setup()
{  
  ////////////////////////////////////////////////////////
  // Serial Device Setup
  SerialSetup();
  
  // Individual Device Setup
  WiFiSetup();
  ServoSetup();
  SinglePinLedSetup();
  
  ////////////////////////////////////////////////////////
  // File I/O
  
  // Check SD card Status - where we Store Config Files
  Serial.print("Initializing SD card...\n");
  if (!SD.begin(4)) 
  {
    Serial.println("ERROR: initialization failed!\n\n");
    return;
  }
  Serial.println("\t initialization done.\n");
  
  //Set up the Wifi Connection settings (IP, WEP & password)
  if(!readWiFiSettings())
  {
    Serial.println("Error reading WiFi initialization file. \n\n");
  }
  
  // Write some default values to the config file
  if(!writeConfig())
  {
    Serial.println("Error writing to /media/sdcard/squirrelConfig.txt file. \n\n");
  }
    
}


/******************************************************
 * loop - This is run as a continuous loop
 ******************************************************/
void loop()
{
  
  //////////////WEB 
  // listen for incoming clients
  WiFiClient client = server.available();
  if (client) 
  {
    Serial.println("new client");
    // an http request ends with a blank line
    boolean currentLineIsBlank = true;
    while (client.connected()) 
    {
      if (client.available()) 
      {
        char c = client.read();
        
        //read char by char HTTP request and store it (just in case)
        if (readString.length() < 100) {
          //store characters to string
          readString += c;
          //Serial.print(c);
         }
        
        
        Serial.write(c);
        // if you've gotten to the end of the line (received a newline
        // character) and the line is blank, the http request has ended,
        // so you can send a reply
        if (c == '\n' && currentLineIsBlank) {
          //DEBUG
          Serial.println(readString); //print to serial monitor for debuging
          
          // send a standard http response header
          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: text/html");
          client.println("Connection: close");  // the connection will be closed after completion of the response
          client.println("Refresh: 5");  // refresh the page automatically every 5 sec
          client.println();
          client.println("<!DOCTYPE HTML>");
          client.println("<html>");
          //set the Squirrel Power background - 
          //  MR: I never found the document root for the YUN web service, but that is where the
          //      Image should go.
          //client.println("<body background=\"\\home\\root\\squirrelPower.jpg\" bgproperties=\"fixed\">"); 
          client.println("<IMG SRC=\"squirrelPower.jpg\" ALT=\"Squirrel Power - may the force be with you!\" WIDTH=250 HEIGHT=176>");

          //Let add a button to kick off something
           client.println("<br />");  
           client.println("<input type=button onClick=\"parent.location=\'?button1on\'\" value=\'Turn On LED\'>");
           client.println("<input type=button onClick=\"parent.location=\'?button1off\'\" value=\'Turn Off LED\'><br />");
           client.println("<br />"); 
           
         // Totally for the Lego to fire from servo!
         // Note the use of the '?' in the page name!  it is important to match against
           client.println("<br />"); 
           client.println("<input type=button onClick=\"parent.location=\'?button2on\'\" value=\'Rotate Left\'>");
           client.println("<input type=button onClick=\"parent.location=\'?button2off\'\" value=\'Rotate Right\'>");
           //client.println("<br />"); 

           client.println("<p>Created by Squirrel Power</p>");  
           client.println("<br />"); 
          
          // output the value of each analog input pin
          for (int analogChannel = 0; analogChannel < 6; analogChannel++) {
            int sensorReading = analogRead(analogChannel);
            client.print("analog input ");
            client.print(analogChannel);
            client.print(" is ");
            client.print(sensorReading);
            client.println("<br />");       
          }
          
          //END of PAGE
          client.println("</html>");
           break;
        }        
        
        if (c == '\n') {
          // you're starting a new line
          currentLineIsBlank = true;
        } 
        else if (c != '\r') {
          // you've gotten a character on the current line
          currentLineIsBlank = false;
        }
      }
    } //end while (client.connected()) 
    
    // give the web browser time to receive the data
    delay(1);
    
    // close the connection:
    client.stop();   
   
    Serial.println("client disonnected");
    
    
    //////////////WEB - responce 
    //controls the Arduino if you press the buttons
           if (readString.indexOf("?button1on") >0)
           {
               digitalWrite(pinLed, HIGH);
               Serial.println("Single LED Light ON \n");
           }
           if (readString.indexOf("?button1off") >0)
           {
               digitalWrite(pinLed, LOW);
               Serial.println("Single LED Light OFF \n");
           }
           
           //More Lego fun...
          //////////////Web Controled SERVO MOTOR 
           if (readString.indexOf("?button2on") >0)
           {
                for(pos = 0; pos < 180; pos += 3)  // goes from 0 degrees to 180 degrees 
                {                                  // in steps of 1 degree 
                  groveServo.write(pos);              // tell servo to go to position in variable 'pos' 
                  delay(15);                       // waits 15ms for the servo to reach the position 
                } 
           }
           if (readString.indexOf("?button2off") >0)
           {
                for(pos = 180; pos>=1; pos-=3)     // goes from 180 degrees to 0 degrees 
                {                                
                  groveServo.write(pos);              // tell servo to go to position in variable 'pos' 
                  delay(15);                       // waits 15ms for the servo to reach the position 
                } 
           }
           
           
            //clearing string for next read
            readString="";  
           
     }// end - if (client)
         
         
  //////////////Analog SERVO MOTOR
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

/******************************************************
 * SinglePinLedBreath - 
 * Not Used, but gives dimming of single LCD
 ******************************************************/
bool SinglePinLedBreath()
{
     for(int i=0; i<256; i++)
    {
        analogWrite(pinLed, i);
        delay(5);               // change delay time can breath faster or slower
    }
    delay(100);
    
    for(int i=254; i>=0; i--)
    {
        analogWrite(pinLed, i);
        delay(5);               // change delay time can breath faster or slower
    }
    delay(500);
}

/******************************************************
 * printWifiStatus
 ******************************************************/
void printWifiStatus() {
  // print the SSID of the network you're attached to:
  Serial.print("\t\tSSID: ");
  Serial.println(WiFi.SSID());

  // print your WiFi shield's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("\t\tIP Address: ");
  Serial.println(ip);

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("\t\tsignal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
}

