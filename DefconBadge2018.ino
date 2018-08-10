
#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ST7735.h> // Hardware-specific library for ST7735
#include <SPI.h>
#include <Adafruit_NeoPixel.h>
#include <RH_RF69.h>
//#ifdef __AVR__
//  #include <avr/power.h>
//#endif

#define PIN 6
#define BRIGHTNESS 2

//Change This Message To Whatever You Want To Send
char* MessageToSend = "Josh";
char* MessageRecieved;

Adafruit_NeoPixel strip = Adafruit_NeoPixel(32, PIN, NEO_GRB + NEO_KHZ800);

// For the breakout, you can use any 2 or 3 pins
// These pins will also work for the 1.8" TFT shield
#define TFT_CS     10
#define TFT_RST    11  // you can also connect this to the Arduino reset
                       // in which case, set this #define pin to -1!
#define TFT_DC     9

//Define Radio
#define RF69_FREQ 433.0
#define RFM69_CS      8
#define RFM69_INT     3
#define RFM69_RST     4
#define LED           13
RH_RF69 rf69(RFM69_CS, RFM69_INT);

// Option 1 (recommended): must use the hardware SPI pins
// (for UNO thats sclk = 13 and sid = 11) and pin 10 must be
// an output. This is much faster - also required if you want
// to use the microSD card (see the image drawing example)

// For 1.44" and 1.8" TFT with ST7735 use
Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS,  TFT_DC, TFT_RST);

// For 1.54" TFT with ST7789
//Adafruit_ST7789 tft = Adafruit_ST7789(TFT_CS,  TFT_DC, TFT_RST);

// Option 2: use any pins but a little slower!
//#define TFT_SCLK 13   // set these to be whatever pins you like!
//#define TFT_MOSI 11   // set these to be whatever pins you like!
//Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_MOSI, TFT_SCLK, TFT_RST);



float p = 3.1415926;

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

  // large block of text
  //tft.fillScreen(ST77XX_BLACK);
  //tft.setRotation(1);
  //tft.setTextSize(3);
  //testdrawtext("Josh", ST77XX_GREEN);
  //testdrawtext("is", ST77XX_GREEN);
  //testdrawtext("1337!", ST77XX_GREEN);
  //delay(10000);

  tft.setTextWrap(false);
  tft.setRotation(1);
  tft.fillScreen(ST77XX_BLACK);
  tft.setCursor(0, 30);
  tft.setTextColor(ST77XX_GREEN);
  tft.setTextSize(3);
  tft.println(" Hack");
  tft.println("  Me");
  //tft.println(" 1337!");

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
  uint8_t len = sizeof(buf);

  if (rf69.waitAvailableTimeout(500))  { 
    // Should be a reply message for us now   
    if (rf69.recv(buf, &len)) {
      Serial.print("Got a reply: ");
      Serial.println((char*)buf);
      MessageRecieved = ((char*)buf);
      tft.fillScreen(ST77XX_BLACK);
      tft.setCursor(0, 30);
      tft.setTextColor(ST77XX_GREEN);
      tft.setTextSize(2);
      tft.println("Hacked");
      tft.println("By");
      tft.println((char*)buf);
      tft.print("RSSI: "); tft.print(rf69.lastRssi());
        if (MessageRecieved = "Josh") {
           colorWipe(strip.Color(0, 255, 0), 50); // Green
        }
        if (MessageRecieved = "Derrick") {
           colorWipe(strip.Color(0, 0, 255), 50); // Blue
        }
        if (MessageRecieved = "Brandon") {
           colorWipe(strip.Color(255, 0, 0), 50); // Red
        }
        if (MessageRecieved = "Rick") {
           colorWipe(strip.Color(255, 128, 0), 50); // Orange
        }
        if (MessageRecieved = "Randle") {
           colorWipe(strip.Color(128, 0, 255), 50); // Purple
        }
    } else {
      Serial.println("Receive failed");
    }
  } else {
    Serial.println("No reply, is another RFM69 listening?");
  }

  //Screen  
  //tft.invertDisplay(true);
  delay(5000);
  //tft.invertDisplay(false);
  //delay(500);
  //tft.fillScreen(ST77XX_BLACK);
  //tft.setCursor(0, 30);
  tft.setTextWrap(false);
  tft.setRotation(1);
  tft.fillScreen(ST77XX_BLACK);
  tft.setCursor(0, 30);
  tft.setTextColor(ST77XX_GREEN);
  tft.setTextSize(3);
  tft.println(" Hack");
  tft.println("  Me");

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


