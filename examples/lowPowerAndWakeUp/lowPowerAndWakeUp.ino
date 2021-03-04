/*!
 * @file lowPowerAndWakeUp.ino
 * @brief Set alarm, and use interrput pin to trigger MCU wake up
 * @n Experiment phenomenon: Set the alarm clock to trigger at a specified time.
 * @n                        Connect SQW pin with DIGITALPIN2.
 * @n                        Print information on serial port after the alarm clock is triggered.
 * @n                        This demo only works on avr
 * @copyright	Copyright (c) 2010 DFRobot Co.Ltd (http://www.dfrobot.com)
 * @licence     The MIT License (MIT)
 * @author [LuoYufeng](yufeng.luo@dfrobot.com)
 * @version  V0.1
 * @date  2021-2-23
 * @url https://github.com/DFRobot/DFRobot_DS323X
 */
#include "DFRobot_DS323X.h"
#include <avr/sleep.h>

volatile  int8_t alarmFlag = 0;

DFRobot_DS323X rtc;
int t = 0;

void setup(void)
{
    Serial.begin(9600);
    /*Wait for the chip to be initialized completely, and then exit*/
    while(rtc.begin() != true){
        Serial.println("failed to init chip, please check if the chip connection is correct. ");
        delay(1000);
    }
    /*!
     *@brief Set the value of pin sqw
     *@param mode eDS323X_OFF             = 0x01 // Not output square wave, enter interrupt mode
     *@n          eDS323X_SquareWave_1Hz  = 0x00 // 1Hz square wave
     *@n          eDS323X_SquareWave_1kHz = 0x08 // 1kHz square wave
     *@n          eDS323X_SquareWave_4kHz = 0x10 // 4kHz square wave
     *@n          eDS323X_SquareWave_8kHz = 0x18 // 8kHz square wave
     */
    rtc.writeSqwPinMode(eDS323X_OFF);
    rtc.enAbleAlarm1Int();
    //rtc.disAbleAlarm1Int();
    //rtc.enAbleAlarm2Int();
    rtc.disAbleAlarm2Int();
    //rtc.setMode();    //Set time mode, default is the 24 hours mode, e24hours, eAM, ePM.
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
     *@param days    Alarm clock (day)
     *@param hours   Alarm clock (hour)
     *@param minutes Alarm clock (minute)
     *@param seconds Alarm clock (second)
     */
    rtc.setAlarm(eSecondsMatch,/*date,0-30*/0,/*hour,1-12 in 12hours,0-23 in 24hours*/0,/*minute,0-59*/0,/*second,0-59*/10);
    
    rtc.setTime(/*year,1900-2100*/2021, /*mouth,1-12*/2, /*date,1-31*/28, /*hour,0-23*/23,/*minute,0-59*/59,/*second,0-59*/55);//Set Set initial time .
    attachInterrupt(0, interrupt, FALLING);
    
    /*!
     *@brief Set avr sleep mode
     */
    set_sleep_mode (SLEEP_MODE_PWR_DOWN);
    sleep_cpu();
    sleep_enable();
    
}
void loop() {
    /*!
     *@brief Judge if the alarm clock is triggered
     *@return true, triggered; false, not triggered
     */
    if(alarmFlag == 1){
        Serial.println("Alarm is up");
        rtc.clearAlarm();
        alarmFlag = 0;
        while (t < 10){
            rtc.getNowTime();
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
            delay(1000);
            t = t + 1;
        }
        t = 0;
    }
    else
        delay(1000);
    /*!
     *@brief Judge if it is power-down 
     *@return if return true, power-down, time needs to reset; false, work well
     */
    if (rtc.lostPower()) {
        Serial.println("RTC lost power, please reset the time!");
    }
    sleep_enable();
    //energy.PowerDown();
}


void interrupt(){
    alarmFlag = 1;
    sleep_disable();
}
