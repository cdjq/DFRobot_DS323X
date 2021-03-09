/*!
 * @file getTimeFromPC.ino
 * @brief get time when the PC compile.
 * @n Experiment phenomenon: You need to close the IDE and recompile, otherwise the correct system time cannot be obtained。 
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


void setup(void)
{
    Serial.begin(9600);
    /*Wait for the chip to be initialized completely, and then exit*/
    while(rtc.begin() != true){
        Serial.println("Failed to init chip, please check if the chip connection is fine. ");
        delay(1000);
    }
    //rtc.setMode();//Set time mode, default in the 24 hours mode, e24hours, eAM, ePM.
    /*!
     *@brief Set the last compiled time as the current time
     *@param comSec  default is 0
     */
    rtc.getCompileTime();//Set the last compiled time as the current time

    /*!
     *@brief enable the 32k output (default is enable)
     */
    //rtc.disAble32k();

    /*!
     *@brief disable the 32k output 
     */
    //rtc.enAble32k();
}

void loop() {
    /*! 
     *@brief Get current time data 
     *@return Current time data
     */
    rtc.getNowTime();
    Serial.print(rtc.year(), DEC);//year
    Serial.print('/');
    Serial.print(rtc.month(), DEC);//month
    Serial.print('/');
    Serial.print(rtc.day(), DEC);//date
    Serial.print(" (");
    Serial.print(rtc.getDayOfTheWeek());//day of week
    Serial.print(") ");
    Serial.print(rtc.hour(), DEC);//hour
    Serial.print(':');
    Serial.print(rtc.minute(), DEC);//minute
    Serial.print(':');
    Serial.print(rtc.second(), DEC);//second
    Serial.print(' ');
    /*if rtc works in 24hours mode,this function doesn't print anything*/
    Serial.print(rtc.getAMorPM());
    Serial.println();
    Serial.print("Temperature: ");
    /*!
     *@brief Get current temperature
     *@return Current temperautre, unit: ℃ 
     */
    Serial.print(rtc.getTemperatureC());
    Serial.println(" C");
    delay(1000);
    /*!
     *@brief Judge if it is power-down
     *@return If retrun true, power down, needs to reset time; false, work well.
     */
    if (rtc.lostPower()) {
        Serial.println("RTC lost power, plrase reset the time!");
    }
}