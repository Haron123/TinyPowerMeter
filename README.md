# TinyPowerMeter 
Shows Current, Voltage and Power on a SSD1306 OLED Display with the help of a INA219 with a shunt of 0.1mOhm  

The sketch when compiled for the attiny85 uses 6142 Bytes (Which yes is a lot), which is mainly due to me choosing programming comfort  
over saving Flash Size. If you wanna reduce code size you would need to :  

Turn every sendManyBytesHex to sendByte as the String used for sendManyBytesHex is hella huge and needs tons of space.   
Whole Libary uses I2C Bitbanging  

Required Libary : https://github.com/Haron123/I2C_bigbangv2 + General Arduino Libaries  

Custom 8x8 Character generator for the ssd1306 using the I2C_bitbangv2 Libary  
Character generator : https://haron.gay/bto.php 
