import asyncio
import serial_asyncio

class Writer(asyncio.Protocol):
    def connection_made(self, transport):
        """Store the serial transport and schedule the task
        to send data.
        """
        self.transport = transport
        print("Writer connection created")
        asnycio.ensure_future(self.send())
        print('Writer.send() scheduled')
        def WriteCommand(serialPort, CMD_Str: str):
  serialPort.write(CMD_Str.encode('ascii'))
      
def ReadResponse(serialPortn) -> str:
  RCV: bytes  =  serialPort.read_until(b'#', None)
  return RCV.decode()





