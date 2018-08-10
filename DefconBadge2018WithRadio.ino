//You will also need to change your message so that you can send your number
//1=Josh, 2=Derrick, 3=Brandon, 4=Rick, 5=Randle, 6=Josh  
char* MessageToSend = "3";
//change this or your badge will broadcaost as Brandon...

#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ST7735.h> // Hardware-specific library for ST7735
#include <SPI.h>
#include <Adafruit_NeoPixel.h>
#include <RH_RF69.h>

#define PIN 6
#define BRIGHTNESS 2 //this is 2 of 255. anything about 100 is pretty blinding

char* MessageRecieved;
String HackedName;

Adafruit_NeoPixel strip = Adafruit_NeoPixel(32, PIN, NEO_GRB + NEO_KHZ800);

// For the breakout, you can use any 2 or 3 pins
// These pins will also work for the 1.8" TFT shield
#define TFT_CS     10
#define TFT_RST    11  
#define TFT_DC     9

//Define Radio
#define RF69_FREQ 433.0
#define RFM69_CS      8
#define RFM69_INT     3
#define RFM69_RST     4
#define LED           13
RH_RF69 rf69(RFM69_CS, RFM69_INT);

// For 1.44" and 1.8" TFT with ST7735 use
Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS,  TFT_DC, TFT_RST);

void setup(void) {
  Serial.begin(9600);
  Serial.println("Initialized");

  //Radio
  pinMode(LED, OUTPUT);     
  pinMode(RFM69_RST, OUTPUT);
  digitalWrite(RFM69_RST, LOW);

   if (!rf69.init()) {
    Serial.println("RFM69 radio init failed");
    while (1);
  }
  Serial.println("RFM69 radio init OK!");
  
  // Defaults after init are 434.0MHz, modulation GFSK_Rb250Fd250, +13dbM (for low power module)
  // No encryption
  if (!rf69.setFrequency(RF69_FREQ)) {
    Serial.println("setFrequency failed");
  }

  rf69.setTxPower(14, true);
  // The encryption key has to be the same as the one in the server
  uint8_t key[] = { 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08,
                    0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08};
  rf69.setEncryptionKey(key);
  
  pinMode(LED, OUTPUT);
  Serial.print("RFM69 radio @");  Serial.print((int)RF69_FREQ);  Serial.println(" MHz");

  //Lights
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
  strip.setBrightness(BRIGHTNESS);

  //Screen
  tft.initR(INITR_144GREENTAB);
  uint16_t time = millis();
  tft.fillScreen(ST77XX_BLACK);
  time = millis() - time;

  Serial.println(time, DEC);
  delay(500);
}

void loop() {

  //Radio
  char* radiopacket = {MessageToSend};
  Serial.print("Sending "); Serial.println(radiopacket);
  
  // Send a message!
  rf69.send((uint8_t *)radiopacket, strlen(radiopacket));
  rf69.waitPacketSent();

  // Now wait for a reply
  uint8_t buf[RH_RF69_MAX_MESSAGE_LEN];
  //uint8_t len = sizeof(buf);
  uint8_t len = sizeof(buf);

  if (rf69.waitAvailableTimeout(500))  { 
    // Should be a reply message for us now   
    if (rf69.recv(buf, &len)) {
      Serial.print("Got a reply: ");
      Serial.println((char*)buf);
      MessageRecieved = ((char*)buf);
      tft.fillScreen(ST77XX_BLACK);
      tft.setCursor(0, 30);     
      if(strcmp(MessageRecieved, "1") == 0) {
        tft.setTextSize(2);
        tft.setTextColor(ST77XX_YELLOW);
        tft.println("Hacked");
        tft.println("By");
        tft.setTextColor(ST77XX_GREEN);
        tft.println("Josh");
        tft.print("RSSI: "); tft.print(rf69.lastRssi());
        colorWipe(strip.Color(0, 255, 0), 50); // Green 
        colorWipe(strip.Color(0, 255, 0), 0); // Green  
        colorWipe(strip.Color(0, 255, 0), 50); // Green  
        colorWipe(strip.Color(0, 255, 0), 0); // Green
        //comment out this goto if you are not Rick
        goto Rick;  
      }
      else if(strcmp(MessageRecieved, "2") == 0) {
        tft.setTextSize(2);
        tft.setTextColor(ST77XX_YELLOW);
        tft.println("Hacked");
        tft.println("By");
        tft.setTextColor(ST77XX_RED);
        tft.println("Derrick");
        tft.print("RSSI: "); tft.print(rf69.lastRssi());
        colorWipe(strip.Color(255, 0, 0), 50); // Red
        colorWipe(strip.Color(0, 0, 0), 50); // Red
        colorWipe(strip.Color(255, 0, 0), 50); // Red 
        colorWipe(strip.Color(0, 0, 0), 50); // Red
        //comment out this goto if you are not Rick
        goto Rick;
      }
      else if(strcmp(MessageRecieved, "3") == 0) {
        tft.setTextSize(2);
        tft.setTextColor(ST77XX_YELLOW);
        tft.println("Hacked");
        tft.println("By");
        tft.setTextColor(ST77XX_BLUE);
        tft.println("Brandon");
        tft.print("RSSI: "); tft.print(rf69.lastRssi());
        colorWipe(strip.Color(0, 0, 255), 50); // Blue
        colorWipe(strip.Color(0, 0, 0), 50); // Blue
        colorWipe(strip.Color(0, 0, 255), 50); // Blue  
        colorWipe(strip.Color(0, 0, 0), 50); // Blue 
        //comment out this goto if you are not Rick
        goto Rick; 
      }
      else if(strcmp(MessageRecieved, "4") == 0) {
        tft.setTextSize(2);
        tft.println("Hacked");
        tft.println("By");
        tft.setTextColor(ST77XX_ORANGE);
        tft.println("Rick");
        tft.print("RSSI: "); tft.print(rf69.lastRssi());
        colorWipe(strip.Color(255, 128, 0), 50); // Orange 
        colorWipe(strip.Color(0, 0, 0), 50); // Orange 
        colorWipe(strip.Color(255, 128, 0), 50); // Orange
        colorWipe(strip.Color(0, 0, 0), 50); // Orange
        //comment out this goto if you are not Rick
        goto Rick;
      }
      else if(strcmp(MessageRecieved, "5") == 0) {
        tft.setTextSize(2);
        tft.setTextColor(ST77XX_YELLOW);
        tft.println("Hacked");
        tft.println("By");
        tft.setTextColor(ST77XX_MAGENTA);
        tft.println("Randle");
        tft.print("RSSI: "); tft.print(rf69.lastRssi());
        colorWipe(strip.Color(128, 0, 255), 50); // Purple
        colorWipe(strip.Color(0, 0, 0), 50); // Purple
        colorWipe(strip.Color(128, 0, 255), 50); // Purple
        colorWipe(strip.Color(0, 0, 0), 50); // Purple
        //comment out this goto if you are not Rick
        goto Rick;
      }
       else if(strcmp(MessageRecieved, "6") == 0) {
        tft.setTextSize(2);
        tft.setTextColor(ST77XX_YELLOW);
        tft.println("Hacked");
        tft.println("By");
        tft.setTextColor(ST77XX_WHITE);
        tft.println("Tatum");
        tft.print("RSSI: "); tft.print(rf69.lastRssi());
        colorWipe(strip.Color(255, 255, 255), 50); // White
        colorWipe(strip.Color(255, 255, 255), 50); // White
        colorWipe(strip.Color(255, 255, 255), 50); // White
        colorWipe(strip.Color(255, 255, 255), 50); // White
        //comment out this goto if you are not Rick
        goto Rick;
      }

      Rick:
      tft.fillScreen(ST77XX_BLACK);
      tft.drawChar( 52, 9, 'I', 0xf7ff, 0x0000,5);
      //building heart, this took many hours and is totally not worth it
      tft.fillCircle(56, 63, 12, 0xf900);
      tft.fillCircle(78, 63, 12, 0xf900);
      tft.fillTriangle(44, 63, 90, 63, 67, 90, 0xf900);
      //clening up area from circle that passes the triangle
      tft.drawLine(43,63,66,90,0x0000);
      tft.drawLine(42,63,65,90,0x0000);
      tft.drawLine(41,63,64,90,0x0000);
      tft.drawLine(40,63,63,90,0x0000);
      tft.drawLine(91,63,68,90,0x0000);
      tft.drawLine(92,63,69,90,0x0000);
      tft.drawLine(93,63,70,90,0x0000);
      tft.drawLine(93,63,71,90,0x0000);
      tft.drawLine(94,63,72,90,0x0000);
      tft.drawChar( 5, 94, 'K', 0xf540, 0x0000,5);
      tft.drawChar( 52, 94, 'G', 0x0760, 0x0000,5);
      tft.drawChar( 98, 94, 'B', 0x01f, 0x0000,5);
    } 
    else {
      Serial.println("Receive failed");
    }
  } else {
    Serial.println("No reply, is another RFM69 listening?");
  }

  tft.setTextWrap(false);
  tft.setRotation(1);
  tft.fillScreen(ST77XX_BLACK);
  tft.drawChar( 22, 37, 'H', 0x3fe5, 0x0000,4);
  tft.drawChar( 48, 37, '4', 0xf8ed, 0x0000,4);
  tft.drawChar( 74, 37, 'c', 0x3fe5, 0x0000,4);
  tft.drawChar( 100, 37, 'k', 0x3fe5, 0x0000,4);
  tft.drawChar( 48, 70, 'M', 0x3fe5, 0x0000,4);
  tft.drawChar( 74, 70, '3', 0xf8ed, 0x0000,4); 

  //Lights
  colorWipe(strip.Color(255, 0, 0), 50); // Red
  colorWipe(strip.Color(0, 255, 0), 50); // Green
  colorWipe(strip.Color(0, 0, 255), 50); // Blue
}

void colorWipe(uint32_t c, uint8_t wait) {
  for(uint16_t i=0; i<strip.numPixels(); i++) {
    strip.setPixelColor(i, c);
    strip.show();
    delay(wait);
  }
}
