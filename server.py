import asyncio
import websockets
from serialPCB import *

async def handler(websocket):
    # handler is called the moment the server receives a connection and opens, 
    # this is where I want to try and open a context manager 
    # for the PCB serial connection.
    async with serial.Serial(PORT, BAUD) as ser:
        # this is a websocket shortcut to iterate over every
        # message reveived over the connection.
        async for message in websocket:
        
         WriteCommand(ser, message)
         print(ReadResponse(ser))

        
async def main():
    # context manager for the server 
    async with websockets.serve(handler, "localhost", 5000):
        await asyncio.Future()  # run forever
    
if __name__ == "__main__":
    asyncio.run(main())
