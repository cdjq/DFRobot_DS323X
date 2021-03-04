#-*- coding: utf-8 -*-
""" 
@file DFRobot_DS323X.py

@brief Define the basic structure of class DFRobot_DS323X

@copyright   Copyright (c) 2010 DFRobot Co.Ltd (http://www.dfrobot.com)
@licence     The MIT License (MIT)
@author [LuoYufeng](yufeng.luo@dfrobot.com)
@version  V1.0
@date  2021-2-25
@get from https://www.dfrobot.com
@url https://github.com/DFRobot/DFRobot_DS323X
"""

import sys
sys.path.append('../')
import smbus 
from ctypes import*
import time


class DFRobot_DS323X:
    
    OFF                = 0x01
    SquareWave_1Hz     = 0x00
    SquareWave_1kHz    = 0x08
    SquareWave_4kHz    = 0x10
    SquareWave_8kHz    = 0x18
    
    H24hours            = 0
    AM                 = 2
    PM                 = 3
    
    EverySecond                  = 0
    SecondsMatch                 = 1
    SecondsMinutesMatch          = 2
    SecondsMinutesHoursMatch     = 3
    SecondsMinutesHoursDateMatch = 4
    SecondsMinutesHoursDayMatch  = 5
    
    EveryMinute                  = 6
    MinutesMatch                 = 7
    MinutesHoursMatch            = 8
    MinutesHoursDateMatch        = 9
    MinutesHoursDayMatch         = 10

    UnknownAlarm                 = 11

    _IIC_ADDRESS        = 0x68

    _SECONDS_FROM_1970_TO_2000  = 946684800
    _REG_RTC_SEC        = 0X00
    _REG_RTC_MIN        = 0X01
    _REG_RTC_HOUR       = 0X02
    _REG_RTC_DAY        = 0X03
    _REG_RTC_DATE       = 0X04
    _REG_RTC_MONTH      = 0X05
    _REG_RTC_YEAR       = 0X06
    _REG_ALM1_SEC       = 0X07
    _REG_ALM1_MIN       = 0X08
    _REG_ALM1_HOUR      = 0X09
    _REG_ALM1_DAY       = 0X0A
    _REG_ALM2_MIN       = 0X0B
    _REG_ALM2_HOUR      = 0X0C
    _REG_ALM2_DAY       = 0X0D
    _REG_CONTROL        = 0x0E
    _REG_STATUS         = 0x0F
    _REG_AGE_OFFSET     = 0X10
    _REG_TEMPERATURE1   = 0x11
    _REG_TEMPERATURE2   = 0x12
    
    rtc_bcd = []
    bcd = []
    _d = 0
    _m = 0
    _y = 0
    _mode = 0
    ss = 0
    mm = 0
    hh = 0
    d = 0
    m = 0
    y = 0
    days_in_month = [31,28,31,30,31,30,31,31,30,31,30,31]
    days_of_the_week = ["Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"]
    hour_of_am = [" ", " ", ",AM", ",PM"] 

    def __init__(self, bus):
        _deviceAddr = self._IIC_ADDRESS
        self.i2cbus=smbus.SMBus(bus)
        self.i2c_addr = self._IIC_ADDRESS
    
    def begin(self):
        if not self.scan():
            return False
        else:
            return True

    def date2days(self, y, m, d):
        if y >= 2000:
            y -= 2000
        days = d
        for i in range(1, m):
            days += self.days_in_month[i - 1]
        if (m > 2 and y % 4) == 0:
            ++days
        return days + 365 * y + int((y + 3) / 4) - 1

    def conv2d(self, p):
        v = 0
        if p >= '0' and p <= '9':
            v = p - '0'
        return 10 * v + ++p - '0'


    def bcd2bin(self, val):
        return val - 6 * (val >> 4)

    def bin2bcd (self, val):
        return val + 6 * int(val / 10)

    '''
    @brief Read the value of pin sqw
    @return mode OFF             = 0x01 # Off
    @n           SquareWave_1Hz  = 0x00 # 1Hz square wave
    @n           SquareWave_1kHz = 0x08 # 1kHz square wave
    @n           SquareWave_4kHz = 0x10 # 4kHz square wave
    @n           SquareWave_8kHz = 0x18 # 8kHz square wave
    '''
    def read_sqw_pin_mode(self):        #Read the value of pin sqw
        mode = self.Control()
        mode = self.read_reg(self._REG_CONTROL)
        return mode.BBSQW

    '''
    @brief Set the vaule of pin sqw
    @param mode OFF             = 0x01 # Not output square wave, enter interrupt mode
    @n          SquareWave_1Hz  = 0x00 # 1Hz square wave
    @n          SquareWave_1kHz = 0x08 # 1kHz square wave
    @n          SquareWave_4kHz = 0x10 # 4kHz square wave
    @n          SquareWave_8kHz = 0x18 # 8kHz square wave
    '''
    def write_sqw_pin_mode(self, mode): #Set the vaule of pin sqw
        ctrl = self.read_reg(self._REG_CONTROL)
        ctrl &= 0x04
        ctrl &= 0x18
        if mode == self.OFF:
            ctrl |= 0x04
        else:
            ctrl |= mode
        
        self.write_reg(self._REG_CONTROL, ctrl);
    
    '''
    @brief get day of week
    @return day of week
    '''
    def day_of_the_week(self):
        day = self.date2days(self._y, self._m, self._d)
        return (day + 6) % 7
    
    def get_day_of_the_week(self):
        return self.days_of_the_week[self.day_of_the_week()]

    '''
    @brief Set mode of time
    @param mode H24hours, AM, PM
    '''
    def set_mode(self, mode = H24hours):
        self._mode = mode

    '''
    @brief Set time  
    @param Year
    @param Month
    @param Date
    @param hour:1-12 in 12hours,0-23 in 24hours
    @param Minute 
    @param Second
    '''
    def set_time(self, year, month, date, hour, minute, second):
        if year >=2000:
            self.y = self.bin2bcd(year - 2000)
            self.m = self.bin2bcd(month + 80)
        else:
            self.y = self.bin2bcd(year - 1900)
            self.m = month
        self.d = self.bin2bcd(date)
        self.hh = self.bin2bcd(hour) | (self._mode << 5)
        self.mm = self.bin2bcd(minute)
        self.ss = self.bin2bcd(second)
        #data = [self.ss, self.mm, self.hh, self.day_of_the_week(), self.d, self.m, self.y]
        self.write_reg(self._REG_RTC_SEC, self.ss)
        self.write_reg(self._REG_RTC_MIN, self.mm)
        self.write_reg(self._REG_RTC_HOUR, self.hh)
        self.write_reg(self._REG_RTC_DATE, self.d)
        self.write_reg(self._REG_RTC_MONTH, self.m)
        self.write_reg(self._REG_RTC_YEAR, self.y)
        statreg = self.read_reg(self._REG_STATUS)
        statreg &= ~0x80
        self.write_reg(self._REG_STATUS, statreg)
    
    '''
    @brief get time mode of now
    @return time mode
    '''
    def get_AM_or_PM(self):
        buffer = self.read_reg(self._REG_RTC_HOUR)
        buffer = buffer & 0x60
        buffer = buffer >> 5
        return self.hour_of_am[buffer]
    
    '''
    @brief get year of now time
    @return year
    '''
    def get_year(self):
        year = self.read_reg(self._REG_RTC_YEAR)
        self._y = self.bcd2bin(year) + 1900
        century = self.read_reg(self._REG_RTC_MONTH)
        if century > 80:
            self._y += 100
        return self._y
    
    '''
    @brief get month of now time
    @return month
    '''
    def get_month(self):
        month = self.read_reg(self._REG_RTC_MONTH)
        self._m = self.bcd2bin(month)
        if self._m > 80:
            self._m -= 80
        return self._m
    
    '''
    @brief get date of now time
    @return date
    '''
    def get_date(self):
        date = self.read_reg(self._REG_RTC_DATE)
        self._d = self.bcd2bin(date)
        return self._d 
    
    '''
    @brief get hour of now time
    @return hour
    '''
    def get_hour(self):
        hour = self.read_reg(self._REG_RTC_HOUR)
        if hour >> 6 == True:
            self._hh = self.bcd2bin(hour & 0x1F)
        else:
            self._hh = self.bcd2bin(hour & 0x3F)
        return self._hh
    
    '''
    @brief get minute of now time
    @return minute
    '''
    def get_minute(self):
        minute = self.read_reg(self._REG_RTC_MIN)
        self._mm = self.bcd2bin(minute)
        return self._mm
    
    '''
    @brief get second of now time
    @return second
    '''
    def get_second(self):
        second = self.read_reg(self._REG_RTC_SEC)
        self._ss = self.bcd2bin(second & 0x7F)
        return self._ss
    
    '''
    @brief get temperature of sensor, unit:℃
    '''
    def get_temperature_C(self):
        buf1 = self.read_reg(self._REG_TEMPERATURE1)
        buf2 = self.read_reg(self._REG_TEMPERATURE2)
        return (buf1 + (buf2>>6)*0.25)
    
    '''
    @brief get temperature of sensor, unit:℉
    '''
    def get_temperature_F(self):
        buf1 = self.read_reg(self._REG_TEMPERATURE1)
        buf2 = self.read_reg(self._REG_TEMPERATURE2)
        c = (buf1 + (buf2>>6)*0.25)
        return c * 9 / 5 + 32
    
    '''
    @brief check if rtc has been lost power
    '''
    def is_lost_power(self):
        status = self.read_reg(_REG_STATUS)
        return status >> 7

    '''
    @brief Set alarm clock
    @param alarmType
    @param days      Alarm clock Day (day)
    @param hours     Alarm clock Hour (hour)
    @param minutes   Alarm clock (minute)
    @param seconds   Alarm clock (second)
    '''
    def set_alarm(self, alarmType, date, hour, minute, second, state = True):
        dates = self.bin2bcd(date)
        hours = self._mode >> 6|self.bin2bcd(hour)
        minutes = self.bin2bcd(minute)
        seconds = self.bin2bcd(second)
        days = self.bin2bcd(self.day_of_the_week())
        if alarmType >= self.UnknownAlarm:
            return
        if alarmType < self.EveryMinute:
            self.write_reg(self._REG_ALM1_SEC, seconds)
            self.write_reg(self._REG_ALM1_MIN, minutes)
            self.write_reg(self._REG_ALM1_HOUR, hours)
            if alarmType == self.SecondsMinutesHoursDateMatch:
                self.write_reg(self._REG_ALM1_DAY, dates)
            else:
                self.write_reg(self._REG_ALM1_DAY, days);
            if alarmType < self.SecondsMinutesHoursDateMatch:
                buffer = self.read_reg(self._REG_ALM1_DAY)
                buffer |= 0x80
                self.write_reg(self._REG_ALM1_DAY, buffer)
            if alarmType < self.SecondsMinutesHoursMatch:
                buffer = self.read_reg(self._REG_ALM1_HOUR)
                buffer |= 0x80
                self.write_reg(self._REG_ALM1_HOUR, buffer)
            if alarmType < self.SecondsMinutesMatch:
                buffer = self.read_reg(self._REG_ALM1_MIN)
                buffer |= 0x80
                self.write_reg(self._REG_ALM1_MIN, buffer)
            if(alarmType == self.EverySecond):
                buffer = self.read_reg(self._REG_ALM1_SEC)
                buffer |= 0x80
                self.write_reg(self._REG_ALM1_SEC, buffer)
            if(alarmType == self.SecondsMinutesHoursDayMatch):
                buffer = self.read_reg(self._REG_ALM1_DAY)
                buffer |= 0x40
                self.write_reg(self._REG_ALM1_DAY, buffer)
            if state == True:
                buffer = self.read_reg(self._REG_CONTROL)
                buffer |= 1
                self.write_reg(self._REG_CONTROL, buffer)
            else:
                buffer = self.read_reg(self._REG_CONTROL)
                buffer &= 0xFE
                self.write_reg(self._REG_CONTROL, buffer)
        else:
            self.write_reg(self._REG_ALM2_MIN, minutes)
            self.write_reg(self._REG_ALM2_HOUR, hours)
            if alarmType == self.MinutesHoursDateMatch:
                self.write_reg(self._REG_ALM2_DAY)
            elif alarmType == self.MinutesHoursDayMatch:
                days |= 0x80
                self.write_reg(self._REG_ALM2_DAY, days)
            if alarmType < self.MinutesHoursDateMatch:
                buffer = self.read_reg(self._REG_ALM2_DAY)
                buffer |= 0x80;
                self.write_reg(self._REG_ALM2_DAY, buffer)
            if alarmType < self.MinutesHoursMatch:
                buffer = self.read_reg(self._REG_ALM2_HOUR)
                buffer |= 0x80
                self.write_reg(self._REG_ALM2_HOUR, buffer)
            if alarmType == self.EveryMinute:
                buffer = self.read_reg(self._REG_ALM2_MIN,)
                buffer |= 0x80
                self.write_reg(self._REG_ALM2_MIN, buffer)
            if state == True:
                buffer = self.read_reg(self._REG_CONTROL)
                buffer |= 2
                self.write_reg(self._REG_CONTROL, buffer)
            else:
                buffer = self.read_reg(self._REG_CONTROL)
                buffer &= 0xFD
                self.write_reg(self._REG_CONTROL, buffer)
        buf = self.read_reg(self._REG_ALM1_MIN)
        self.clear_alarm()
        return
    
    '''
    @brief enable the interrupt of alarm1
    '''
    def enable_alarm1_int(self):
        conReg = self.read_reg(self._REG_CONTROL)
        conReg |= 0x01
        self.write_reg(self._REG_CONTROL, conReg)
    
    '''
    @brief disable the interrupt of alarm1
    '''
    def disable_alarm1_int(self):
        conReg = self.read_reg(self._REG_CONTROL)
        conReg &= 0xFE
        self.write_reg(self._REG_CONTROL, conReg)
    
    '''
    @brief enable the interrupt of alarm2
    '''
    def enable_alarm2_int(self):
        conReg = self.read_reg(self._REG_CONTROL)
        conReg |= 0x02
        self.write_reg(self._REG_CONTROL, conReg)
    
    '''
    @brief disable the interrupt of alarm2
    '''
    def disable_alarm2_int(self):
        conReg = self.read_reg(self._REG_CONTROL)
        conReg &= 0xFD
        self.write_reg(self._REG_CONTROL, conReg)
    
    '''
    @brief check if alarm flag has been trigger
    '''
    def is_alarm(self):
        staReg = self.read_reg(self._REG_STATUS)
        return staReg&3
    
    '''
    @brief clear alarm flag trigger
    '''
    def clear_alarm(self):
        staReg = self.read_reg(self._REG_STATUS)
        staReg &= 0xFC
        self.write_reg(self._REG_STATUS, staReg)

    '''
    @brief enable the 32k output 
    '''
    def enable_32k(self):
        staReg = self.read_reg(self._REG_STATUS)
        staReg.en32kHZ = 1
        self.write_reg(self._REG_STATUS, staReg)
    
    '''
    @brief disable the 32k output (default is enable)
    '''
    def disable_32k(self):
        staReg = self.read_reg(self._REG_STATUS)
        staReg.en32kHZ = 0
        self.write_reg(self._REG_STATUS, staReg)

    '''
    @brief write data into the SRAM
    @param reg, address of SRAM
    @param data
    '''
    def write_SRAM(self, reg, data):
        self.write_reg(reg, data, 1)

    '''
    @brief read data of the SRAM
    @param reg, address of SRAM
    '''
    def read_SRAM(self, reg):
        self.read_reg(reg, buf, 1)
        return buf

    '''
    @brief clear the SRAM
    '''
    def clear_SRAM(self, reg):
        self.write_reg(reg, 0x00)

    def write_reg(self, reg, buff):
        self.i2cbus.write_byte_data(self.i2c_addr, reg, buff)
        

    def read_reg(self, reg):
        return self.i2cbus.read_byte_data(self.i2c_addr, reg) 

    def scan(self):
        try:
            self.i2cbus.read_byte(self.i2c_addr)
            return True
        except:
            print("I2C init fail")
            return False
