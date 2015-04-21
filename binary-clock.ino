#define sclk 2
#define mosi 3
#define dc   4
#define cs   5
#define rst  6

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1351.h>
#include <SPI.h>
#include <Time.h>

// Option 1: use any pins but a little slower
Adafruit_SSD1351 tft = Adafruit_SSD1351(cs, dc, mosi, sclk, rst);  

void setup() {
//  setTime(17, 21, 30, 19, 4, 2015); 
  setCurrentTime();
  
  Serial.begin(9600);
  tft.begin();
  
  tft.setTextSize(1);
  tft.fillScreen(hex("0001"));
}

tmElements_t tm;
int last_h;
int last_m;
int last_s;
String p;
String last_p;

void loop() {
  time_t t = now();
  
  int h = hourFormat12(t);
  int m = minute(t);
  int s = second(t);
  
  if (isPM()) p = "pm";
  if (isAM()) p = "am";
  
  if (h != last_h) {
    printHour(h);
    last_h = h;
  }
  
  if (m != last_m) {
    printMin(m);
    last_m = m;
  }
  
  if (s != last_s) {
    printSec(s);
    last_s = s;
  }
  
  if (p != last_p) {
    printPeriod(p);
    last_p = p;
  }
}

void setCurrentTime() {
  
  // Date
  char mo_[12];
  int d_, y_;
  uint8_t mo_index;
  char *mo_names[12] = {
    "Jan", "Feb", "Mar", "Apr", "May", "Jun",
    "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"
  };

  if (sscanf(__DATE__, "%s %d %d", mo_, &d_, &y_) != 3) {
    return;
  }
  
  for (mo_index = 0; mo_index < 12; mo_index++) {
    if (strcmp(mo_, mo_names[mo_index]) == 0) break;
  }
  
  if (mo_index >= 12) {
    return;
  }
  
  // Time
  int h_, m_, s_;

  if (sscanf(__TIME__, "%d:%d:%d", &h_, &m_, &s_) != 3) {
    return;
  }
  
  tm.Day = d_;
  tm.Month = mo_index + 1;
  tm.Year = CalendarYrToTm(y_);
  tm.Hour = h_;
  tm.Minute = m_;
  tm.Second = s_;
  
  setTime(makeTime(tm));
}

void printHour(int h) {
  tft.fillRect(20, 15, 98, 12, hex("0001"));
  tft.setCursor(10, 15);
  tft.setTextColor(hex("3330"));
  tft.print("h ");
  printBin(h);
  printInt(h);
}

void printMin(int m) {
  tft.fillRect(20, 35, 98, 12, hex("0001"));
  tft.setCursor(10, 35);
  tft.setTextColor(hex("3330"));
  tft.print("m ");
  printBin(m);
  printInt(m);
}

void printSec(int s) {
  tft.fillRect(20, 55, 98, 12, hex("0001"));
  tft.setCursor(10, 55);
  tft.setTextColor(hex("3330"));
  tft.print("s ");
  printBin(s);
  printInt(s);
}

void printPeriod(String p) {
  tft.fillRect(20, 75, 98, 12, hex("0001"));
  tft.setCursor(10, 75);
  tft.setTextColor(hex("1105"));
  tft.print(p);
}

void printBin(int n) {
  tft.setTextColor(hex("ffff"));
  if (n < 2)   tft.print(0);
  if (n < 4)   tft.print(0);
  if (n < 8)   tft.print(0);
  if (n < 16)  tft.print(0);
  if (n < 32)  tft.print(0);
//  if (n < 64)  tft.print(0);
//  if (n < 128) tft.print(0);
//  if (n < 256) tft.print(0);
//  if (n < 512) tft.print(0);
  tft.print(n, BIN);
}

void printInt(int n) {
  tft.setTextColor(hex("1105"));
  tft.print(" (");
  if (n < 10) tft.print(0);
  tft.print(n);
  tft.print(")");
}

int hex(char *s) {
  int x = 0;
  
  for(;;) {
    char c = *s;
    
    if (c >= '0' && c <= '9') {
      x *= 16;
      x += c - '0'; 
    } else if (c >= 'A' && c <= 'F') {
      x *= 16;
      x += (c - 'A') + 10; 
    } else if (c >= 'a' && c <= 'f') {
      x *= 16;
      x += (c - 'a') + 10;
    } else {
      break;
    }
    
    s++;
  }
  
  return x;
}
