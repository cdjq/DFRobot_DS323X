/*!
 * @file readwriteSRAM.ino
 * @brief Show current time 
 * @n Experiment phenomenon: read data every 1 seconds and print it on serial port. 
 *
 * @copyright	Copyright (c) 2010 DFRobot Co.Ltd (http://www.dfrobot.com)
 * @licence     The MIT License (MIT)
 * @author [LuoYufeng](yufeng.luo@dfrobot.com)
 * @version  V0.1
 * @date  2021-2-23
 * @url https://github.com/DFRobot/DFRobot_DS323X
 */
#include "DFRobot_DS323X.h"

DFRobot_DS323X rtc;
uint8_t buffer[] = {0x10,0x11,0x24,0x36,0x45,0x65,0x17,0x30,0x78,0xA4,0x89};
int i = 0;

void setup(void)
{
    Serial.begin(9600);
    /*Wait for the chip to be initialized completely, and then exit*/
    while(rtc.begin() != true){
        Serial.println("Failed to init chip, please check if the chip connection is fine. ");
        delay(1000);
    }
    for (uint8_t reg = 0x14; reg < 0x1F; reg++){
        rtc.clearSRAM(reg);
    }
    for (uint8_t reg = 0x14; reg < 0x1F; reg++){
        rtc.writeSRAM(reg,buffer[i]);
        i++;
    }
    delay(1000);
}

void loop() {
    for (uint8_t reg = 0x14; reg < 0x1F; reg++){
        Serial.print(rtc.readSRAM(reg),HEX);
        Serial.print(" ");
        delay(1000);
    }
    Serial.println();
}