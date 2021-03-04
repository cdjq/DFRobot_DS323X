/*!
 * @file setAlarmInterrupt.ino
 * @brief Set alarm, and use interrput pin to trigger it
 * @n Experiment phenomenon: set the alarm clock to trigger at a specified time 
 * @n                        connect SQW pin with DIGITALPIN2
 * @n                        print information on serial port after the alarm clock is triggered.
 * @copyright	Copyright (c) 2010 DFRobot Co.Ltd (http://www.dfrobot.com)
 * @licence     The MIT License (MIT)
 * @author [LuoYufeng](yufeng.luo@dfrobot.com)
 * @version  V0.1
 * @date  2021-2-23
 * @url https://github.com/DFRobot/DFRobot_DS323X
 */
#include "DFRobot_DS323X.h"

volatile  int8_t alarmFlag = 0;

DFRobot_DS323X rtc;

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
    
    /*!
     *@brief enable Alarm1 interrupt
     */
    rtc.enAbleAlarm1Int();
    
    /*!
     *@brief disable Alarm1 interrupt
     */
    //rtc.disAbleAlarm1Int();
    
    /*!
     *@brief enable Alarm2 interrupt
     */
    rtc.enAbleAlarm2Int();
    
    /*!
     *@brief disable Alarm2 interrupt
     */
    //rtc.disAbleAlarm2Int();
    
    /*!
     *@brief Set alarm clock 
     *@param alarmType Alarm clock working mode typedef enum{
     *@n                                  eEverySecond,
     *@n                                  eSecondsMatch,
     *@n                                  eSecondsMinutesMatch,
     *@n                                  eSecondsMinutesHoursMatch,
     *@n                                  eSecondsMinutesHoursDateMatch,
     *@n                                  eSecondsMinutesHoursDayMatch, //Alarm1
     *@n
     *@n                                  eEveryMinute,
     *@n                                  eMinutesMatch,
     *@n                                  eMinutesHoursMatch,
     *@n                                  eMinutesHoursDateMatch,
     *@n                                  eMinutesHoursDayMatch,        //Alarm2
     *@n                                  eUnknownAlarm
     *@n                                  }eAlarmTypes;
     *@param days    Alarm clock (day)
     *@param hours   Alarm clock (hour)
     *@param mode:   e24hours, eAM, ePM
     *@param minutes Alarm clock (minute)
     *@param seconds Alarm clock (second)
     */
    //Alarm1
    rtc.setAlarm(eSecondsMatch,/*date,0-30*/1,/*hour,1-12 in 12hours,0-23 in 24hours*/0,/*minute,0-59*/0,/*second,0-59*/10);
    //Alarm2
    rtc.setAlarm(eMinutesHoursDayMatch,/*date,0-30*/1,/*hour,1-12 in 12hours,0-23 in 24hours*/0,
                 /*minute,0-59*/0,/*second,0-59, this argument doesn't work in Alarm2*/0);
    /*!
     *@brief Judge if it is power-down 
     *@return if return true, power-down, time needs to reset; false, work well
     */
    if (rtc.lostPower())
        rtc.setTime(/*year,1900-2099*/2021, /*mouth,1-12*/2, /*date,1-31*/28, /*hour,0-23*/23,/*minute,0-59*/59,/*second,0-59*/55);//Set Set initial time .
    attachInterrupt(0, interrupt, FALLING);
}
void loop() {
    /*!
     *@brief Judge if the alarm clock is triggered
     *@return true, triggered; false, not triggered
     */
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
    if(alarmFlag == 1){
        alarmFlag = 0;
        Serial.println("Alarm clock is triggered.");
        delay(1000);
        rtc.clearAlarm();
    }
    else
        delay(1000);
    if (rtc.lostPower()) {
        Serial.println("RTC lost power, please reset the time!");
    }
}

void interrupt(){
  alarmFlag = 1;
}
