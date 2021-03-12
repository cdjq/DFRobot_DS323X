# DFRobot_DS323X
DS323X is a low-cost, extremely accurate, I2C real-time clock(RTC) module. It can provide ±5%ppm accuracy (no more than 0.432s error) within the whole temperature range and life span, and is compatible with 2.5-5.5V wide voltage range. The current comsuption of the module is as low as 2uA when powered by batteries. This module can be used to measure ambient temperature with ±3℃ accuracy.   <br>

![正反面svg效果图](https://github.com/ouki-wang/DFRobot_Sensor/raw/master/resources/images/SEN0245svg1.png)


## Product Link （链接到英文商城）
    SKU：产品名称
   
## Table of Contents

* [Summary](#summary)
* [Installation](#installation)
* [Methods](#methods)
* [Compatibility](#compatibility)
* [History](#history)
* [Credits](#credits)

## Summary

* 获取实时时间，初始时间需要用户自行设置，可以获取编译时间，NTP时间或者自行设置时刻，最小单位为秒 <br>
* 设置闹钟，用户可设置两个闹钟，并在串口或中断引脚读取串口触发flag <br>
* 测量气温，读取气温的功能仅作参考 <br>
* 读写SRAM，仅用于DS3232,有236字节的SRAM可供读写 <br>

## Installation

Download this library into RaspberryPi, and run demo with Python
## Methods

```Python

  '''
  @brief Init chip 
  @return True means IIC communication succeeds, false means it fails. 
  '''
  def begin()
  
  '''
  @brief get year of now time
  @return year
  '''
  def get_year()
  
  '''
  @brief get month of now time
  @return month
  '''
  def get_month()
  
  '''
  @brief get date of now time
  @return date
  '''
  def get_date()
  
  '''
  @brief get minute of now time
  @return minute
  '''
  def get_minute()
  
  '''
  @brief get second of now time
  @return second
  '''
  def get_second()
  
  '''
  @brief get day of week
  @return day of week
  '''
  def get_day_of_the_week()
  
  '''
  @brief Set time  
  @param Year
  @param Month
  @param Date
  @param hour:1-12 in 12hours,0-23 in 24hours
  @param Minute 
  @param Second
  '''
  def set_time(year, month, date, hour, minute, second)
  
  '''
  @brief get time mode of now
  @return time mode
  '''
  def get_AM_or_PM()
  
  '''
  @brief get temperature of sensor, unit:℃
  '''
  def get_temperature_C()
  
  '''
  @brief get temperature of sensor, unit:℉
  '''
  def get_temperature_F()
  
  '''
  @brief check if rtc has been lost power
  '''
  def is_lost_power()
  
  '''
  @brief Read the value of pin sqw
  @return mode OFF             = 0x1C # Off
  @n           SquareWave_1Hz  = 0x00 # 1Hz square wave
  @n           SquareWave_1kHz = 0x08 # 1kHz square wave
  @n           SquareWave_4kHz = 0x10 # 4kHz square wave
  @n           SquareWave_8kHz = 0x18 # 8kHz square wave
  '''
  def read_sqw_pin_mode()
  
  '''
  @brief Set the vaule of pin sqw
  @param mode OFF             = 0x1C # Not output square wave, enter interrupt mode
  @n          SquareWave_1Hz  = 0x00 # 1Hz square wave
  @n          SquareWave_1kHz = 0x08 # 1kHz square wave
  @n          SquareWave_4kHz = 0x10 # 4kHz square wave
  @n          SquareWave_8kHz = 0x18 # 8kHz square wave
  '''
  def write_sqw_pin_mode(mode);
  
  '''
  @brief Set alarm clock
  @param alarmType:EverySecond,
  @n               SecondsMatch,
  @n               SecondsMinutesMatch,
  @n               SecondsMinutesHoursMatch,
  @n               SecondsMinutesHoursDateMatch,
  @n               SecondsMinutesHoursDayMatch, #Alarm1
  @n               UnknownAlarm
  @param days      Alarm clock Day (day)
  @param hours     Alarm clock Hour (hour)
  @param minutes   Alarm clock (minute)
  @param seconds   Alarm clock (second)
  '''
  def set_alarm1(alarmType, date, hour, minute, second)
  
  '''
  @brief Set alarm clock
  @param alarmType:EveryMinute,
  @n               MinutesMatch,
  @n               MinutesHoursMatch,
  @n               MinutesHoursDateMatch,
  @n               MinutesHoursDayMatch,        #Alarm2
  @n               UnknownAlarm
  @param days      Alarm clock Day (day)
  @param hours     Alarm clock Hour (hour)
  @param minutes   Alarm clock (minute)
  '''
  def set_alarm2(alarmType, date, hour, minute)
  
  '''
  @brief check if alarm flag has been trigger
  '''
  def is_alarm_trig()
  
  '''
  @brief clear alarm flag trigger
  '''
  def clear_alarm()
  
  '''
  @brief enable or disable the interrupt of alarm 
  '''
  def enable_alarm1_int()
  def disable_alarm1_int()
  def enable_alarm2_int()
  def disable_alarm2_int()
  
  '''
  @brief enable the 32k output 
  '''
  def enable_32k()
  
  '''
  @brief disable the 32k output (default is enable)
  '''
  def disable_32k()
```

## Compatibility

MCU                | Work Well    | Work Wrong   | Untested    | Remarks
------------------ | :----------: | :----------: | :---------: | -----
RaspberryPi        |      √       |              |             | 


## History

- Data 2021-3-1
- Version V0.1


## Credits

Written by(yufeng.luo@dfrobot.com), 2019. (Welcome to our [website](https://www.dfrobot.com/))





