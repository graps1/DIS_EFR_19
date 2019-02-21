#include "Wire.h"
#include "Arduino.h"
#include "SPI.h"

#define RES  23

#define ASDA  18
#define ASCL  19

// R/W signal tied LOW (always write)
// SA0 signal tied LOW

void init_oled();
void send_packet(unsigned char x);
void data(unsigned char d);
void command(unsigned char c);
void writeRow(String text);
void setCursor(int i);
