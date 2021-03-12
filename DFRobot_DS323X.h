/*!
 * @file DFRobot_DS323X.h
 * @brief Define the basic structure of class DFRobot_DS323X 
 * @copyright   Copyright (c) 2010 DFRobot Co.Ltd (http://www.dfrobot.com)
 * @licence     The MIT License (MIT)
 * @author [yufeng](yufeng.luo@dfrobot.com)
 * @version  V1.0
 * @date  2021-2-23
 * @url https://github.com/DFRobot/DFRobot_DS323X
 */

#ifndef _DFRobot_DS323X_H
#define _DFRobot_DS323X_H

#include "Arduino.h"
#include <Wire.h>


/*I2C ADDRESS*/
#define DS323X_IIC_ADDRESS          0x68

#define DS323X_REG_RTC_SEC          0X00
#define DS323X_REG_RTC_MIN          0X01
#define DS323X_REG_RTC_HOUR         0X02
#define DS323X_REG_RTC_DAY          0X03
#define DS323X_REG_RTC_DATE         0X04
#define DS323X_REG_RTC_MONTH        0X05
#define DS323X_REG_RTC_YEAR         0X06
#define DS323X_REG_ALM1_SEC         0X07
#define DS323X_REG_ALM1_MIN         0X08
#define DS323X_REG_ALM1_HOUR        0X09
#define DS323X_REG_ALM1_DAY         0X0A
#define DS323X_REG_ALM2_MIN         0X0B
#define DS323X_REG_ALM2_HOUR        0X0C
#define DS323X_REG_ALM2_DAY         0X0D
#define DS323X_REG_CONTROL          0x0E  // Control register
#define DS323X_REG_STATUS           0x0F  // Status register
#define DS323X_REG_AGE_OFFSET       0X10
#define DS323X_REG_TEMPERATURE      0x11  // temperature register
//Open this macro to see the detailed running process of the program 
//#define ENABLE_DBG

#ifdef ENABLE_DBG
#define DBG(...) {Serial.print("[");Serial.print(__FUNCTION__); Serial.print("(): "); Serial.print(__LINE__); Serial.print(" ] "); Serial.println(__VA_ARGS__);}
#else
#define DBG(...)
#endif




class DFRobot_DS323X
{
public:

    typedef enum{
        eOFF             = 0x1C, // Not output square wave, enter interrupt mode 
        eSquareWave_1Hz  = 0x00, // 1Hz square wave
        eSquareWave_1kHz = 0x08, // 1kHz square wave
        eSquareWave_4kHz = 0x10, // 4kHz square wave
        eSquareWave_8kHz = 0x18  // 8kHz square wave
    }eSqwPinMode_t;

    typedef enum{
        e24hours = 0,
        eAM = 2,
        ePM = 3
    }ehours;
    
    typedef enum{
        eEverySecond,                  //repeat in every second
        eSecondsMatch,                 //repeat in every minute
        eSecondsMinutesMatch,          //repeat in every hour
        eSecondsMinutesHoursMatch,     //repeat in every day
        eSecondsMinutesHoursDateMatch, //repeat in every month
        eSecondsMinutesHoursDayMatch,  //repeat in every week
        //Alarm1
        
        eEveryMinute,                  //repeat in every minute
        eMinutesMatch,                 //repeat in every hour
        eMinutesHoursMatch,            //repeat in every day
        eMinutesHoursDateMatch,        //repeat in every month
        eMinutesHoursDayMatch,         //repeat in every week
        //Alarm2
        eUnknownAlarm
    }eAlarmTypes;

    /**
     * @brief Constructor 
     * @param Input Wire address
     */
    DFRobot_DS323X(TwoWire *pWire = &Wire){_pWire = pWire;};
    ~DFRobot_DS323X();
    /*!
     *@brief Init chip 
     *@return True means IIC communication succeeds, false means it fails.
     */
    bool begin(void);
    /*!
     *@brief Get year
     *@return Year
     */
    uint16_t getYear();
    /*!
     *@brief Get month
     *@return Month
     */
    uint8_t  getMonth();
    /*!
     *@brief Get date
     *@return Date
     */
    uint8_t  getDate(); 
    /*!
     *@brief Get hour
     *@return Hour
     */
    uint8_t  getHour();
    /*!
     *@brief Get minute
     *@return Minute
     */
    uint8_t  getMinute();
    /*!
     *@brief Get second
     *@return Second
     */
    uint8_t  getSecond();
    
    /*!
     *@brief Set mode
     *@param ehours:e24hours, eAM, ePM. default is e24hours
     */
    void setMode(ehours mode = e24hours)  { _mode = mode; }
    
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
     *@brief get day of week
     *@return day of week
     */
    const char* getDayOfWeek();
    
    /*!
     *@brief Get current temperature, 
     *@return temperature, unit:℃
     */
    float getTemperatureC();
    
    /*!
     *@brief Judge if it is power-down 
     *@return If retrun true, power down, time needs to reset; false, work well. 
     */
    bool isLostPower(void);
    
    /*!
     *@brief Read the value of pin sqw
     *@return Explanation of the readings in enumeration variable eDS323XSqwPinMode_t
     */
    eSqwPinMode_t readSqwPinMode();
    
    /*!
     *@brief Set the vaule of pin sqw
     *@param dt Explanation of the witten value in enumeration variable eDS323XSqwPinMode_t
     */
    void writeSqwPinMode(eSqwPinMode_t mode);
    
    /*!
     *@brief Set the last compiled time as the current time
     *@param comSec 补偿时间，由第一次上传后串口打印的时间减去PC系统时间所得的值，单位：秒
     */
    void getCompileTime(uint8_t comSec = 0);
    
    /*!
     *@brief Set alarm clock 
     *@param alarmType Alarm working mode
     *@param days    Alarm clock (day)
     *@param hours   Alarm clock (hour)
     *@param minutes Alarm clock (minute)
     *@param seconds Alarm clock (second)
     */
    void setAlarm(eAlarmTypes alarmType,int16_t days,int8_t hours,int8_t minutes,int8_t seconds);
    
    /*!
     *@brief enable or disable the interrupt of alarm 
     */
    void enableAlarm1Int();
    void disableAlarm1Int();
    void enableAlarm2Int();
    void disableAlarm2Int();
    
    /*!
     *@brief output AM or PM of time 
     *@return AM or PM, 24 hours mode return null
     */
    const char* getAMorPM();
    
    /*!
     *@brief Judge if the alarm clock is triggered
     *@return true, triggered; false, not trigger
     */
    uint8_t isAlarmTrig();
    
    /*!
     *@brief Clear alarm flag 
     */
    void clearAlarm();
    
    /*!
     *@brief enable the 32k output 
     */
    void enable32k();
    
    /*!
     *@brief disable the 32k output 
     */
    void disable32k();
    
    /*!
     *@brief write, read and clear the SRAM
     *@param reg 0x14~0xFF
     *@param data uint8_t HEX
     */
    void writeSRAM(uint8_t reg, uint8_t data);
    uint8_t readSRAM(uint8_t reg);
    void clearSRAM(uint8_t reg);
    

protected:
    virtual void writeReg(uint8_t reg, const void* pBuf, size_t size);
    virtual uint8_t readReg(uint8_t reg, const void* pBuf, size_t size);
    
    static uint8_t bcd2bin(uint8_t val);
    static uint8_t bin2bcd(uint8_t val);

    uint8_t y,    ///< Year Offset
            m,    ///< Months
            d,    ///< Days
            hh,   ///< Hours
            mm,   ///< Minutes
            ss;   ///< Seconds

private:
    TwoWire *_pWire;
    uint8_t _deviceAddr = DS323X_IIC_ADDRESS;
    uint8_t rtc_bcd[7];
    uint8_t bcd[7];
    uint8_t  dayOfTheWeek() const ;
    ehours _mode;
    uint8_t  _ss,_mm,_hh,_d,_m;
    uint16_t _y;
    const char* daysOfTheWeek[7]PROGMEM = {"Sun", "Mon", "Tues", "Wed", "Thur", "Fri", "Sat"}; 
    const char* hourOfAM[4]PROGMEM = {"", "", "AM", "PM"}; 
};

#endif
