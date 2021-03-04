#-*- coding: utf-8 -*-
'''
@file get_time_and_temp.py

@brief Through the example, you can get the time and temperature:
@n     Experiment phenomenon: read data every 1 second and print it on terminal .

@Copyright   Copyright (c) 2010 DFRobot Co.Ltd (http://www.dfrobot.com)
@licence   The MIT License (MIT)

@author [LuoYufeng](yufeng.luo@dfrobot.com)
@url https://github.com/DFRobot/DFRobot_DS323X
@version  V1.0
@date  2021-2-25s
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
@brief Set the vaule of pin sqw
@param mode OFF             = 0x01 # Not output square wave, enter interrupt mode
@n          SquareWave_1Hz  = 0x00 # 1Hz square wave
@n          SquareWave_1kHz = 0x08 # 1kHz square wave
@n          SquareWave_4kHz = 0x10 # 4kHz square wave
@n          SquareWave_8kHz = 0x18 # 8kHz square wave
'''
rtc.write_sqw_pin_mode(rtc.SquareWave_1Hz)
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
@brief Set mode of time
@param mode H24hours, AM, PM
'''
rtc.set_mode(rtc.AM)

'''
@brief Set time  
@param Year
@param Month
@param Date
@param hour:1-12 in 12hours,0-23 in 24hours
@param Minute 
@param Second
'''
rtc.set_time(year=2021,month=2,date=28,hour=11,minute=59,second=55)

#rtc.disAble32k();#disable the 32k output (default is enable)

#rtc.enAble32k();#enable the 32k output 

def main():
    while True:
        temp = rtc.get_temperature_C()
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
        #5.如果您很熟悉python，可以这样打印数据
        print("{0}/{1}/{2},{3},{4}:{5}:{6}{7}".format(rtc.get_year(),rtc.get_month(),rtc.get_date(),\
        rtc.get_day_of_the_week(),rtc.get_hour(),rtc.get_minute(),rtc.get_second(),rtc.get_AM_or_PM()))#print now time
        
        print(temp)
        print(" ")
        time.sleep(1)

if __name__ == "__main__":
    main()

