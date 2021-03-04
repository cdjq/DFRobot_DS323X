/*!
 * @file setAlarmInQuery.ino
 * @brief Set alarm clock 
 * @n Experiment phenomenon: set the alarm clock to trigger at a sepcified time
 * @n           Trigger when the set time of clock is the same with the RTC time
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
        Serial.println("failed to init chip, please check if the chip connection is fine");
        delay(1000);
    }
    /*!
     *@brief Set the vaule of pin sqw
     *@param mode eDS323X_OFF             = 0x01 // Not output square wave, enter interrupt mode
     *@n          eDS323X_SquareWave_1Hz  = 0x00 // 1Hz square wave
     *@n          eDS323X_SquareWave_1kHz = 0x08 // 1kHz square wave
     *@n          eDS323X_SquareWave_4kHz = 0x10 // 4kHz square wave
     *@n          eDS323X_SquareWave_8kHz = 0x18 // 8kHz square wave
     */
    rtc.writeSqwPinMode(eDS323X_OFF);
    //rtc.setMode();//Set time mode, default in the 24 hours mode, e24hours, eAM, ePM.
    /*!
     *@brief Set alarm clock
     *@param alarmType Alarm clock working mode typedef enum{
     *@n                                  eEverySecond,
     *@n                                  eSecondsMatch,
     *@n                                  eSecondsMinutesMatch,
     *@n                                  eSecondsMinutesHoursMatch,
     *@n                                  eSecondsMinutesHoursDateMatch,
     *@n                                  eSecondsMinutesHoursDayMatch, //Alarm1
     *@n                                  eEveryMinute,
     *@n                                  eMinutesMatch,
     *@n                                  eMinutesHoursMatch,
     *@n                                  eMinutesHoursDateMatch,
     *@n                                  eMinutesHoursDayMatch,        //Alarm2
     *@n                                  eUnknownAlarm
     *@n                                  }eAlarmTypes;
     *@param days    Alarm clock Day (day)
     *@param hours   Alarm clock Hour (hour)
     *@param mode:   e24hours, eAM, ePM
     *@param minutes Alarm clock (minute)
     *@param seconds Alarm clock (second)
     */
    rtc.setAlarm(eSecondsMatch,/*date,0-30*/1,/*hour,1-12 in 12hours,0-23 in 24hours*/0,/*minute,0-59*/0,/*second,0-59*/10);//Alarm1
    rtc.setAlarm(eMinutesHoursDateMatch,/*date,0-30*/1,/*hour,1-12 in 12hours,0-23 in 24hours*/0,/*minute,0-59*/0,/*second,0-59*/5);//Alarm2
    if (rtc.lostPower())
        rtc.setTime(/*year,1901-2099*/2021, /*mouth,1-12*/2, /*date,1-31*/28, /*hour,0-23*/23,/*minute,0-59*/59,\
                    /*second,0-59, this argument doesn't work in Alarm2*/55);//Set Set initial time .
}
void loop() {
    /*!
     *@brief Get current time data 
     *@return Current time data
     */
    rtc.getNowTime();
    /*!
     *@brief Judge if the alarm clock is triggered
     *@return true, triggered; false, not triggered
     */
    if (rtc.isAlarm()){ // If the alarm bit is set
        Serial.println("Alarm clock is triggered.");
        /*!
         *@brief Clear trigger flag
         */
        rtc.clearAlarm();
    }
    Serial.print(rtc.year(), DEC);
    Serial.print('/');
    Serial.print(rtc.month(), DEC);
    Serial.print('/');
    Serial.print(rtc.day(), DEC);
    Serial.print(" (");
    Serial.print(rtc.getDayOfTheWeek());
    Serial.print(") ");
    Serial.print(rtc.hour(), DEC);
    Serial.print(':');
    Serial.print(rtc.minute(), DEC);
    Serial.print(':');
    Serial.print(rtc.second(), DEC);
    Serial.print(' ');
    /*if rtc works in 24hours mode,this function doesn't print anything*/
    Serial.print(rtc.getAMorPM());
    Serial.println();
    if (rtc.lostPower()) {
        Serial.println("RTC lost power, please reset the time!");
    }
    delay(1000);
}
