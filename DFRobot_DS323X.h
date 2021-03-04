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

#if ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif
#include <Wire.h>


/*I2C ADDRESS*/
#define DS323X_IIC_ADDRESS          0x68

#define SECONDS_FROM_1970_TO_2000    946684800
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

typedef enum{
    eDS323X_OFF             = 0x01, // Not output square wave, enter interrupt mode 
    eDS323X_SquareWave_1Hz  = 0x00, // 1Hz square wave
    eDS323X_SquareWave_1kHz = 0x08, // 1kHz square wave
    eDS323X_SquareWave_4kHz = 0x10, // 4kHz square wave
    eDS323X_SquareWave_8kHz = 0x18  // 8kHz square wave
}eDs3231MSqwPinMode_t;


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

class DFRobot_DS323X
{
public:
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
     *@brief Get current time data
     */
    void getNowTime();
    /*!
     *@brief Get year
     *@return Year
     */
    uint16_t year()         const { return _y; }
    /*!
     *@brief Get month
     *@return Month
     */
    uint8_t  month()        const { return _m; }
    /*!
     *@brief Get day
     *@return Day
     */
    uint8_t  day()          const { return _d; }
    /*!
     *@brief Get hour
     *@return Hour
     */
    uint8_t  hour()         const { return _hh; }
    /*!
     *@brief Get minute
     *@return Minute
     */
    uint8_t  minute()       const { return _mm; }
    /*!
     *@brief Get second
     *@return Second
     */
    uint8_t  second()       const { return _ss; }
    
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
    const char* getDayOfTheWeek();
    
    /*!
     *@brief Adjust current time 
     */
    float getTemperatureC();
    
    /*!
     *@brief Judge if it is power-down 
     *@return If retrun true, power down, time needs to reset; false, work well. 
     */
    bool lostPower(void);
    
    /*!
     *@brief Read the value of pin sqw
     *@return Explanation of the readings in enumeration variable eDs3231MSqwPinMode_t
     */
    eDs3231MSqwPinMode_t readSqwPinMode();
    
    /*!
     *@brief Set the vaule of pin sqw
     *@param dt Explanation of the witten value in enumeration variable eDs3231MSqwPinMode_t
     */
    void writeSqwPinMode(eDs3231MSqwPinMode_t mode);
    
    /*!
     *@brief Set the last compiled time as the current time
     *@param comSec  default is 0
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
    void setAlarm(eAlarmTypes alarmType,int16_t days,int8_t hours,int8_t minutes,int8_t seconds, const bool state  = true);
    
    /*!
     *@brief enable or disable the interrupt of alarm 
     */
    void enAbleAlarm1Int();
    void disAbleAlarm1Int();
    void enAbleAlarm2Int();
    void disAbleAlarm2Int();
    
    /*!
     *@brief output AM or PM of time 
     */
    const char* getAMorPM();
    
    /*!
     *@brief Judge if the alarm clock is triggered
     *@return true, triggered; false, not trigger
     */
    uint8_t isAlarm();
    
    /*!
     *@brief Clear trigger 
     */
    void clearAlarm();
    
    /*!
     *@brief enable the 32k output 
     */
    void enAble32k();
    
    /*!
     *@brief disable the 32k output 
     */
    void disAble32k();
    
    /*!
     *@brief write, read and clear the SRAM
     *@param reg 0x14~0xFF
     *@param data uint8_t HEX
     */
    void writeSRAM(uint8_t reg, uint8_t data);
    uint8_t readSRAM(uint8_t reg);
    void clearSRAM(uint8_t reg);
    
    
    
    uint8_t rtc[7];
    /*!
     *@brief Write init time 
     *@param date Write init date 
     *@param time Write init time 
     */
    
    uint8_t  dayOfTheWeek() const ;

protected:
    virtual void writeReg(uint8_t reg, const void* pBuf, size_t size);
    virtual uint8_t readReg(uint8_t reg, const void* pBuf, size_t size);
    
    /*!
     *@brief BCD code to BIN code
     *@param val Input BCD code
     *@return Return BIN code
     */
    static uint8_t bcd2bin(uint8_t val);
    /*!
     *@brief BIN code to BCD code
     *@param val Input BIN code
     *@return Return BCD code
     */
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
    ehours _mode;
    uint8_t  _ss,_mm,_hh,_d,_m;
    uint16_t _y;
    const char* daysOfTheWeek[7] = {"Sun", "Mon", "Tues", "Wed", "Thur", "Fri", "Sat"}; 
    const char* hourOfAM[4] = {"", "", "AM", "PM"}; 
};

#endif
