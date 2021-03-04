#-*- coding: utf-8 -*-
'''
@file get_time_from_NTP.py

@brief Through the example, you can get the accurate time from raspbrrryPi:
@n     Experiment phenomenon: read data every 1 seconds and print it on terminal .

@Copyright Copyright (c) 2010 DFRobot Co.Ltd (http://www.dfrobot.com)
@licence   The MIT License (MIT)

@author [LuoYufeng](yufeng.luo@dfrobot.com)
@url https://github.com/DFRobot/DFRobot_DS323X
@version  V1.0
@date  2021-2-25
'''
import sys
sys.path.append('../')
import time
import datetime
from DFRobot_DS323X import *

rtc = DFRobot_DS323X(bus=1)

#begin return True if succeed, otherwise return False

while not rtc.begin():
    time.sleep(2)

t_time = time.localtime()
year = t_time.tm_year#Set year from NTP server
month = t_time.tm_mon#Set the months from NTP server
date = t_time.tm_mday#Set the dates from NTP server
hour = t_time.tm_hour#Set the hours from NTP server
minute = t_time.tm_min#Set the minutes from NTP server
second = t_time.tm_sec#Set the seconds from NTP server

rtc.set_time(year, month, date, hour, minute, second)

#rtc.disAble32k();#disable the 32k output (default is enable)

#rtc.enAble32k();#enable the 32k output 

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
