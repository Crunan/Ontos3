import asyncio
import json
import logging
import websockets
import serial_asyncio

async def readResponse(r) -> str:
    rsp = await r.readuntil(b'#')
    print(rsp)
    return rsp
    
async def writeCommand(w, cmd):
    w.write(cmd.encode('ascii'))
    await w.drain()

async def serial_handler(websocket):
    reader, writer = await serial_asyncio.open_serial_connection(url='/dev/ttyUSB0', baudrate=57600) 
    async for message in websocket:
        print(message)
        if message == "poll":
            RCV = await poll(writer, reader)
            await websocket.send(RCV)
        elif message == "startup":
            await startup(writer, reader)
            await websocket.send("startup complete")
        #elif
        #else:
        #    await writeCommand(writer, message)
        #    RCV = await readResponse(reader)
   
async def poll(w, r) -> str:
    await writeCommand(w, "$91%") 
    pollResponse = await readResponse(r)
    return pollResponse
    
async def startup(w, r) -> complex:
    await howManyMFCs(w, r)
    await BatchIDLogging(w, r)
    await RecipeMBStartPos(w, r)
    await RecipeRFPower(w, r)
    await RecipeMFC1Flow(w, r)
    await RecipeMFC2Flow(w, r)
    await RecipeMFC3Flow(w, r)
    await RecipeMFC4Flow(w, r)
    await MFC1Range(w, r)
    await MFC2Range(w, r)
    await MFC3Range(w, r)
    await MFC4Range(w, r)
    await RFMaxPower(w, r)
    await TunerAutoMode(w, r)
    await PlasmaOn(w, r)
     
        #task4 = startupTasks.create_task(RecipeRFPower(w, r))
        #task5 = startupTasks.create_task(RecipeMFC1Flow(w, r))
        #task6 = startupTasks.create_task(RecipeMFC2Flow(w, r))
        #task7 = startupTasks.create_task(RecipeMFC3Flow(w, r))
        #task8 = startupTasks.create_task(RecipeMFC4Flow(w, r))
        #task9 = startupTasks.create_task(MFC1Range(w, r)) 
        #task10 = startupTasks.create_task(MFC2Range(w, r)) 
        #task11 = startupTasks.create_task(MFC3Range(w, r)) 
        #task12 = startupTasks.create_task(MFC4Range(w, r)) 
        #task13 = startupTasks.create_task(RFMaxPower(w, r))
        #task14 = startupTasks.create_task(TunerAutoMode(w, r))
        #task15 = startupTasks.create_task(PlasmaOn(w, r))

async def howManyMFCs(w, r) -> int:
    await writeCommand(w, "$2A002%") 
    val = await readResponse(r)
    return val
async def BatchIDLogging(w, r) -> int:
    await writeCommand(w, "$2A011%") 
    val = await readResponse(r)
    return val 
async def RecipeMBStartPos(w, r) -> float:
    await writeCommand(w, "$2A606%") 
    val = await readResponse(r)
    return val  
async def RecipeRFPower(w, r) -> int:
    await writeCommand(w, "$2A605%") 
    val = await readResponse(r)
    return val 
async def RecipeMFC4Flow(w, r) -> float:
    await writeCommand(w, "$2A604%") 
    val = await readResponse(r)
    return val  
async def RecipeMFC3Flow(w, r) -> float:
    await writeCommand(w, "$2A603%") 
    val = await readResponse(r)
    return val   
async def RecipeMFC2Flow(w, r) -> float:
    await writeCommand(w, "$2A602%") 
    val = await readResponse(r)
    return val 
async def RecipeMFC1Flow(w, r) -> float:
    await writeCommand(w, "$2A601%") 
    val = await readResponse(r)
    return val 
async def MFC4Range(w, r) -> float:
    await writeCommand(w, "$8504%") 
    val = await readResponse(r)
    return val  
async def MFC3Range(w, r) -> float:
    await writeCommand(w, "$8503%") 
    val = await readResponse(r)
    return val  
async def MFC2Range(w, r) -> float:
    await writeCommand(w, "$8502%") 
    val = await readResponse(r)
    return val  
async def MFC1Range(w, r) -> float:
    await writeCommand(w, "$8501%") 
    val = await readResponse(r)
    return val  
async def RFMaxPower(w, r) -> int:
    await writeCommand(w, "$2A705%") 
    val = await readResponse(r)
    return val  
async def TunerAutoMode(w, r) -> int:
    await writeCommand(w, "$89%") 
    val = await readResponse(r)
    return val  
async def PlasmaOn(w, r) -> int:
    await writeCommand(w, "$8700%") 
    val = await readResponse(r)
    return val  
#async def producer_handler(websocket):
    #while True:
        #await websocket.send()

#async def handler(websocket):
    # handler is called the moment the server receives a connection and opens, 
    # this is where I want to try and open a context manager 
    # for the PCB serial connection and the interface handlers.
#    async with asyncio.TaskGroup() as handlers:
#        consumer_task = handlers.create_task(consumer_handler(websocket))
#        producer_task = handlers.create_task(producer_handler(websocket))

async def main():
    # context manager for the server 
    async with websockets.serve(serial_handler, "localhost", 5000):
        await asyncio.Future()  # run forever

# asyncio.run() function begins our event loop by calling a coroutine function (namely main()) 
if __name__ == "__main__":
    asyncio.run(main()) 
