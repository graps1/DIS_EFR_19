/* Arduino Slow Software I2C Master */

#include <TLC59116.h>

TLC59116::TLC59116(uint8_t address) {
  addr = address;
}

void TLC59116::init(void){

  Wire.begin();

  Wire.beginTransmission(addr);
  Wire.send(0);
  Wire.send(0);
  Wire.endTransmission();
  Wire.beginTransmission(addr);
  Wire.send(0x01);  // Mode 2 Register
  Wire.send(0x20);     // Enable Blinking
  Wire.endTransmission();
  Wire.beginTransmission(addr);
  Wire.send(0x02);  // Brightness Register LED0
  Wire.send(0xFF);  // Brightness-Wert, 0xFF = 100%
  Wire.endTransmission();
  Wire.beginTransmission(addr);
  Wire.send(0x03);  // Brightness Register LED1
  Wire.send(0xFF);  // Brightness-Wert, 0xFF = 100%
  Wire.endTransmission();
  Wire.beginTransmission(addr);
  Wire.send(0x04);  // Brightness Register LED2
  Wire.send(0xFF);  // Brightness-Wert, 0xFF = 100%
  Wire.endTransmission();
  Wire.beginTransmission(addr);
  Wire.send(0x05);  // Brightness Register LED3
  Wire.send(0xFF);  // Brightness-Wert, 0xFF = 100%
  Wire.endTransmission();
  Wire.beginTransmission(addr);
  Wire.send(0x12);  // Duty-Cycle Register
  Wire.send(0x77);  // Blink-PWM, 0x77 = 50%
  Wire.endTransmission();
}

void TLC59116::setStates(uint8_t Drive_LED, uint8_t Reku_LED, bool VCU_LED, int periodendauer){
  uint8_t ledOut0 = 0b00000000;
  ledOut0 |= Drive_LED;
  ledOut0 |= Reku_LED << 4;
  ledOut0 |= VCU_LED << 6;



  Wire.beginTransmission(addr);
  Wire.send(0x14);  // Led0-Register
  Wire.send(ledOut0);
  Wire.endTransmission();
  Wire.beginTransmission(addr);
  Wire.send(0x13);    // Frequency-Register
  Wire.send(periodendauer*24/1000-1);    // Blink-Frequenz 0x17 = 1 Hz
  Wire.endTransmission();

}
