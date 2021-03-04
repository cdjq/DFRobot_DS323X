/*!
 * @file DFRobot_DS323X.cpp
 * @brief Define the basic structure of class DFRobot_DS323X
 * @copyright	Copyright (c) 2010 DFRobot Co.Ltd (http://www.dfrobot.com)
 * @licence     The MIT License (MIT)
 * @author [yufeng](yufeng.luo@dfrobot.com)
 * @version  V1.0
 * @date  2021-2-23
 * @url https://github.com/DFRobot/DFRobot_DS323X
 */

#include <DFRobot_DS323X.h>
const uint8_t daysInMonth [] PROGMEM={31,28,31,30,31,30,31,31,30,31,30,31};

static uint16_t date2days(uint16_t y, uint8_t m, uint8_t d) {
    if (y >= 2000)
        y -= 2000;                              // Remove year offset
    uint16_t days = d;                          // Store numbers of days
    for (uint8_t i = 1; i < m; ++i){
        days += pgm_read_byte(daysInMonth + i - 1); // Add number of days for each month
    } if (m > 2 && y % 4 == 0)
        ++days;                                 // Deal with leap years
    return days + 365 * y + (y + 3) / 4 - 1;    // Return computed value
}

static uint8_t conv2d(const char* p) {
    uint8_t v = 0;
    if ('0' <= *p && *p <= '9')
        v = *p - '0';
    return 10 * v + *++p - '0';
}

DFRobot_DS323X::~DFRobot_DS323X() {}

bool DFRobot_DS323X::begin(void)
{
    Wire.begin();
    delay(100);
    Wire.beginTransmission(_deviceAddr);
    if(Wire.endTransmission() == 0)
        return true;
    else
        return false;
}

uint8_t DFRobot_DS323X::bcd2bin(uint8_t val){
    return val - 6 * (val >> 4);
}

uint8_t DFRobot_DS323X::bin2bcd (uint8_t val){
    return val + 6 * (val / 10);
}

eDs3231MSqwPinMode_t DFRobot_DS323X::readSqwPinMode(){
    int mode[1];
    readReg(DS323X_REG_CONTROL, mode, 1);
    mode[0] &= 0x93;
    return static_cast<eDs3231MSqwPinMode_t>(mode[0]);
}

void DFRobot_DS323X::writeSqwPinMode(eDs3231MSqwPinMode_t mode){
    uint8_t ctrl[1];
    readReg(DS323X_REG_CONTROL, ctrl, 1);
    ctrl[0] &= ~0x04;
    ctrl[0] &= ~0x18;
    if (mode == eDS323X_OFF) 
        ctrl[0] |= 0x04;
    else
        ctrl[0] |= mode;
    writeReg(DS323X_REG_CONTROL, ctrl, 1);
}

void DFRobot_DS323X::getCompileTime (uint8_t comTime){
    char buff[11];
    uint8_t min = 0;
    memcpy_P(buff, F(__DATE__), 11);
    y = conv2d(buff + 9);
    // Jan Feb Mar Apr May Jun Jul Aug Sep Oct Nov Dec
    switch (buff[0]) {
        case 'J': m = (buff[1] == 'a') ? 1 : ((buff[2] == 'n') ? 6 : 7); break;
        case 'F': m = 2; break;
        case 'A': m = buff[2] == 'r' ? 4 : 8; break;
        case 'M': m = buff[2] == 'r' ? 3 : 5; break;
        case 'S': m = 9; break;
        case 'O': m = 10; break;
        case 'N': m = 11; break;
        case 'D': m = 12; break;
    }
    m += 80;
    d = conv2d(buff + 4);
    memcpy_P(buff, F(__TIME__), 8);
    hh = bin2bcd(conv2d(buff));
    ss = conv2d(buff + 6) + comTime;
    while (ss >60){
        ss -= 60;
        min += 1;
    }
    mm = conv2d(buff + 3) + min;
    uint8_t buffer[] = {bin2bcd(ss),bin2bcd(mm),hh,dayOfTheWeek(),bin2bcd(d),bin2bcd(m),bin2bcd(y)};
    writeReg(DS323X_REG_RTC_SEC, buffer, 7);
    uint8_t statreg[1];
    readReg(DS323X_REG_STATUS, statreg, 1);
    statreg[0] &= ~0x80; // flip OSF bit
    writeReg(DS323X_REG_STATUS, statreg, 1);
}

uint8_t DFRobot_DS323X::dayOfTheWeek() const {
  uint16_t day = date2days(_y, _m, _d); // compute the number of days
  return (day + 6) % 7;                 // Jan 1, 2000 is a Saturday
} 

const char* DFRobot_DS323X::getDayOfTheWeek(){
    return daysOfTheWeek[dayOfTheWeek()];
}

void DFRobot_DS323X::setTime(uint16_t year, uint8_t month, uint8_t date, uint8_t hour, uint8_t minute, uint8_t second){
    if (year >=2000){
        y = year - 2000;
        m = month + 80;
    }
    else{
        y = year - 1900;
        m = month;
    }
    d = date;
    hh = (_mode << 5|bin2bcd(hour));
    mm = minute;
    ss = second;
    uint8_t buffer[] = {bin2bcd(ss),bin2bcd(mm),hh,dayOfTheWeek(),bin2bcd(d),bin2bcd(m),bin2bcd(y)};
    writeReg(DS323X_REG_RTC_SEC, buffer, 7);
    uint8_t statreg[1];
    readReg(DS323X_REG_STATUS, statreg, 1);
    statreg[0] &= ~0x80; // flip OSF bit
    writeReg(DS323X_REG_STATUS, statreg, 1);
}

const char* DFRobot_DS323X::getAMorPM(){
    uint8_t buffer[1];
    readReg(DS323X_REG_RTC_HOUR, buffer, 1);
    buffer[0] = buffer[0] << 1;
    buffer[0] = buffer[0] >> 6;
    return hourOfAM[buffer[0]];
}

void DFRobot_DS323X::getNowTime(){
    readReg(DS323X_REG_RTC_SEC, bcd, 7);
    _ss = bcd2bin(bcd[0] & 0x7F);
    _mm = bcd2bin(bcd[1]);
    if(bcd[2]&0x40){
        bcd[2] = bcd[2] << 3;
        _hh = bcd2bin(bcd[2] >> 3);
    }
    else{
        bcd[2] = bcd[2] << 2;
        _hh = bcd2bin(bcd[2] >> 2);
    }
    _d = bcd2bin(bcd[4]);
    _m = bcd2bin(bcd[5]);
    _y = bcd2bin(bcd[6]) + 1900;
    if(bcd[5] > 80){
        _y += 100;
        _m -= 80;
    }
}

float DFRobot_DS323X::getTemperatureC(){
    uint8_t buf[2];
    readReg(DS323X_REG_TEMPERATURE, buf, 2);
    return ((float)buf[0] + (buf[1]>>6)*0.25f);
}

bool DFRobot_DS323X::lostPower(void) {
    uint8_t status[1];
    readReg(DS323X_REG_STATUS, status, 1);
    return status[0] >> 7;
}

void DFRobot_DS323X::setAlarm(eAlarmTypes alarmType, int16_t date,int8_t hour,
                               int8_t minute,int8_t second, const bool state ){
    int16_t dates[] = {bin2bcd(date)};
    int8_t hours[] = {_mode << 5|bin2bcd(hour)};
    int8_t minutes[] = {bin2bcd(minute)};
    int8_t seconds[] = {bin2bcd(second)};
    uint8_t days[] = {bin2bcd(dayOfTheWeek())};
    uint8_t buffer[1];
    if (alarmType >= eUnknownAlarm)
        return;
    if (alarmType < eEveryMinute){
        writeReg(DS323X_REG_ALM1_SEC, seconds, 1);
        writeReg(DS323X_REG_ALM1_MIN, minutes, 1);
        writeReg(DS323X_REG_ALM1_HOUR, hours, 1);
        if (alarmType == eSecondsMinutesHoursDateMatch)
            writeReg(DS323X_REG_ALM1_DAY, dates, 1);
        else
            writeReg(DS323X_REG_ALM1_DAY, days, 1);
        if(alarmType<eSecondsMinutesHoursDateMatch){
            readReg(DS323X_REG_ALM1_DAY, buffer, 1);
            buffer[0] |= 0x80;
            writeReg(DS323X_REG_ALM1_DAY, buffer, 1);
        }
        if(alarmType<eSecondsMinutesHoursMatch){
            readReg(DS323X_REG_ALM1_HOUR, buffer, 1);
            buffer[0] |= 0x80;
            writeReg(DS323X_REG_ALM1_HOUR, buffer, 1);
        }
        if(alarmType<eSecondsMinutesMatch){
            readReg(DS323X_REG_ALM1_MIN, buffer, 1);
            buffer[0] |= 0x80;
            writeReg(DS323X_REG_ALM1_MIN, buffer, 1);
        }
        if(alarmType==eEverySecond){
            readReg(DS323X_REG_ALM1_SEC, buffer, 1);
            buffer[0] |= 0x80;
            writeReg(DS323X_REG_ALM1_SEC, buffer, 1);
        }
        if(alarmType==eSecondsMinutesHoursDayMatch){
            readReg(DS323X_REG_ALM1_DAY, buffer, 1);
            buffer[0] |= 0x40;
            writeReg(DS323X_REG_ALM1_DAY, buffer, 1);
        }
        if (state){
            readReg(DS323X_REG_CONTROL, buffer, 1);
            buffer[0] |= 1;
            writeReg(DS323X_REG_CONTROL, buffer, 1);
        }
        else{
            readReg(DS323X_REG_CONTROL, buffer, 1);
            buffer[0] &= 0xFE;
            writeReg(DS323X_REG_CONTROL, buffer, 1);
        }
    }
    else{
        writeReg(DS323X_REG_ALM2_MIN, minutes, 1);
        writeReg(DS323X_REG_ALM2_HOUR, hours, 1);
        if(alarmType == eMinutesHoursDateMatch)
            writeReg(DS323X_REG_ALM2_DAY, dates, 1);
        else if (alarmType == eMinutesHoursDayMatch){
            days[0] |= 0x80;
            writeReg(DS323X_REG_ALM2_DAY, days, 1);
        }
        if(alarmType < eMinutesHoursDateMatch){
            readReg(DS323X_REG_ALM2_DAY, buffer, 1);
            buffer[0] |= 0x80;
            writeReg(DS323X_REG_ALM2_DAY, buffer, 1);
        }
        if(alarmType < eMinutesHoursMatch){
            readReg(DS323X_REG_ALM2_HOUR, buffer, 1);
            buffer[0] |= 0x80;
            writeReg(DS323X_REG_ALM2_HOUR, buffer, 1);
        }
        if(alarmType == eEveryMinute){
            readReg(DS323X_REG_ALM2_MIN, buffer, 1);
            buffer[0] |= 0x80;
            writeReg(DS323X_REG_ALM2_MIN, buffer, 1);
        }
        if (state){
            readReg(DS323X_REG_CONTROL, buffer, 1);
            buffer[0] |= 2;
            writeReg(DS323X_REG_CONTROL, buffer, 1);
        }
        else{
            readReg(DS323X_REG_CONTROL, buffer, 1);
            buffer[0] &= 0xFD;
            writeReg(DS323X_REG_CONTROL, buffer, 1);
        }
    } // of if-then-else use alarm 1 or 2
    clearAlarm(); // Clear the alarm state
    return;
}

void DFRobot_DS323X::enAbleAlarm1Int(){
    uint8_t crtl[1];
    readReg(DS323X_REG_CONTROL, crtl, 1);
    crtl[0] |= 0x01;
    writeReg(DS323X_REG_CONTROL, crtl, 1);
}

void DFRobot_DS323X::disAbleAlarm1Int(){
    uint8_t crtl[1];
    readReg(DS323X_REG_CONTROL, crtl, 1);
    crtl[0] &= 0xFE;
    writeReg(DS323X_REG_CONTROL, crtl, 1);
}

void DFRobot_DS323X::enAbleAlarm2Int(){
    uint8_t crtl[1];
    readReg(DS323X_REG_CONTROL, crtl, 1);
    crtl[0] |= 0x02;
    writeReg(DS323X_REG_CONTROL, crtl, 1);
}

void DFRobot_DS323X::disAbleAlarm2Int(){
    uint8_t crtl[1];
    readReg(DS323X_REG_CONTROL, crtl, 1);
    crtl[0] &= 0xFD;
    writeReg(DS323X_REG_CONTROL, crtl, 1);
}

uint8_t DFRobot_DS323X::isAlarm() {
    uint8_t status[1];
    readReg(DS323X_REG_STATUS, status, 1);
    return status[0]&3; // Alarm if either of 2 LSBits set
}

void DFRobot_DS323X::clearAlarm(){
    uint8_t status[1];
    readReg(DS323X_REG_STATUS, status, 1);
    status[0] &= 0xFC;
    writeReg(DS323X_REG_STATUS, status, 1);
} 

void DFRobot_DS323X::enAble32k(){
    uint8_t status[1];
    readReg(DS323X_REG_STATUS, status, 1);
    status[0] |= 0x08;
    writeReg(DS323X_REG_STATUS, status, 1);
}

void DFRobot_DS323X::disAble32k(){
    uint8_t status[1];
    readReg(DS323X_REG_STATUS, status, 1);
    status[0] &= 0xF7;
    writeReg(DS323X_REG_STATUS, status, 1);
}


void DFRobot_DS323X::writeSRAM(uint8_t reg, uint8_t data){
    uint8_t buf[1] = {data};
    writeReg(reg, buf, 1);
}

uint8_t DFRobot_DS323X::readSRAM(uint8_t reg){
    uint8_t buf[1];
    readReg(reg, buf, 1);
    return buf[0];
}

void DFRobot_DS323X::clearSRAM(uint8_t reg){
    uint8_t buf[1] = {0x00};
    writeReg(reg, buf, 1);
}

void DFRobot_DS323X::writeReg(uint8_t reg, const void* pBuf, size_t size)
{
    if(pBuf == NULL){
        DBG("pBuf ERROR!! : null pointer");
    }
    uint8_t * _pBuf = (uint8_t *)pBuf;
    _pWire->beginTransmission(_deviceAddr);
    _pWire->write(&reg, 1);
    
    for(uint16_t i = 0; i < size; i++){
        _pWire->write(_pBuf[i]);
    }
    _pWire->endTransmission();
}

uint8_t DFRobot_DS323X::readReg(uint8_t reg, const void* pBuf, size_t size)
{
    if(pBuf == NULL){
        DBG("pBuf ERROR!! : null pointer");
    }
    uint8_t * _pBuf = (uint8_t *)pBuf;
    _pWire->beginTransmission(_deviceAddr);
    _pWire->write(&reg, 1);
    
    if( _pWire->endTransmission() != 0){
        return 0;
    }

    _pWire->requestFrom(_deviceAddr, (uint8_t) size);
    for(uint16_t i = 0; i < size; i++){
        _pBuf[i] = _pWire->read();
    }
    return size;
}
