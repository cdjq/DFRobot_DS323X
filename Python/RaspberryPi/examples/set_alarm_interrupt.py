#-*- coding: utf-8 -*-
'''
@file set_alarm_interrupt.py

@brief Through the example, you can set alarm clock in interrupt:
@n     Experiment phenomenon: read data every 1 seconds and print it on terminal .

@Copyright   Copyright (c) 2010 DFRobot Co.Ltd (http://www.dfrobot.com)
@licence   The MIT License (MIT)

@author [LuoYufeng](yufeng.luo@dfrobot.com)
@url https://github.com/DFRobot/DFRobot_DS323X
@version  V1.0
@date  2020-2-12
'''
import sys
sys.path.append('../')
import time
import threading
import RPi.GPIO as GPIO
from DFRobot_DS323X import *

rtc = DFRobot_DS323X(bus=1)

GPIO.setmode(GPIO.BCM)


#begin return True if succeed, otherwise return False
while not rtc.begin():
    time.sleep(2)

'''
@brief enable the interrupt of alarm1
'''
rtc.enable_alarm1_int();#@enable Alarm1 interrupt
'''
@brief Set the vaule of pin sqw
@param mode OFF             = 0x01 # Not output square wave, enter interrupt mode
@n          SquareWave_1Hz  = 0x00 # 1Hz square wave
@n          SquareWave_1kHz = 0x08 # 1kHz square wave
@n          SquareWave_4kHz = 0x10 # 4kHz square wave
@n          SquareWave_8kHz = 0x18 # 8kHz square wave
'''
rtc.write_sqw_pin_mode(rtc.OFF)
'''
@brief Read the value of pin sqw
@return mode OFF             = 0x01 # Off
@n           SquareWave_1Hz  = 0x00 # 1Hz square wave
@n           SquareWave_1kHz = 0x08 # 1kHz square wave
@n           SquareWave_4kHz = 0x10 # 4kHz square wave
@n           SquareWave_8kHz = 0x18 # 8kHz square wave
'''
#rtc.read_sqw_pin_mode()
'''
@brief Set the last compiled time as the current time
'''
#rtc.set_mode(rtc.PM)
rtc.set_time(2021,2,28,23,59,55)

'''
@brief Set alarm clock
@param alarmType:EverySecond,
@n               SecondsMatch,
@n               SecondsMinutesMatch,
@n               SecondsMinutesHoursMatch,
@n               SecondsMinutesHoursDateMatch,
@n               SecondsMinutesHoursDayMatch, #Alarm1
@n
@n               EveryMinute,
@n               MinutesMatch,
@n               MinutesHoursMatch,
@n               MinutesHoursDateMatch,
@n               MinutesHoursDayMatch,        #Alarm2
@n               UnknownAlarm
@param days      Alarm clock Day (day)
@param hours     Alarm clock Hour (hour)
@param mode:     H24hours, AM, PM
@param minutes   Alarm clock (minute)
@param seconds   Alarm clock (second)
'''
rtc.set_alarm(alarmType=rtc.SecondsMatch,date=1,hour=0,minute=0,second=5)
rtc.set_alarm(alarmType=rtc.MinutesHoursDayMatch,date=1,hour=0,minute=0,second=5)
'''
@brief disable the 32k output (default is enable)
'''
#rtc.disAble32k();

'''
@brief enable the 32k output 
'''
#rtc.enAble32k();

IO1 = 21#set interrupt pin

def IO1_callback(channel):#callback function
    global rtc
    rtc.clear_alarm()
    print("Alarm clock is triggered.")
 
GPIO.setup(IO1, GPIO.IN)
'''
@brief 当IO1从低电平变为高电平，发生RISING电平跳变时，运行IO1_callback方法
'''
GPIO.add_event_detect(IO1, GPIO.FALLING, callback = IO1_callback)

def main():
    while True:
        #如果您不是很熟悉python，可以这样打印
        '''
        print(rtc.get_year()),
        print("/"),
        print(rtc.get_month()),
        print("/"),
        print(rtc.get_date()),
        print(","),
        print(rtc.get_day_of_the_week()),
        print(","),
        print(rtc.get_hour()),
        print(":"),
        print(rtc.get_minute()),
        print(":"),
        print(rtc.get_second()),
        print(","),
        print(rtc.get_AM_or_PM())
        '''
        #如果您很熟悉python，可以这样打印数据
        print("{0}/{1}/{2},{3},{4}:{5}:{6}{7}".format(rtc.get_year(),rtc.get_month(),rtc.get_date(),\
        rtc.get_day_of_the_week(),rtc.get_hour(),rtc.get_minute(),rtc.get_second(),rtc.get_AM_or_PM()))#print now time
        
        print(" ")
        time.sleep(1)

if __name__ == "__main__":
    main()