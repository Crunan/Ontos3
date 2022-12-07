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
    await resetCTL(writer, reader)
    async for message in websocket:
        print(message)
        match message.split(":"): 
            case ["poll"]:
                rcv = await poll(writer, reader)
                websocketReadyData = await organizePollData(rcv)
                await websocket.send(json.dumps(websocketReadyData))
            case ["startup"]:
                startupDict = await startup(writer, reader)
                await websocket.send(json.dumps(startupDict))
                #send JSON parsed stuff await websocket.send("startup complete")
            case ["setMFCs", mfc, sp]:
                await set_mfcs(writer, reader, (mfc), (sp))
            case ["Tuner Mode", TMsp]:
                await TunerModeSet(writer, reader, (TMsp))
            case ["RFSet", RFsp]:
                await RFSet(writer, reader, (RFsp))
            case ["MBSet", MBsp]:
                await MBSet(writer, reader, (MBsp))
            case ["executeRecipe", sp]:
                await execute_recipe(writer, reader, (sp))

  
async def resetCTL(w, r):
    await writeCommand(w, "$90%")
    await readResponse(r)

async def update_status(data):
    hex_bits = int(data, base=16)
    status_dict = {}
    for n in range(0, 16):
      led_number = "led_bit_" + str(n)

      if (hex_bits & (1<<n) > 0):
        status_dict[led_number] = "on"
      else:  
        status_dict[led_number] = "off"

    return status_dict

async def organizePollData(d):
    strVar = d[3:-1] #Lop off the first three characters
    dataParsed = strVar.split(';') 
    PCBPollData = {
        "PCBStatus": {
        },
        "MBTuner": {
            "actualPosition": float(dataParsed[1])
        },
        "RFSupply": {
            "powerForward": int(dataParsed[2], 10),
            "powerReflected": int(dataParsed[3], 10) 
        },
        "PlasmaHead": {
            "plasmaOn": int(dataParsed[4], 16) 
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
    PCBPollData["PCBStatus"] = await update_status(dataParsed[0]) 
    return PCBPollData

async def set_mfcs(w, r, mfc, sp):
    mfc_recipe_cmd = "$41" + mfc + sp + "%"
    await writeCommand(w, mfc_recipe_cmd)
    await readResponse(r)

async def RFSet(w, r, RFsp):
    RFRecipeCMD = "$42" + RFsp + "%"
    await writeCommand(w, RFRecipeCMD)
    await readResponse(r)

async def MBSet(w, r, MBsp):
    MBRecipeCMD = "$43" + MBsp + "%"
    await writeCommand(w, MBRecipeCMD)
    await readResponse(r)

async def TunerModeSet(w, r, TMsp):
    TMRecipeCMD = "$860" + TMsp + "%"
    await writeCommand(w, TMRecipeCMD)
    await readResponse(r)

async def execute_recipe(w, r, sp):
    execute_recipe_cmd = "$870" + sp + "%"
    await writeCommand(w, execute_recipe_cmd)
    await readResponse(r)

async def DefaultRecipe(w, r):
    DefaultRecipeCMD = "$44%"
    await writeCommand(w, DefaultRecipeCMD) 
    await readResponse(r)

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
    dataParsed = rcv[7:-1] 
    howManyMFCs = {"PCBStatus": {"numMFCs": int(dataParsed, 10) }}
    return howManyMFCs
       
async def BatchIDLogging(w, r) -> bool:
    await writeCommand(w, "$2A011%") 
    rcv = await readResponse(r)
    dataParsed = rcv[7:-1] 
    BatchIDLogging = {"PCBStatus": {"BatchIDLogging": bool(dataParsed) }}
    return BatchIDLogging
     
async def RecipeMBStartPos(w, r) -> float:
    await writeCommand(w, "$2A606%") 
    rcv = await readResponse(r)
    dataParsed = rcv[7:-1] 
    RecipeStartPos = {"MBTuner": {"recipeStartPos": float(dataParsed) }}
    return RecipeStartPos
    
async def RecipeRFPower(w, r) -> float:
    await writeCommand(w, "$2A605%") 
    rcv = await readResponse(r)
    dataParsed = rcv[7:-1] 
    RecipeRFPower = {"RFSupply": {"recipeRF": float(dataParsed) }}
    return RecipeRFPower

async def RecipeMFC4Flow(w, r) -> float:
    await writeCommand(w, "$2A604%") 
    rcv = await readResponse(r)
    dataParsed  = rcv[7:-1] 
    RecipeMFC4Flow = {"MFC4": {"recipeFlow": float(dataParsed) }}
    return RecipeMFC4Flow 
async def RecipeMFC3Flow(w, r) -> float:
    await writeCommand(w, "$2A603%") 
    rcv = await readResponse(r)
    dataParsed = rcv[7:-1]
    RecipeMFC3Flow = {"MFC3": {"recipeFlow": float(dataParsed) }}
    return RecipeMFC3Flow  
async def RecipeMFC2Flow(w, r) -> float:
    await writeCommand(w, "$2A602%") 
    rcv = await readResponse(r)
    dataParsed = rcv[7:-1] 
    RecipeMFC2Flow = {"MFC2": {"recipeFlow": float(dataParsed) }}
    return RecipeMFC2Flow  
async def RecipeMFC1Flow(w, r) -> float:
    await writeCommand(w, "$2A601%") 
    rcv = await readResponse(r)
    dataParsed = rcv[7:-1] 
    RecipeMFC1Flow = {"MFC1": {"recipeFlow": float(dataParsed) }}
    return RecipeMFC1Flow  


async def MFC4Range(w, r) -> float:
    await writeCommand(w, "$8504%") 
    rcv = await readResponse(r)
    dataParsed = rcv[5:-1] 
    MFC4Range = {"MFC4": {"range": float(dataParsed) }}
    return MFC4Range
async def MFC3Range(w, r) -> float:
    await writeCommand(w, "$8503%") 
    rcv = await readResponse(r)
    dataParsed = rcv[5:-1] 
    MFC3Range = {"MFC3": {"range": float(dataParsed) }}
    return MFC3Range
async def MFC2Range(w, r) -> float:
    await writeCommand(w, "$8502%") 
    rcv = await readResponse(r)
    dataParsed = rcv[5:-1] 
    MFC2Range = {"MFC2": {"range": float(dataParsed) }}
    return MFC2Range
async def MFC1Range(w, r) -> float:
    await writeCommand(w, "$8501%") 
    rcv = await readResponse(r)
    dataParsed = rcv[5:-1] 
    MFC1Range = {"MFC1": {"range": float(dataParsed) }}
    return MFC1Range

async def RFMaxPower(w, r) -> float:
    await writeCommand(w, "$2A705%") 
    rcv = await readResponse(r)
    dataParsed = rcv[7:-1] 
    RFMax = {"RFSupply": {"maxPowerForward": float(dataParsed) }}
    return RFMax

async def TunerAutoMode(w, r) -> bool:
    await writeCommand(w, "$89%") 
    rcv = await readResponse(r)
    dataParsed = rcv[3:-1] 
    tunerMode = {"MBTuner": {"autoMode": bool(dataParsed) }}
    return tunerMode

async def main():
    # context manager for the server 
    async with websockets.serve(serial_handler, "localhost", 5000):
        await asyncio.Future()  # run forever

# asyncio.run() function begins our event loop by calling a coroutine function (namely main()) 
if __name__ == "__main__":
    asyncio.run(main()) 
