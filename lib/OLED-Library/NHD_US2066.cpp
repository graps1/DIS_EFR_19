#include "NHD_US2066.h"


const char ADDR_display = 0x3C;  //3C or 78
unsigned char tx_packet[]={0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};


void writeRow(String text) {
  uint i;
  for(i=0;i<20;i++) {
    data(text[i]);
  }
}

void setCursor(int zeile) {
  uint8_t position;
  switch (zeile) {
    case 0:
    position = 0x00;
    break;
    case 1:
    position = 0x20;
    break;
    case 2:
    position = 0x40;
    break;
    case 3:
    position = 0x60;
    break;
  }
  command(0x80 | position);
}

void command(unsigned char c)
{
  tx_packet[0] = 0x00;
  tx_packet[1] = c;
  send_packet(2);

}
void data(unsigned char d)
{
  tx_packet[0] = 0x40;
  tx_packet[1] = d;
  send_packet(2);
}

void send_packet(unsigned char x)
{
  unsigned char ix;

  Wire.beginTransmission(ADDR_display);
  for(ix=0;ix<x;ix++)
  {
    Wire.write(tx_packet[ix]);
  }
  Wire.endTransmission();
}

void init_oled()
{
  pinMode(ASCL, OUTPUT);      //set Arduino I2C lines as outputs
  pinMode(ASDA, OUTPUT);
  pinMode(RES, OUTPUT);
  digitalWrite(ASCL, LOW);
  digitalWrite(ASDA, LOW);
  digitalWrite(RES, HIGH);
  delay(10);
  Wire.begin();
  delay(10);
  command(0x2A);  //function set (extended command set)
	command(0x71);  //function selection A, disable internal Vdd regualtor
	data(0x00);
	command(0x28);  //function set (fundamental command set)
	command(0x08);  //display off, cursor off, blink off
	command(0x2A);  //function set (extended command set)
	command(0x79);  //OLED command set enabled
	command(0xD5);  //set display clock divide ratio/oscillator frequency
	command(0x70);  //set display clock divide ratio/oscillator frequency
	command(0x78);  //OLED command set disabled
	command(0x09);  //extended function set (4-lines)
  command(0x06);  //COM SEG direction
	command(0x72);  //function selection B, disable internal Vdd regualtor
  command(0x76);  //ROM C selection
	data(0x00);     //ROM CGRAM selection
	command(0x2A);  //function set (extended command set)
	command(0x79);  //OLED command set enabled
	command(0xDA);  //set SEG pins hardware configuration
	command(0x10);  //set SEG pins ... NOTE: When using NHD-0216AW-XB3 or NHD_0216MW_XB3 change to (0x00)
	command(0xDC);  //function selection C
	command(0x00);  //function selection C
	command(0x81);  //set contrast control
	command(0x7F);  //set contrast control
	command(0xD9);  //set phase length
	command(0xF1);  //set phase length
	command(0xDB);  //set VCOMH deselect level
	command(0x40);  //set VCOMH deselect level
	command(0x78);  //OLED command set disabled
	command(0x28);  //function set (fundamental command set)
	command(0x01);  //clear display
	command(0x80);  //set DDRAM address to 0x00
	command(0x0C);  //display ON
  delay(100);
  //Wire.begin();
  //delay(10);
}
