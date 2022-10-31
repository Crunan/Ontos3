import serial 

def WriteEncodedCommand(CMD_Str: str):
  SerialPort1.write(CMD_Str.encode('ascii'))
      
def ReadResponseThenDecode() -> str:
  RCV: bytes  =  SerialPort1.read_until(b'#', None)
  return RCV.decode()

SerialPort1 = serial.Serial("/dev/ttyUSB0", 57600)
WriteEncodedCommand'$90%')
print(ReadResponseThenDecode())
  
