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
                websocketReadyData = organizePollData(rcv)
                await websocket.send(json.dumps(websocketReadyData))
            case "startup":
                startupDict = await startup(writer, reader)
                await websocket.send(json.dumps(startupDict))
                #send JSON parsed stuff await websocket.send("startup complete")
            case "plasmaOn":
                rcv = await plasmaOn(writer, reader)
                await websocket.send(rcv)
  
def organizePollData(d):
    howManyValuesInPoll = d.count(';')
    strVar = d[3:] #Lop off the first three characters
    strVar = strVar.rstrip('#')
    dataParsed = strVar.split(';') 
    PCBPollData: dict
    PCBPollData = {
        "CTLPCBStatus": {
            "statusBits": int(dataParsed[0], 16)
        },
        "MBTuner": {
            "actualPosition": float(dataParsed[1])
        },
        "RFSupply": {
            "powerForward": int(dataParsed[2], 10),
            "powerReflected": int(dataParsed[3], 10) 
        },
        "PlasmaHead": {
            "plasmaStatus": int(dataParsed[4], 16) 
        },
        "MFC4": {
            "actualFlow": float(dataParsed[5])
        },
        "MFC3": {
            "actualFlow": float(dataParsed[6])
        },
        "MFC2": {
            "actualFlow": float(dataParsed[7])
        },
        "MFC1": {
            "actualFlow": float(dataParsed[8])
        }
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
    
async def startup(w, r) -> dict:
    a = await howManyMFCs(w, r)
    b = await BatchIDLogging(w, r)
    c = await RecipeMBStartPos(w, r)
    d = await RecipeRFPower(w, r)
    e = await RecipeMFC1Flow(w, r)
    f = await RecipeMFC2Flow(w, r)
    g = await RecipeMFC3Flow(w, r)
    h = await RecipeMFC4Flow(w, r)
    i = await MFC4Range(w, r)
    j = await MFC3Range(w, r)
    k = await MFC2Range(w, r)
    l = await MFC1Range(w, r)
    m = await RFMaxPower(w, r)
    n = await TunerAutoMode(w, r)
    startupData = { 
            "howManyMFCs"       : a,
            "BatchIDLogging"    : b,
            "RecipeMBStartPos"  : c,
            "RecipeRFPower"     : d,
            "RecipeMFC1Flow"    : e,
            "RecipeMFC2Flow"    : f,
            "RecipeMFC3Flow"    : g,
            "RecipeMFC4Flow"    : h,
            "MFC4Range"         : i,
            "MFC3Range"         : j,
            "MFC2Range"         : k,
            "MFC1Range"         : l,
            "RFMaxPower"        : m,
            "TunerAutoMode"     : n
    }
    return startupData
     
async def howManyMFCs(w, r) -> int:
    await writeCommand(w, "$2A002%") 
    rcv = await readResponse(r)
    strVar = rcv[3:] #Lop off the first three characters
    strVar = strVar.rstrip('#')
    dataParsed = strVar.split(';') 
    howManyMFCs = {
        "CTLPCBStatus": {
            "numMFCs": int(dataParsed[1], 10)
        }
    }
    #print(howManyMFCs)
    return howManyMFCs
       
async def BatchIDLogging(w, r) -> bool:
    await writeCommand(w, "$2A011%") 
    rcv = await readResponse(r)
    strVar = rcv[3:] #Lop off the first three characters
    strVar = strVar.rstrip('#')
    dataParsed = strVar.split(';') 
    BatchIDLogging = {
        "CTLPCBStatus": {
            "BatchIDLogging": bool(dataParsed[1])
        }
    }
    #print(BatchIDLogging)
    return BatchIDLogging
     
async def RecipeMBStartPos(w, r) -> float:
    await writeCommand(w, "$2A606%") 
    rcv = await readResponse(r)
    strVar = rcv[3:] #Lop off the first three characters
    strVar = strVar.rstrip('#')
    dataParsed = strVar.split(';') 
    RecipeStartPos = {
        "MBTuner": {
            "recipeStartPos": float(dataParsed[1])
        }
    }
    #print(RecipeStartPos)
    return RecipeStartPos
    
async def RecipeRFPower(w, r) -> float:
    await writeCommand(w, "$2A605%") 
    rcv = await readResponse(r)
    strVar = rcv[3:] #Lop off the first three characters
    strVar = strVar.rstrip('#')
    dataParsed = strVar.split(';') 
    RecipeRFPower = {
        "RFSupply": {
            "recipeRF": float(dataParsed[1])
        }
    }
    #print(RecipeRFPower)
    return RecipeRFPower

async def RecipeMFC4Flow(w, r) -> float:
    await writeCommand(w, "$2A604%") 
    rcv = await readResponse(r)
    strVar = rcv[3:] #Lop off the first three characters
    strVar = strVar.rstrip('#')
    dataParsed = strVar.split(';') 
    RecipeMFC4Flow = {
        "MFC4": {
            "recipeFlow": float(dataParsed[1])
        }
    }
    #print(RecipeMFC4Flow)
    return RecipeMFC4Flow

async def RecipeMFC3Flow(w, r) -> float:
    await writeCommand(w, "$2A603%") 
    rcv = await readResponse(r)
    strVar = rcv[3:] #Lop off the first three characters
    strVar = strVar.rstrip('#')
    dataParsed = strVar.split(';') 
    RecipeMFC3Flow = {
        "MFC3": {
            "recipeFlow": float(dataParsed[1])
        }
    }
    #print(RecipeMFC3Flow)
    return RecipeMFC3Flow


async def RecipeMFC2Flow(w, r) -> float:
    await writeCommand(w, "$2A602%") 
    rcv = await readResponse(r)
    strVar = rcv[3:] #Lop off the first three characters
    strVar = strVar.rstrip('#')
    dataParsed = strVar.split(';') 
    RecipeMFC2Flow = {
        "MFC2": {
            "recipeFlow": float(dataParsed[1])
        }
    }
    #print(RecipeMFC2Flow)
    return RecipeMFC2Flow


async def RecipeMFC1Flow(w, r) -> float:
    await writeCommand(w, "$2A601%") 
    rcv = await readResponse(r)
    strVar = rcv[3:] #Lop off the first three characters
    strVar = strVar.rstrip('#')
    dataParsed = strVar.split(';') 
    RecipeMFC1Flow = {
        "MFC1": {
            "recipeFlow": float(dataParsed[1])
        }
    }
    #print(RecipeMFC1Flow)
    return RecipeMFC1Flow


async def MFC4Range(w, r) -> float:
    await writeCommand(w, "$8504%") 
    rcv = await readResponse(r)
    strVar = rcv[5:] #Lop off the first five characters
    dataParsed = strVar.rstrip('#')
    MFC4Range = {
        "MFC4": {
            "range": float(dataParsed[1])
        }
    }
    #print(MFC4Range)
    return MFC4Range

async def MFC3Range(w, r) -> float:
    await writeCommand(w, "$8503%") 
    rcv = await readResponse(r)
    strVar = rcv[5:] #Lop off the first five characters
    dataParsed = strVar.rstrip('#')
    MFC3Range = {
        "MFC3": {
            "range": float(dataParsed[1])
        }
    }
    #print(MFC3Range)
    return MFC3Range

async def MFC2Range(w, r) -> float:
    await writeCommand(w, "$8502%") 
    rcv = await readResponse(r)
    strVar = rcv[5:] #Lop off the first five characters
    dataParsed = strVar.rstrip('#')
    MFC2Range = {
        "MFC2": {
            "range": float(dataParsed[1])
        }
    }
    #print(MFC2Range)
    return MFC2Range


async def MFC1Range(w, r) -> float:
    await writeCommand(w, "$8501%") 
    rcv = await readResponse(r)
    strVar = rcv[5:] #Lop off the first five characters
    dataParsed = strVar.rstrip('#')
    MFC1Range = {
        "MFC1": {
            "range": float(dataParsed[1])
        }
    }
    #print(MFC1Range)
    return MFC1Range

async def RFMaxPower(w, r) -> float:
    await writeCommand(w, "$2A705%") 
    rcv = await readResponse(r)
    strVar = rcv[3:] #Lop off the first three characters
    strVar = strVar.rstrip('#')
    dataParsed = strVar.split(';') 
    RFMax = {
        "RFSupply": {
            "maxPowerForward": float(dataParsed[1])
        }
    }
    #print(RFMax)
    return RFMax


async def TunerAutoMode(w, r) -> bool:
    await writeCommand(w, "$89%") 
    rcv = await readResponse(r)
    strVar = rcv[3:] #Lop off the first three characters
    dataParsed = strVar.rstrip('#')
    tunerMode = {
        "MBTuner": {
            "autoMode": bool(dataParsed[1])
        }
    }
    #print(tunerMode)
    return tunerMode

async def main():
    # context manager for the server 
    async with websockets.serve(serial_handler, "localhost", 5000):
        await asyncio.Future()  # run forever

# asyncio.run() function begins our event loop by calling a coroutine function (namely main()) 
if __name__ == "__main__":
    asyncio.run(main()) 
