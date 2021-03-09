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

* Read the time the program was last compiled. <br>
* Set a alarm clock to trigger at a specified time. <br>
* Measure ambient temperature. <br>

## Installation

To use this library, first download the library file, paste it into the \Arduino\libraries directory, then open the examples folder and run the demo in the folder.

## Methods

```C++

  DFRobot_DS323X(TwoWire *pWire = &Wire){_pWire = pWire;};
  ~DFRobot_DS323X();
  /*!
   *@brief Init chip 
   *@return True means IIC communication succeeds, false means it fails. 
   */
  bool begin(void);
  /*!
   *@brief Get current time data
   */
  void getNowTime();
  
  /*!
   *@brief get year
   *@return year
   */
  uint16_t year();
  
  /*!
   *@brief get month
   *@return month
   */
  uint8_t  month();
  
  /*!
   *@brief get date
   *@return date
   */
  uint8_t  day();
  
  /*!
   *@brief get hour
   *@return hour
   */
  uint8_t  hour();
  
  /*!
   *@brief get minute
   *@return minute
   */
  uint8_t  minute();
  
  /*!
   *@brief get second
   *@return second
   */
  uint8_t  second();
  
  /*!
   *@brief get day of week
   *@return day of week
   */
  uint8_t getDayOfTheWeek();
  
  /*!
   *@brief Set mode
   *@param ehours:e24hours, eAM, ePM. default is e24hours
   */
  void setMode(ehours mode = e24hours);
  
  /*!
   *@brief Set time  
   *@param Year
   *@param Month
   *@param Date
   *@param hour:1-12 in 12hours,0-23 in 24hours
   *@param Minute 
   *@param Second
   */
  void setTime(uint16_t year, uint8_t month, uint8_t date, uint8_t hour, uint8_t minute, uint8_t second);
  
  /*!
   *@brief Set the last compiled time as the current time
   *@param comSec  Compensation time , default is 0s
   */
  void getCompileTime(uint8_t comSec);
  
  
  /*!
   *@brief output AM or PM of time 
   *@return AM or PM, 24hours is null
   */
  const char* getAMorPM();
  
  /*!
   *@brief Get current temperature 
   *@return Current temperautre, unit: ℃ 
   */
  float getTemperatureC();
  
  /*!
   *@brief Judge if it is power-down 
   *@return If retrun true, power down, time needs to reset; false, work well. 
   */
  bool lostPower(void);
  
  /*!
   *@brief Read the value of pin sqw
   *@return eDS323XM_OFF             = 0x01 // Off
   *@n      eDS323XM_SquareWave_1Hz  = 0x00 // 1Hz square wave
   *@n      eDS323XM_SquareWave_1kHz = 0x08 // 1kHz square wave
   *@n      eDS323XM_SquareWave_4kHz = 0x10 // 4kHz square wave
   *@n      eDS323XM_SquareWave_8kHz = 0x18 // 8kHz square wave
   */
  eDs3231MSqwPinMode_t readSqwPinMode();
  
  /*!
   *@brief Set the vaule of pin sqw
   *@param mode eDS323XM_OFF             = 0x01 // Off
   *@n          eDS323XM_SquareWave_1Hz  = 0x00 // 1Hz square wave
   *@n          eDS323XM_SquareWave_1kHz = 0x08 // 1kHz square wave
   *@n          eDS323XM_SquareWave_4kHz = 0x10 // 4kHz square wave
   *@n          eDS323XM_SquareWave_8kHz = 0x18 // 8kHz square wave
   */
  void writeSqwPinMode(eDs3231MSqwPinMode_t mode);
  
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
   *@param days    Alarm clock Day (day)
   *@param hours   Alarm clock Hour (hour)
   *@param mode:   e24hours, eAM, ePM
   *@param minutes Alarm clock (minute)
   *@param seconds Alarm clock (second)
   */
  void setAlarm(const uint8_t alarmType,int16_t days,int8_t hours,ehours mode,
                int8_t minutes,int8_t seconds, const bool state  = true);
  
  /*!
   *@brief Judge if the alarm clock is triggered 
   *@return true, triggered; false, not trigger
   */
  bool isAlarm();
  /*!
   *@brief Clear trigger flag
   */
  void clearAlarm();
  
  /*!
   *@brief enable or disable the interrupt of alarm 
   */
  void enAbleAlarm1Int();
  void disAbleAlarm1Int();
  void enAbleAlarm2Int();
  void disAbleAlarm2Int();
  
  /*!
   *@brief enable the 32k output 
   */
  void enAble32k();
  
  /*!
   *@brief disable the 32k output 
   */
  void disAble32k();
    
  /*!
   *@brief write, read and clear the SRAM, only work on DS3232
   */
  void writeSRAM(uint8_t reg, uint8_t data);
  uint8_t readSRAM(uint8_t reg);
  void clearSRAM(uint8_t reg);
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





