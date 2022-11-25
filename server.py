import asyncio
import json
import logging
import websockets
import serial_asyncio

async def readResponse(r) -> str:
    rsp = await r.readuntil(b'#')
    print(rsp.decode())
    return rsp.decode()
    
async def writeCommand(w, cmd):
    w.write(cmd.encode('ascii'))
    await w.drain()

async def serial_handler(websocket):
    reader, writer = await serial_asyncio.open_serial_connection(url='/dev/ttyUSB0', baudrate=57600) 
    async for message in websocket:
        print(message)
        match message: 
            case "poll":
                rcv = await poll(writer, reader)
                websocketReadyData = organizeRCVData(rcv)
                await websocket.send(json.dumps(websocketReadyData))
            case "startup":
                await startup(writer, reader)
                #send JSON parsed stuff await websocket.send("startup complete")
            case "plasmaOn":
                rcv = await plasmaOn(writer, reader)
                await websocket.send(rcv)
  
def organizeRCVData(d):
    howManyValuesInPoll = d.count(';')
    strVar = d[3:] #Lop off the first three characters
    strVar = strVar.rstrip('#')
    dataParsed = strVar.split(';') 
    PCBPollData: dict
    PCBPollData = {
        "CTLPCBStatus": [
            {"statusBits": dataParsed[0] }
        ],
        "MBTuner": [
            {"actualPosition": dataParsed[1] }
        ],
        "RFSupply": [
            {"PowerForward": dataParsed[2] },
            {"PowerReflected": dataParsed[3] }
        ],
        "PlasmaHead": [
            {"PlasmaStatus": dataParsed[4] }
        ],
        "MFC": [
            {"ActualFlow": dataParsed[5] },
            {"ActualFlow": dataParsed[6] },
            {"ActualFlow": dataParsed[7] },
            {"ActualFlow": dataParsed[8] }
        ],
   }
    return PCBPollData

async def plasmaOn(w, r):
    await writeCommand(w, "$8701%")
    return await readResponse(r)
async def plasmaOff(w, r):
    await writeCommand(w, "$8700%")
    return await readResponse(r)

async def poll(w, r) -> str:
    await writeCommand(w, "$91%") 
    return await readResponse(r)
    
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
     
async def howManyMFCs(w, r) -> int:
    await writeCommand(w, "$2A002%") 
    return await readResponse(r)
async def BatchIDLogging(w, r) -> int:
    await writeCommand(w, "$2A011%") 
    return await readResponse(r)
async def RecipeMBStartPos(w, r) -> float:
    await writeCommand(w, "$2A606%") 
    return await readResponse(r)
async def RecipeRFPower(w, r) -> int:
    await writeCommand(w, "$2A605%") 
    return await readResponse(r)
async def RecipeMFC4Flow(w, r) -> float:
    await writeCommand(w, "$2A604%") 
    return await readResponse(r)
async def RecipeMFC3Flow(w, r) -> float:
    await writeCommand(w, "$2A603%") 
    return await readResponse(r)
async def RecipeMFC2Flow(w, r) -> float:
    await writeCommand(w, "$2A602%") 
    return await readResponse(r)
async def RecipeMFC1Flow(w, r) -> float:
    await writeCommand(w, "$2A601%") 
    return await readResponse(r)
async def MFC4Range(w, r) -> float:
    await writeCommand(w, "$8504%") 
    return await readResponse(r)
async def MFC3Range(w, r) -> float:
    await writeCommand(w, "$8503%") 
    return await readResponse(r)
async def MFC2Range(w, r) -> float:
    await writeCommand(w, "$8502%") 
    return await readResponse(r)
async def MFC1Range(w, r) -> float:
    await writeCommand(w, "$8501%") 
    return await readResponse(r)
async def RFMaxPower(w, r) -> int:
    await writeCommand(w, "$2A705%") 
    return await readResponse(r)
async def TunerAutoMode(w, r) -> int:
    await writeCommand(w, "$89%") 
    return await readResponse(r)
async def PlasmaOn(w, r) -> int:
    await writeCommand(w, "$8700%") 
    return await readResponse(r)

async def main():
    # context manager for the server 
    async with websockets.serve(serial_handler, "localhost", 5000):
        await asyncio.Future()  # run forever

# asyncio.run() function begins our event loop by calling a coroutine function (namely main()) 
if __name__ == "__main__":
    asyncio.run(main()) 
