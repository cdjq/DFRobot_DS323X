# DFRobot_DS323X
DS323X is a low-cost, extremely accurate, I2C real-time clock(RTC) module. It can provide ±5%ppm accuracy (no more than 0.432s error) within the whole temperature range and life span, and is compatible with 2.5-5.5V wide voltage range. The current comsuption of the module is as low as 2uA when powered by batteries. This module can be used to measure ambient temperature with ±3℃ accuracy. and it has 25 times seismic resistance of the later as well as better temperature stability.   <br>

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
* 设置闹钟，用户可设置两个闹钟，闹钟可触发中断引脚下降沿脉冲信号 <br>
* 测量气温，读取芯片温度，误差±0.5摄氏度 <br>
* 读写DS3232芯片内部236字节SRAM<br>

## Installation

To use this library, first download the library file, paste it into the \Arduino\libraries directory, then open the examples folder and run the demo in the folder.

## Methods

```C++

  DFRobot_DS323X(TwoWire *pWire = &Wire);
  ~DFRobot_DS323X();
  /*!
   *@brief Init chip 
   *@return True means IIC communication succeeds, false means it fails. 
   */
  bool begin(void);

  /*!
   *@brief get year of now time
   *@return year
   */
  uint16_t getYear();
  
  /*!
   *@brief get month of now time
   *@return month
   */
  uint8_t getMonth();
  
  /*!
   *@brief get date of now time
   *@return date
   */
  uint8_t getDate();
  
  /*!
   *@brief get hour of now time
   *@return hour
   */
  uint8_t getHour();
  
  /*!
   *@brief get minute of now time
   *@return minute
   */
  uint8_t getMinute();
  
  /*!
   *@brief get second of now time
   *@return second
   */
  uint8_t getSecond();
  
  /*!
   *@brief get day of week
   *@return day of week
   */
  String getDayOfWeek();
  
  /*!
   *@brief Set hour system of time
   *@param eHours_t:e24hours, e12hours. default is e24hours
   */
  void setHourSystem(eHours_t mode = e24hours);
  
  /*!
   *@brief Set time into rtc and take effect immediately
   *@param year, 1900~2100
   *@param month, 1~12
   *@param date, 1~31
   *@param hour, 0~23
   *@param minute, 0~59
   *@param second, 0~59
   */
  void setTime(uint16_t year, uint8_t month, uint8_t date, uint8_t hour, uint8_t minute, uint8_t second);
  
  /*!
   *@brief Set the last compiled time as the current time
   *@param comSec, 补偿时间，由第一次上传后串口打印的时间减去PC系统时间所得的值，单位：秒
   */
  void getCompileTime(uint8_t comSec);
  
  
  /*!
   *@brief output AM or PM of time 
   *@return AM or PM, 24hours return empty string 
   */
  String getAMorPM();
  
  /*!
   *@brief Get current temperature 
   *@return Current temperautre, unit: ℃ 
   */
  float getTemperatureC();
  
  /*!
   *@brief Judge if it is power-down 
   *@return true 表示rtc曾经断电，需要重新设置时间;false 表示rtc工作正常 
   */
  bool isLostPower(void);
  
  /*!
   *@brief Read the value of pin sqw
   *@return eSquareWave_OFF  = 0x1C // Not output square wave, enter interrupt mode 
   *@n      eSquareWave_1Hz  = 0x00 // 1Hz square wave
   *@n      eSquareWave_1kHz = 0x08 // 1kHz square wave
   *@n      eSquareWave_4kHz = 0x10 // 4kHz square wave
   *@n      eSquareWave_8kHz = 0x18 // 8kHz square wave
   */
  eSqwPinMode_t readSqwPinMode();
  
  /*!
   *@brief Set the vaule of pin sqw
   *@param mode eSquareWave_OFF  = 0x1C // Not output square wave, enter interrupt mode 
   *@n          eSquareWave_1Hz  = 0x00 // 1Hz square wave
   *@n          eSquareWave_1kHz = 0x08 // 1kHz square wave
   *@n          eSquareWave_4kHz = 0x10 // 4kHz square wave
   *@n          eSquareWave_8kHz = 0x18 // 8kHz square wave
   */
  void writeSqwPinMode(eSqwPinMode_t mode);
  
  /*!
   *@brief Set alarm1 clock
   *@param alarmType Alarm clock working mode typedef enum{
   *@n                                  eEverySecond,                   //repeat in every second
   *@n                                  eSecondsMatch,                  //repeat in every minute
   *@n                                  eSecondsMinutesMatch,           //repeat in every hour
   *@n                                  eSecondsMinutesHoursMatch,      //repeat in every day
   *@n                                  eSecondsMinutesHoursDateMatch,  //repeat in every month
   *@n                                  eSecondsMinutesHoursDayMatch,   //repeat in every week  //Alarm1
   *@n                                  eUnknownAlarm1
   *@n                                  }eAlarm1Types_t;
   *@param days    Alarm clock Day (0-31)
   *@param hours   Alarm clock Hour (0-23)
   *@param minutes Alarm clock Minute (0-59)
   *@param seconds Alarm clock Second (0-59)
   */
  void setAlarm1(const uint8_t alarmType,int16_t days,int8_t hours,eHours_t mode,
                int8_t minutes,int8_t seconds);
  
  /*!
   *@brief Set alarm2 clock
   *@param alarmType Alarm clock working mode typedef enum{
   *@n                                  eEveryMinute,           //repeat in every minute
   *@n                                  eMinutesMatch,          //repeat in every hour
   *@n                                  eMinutesHoursMatch,     //repeat in every day
   *@n                                  eMinutesHoursDateMatch, //repeat in every month
   *@n                                  eMinutesHoursDayMatch,  //repeat in every week      //Alarm2
   *@n                                  eUnknownAlarm2
   *@n                                  }eAlarm2Types_t;
   *@param days    Alarm clock Day (0-31)
   *@param hours   Alarm clock Hour (0-23)
   *@param minutes Alarm clock Minute (0-59)
   */
  void setAlarm2(const uint8_t alarmType,int16_t days,int8_t hours,eHours_t mode,
                int8_t minutes);
  
  /*!
   *@brief Judge if the alarm clock is triggered 
   *@return true, triggered; false, not trigger
   */
  bool isAlarmTrig();
  
  /*!
   *@brief Clear trigger flag
   */
  void clearAlarm();
  
  /*!
   *@brief enable or disable the interrupt of alarm 
   */
  void enableAlarm1Int();
  void disableAlarm1Int();
  void enableAlarm2Int();
  void disableAlarm2Int();
  
  /*!
   *@brief enable or disable the 32k pin output 
   */
  void enable32k();
  void disable32k();
    
  /*!
   *@brief write, read and clear the SRAM
   *@param addr 0x14~0xFF
   *@param data uint8_t HEX
   */
  void writeSRAM(uint8_t addr, uint8_t data);
  uint8_t readSRAM(uint8_t addr);
  void clearSRAM(uint8_t addr);
```

## Compatibility

MCU                | Work Well    | Work Wrong   | Untested    | Remarks
------------------ | :----------: | :----------: | :---------: | -----
Arduino uno        |      √       |              |             | 
Mega2560        |      √       |              |             | 
Leonardo        |      √       |              |             | 
ESP32        |      √       |              |             | 
micro:bit        |      √       |              |             | 


## History

- Data 2021-3-1
- Version V0.1


## Credits

Written by(yufeng.luo@dfrobot.com), 2019. (Welcome to our [website](https://www.dfrobot.com/))





