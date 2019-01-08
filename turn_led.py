import serial
import sys
import time


ser = serial.Serial('/dev/ttyACM1',9600)
time.sleep(2)
x = sys.argv[1]
ser.write(b'1')

if x == '1':
  ser.write(b'1')
  print('AAAAAA1')
if x == '2':
  ser.write(b'2')
  print('AAAAAA2')
if x== '3':
  ser.write(b'3')
  print('AAAAAA3')
ser.close()


