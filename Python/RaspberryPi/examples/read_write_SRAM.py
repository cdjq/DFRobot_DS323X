#-*- coding: utf-8 -*-
'''
@file get_time_and_temp.py

@brief Through the example, you can read and write data on SRAM:
@n     Experiment phenomenon: read data every 1 second and print it on terminal .

@Copyright   Copyright (c) 2010 DFRobot Co.Ltd (http://www.dfrobot.com)
@licence   The MIT License (MIT)

@author [LuoYufeng](yufeng.luo@dfrobot.com)
@url https://github.com/DFRobot/DFRobot_DS323X
@version  V1.0
@date  2021-3-4
'''
import sys
sys.path.append('../')
import time
from DFRobot_DS323X import *

rtc = DFRobot_DS323X(bus=1)

#begin return True if succeed, otherwise return False
while not rtc.begin():
    time.sleep(2)
i = 0

data1 = [22,1,15,78,65,49,56,49,25,47,89]

'''
@brief clear the SRAM
'''
for reg in range(0x14,0x1F):
    rtc.clear_SRAM(reg)

'''
@brief write data into the SRAM
@param reg, address of SRAM 0x14~0xFF
@param data
'''
for reg in range(0x14,0x1F):
    rtc.write_SRAM(reg, data1[i])
    i += 1

'''
@brief read data of the SRAM
@param reg, address of SRAM 0x14~0xFF
@return data stored in SRAM
'''
def main():
    while True:
        for reg in range(0x14,0x1F):
            print(rtc.read_SRAM(reg))
            time.sleep(0.1)

if __name__ == "__main__":
    main()

