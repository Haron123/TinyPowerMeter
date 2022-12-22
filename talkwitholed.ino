#define SCL_PIN 1
#define SDA_PIN 0
#include "i2c_bitbangv2.h"

void setup() {

  starti2c(SCL_PIN, SDA_PIN);
  delay(1000);
  InitializeSSD1306(SCL_PIN, SDA_PIN);
  resetCursor(SCL_PIN, SDA_PIN);
  fullclearDisplay(SCL_PIN, SDA_PIN);
  
}

void loop() {
  // It is to note that writeHex and all other Functions
  // that utilize the sendManyBytes Function, will use a lot more space than
  // If the bytes are sent via sendByte seperately.
  
  // Write the voltage on the display
  fastclearDisplay(SCL_PIN, SDA_PIN);
  setLine(0, 0, SCL_PIN, SDA_PIN);
  writeM(SCL_PIN, SDA_PIN);
  writeManyNumbers(getBusVoltage(SCL_PIN, SDA_PIN), SCL_PIN, SDA_PIN);
  writeHex(F("00e0102010e000000000304080403000"), SCL_PIN, SDA_PIN);

  // Write the Current on the display
  setLine(1, 0, SCL_PIN, SDA_PIN);
  writeHex(F("f888888800f88080f80000f8286890000000009000000000"), SCL_PIN, SDA_PIN);
  float current = ((getShuntVoltage(SCL_PIN, SDA_PIN) / 100.0) / 0.12); // Calculates current with the shunt resistor
  writeManyNumbers(current, SCL_PIN, SDA_PIN);
  writeHex(F("00e0102010e0000000e0504850e00000"), SCL_PIN, SDA_PIN);

  //Write the Power on the display
  setLine(2, 0, SCL_PIN, SDA_PIN);
  writeHex(F("f828283800f88888f80000f880e080f80000009000000000"), SCL_PIN, SDA_PIN);
  writeManyNumbers((current * getBusVoltage(SCL_PIN, SDA_PIN)) / 1000, SCL_PIN, SDA_PIN);
  writeHex(F("00f0081008f0000000f880e080f80000"), SCL_PIN, SDA_PIN);
}

 // SSD1306 CODE##################################
void InitializeSSD1306(byte clockpin, byte datapin)
{
  
  startcond(clockpin, datapin);
  sendAddress(0x3C, clockpin, datapin);
  sendWrite(clockpin, datapin);
  checkAck(clockpin, datapin);
  sendByte(0x00, clockpin, datapin);
  checkAck(clockpin, datapin);
  sendByte(0xAE,clockpin, datapin);
  checkAck(clockpin, datapin);

  
  // Multiplex Ratio
  sendByte(0xA8, clockpin, datapin);
  checkAck(clockpin, datapin);
  sendByte(0x3F, clockpin, datapin);
  checkAck(clockpin, datapin);
  // Display Offset
  sendByte(0xD3, clockpin, datapin);
  checkAck(clockpin, datapin);
  sendByte(0x00, clockpin, datapin);
  checkAck(clockpin, datapin);
  // Display Start line
  sendByte(0x40, clockpin, datapin);
  checkAck(clockpin, datapin);
  // Set Segment remap
  sendByte(0xA1, clockpin, datapin);
  checkAck(clockpin, datapin);
  // Com Direction
  sendByte(0xC8, clockpin, datapin);
  checkAck(clockpin, datapin);
  // Com Pins hardware config
  sendByte(0xDA, clockpin, datapin);
  checkAck(clockpin, datapin);
  sendByte(0x02, clockpin, datapin);
  checkAck(clockpin, datapin);
  // Contrast
  sendByte(0x81, clockpin, datapin);
  checkAck(clockpin, datapin);
  sendByte(0xFF, clockpin, datapin);
  checkAck(clockpin, datapin);
  // Disable entire Display on
  sendByte(0xA4, clockpin, datapin);
  checkAck(clockpin, datapin);
  // Normal mode
  sendByte(0xA6, clockpin, datapin);
  checkAck(clockpin, datapin);
  // Osc Freq
  sendByte(0xD5, clockpin, datapin);
  checkAck(clockpin, datapin);
  sendByte(0x80, clockpin, datapin);
  checkAck(clockpin, datapin);
  // Charge pump
  sendByte(0x8D, clockpin, datapin);
  checkAck(clockpin, datapin);
  sendByte(0x14, clockpin, datapin);
  checkAck(clockpin, datapin);
  // Addressing Mode : Horinzontal mode
  sendByte(0x20, clockpin, datapin);
  checkAck(clockpin, datapin);
  sendByte(0x00, clockpin, datapin);
  checkAck(clockpin, datapin);
  // Set column Adress
  sendByte(0x21, clockpin, datapin);
  checkAck(clockpin, datapin);
  sendByte(10, clockpin, datapin);
  checkAck(clockpin, datapin);
  sendByte(125, clockpin, datapin);
  checkAck(clockpin, datapin);
  // Set page address
  sendByte(0x22, clockpin, datapin);
  checkAck(clockpin, datapin);
  sendByte(1, clockpin, datapin);
  checkAck(clockpin, datapin);
  sendByte(7, clockpin, datapin);
  checkAck(clockpin, datapin);

  sendByte(0xAF, clockpin, datapin);
  checkAck(clockpin, datapin);
  stopcond(clockpin, datapin);

  // Im tired of reading datasheets :D
  
  
}
// If not used can be deleted to save space
// contrast : 0 to 255
void setContrast(byte contrast, byte clockpin, byte datapin)
{
  startcond(clockpin, datapin);
  sendAddress(0x3C, clockpin, datapin);
  sendWrite(clockpin, datapin);
  checkAck(clockpin, datapin);
  sendByte(0x00, clockpin, datapin);
  checkAck(clockpin, datapin);
  sendByte(0x81, clockpin, datapin);
  checkAck(clockpin, datapin);
  sendByte(contrast, clockpin, datapin);
  checkAck(clockpin, datapin);
  stopcond(clockpin, datapin);
}

void displayOn(byte clockpin, byte datapin)
{
  startcond(clockpin, datapin);
  sendAddress(0x3C, clockpin, datapin);
  sendWrite(clockpin, datapin);
  checkAck(clockpin, datapin);
  sendByte(0x00, clockpin, datapin);
  checkAck(clockpin, datapin);
  sendByte(0xAF, clockpin, datapin);
  checkAck(clockpin, datapin);
  stopcond(clockpin, datapin);
}

void displayOff(byte clockpin, byte datapin)
{
  startcond(clockpin, datapin);
  sendAddress(0x3C, clockpin, datapin);
  sendWrite(clockpin, datapin);
  checkAck(clockpin, datapin);
  sendByte(0x00, clockpin, datapin);
  checkAck(clockpin, datapin);
  sendByte(0xAE, clockpin, datapin);
  checkAck(clockpin, datapin);
  stopcond(clockpin, datapin);
}


void resetCursor(byte clockpin, byte datapin)
{
  startcond(clockpin, datapin);
  sendAddress(0x3C, clockpin, datapin);
  sendWrite(clockpin, datapin);
  checkAck(clockpin, datapin);
  sendByte(0x00, clockpin, datapin);
  checkAck(clockpin, datapin);
  sendByte(0x22, clockpin, datapin);
  checkAck(clockpin, datapin);
  sendByte(0x00, clockpin, datapin);
  checkAck(clockpin, datapin);
  sendByte(0x07, clockpin, datapin);
  checkAck(clockpin, datapin);
  stopcond(clockpin, datapin);

  startcond(clockpin, datapin);
  sendAddress(0x3C, clockpin, datapin);
  sendWrite(clockpin, datapin);
  checkAck(clockpin, datapin);
  sendByte(0x00, clockpin, datapin);
  checkAck(clockpin, datapin);
  sendByte(0x21, clockpin, datapin);
  checkAck(clockpin, datapin);
  sendByte(0x0, clockpin, datapin);
  checkAck(clockpin, datapin);
  sendByte(0x7F, clockpin, datapin);
  checkAck(clockpin, datapin);
  stopcond(clockpin, datapin);
}

void setLine(byte x, byte y, byte clockpin, byte datapin)
{
  startcond(clockpin, datapin);
  sendAddress(0x3C, clockpin, datapin);
  sendWrite(clockpin, datapin);
  checkAck(clockpin, datapin);
  sendByte(0x00, clockpin, datapin);
  checkAck(clockpin, datapin);
  sendByte(0x22, clockpin, datapin);
  checkAck(clockpin, datapin);
  sendByte(x, clockpin, datapin);
  checkAck(clockpin, datapin);
  sendByte(7, clockpin, datapin);
  checkAck(clockpin, datapin);
  stopcond(clockpin, datapin);

  startcond(clockpin, datapin);
  sendAddress(0x3C, clockpin, datapin);
  sendWrite(clockpin, datapin);
  checkAck(clockpin, datapin);
  sendByte(0x00, clockpin, datapin);
  checkAck(clockpin, datapin);
  sendByte(0x21, clockpin, datapin);
  checkAck(clockpin, datapin);
  sendByte(y, clockpin, datapin);
  checkAck(clockpin, datapin);
  sendByte(0x7F, clockpin, datapin);
  checkAck(clockpin, datapin);
  stopcond(clockpin, datapin);
}

void fastclearDisplay(byte clockpin, byte datapin)
{
  startcond(clockpin, datapin);
  sendAddress(0x3C, clockpin, datapin);
  sendWrite(clockpin, datapin);
  checkAck(clockpin, datapin);
  sendByte(0x40, clockpin, datapin);
  checkAck(clockpin, datapin);

  for(short i = 0; i < 512; ++i)
  {
    sendByte(0x00, clockpin, datapin);
    checkAck(clockpin, datapin);
  }
  stopcond(clockpin,datapin);
}

void fullclearDisplay(byte clockpin, byte datapin)
{
  startcond(clockpin, datapin);
  sendAddress(0x3C, clockpin, datapin);
  sendWrite(clockpin, datapin);
  checkAck(clockpin, datapin);
  sendByte(0x40, clockpin, datapin);
  checkAck(clockpin, datapin);

  for(short i = 0; i < 5120; ++i)
  {
    sendByte(0x00, clockpin, datapin);
    checkAck(clockpin, datapin);
  }
  stopcond(clockpin,datapin);
}

void writeDisplay(byte clockpin, byte datapin)
{
  startcond(clockpin, datapin);
  sendAddress(0x3C, clockpin, datapin);
  sendWrite(clockpin, datapin);
  checkAck(clockpin, datapin);
  sendByte(0x40, clockpin, datapin);
  checkAck(clockpin, datapin);

  for(short i = 0; i < 512; ++i)
  {
    sendByte(0x11, clockpin, datapin);
    checkAck(clockpin, datapin);
  }
  stopcond(clockpin,datapin);
}

void writeM(byte clockpin, byte datapin)
{
  startcond(clockpin, datapin);
  sendAddress(0x3C, clockpin, datapin);
  sendWrite(clockpin, datapin);
  checkAck(clockpin, datapin);
  sendByte(0x40, clockpin, datapin);
  checkAck(clockpin, datapin);
  sendManyBytesHex(F("f8a8a87000788080f80010a8a8a84000000020202020000000186080601800000000000050000000"), clockpin, datapin);
}
// Expects a number between 00000 - 99999
void writeManyNumbers(short number, byte clockpin, byte datapin)
{
 
  writeNumber(number / 10000, clockpin, datapin);
  writeNumber((number / 1000) % 10, clockpin, datapin);
  writeNumber((number / 100) % 10, clockpin, datapin);
  writeNumber((number / 10) % 10, clockpin, datapin);
  writeNumber(number % 10, clockpin, datapin);
}

void writeNumber(byte number, byte clockpin, byte datapin)
{
  startcond(clockpin, datapin);
  sendAddress(0x3C, clockpin, datapin);
  sendWrite(clockpin, datapin);
  checkAck(clockpin, datapin);
  sendByte(0x40, clockpin, datapin);
  checkAck(clockpin, datapin);
  if(number == 1)
  {
    sendManyBytesHex(F("00000008f8000000"), clockpin, datapin);
  }
  else if(number == 2)
  {
    sendManyBytesHex(F("0040a8a8a8100000"), clockpin, datapin);
  }
  else if(number == 3)
  {
    sendManyBytesHex(F("0000a8a8a8500000"), clockpin, datapin);
  }
  else if(number == 4)
  {
    sendManyBytesHex(F("0000382020f80000"), clockpin, datapin);
  }
  else if(number == 5)
  {
    sendManyBytesHex(F("0000b8a8a8480000"), clockpin, datapin);
  }
  else if(number == 6)
  {
    sendManyBytesHex(F("0000f8a8a8e80000"), clockpin, datapin);
  }
  else if(number == 7)
  {
    sendManyBytesHex(F("0000080808f80000"), clockpin, datapin);
  }
  else if(number == 8)
  {
    sendManyBytesHex("0000f8a8a8f80000", clockpin, datapin);
  }
  else if(number == 9)
  {
    sendManyBytesHex(F("000038a8a8f80000"), clockpin, datapin);
  }
  else if(number == 0)
  {
    sendManyBytesHex(F("0000f88888f80000"), clockpin, datapin);
  }
}

void writeHex(String hex, byte clockpin, byte datapin)
{
    startcond(clockpin, datapin);
    sendAddress(0x3C, clockpin, datapin);
    sendWrite(clockpin, datapin);
    checkAck(clockpin, datapin);
    sendByte(0x40, clockpin, datapin);
    checkAck(clockpin, datapin);
    sendManyBytesHex(hex, clockpin, datapin);
}


// SSD1306 CODE END #################################################

// INA219 CODE START ##################################################
short getBusVoltage(byte clockpin, byte datapin)
{
  // Sends Address + Write Bit + Pointer Address
  startcond(clockpin, datapin);
  sendAddress(0x40, clockpin, datapin);
  sendWrite(clockpin, datapin);
  checkAck(clockpin, datapin);
  sendByte(0x02, clockpin, datapin);
  checkAck(clockpin, datapin);

  // Sends Address + Read Bit
  startcond(clockpin, datapin);
  sendAddress(0x40, clockpin, datapin);
  sendRead(clockpin, datapin);
  checkAck(clockpin, datapin);

  // Gets data by the slave + acknowledged received data(can add condition if needed)
  byte higher = getByte(clockpin, datapin);
  sendAck(clockpin, datapin);

  // Gets data by the slave + acknowledged received data(can add condition if needed)
  byte lower = getByte(clockpin, datapin);
  sendAck(clockpin, datapin);

  // Stops I2C comm
  stopcond(clockpin, datapin);

  short data = (((higher << 8) | lower) >> 3);
  return data * 4;
}

short getShuntVoltage(byte clockpin, byte datapin)
{
  // Sends Address + Write Bit + Pointer Address
  startcond(clockpin, datapin);
  sendAddress(0x40, clockpin, datapin);
  sendWrite(clockpin, datapin);
  checkAck(clockpin, datapin);
  sendByte(0x01, clockpin, datapin);
  checkAck(clockpin, datapin);

  // Sends Address + Read Bit
  startcond(clockpin, datapin);
  sendAddress(0x40, clockpin, datapin);
  sendRead(clockpin, datapin);
  checkAck(clockpin, datapin);

  // Gets data by the slave + acknowledge received data(can add condition if needed)
  byte higher = getByte(clockpin, datapin);
  sendAck(clockpin, datapin);

  // Gets data by the slave + acknowledge received data(can add condition if needed)
  byte lower = getByte(clockpin, datapin);
  sendAck(clockpin, datapin);
  
  // Stops I2C comm
  stopcond(clockpin, datapin);

  short data = (((higher << 8) | lower) >> 3);
  return data * 8;
}
