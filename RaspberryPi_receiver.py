import serial
from time import sleep
#DEFINING SERIAL PARAMS
COM = "/dev/ttyUSB0"
BAUD = 9600
ser = serial.Serial(COM, BAUD, timeout = .1)
print("wait")
print(ser.name) #debug
while True:
    val = ser.readline().strip()
    if len(val) >0 :
        print val
