#ifndef TLC59116_H
#define TLC59116_H

#include <Arduino.h>
#include <Wire.h>

#define ON 0b00000001
#define OFF 0b00000000
#define BLINK 0b00000011

class TLC59116 {
  public:
    TLC59116(uint8_t addr);
    void init(void);
    void setStates(uint8_t Drive_LED, uint8_t Reku_LED, bool VCU_LED, int periodendauer);
    bool error;

  private:
    uint8_t addr;
};

#endif
