 /*
  
  Leds UDP Receiver Huzzah
     
 Description:
 * Firmware receiving RGB LED pixel data over UDP packets.

 Software:
 * FastLED Library 
 * Wifi Library 
 * WifiUdp Library
 
 Hardware:
* Adafruit Feather HUZZAH with ESP8266 WiFi
* WS2812B Led Strip
* 74AHCT125 - Quad Level-Shifter (3V to 5V) 
* 

 created 14 March 2018
 Based on Claude Heintz "sACN2Ws2811.ino"
 This code is under A Creative Commons Attribution/Share-Alike License http://creativecommons.org/licenses/by-sa/4.0/
   (2018) by Imanol Gómez

 */
 


#define DEBUG

#ifdef DEBUG
 #define DEBUG_PRINT_LN(x) Serial.println(x)
 #define DEBUG_PRINT(x) Serial.print(x)
#else
 #define DEBUG_PRINT(x)
 #define DEBUG_PRINT_LN(x)
#endif

#include <ESP8266WiFi.h>
#include "FastLED.h"
#include <WiFiUdp.h>


#define DATA_PIN_1    5
#define DATA_PIN_2    4
#define LED_TYPE    WS2812
//#define LED_TYPE    NEOPIXEL
#define COLOR_ORDER GRB
#define NUM_LEDS 150
#define TEST_DELAY 1000
CRGBArray<NUM_LEDS> leds1;
CRGBArray<NUM_LEDS> leds2;

#define OUTPUT_CHANNEL 0
#define HEADER_SIZE 6
#define BUFFER_MAX 2048
#define PORT 2390

const byte channelwidth = 3; //3 channels per pixel
//byte channel; //channel number for each pixel RGB
volatile byte currentcounter = 0; //counter for data reception
byte previouscounter = 0; //counter for data reception 
unsigned long now = 0; //current time value

char ssid[] = "TP-LINK_54E4"; //  your network SSID (name)
char pass[] = "27155332";    // your network password (use for WPA, or use as key for WEP)

//const char ssid[]     = "Don't worry, be happy!";
//const char pass[] = "whyistheskysohigh?";


IPAddress ip(192, 168, 0, 21); //  Fixed IP
IPAddress gateway(192, 168, 0, 1); // set gateway to match your network
IPAddress subnet(255, 255, 255, 0); // set subnet mask to match your network

//Are we currently connected?
boolean connected = false;


WiFiUDP Udp;

unsigned char packetBuffer[BUFFER_MAX];
char ReplyBuffer[] = "acknowledged";


// the setup function runs once when you press reset or power the board
void setup() {

  setupSerial();
  setupWifi();  
  setupLeds();
  
}


void setupLeds() 
{
    FastLED.addLeds<LED_TYPE,DATA_PIN_1,COLOR_ORDER>(leds1, NUM_LEDS);
    FastLED.addLeds<LED_TYPE,DATA_PIN_2,COLOR_ORDER>(leds2, NUM_LEDS);
    //FastLED.setMaxPowerInVoltsAndMilliamps (5, 2100);
    FastLED.clear();
}

void setupWifi() {

//  initializeWifi();
//  
//  // Print WiFi MAC address:
//  printMacAddress();
//
//  connectWifi();
//  printWiFiStatus();

  connectToWiFi(ssid, pass);
  
}

void setupSerial() {
   //Initialize serial and wait for port to open:
  #ifdef DEBUG
    Serial.begin(9600);
  #endif
  delay(100);
  DEBUG_PRINT_LN("Starting Software!!");
}


void initializeWifi() {
   
   // udp.stop();
    //udp.flush();
    WiFi.disconnect(true);
    WiFi.config(ip, gateway, subnet);
    WiFi.setAutoReconnect(true);
}



// the loop function runs over and over again forever
void loop() {
 int packetSize = Udp.parsePacket();
    
    if(packetSize)
    {
      //DEBUG_PRINT_LN("Received Data!!!");
      Udp.read(packetBuffer,BUFFER_MAX); //read UDP packet
      
      int count = checkProtocolHeaders(packetBuffer, packetSize);
      if (count) 
      {
              parseRGBReceived(packetBuffer, count); //process data function
              currentcounter++;  //increase counter by 1 each time through 
      }
    }

}


void connectWifi() {

  int ledState = 0;
     // attempt to connect to WiFi network:
   DEBUG_PRINT("Attempting to connect to SSID: ");
   DEBUG_PRINT_LN(ssid);


  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED) 
  {
    // Blink LED while we're connecting:
    digitalWrite(LED_BUILTIN, ledState);
    ledState = (ledState + 1) % 2; // Flip ledState
    delay(500);
    DEBUG_PRINT(".");
  }
 
   DEBUG_PRINT("\nConnected to SSID: ");
   DEBUG_PRINT_LN(ssid);

    //Serial.print("\nStarting connection to UDP port ");
    //Serial.println(udpPort);
    // if you get a connection, report back via serial:
    //udp.begin(udpPort);
    //udp.flush();
}

void connectToWiFi(const char * ssid, const char * pwd){
  Serial.println("Connecting to WiFi network: " + String(ssid));

  // delete old config
  WiFi.disconnect(true);
  delay(100);
  //register event handlerpin
  WiFi.onEvent(WiFiEvent);
  WiFi.config(ip, gateway, subnet);
  WiFi.setAutoReconnect(true);
  WiFi.setAutoConnect(true);
  
  //Initiate connection
  WiFi.begin(ssid, pwd);

  Serial.println("Waiting for WIFI connection...");
}

//wifi event handler
void WiFiEvent(WiFiEvent_t event){
    switch(event) {
      case WIFI_EVENT_STAMODE_GOT_IP:
          //When connected set 
          DEBUG_PRINT("WiFi connected! IP address: ");
          DEBUG_PRINT_LN(WiFi.localIP());  
          //initializes the UDP state
          //This initializes the transfer buffer
          Udp.begin(PORT);
          DEBUG_PRINT("Listening to port: ");
          DEBUG_PRINT_LN(PORT); 
          connected = true;
          break;
      case WIFI_EVENT_STAMODE_DISCONNECTED:
          DEBUG_PRINT_LN("WiFi lost connection");
          connected = false;
          //software_Reset();
          break;
    }
}



int checkProtocolHeaders(const unsigned char* messagein, int messagelength) {

  
  if ( messagein[0] == 0x10 && messagein[1] == 0x41 && messagein[2] == 0x37) { 
      // 0x41 = 'A'
      // 0x37 = '7'
      //DEBUG_PRINT("Data Size: ");
      int data_size = BtoI((byte)messagein[3],(byte)messagein[4]); // number of values plus start code
      
     // messagein[3] * 256 + messagein[4]; // number of values plus start code
      //DEBUG_PRINT_LN(data_size);
      //DEBUG_PRINT_LN(messagelength-HEADER_SIZE);

      if ( (messagelength-HEADER_SIZE) == data_size ) 
      {
        return data_size; //Return how many values are in the packet.
      }
        
    }
  return 0;
}

void parseRGBReceived(unsigned char* pbuff, int count) 
{
  //DEBUG_PRINT_LN ("DMX Packet Received");
  int output_channel = pbuff[5];
  if ( output_channel == OUTPUT_CHANNEL) 
  {
    int channel = 0; //reset RGB channel assignment to 0 each time through loop.
    for (int i = 0; i < NUM_LEDS; i++) //loop to assign 3 channels to each pixel
    {
        leds1[i] = CRGB(pbuff[HEADER_SIZE + channel], pbuff[HEADER_SIZE + (channel +1)], pbuff[HEADER_SIZE + (channel +2)]);
        channel +=channelwidth; //increase last channel number by channel width
    }
  } 

 else if( output_channel == (OUTPUT_CHANNEL+1))
  {
      int channel = 0; //reset RGB channel assignment to 0 each time through loop.
      for (int i = 0; i < NUM_LEDS; i++) //loop to assign 3 channels to each pixel
      {
          leds2[i] = CRGB(pbuff[HEADER_SIZE + channel], pbuff[HEADER_SIZE + (channel +1)], pbuff[HEADER_SIZE + (channel +2)]);
          channel +=channelwidth; //increase last channel number by channel width
      }
  }
  
  FastLED.show(); //send data to pixels
}

int BtoI(byte a, byte b)
{
  return (a<<8)+b;
}



