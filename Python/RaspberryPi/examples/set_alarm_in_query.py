#-*- coding: utf-8 -*-
'''
file set_alarm_in_query.py

@brief Through the example, you can set alarm clock in query:
@n     Experiment phenomenon: read data every 1 seconds and print it on terminal.

@Copyright   Copyright (c) 2010 DFRobot Co.Ltd (http://www.dfrobot.com)
@licence   The MIT License (MIT)

@author [LuoYufeng](yufeng.luo@dfrobot.com)
@url https://github.com/DFRobot/DFRobot_DS323X
@version  V1.0
@date  2021-3-4
'''
import sys
sys.path.append('../')
import time
from DFRobot_DS323X import *

rtc = DFRobot_DS323X(bus=1)

#begin return True if succeed, otherwise return False
while not rtc.begin():
    time.sleep(2)

'''
@brief Set the last compiled time as the current time
'''
rtc.set_time(2021,2,28,23,59,55)

'''
@brief Set alarm1 clock
@param alarmType:EverySecond,
@n               SecondsMatch,
@n               SecondsMinutesMatch,
@n               SecondsMinutesHoursMatch,
@n               SecondsMinutesHoursDateMatch,
@n               SecondsMinutesHoursDayMatch, #Alarm1
@n               UnknownAlarm
@param days      Alarm clock Day (day)
@param hours     Alarm clock Hour (hour)
@param mode:     H24hours, AM, PM
@param minutes   Alarm clock (minute)
@param seconds   Alarm clock (second)
'''
rtc.set_alarm1(alarmType=rtc.SecondsMatch,date=1,hour=0,minute=0,second=5)
'''
@brief Set alarm2 clock
@param alarmType:EveryMinute,
@n               MinutesMatch,
@n               MinutesHoursMatch,
@n               MinutesHoursDateMatch,
@n               MinutesHoursDayMatch,        #Alarm2
@n               UnknownAlarm
@param days      Alarm clock Day (day)
@param hours     Alarm clock Hour (hour)
@param mode:     H24hours, AM, PM
@param minutes   Alarm clock (minute)
'''
rtc.set_alarm2(alarmType=rtc.MinutesHoursDayMatch,date=1,hour=0,minute=0)

def main():
    while True:
        if rtc.is_lost_power() == 1:
            print("RTC lost power, plrase reset the time!")
        print("{0}/{1}/{2},{3},{4}:{5}:{6}{7}".format(rtc.get_year(),rtc.get_month(),rtc.get_date(),\
        rtc.get_day_of_the_week(),rtc.get_hour(),rtc.get_minute(),rtc.get_second(),rtc.get_AM_or_PM()))#print now time
        if rtc.is_alarm_trig() != 0:
            print("Alarm clock is triggered.")
            rtc.clear_alarm()
        print(" ")
        time.sleep(1)

if __name__ == "__main__":
    main()