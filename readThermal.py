import serial
import numpy as np
from statistics import mean

#DEFINE PORT HERE
port = 'COM10'
ard = serial.Serial(port,9600,timeout=5)

while True:

    # Serial read section
    msg = ard.readline()
    print ("Message from arduino: ")
    msgTrueString = msg.decode('utf-8')
    arrTemp = msgTrueString.split(',')
    sumTemp = 0
    for x in arrTemp:
        sumTemp = sumTemp + float(x)
    
    averageTemp = sumTemp/4
    #CALIBRATE HEAR
    averageTemp = averageTemp+5.5
    print(averageTemp)
    