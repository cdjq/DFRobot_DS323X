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
     *@n                                  }eAlarm1Types;
     *@param days    Alarm clock Day (day)
     *@param hours   Alarm clock Hour (hour)
     *@param mode:   e24hours, eAM, ePM
     *@param minutes Alarm clock (minute)
     *@param seconds Alarm clock (second)
     */
    rtc.setAlarm1(rtc.eSecondsMatch,/*date,0-30*/1,/*hour,1-12 in 12hours,0-23 in 24hours*/0,/*minute,0-59*/0,/*second,0-59*/11);//Alarm1
    /*!
     *@brief Set alarm clock
     *@param alarmType Alarm clock working mode typedef enum{
     *@n                                  eEveryMinute,
     *@n                                  eMinutesMatch,
     *@n                                  eMinutesHoursMatch,
     *@n                                  eMinutesHoursDateMatch,
     *@n                                  eMinutesHoursDayMatch,        //Alarm2
     *@n                                  eUnknownAlarm
     *@n                                  }eAlarm2Types;
     *@param days    Alarm clock Day (day)
     *@param hours   Alarm clock Hour (hour)
     *@param mode:   e24hours, eAM, ePM
     *@param minutes Alarm clock (minute)
     *@param seconds Alarm clock (second)
     */
    rtc.setAlarm2(rtc.eMinutesHoursDateMatch,/*date,0-30*/1,/*hour,1-12 in 12hours,0-23 in 24hours*/1,/*minute,0-59*/0);//Alarm2
    if (rtc.isLostPower())
        rtc.setTime(/*year,1901-2099*/2021, /*mouth,1-12*/2, /*date,1-31*/28, /*hour,0-23*/23,/*minute,0-59*/59,\
                    /*second,0-59, this argument doesn't work in Alarm2*/55);//Set Set initial time .
}
void loop() {
    /*!
     *@brief Judge if the alarm clock is triggered
     *@return true, triggered; false, not triggered
     */
    if (rtc.isAlarmTrig()){ // If the alarm bit is set
        Serial.println("Alarm clock is triggered.");
        /*!
         *@brief Clear trigger flag
         */
        rtc.clearAlarm();
    }
    Serial.print(rtc.getYear(), DEC);
    Serial.print('/');
    Serial.print(rtc.getMonth(), DEC);
    Serial.print('/');
    Serial.print(rtc.getDate(), DEC);
    Serial.print(" (");
    Serial.print(rtc.getDayOfWeek());
    Serial.print(") ");
    Serial.print(rtc.getHour(), DEC);
    Serial.print(':');
    Serial.print(rtc.getMinute(), DEC);
    Serial.print(':');
    Serial.print(rtc.getSecond(), DEC);
    Serial.print(' ');
    /*if rtc works in 24hours mode,this function doesn't print anything*/
    Serial.print(rtc.getAMorPM());
    Serial.println();
    if (rtc.isLostPower()) {
        Serial.println("RTC lost power, please reset the time!");
    }
    delay(1000);
}
