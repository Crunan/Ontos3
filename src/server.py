import asyncio
import json
from abc import ABC, abstractmethod
from dataclasses import dataclass, field
from enum import Enum, auto
from typing import Any, Callable, Optional, Protocol

import serial
import serial_asyncio
import websockets


class LightTower(Enum):
    ERROR = auto()
    READY = auto()
    ACTIVE = auto()
    IDLE = auto()


class AbortCodes(Enum):
    NO_N2 = auto()
    NO_HEARTBEAT = auto()
    NO_GAS_1 = auto()
    NO_GAS_2 = auto()
    NO_GAS_3 = auto()
    NO_GAS_4 = auto()
    BAD_HELIUM = auto()
    ESTOP = auto()
    NO_RS485 = auto()
    PWR_FWD_LOW = auto()
    OVER_TEMP = auto()


@dataclass
class CTLRecipe:
    power: int
    matchbox_start_position: int
    matchbox_auto_tune: bool
    mfc_1_flow: float
    mfc_2_flow: float
    mfc_3_flow: float
    mfc_4_flow: float
    execute_recipe: bool
    load_Recipe: bool


@dataclass
class AUXRecipe:
    gap: int
    thickness: int
    cycles: int
    overlap: int
    speed: int


@dataclass
class Recipe:
    name: str
    control_pcb: CTLRecipe
    aux_pcb: AUXRecipe


@dataclass
class MatchboxTuner:
    actual_position: float
    auto_tuner: bool


@dataclass
class RFSupply:
    power_forward: int
    power_reflected: int
    max_power: int


@dataclass
class MFC:
    range: float
    actual_flow: float


@dataclass
class PlasmaHead:
    plasma_active: bool
    temperature: int
    position: float
    slit_length: float
    slit_width: float


@dataclass
class Commands:
    command: dict[str, Callable[[Any], Any]] = field(default_factory=dict)

    def get_command(self, command: str) -> Callable[[], Any]:
        return self.command.get(command)()


@dataclass
class Parameters:
    commands: Commands


class Connection(Protocol):
    async def open_connection(self, port_path: str, baud_rate: int):
        ...


@dataclass
class SerialConnection:
    writer: asyncio.StreamWriter
    reader: asyncio.StreamReader

    async def write_command(self, cmd: str) -> None:
        self.writer.write(cmd.encode("ascii"))
        await self.writer.drain()

    async def read_response(self) -> str:
        response = await self.reader.readuntil(b"#")
        print(response.decode())
        return response.decode()

    async def set_reader_writer(self, connection: Connection):
        self.reader, self.writer = connection.open_connection()


async def open_connection(
    self, port_path: str, baud_rate: int
) -> tuple[asyncio.StreamReader, asyncio.StreamWriter]:
    self.reader, self.writer = serial_asyncio.open_serial_connection(
        url=port_path, baudrate=baud_rate
    )


class PCB(ABC):
    firmware_version: int
    serial_number: int

    commands: Commands
    parameter: Parameters
    serial_connection: SerialConnection

    @abstractmethod
    def get_firmware(self):
        pass

    @abstractmethod
    def set_serial_connection(self, port: serial.Serial):
        pass


@dataclass
class CTL(PCB):
    matchbox_tuner: MatchboxTuner
    rf_supply: RFSupply
    mfc: list[MFC]
    plasma_head: PlasmaHead

    ctl_recipe: Optional[CTLRecipe]
    aux_recipe: Optional[AUXRecipe]

    def get_firmware(self):
        pass

    def set_port(self, port: serial.Serial):
        self.port = port


@dataclass
class Laser:
    position: int
    status: bool


@dataclass
class Axis:
    state: int
    error: int
    position: float
    max_speed: int
    speed: int
    home_position: float


@dataclass
class Stage:
    x_axis: Axis
    y_axis: Axis
    z_axis: Axis

    pins_active: bool
    pins_buried_position: float
    pins_exposed_position: float
    two_spot_active: bool

    async def toggle_two_spot(self):
        if self.two_spot_active:
            await writeCommand("$BE%")
            await readResponse()
        else:
            await writeCommand("$BF%")
            await readResponse(r)


@dataclass
class AUX(PCB):
    recipe: Optional[AUXRecipe]
    stage: Stage

    def get_firmware(self):
        pass


async def organize_poll_data(d, e):
    strVar = d[3:-1]  # Lop off the first three characters
    dataParsed = strVar.split(";")
    pcb_poll_data = {
        "PCBStatus": {},
        "MBTuner": {"actualPosition": float(dataParsed[1])},
        "RFSupply": {
            "powerForward": int(dataParsed[2], 10),
            "powerReflected": int(dataParsed[3], 10),
        },
        "PlasmaHead": {"plasmaOn": int(dataParsed[4], 16)},
        "MFC4": {"actualFlow": float(dataParsed[5])},
        "MFC3": {"actualFlow": float(dataParsed[6])},
        "MFC2": {"actualFlow": float(dataParsed[7])},
        "MFC1": {"actualFlow": float(dataParsed[8])},
    }
    pcb_poll_data["PCBStatus"] = await update_status(dataParsed[0], e)
    print(pcb_poll_data)
    return pcb_poll_data


async def two_spot(w, r, state):
    match state:
        case "1":
            await writeCommand(w, "$BE%")
            await readResponse(r)
        case "0":
            await writeCommand(w, "$BF%")
            await readResponse(r)


async def tune_matchbox(w, r, d):
    match d:
        case "1":
            await writeCommand(w, "$11010032%")
            await readResponse(r)
        case "0":
            await writeCommand(w, "$11000032%")
            await readResponse(r)


async def set_light_tower(w, r, state):
    match state:
        case "1":
            await writeCommand(w, "$CB01%")
            await readResponse(r)
        case "2":
            await writeCommand(w, "$CB02%")
            await readResponse(r)
        case "3":
            await writeCommand(w, "$CB03%")
            await readResponse(r)


async def get_abort_code(w, r):
    await writeCommand(w, "$8B%")
    code = await readResponse(r)
    abort_code = int(code[3:-1], 16)  # Lop off the first three characters and # char

    match abort_code:
        case 0x1:
            error_code = {"AbortCodes": {"NO_N2": True}}
            return error_code
        case 0x2:
            error_code = {"AbortCodes": {"NO_HEARTBEAT": True}}
            return error_code
        case 0x3:
            error_code = {"AbortCodes": {"NO_GAS_1": True}}
            return error_code
        case 0x4:
            error_code = {"AbortCodes": {"NO_GAS_2": True}}
            return error_code
        case 0x5:
            error_code = {"AbortCodes": {"NO_GAS_3": True}}
            return error_code
        case 0x6:
            error_code = {"AbortCodes": {"NO_GAS_4": True}}
            return error_code
        case 0x7:
            error_code = {"AbortCodes": {"BAD_HELIUM": True}}
            return error_code
        case 0x8:
            error_code = {"AbortCodes": {"ESTOP": True}}
            return error_code
        case 0x9:
            error_code = {"AbortCodes": {"NO_RS485": True}}
            return error_code
        case 0xA:
            error_code = {"AbortCodes": {"PWR_FWD_LOW": True}}
            return error_code
        case 0xB:
            error_code = {"AbortCodes": {"OVER_TEMP": True}}
            return error_code


async def load_recipe(w, r, file_name):
    path = "./ontos3/recipes/"
    file_to_open = path + file_name

    recipe_file = open(file_to_open)
    recipe_data = {"Recipe": json.load(recipe_file)}
    print(recipe_data)
    return recipe_data


async def reset_ctl(w, r):
    await writeCommand(w, "$90%")
    await readResponse(r)
    await asyncio.sleep(0.1)


async def reset_3_axis_ctl(w, r):
    await writeCommand(w, "$A9%")
    await readResponse(r)
    await asyncio.sleep(0.1)


async def stop_motors(w, r):
    await writeCommand(w, "$B3%")
    return await readResponse(r)


async def init_stage_x(w, r):
    await writeCommand(w, "$B500%")
    return await readResponse(r)


async def init_stage_y(w, r):
    await writeCommand(w, "$B501%")
    return await readResponse(r)


async def init_stage_z(w, r):
    await writeCommand(w, "$B502%")
    return await readResponse(r)


async def init_stage_all(w, r):
    await stop_motors(w, r)
    x = await init_stage_x(w, r)
    y = await init_stage_y(w, r)
    z = await init_stage_z(w, r)
    rcv = x, y, z
    return rcv


async def load_stage(w, r):
    zpark = 6.00
    zhome = 29.25

    xhome = 115.58
    xspeed = 15.00

    yhome = 200.00
    yspeed = 15.00

    strVar = "$B400" + str(xspeed) + "%"
    await writeCommand(w, strVar)
    await readResponse(r)
    print("load started")

    strVar = "$B4025.00%"
    await writeCommand(w, strVar)
    await readResponse(r)

    strVar = "$B602" + str(zpark) + "%"
    await writeCommand(w, strVar)
    await readResponse(r)

    strVar = "$B400" + str(xspeed) + "%"
    await writeCommand(w, strVar)
    await readResponse(r)

    strVar = "$B401" + str(yspeed) + "%"
    await writeCommand(w, strVar)
    await readResponse(r)

    strVar = "$B600" + str(xhome) + "%"
    await writeCommand(w, strVar)
    await readResponse(r)

    strVar = "$B601" + str(yhome) + "%"
    await writeCommand(w, strVar)
    await readResponse(r)

    await asyncio.sleep(5)
    strVar = "$B602" + str(zhome) + "%"
    await writeCommand(w, strVar)
    await readResponse(r)

    await asyncio.sleep(5)

    await writeCommand(w, "$B3%")
    await readResponse(r)


async def update_status(data, e):
    hex_bits = int(data, base=16)
    status_dict = {}
    for n in range(0, 16):
        led_number = "led_bit_" + str(n)

        if hex_bits & (1 << n) > 0:
            status_dict[led_number] = "on"
            if n == 7:
                e.set()
                print(e.is_set())
        else:
            status_dict[led_number] = "off"

    return status_dict


async def temperature(w, r) -> float:
    await writeCommand(w, "$C5%")
    rcv = await readResponse(r)

    temp = rcv[3:-1]  # Lop off the first three characters
    temperature = {
        "PlasmaHead": {
            "temp": float(temp),
        }
    }
    print(temperature)
    return temperature


async def organize_3_axis_poll_data(d):
    strVar = d[3:-1]  # Lop off the first three characters
    data_parsed = strVar.split(";")
    LED_status = data_parsed[0] + data_parsed[1]
    xyz_same = await check_xyz(data_parsed[2], data_parsed[5], data_parsed[8])
    pcb_poll_data = {
        "LEDS": {},
        "Status": {"XYZSameState": bool(xyz_same)},
        "X": {
            "state": int(data_parsed[2]),
            "err": int(data_parsed[3], 16),
            "position": float(data_parsed[4]),
        },
        "Y": {
            "state": int(data_parsed[5]),
            "err": int(data_parsed[6], 16),
            "position": float(data_parsed[7]),
        },
        "Z": {
            "state": int(data_parsed[8]),
            "err": int(data_parsed[9], 16),
            "position": float(data_parsed[10]),
        },
    }
    pcb_poll_data["LEDS"] = await update_status(LED_status)
    print(pcb_poll_data)
    return pcb_poll_data


async def check_xyz(x, y, z) -> str:
    print(x, y, z)
    if (x == y) and (x == z):
        return True

    return False


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


async def poll_ctl(w, r) -> str:
    await writeCommand(w, "$91%")
    return await readResponse(r)


async def poll_3_axis(w, r) -> str:
    await writeCommand(w, "$C0%")
    return await readResponse(r)


async def startup_3_AXIS(w, r) -> dict:
    o = await stage_x_values(w, r)
    p = await stage_y_values(w, r)
    q = await stage_z_values(w, r)

    startup_data = {"X": o, "Y": p, "Z": q}
    print(startup_data)
    return startup_data


async def startup_CTL(w, r) -> dict:
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
        "howManyMFCs": a,
        "BatchIDLogging": b,
        "RecipeMBStartPos": c,
        "RecipeRFPower": d,
        "RecipeMFC1Flow": e,
        "RecipeMFC2Flow": f,
        "RecipeMFC3Flow": g,
        "RecipeMFC4Flow": h,
        "MFC4Range": i,
        "MFC3Range": j,
        "MFC2Range": k,
        "MFC1Range": l,
        "RFMaxPower": m,
        "TunerAutoMode": n,
    }
    print(startupData)
    return startupData


async def stage_z_values(w, r) -> int:
    await writeCommand(w, "$DA307%")
    rcv = await readResponse(r)
    dataParsed = rcv[7:-1]
    z_max_speed = float(dataParsed)

    await writeCommand(w, "$DA532%")
    rcv = await readResponse(r)
    dataParsed = rcv[7:-1]
    home_pos = float(dataParsed)

    await writeCommand(w, "$DA530%")
    rcv = await readResponse(r)
    dataParsed = rcv[7:-1]
    plasma_head_pos = float(dataParsed)

    await writeCommand(w, "$DA542%")
    rcv = await readResponse(r)
    dataParsed = rcv[7:-1]
    safety_gap = float(dataParsed)

    await writeCommand(w, "$DA543%")
    rcv = await readResponse(r)
    dataParsed = rcv[7:-1]
    pins_buried = float(dataParsed)

    await writeCommand(w, "$DA544%")
    rcv = await readResponse(r)
    dataParsed = rcv[7:-1]
    pins_exposed = float(dataParsed)

    z_values = {
        "max_speed": z_max_speed,
        "home_position": home_pos,
        "plasma_head_position": plasma_head_pos,
        "plasma_safety_gap": safety_gap,
        "pins_buried_position": pins_buried,
        "pins_exposed_position": pins_exposed,
    }
    return z_values


async def stage_y_values(w, r) -> int:
    await writeCommand(w, "$DA207%")
    rcv = await readResponse(r)
    dataParsed = rcv[7:-1]
    y_max_speed = float(dataParsed)

    await writeCommand(w, "$DA522%")
    rcv = await readResponse(r)
    dataParsed = rcv[7:-1]
    home_pos = float(dataParsed)

    await writeCommand(w, "$DA521%")
    rcv = await readResponse(r)
    dataParsed = rcv[7:-1]
    plasma_head_pos = float(dataParsed)

    await writeCommand(w, "$DA541%")
    rcv = await readResponse(r)
    dataParsed = rcv[7:-1]
    slit_width = float(dataParsed)

    await writeCommand(w, "$DA521%")
    rcv = await readResponse(r)
    dataParsed = rcv[7:-1]
    laser_pos = float(dataParsed)

    y_values = {
        "max_speed": y_max_speed,
        "home_position": home_pos,
        "plasma_head_position": plasma_head_pos,
        "plasma_slit_width": slit_width,
        "laser_position": laser_pos,
    }
    return y_values


async def stage_x_values(w, r) -> int:
    await writeCommand(w, "$DA107%")
    rcv = await readResponse(r)
    dataParsed = rcv[7:-1]
    x_max_speed = float(dataParsed)

    await writeCommand(w, "$DA512%")
    rcv = await readResponse(r)
    dataParsed = rcv[7:-1]
    home_pos = float(dataParsed)

    await writeCommand(w, "$DA510%")
    rcv = await readResponse(r)
    dataParsed = rcv[7:-1]
    plasma_head_pos = float(dataParsed)

    await writeCommand(w, "$DA540%")
    rcv = await readResponse(r)
    dataParsed = rcv[7:-1]
    slit_len = float(dataParsed)

    await writeCommand(w, "$DA511%")
    rcv = await readResponse(r)
    dataParsed = rcv[7:-1]
    laser_pos = float(dataParsed)

    x_values = {
        "max_speed": x_max_speed,
        "home_position": home_pos,
        "plasma_head_pos": plasma_head_pos,
        "plasma_slit_length": slit_len,
        "laser_position": laser_pos,
    }
    return x_values


async def howManyMFCs(w, r) -> int:
    await writeCommand(w, "$2A002%")
    rcv = await readResponse(r)
    dataParsed = rcv[7:-1]
    num_mfcs = {"Settings": {"numMFCS": int(dataParsed)}}
    return num_mfcs


async def BatchIDLogging(w, r) -> bool:
    await writeCommand(w, "$2A011%")
    rcv = await readResponse(r)
    dataParsed = rcv[7:-1]
    BatchIDLogging = {"PCBStatus": {"BatchIDLogging": bool(dataParsed)}}
    return BatchIDLogging


async def RecipeMBStartPos(w, r) -> float:
    await writeCommand(w, "$2A606%")
    rcv = await readResponse(r)
    dataParsed = rcv[7:-1]
    RecipeStartPos = {"MBTuner": {"recipeStartPos": float(dataParsed)}}
    return RecipeStartPos


async def RecipeRFPower(w, r) -> float:
    await writeCommand(w, "$2A605%")
    rcv = await readResponse(r)
    dataParsed = rcv[7:-1]
    RecipeRFPower = {"RFSupply": {"recipeRF": float(dataParsed)}}
    return RecipeRFPower


async def RecipeMFC4Flow(w, r) -> float:
    await writeCommand(w, "$2A604%")
    rcv = await readResponse(r)
    dataParsed = rcv[7:-1]
    RecipeMFC4Flow = {"MFC4": {"recipeFlow": float(dataParsed)}}
    return RecipeMFC4Flow


async def RecipeMFC3Flow(w, r) -> float:
    await writeCommand(w, "$2A603%")
    rcv = await readResponse(r)
    dataParsed = rcv[7:-1]
    RecipeMFC3Flow = {"MFC3": {"recipeFlow": float(dataParsed)}}
    return RecipeMFC3Flow


async def RecipeMFC2Flow(w, r) -> float:
    await writeCommand(w, "$2A602%")
    rcv = await readResponse(r)
    dataParsed = rcv[7:-1]
    RecipeMFC2Flow = {"MFC2": {"recipeFlow": float(dataParsed)}}
    return RecipeMFC2Flow


async def RecipeMFC1Flow(w, r) -> float:
    await writeCommand(w, "$2A601%")
    rcv = await readResponse(r)
    dataParsed = rcv[7:-1]
    RecipeMFC1Flow = {"MFC1": {"recipeFlow": float(dataParsed)}}
    return RecipeMFC1Flow


async def MFC4Range(w, r) -> float:
    await writeCommand(w, "$8504%")
    rcv = await readResponse(r)
    dataParsed = rcv[5:-1]
    MFC4Range = {"MFC4": {"range": float(dataParsed)}}
    return MFC4Range


async def MFC3Range(w, r) -> float:
    await writeCommand(w, "$8503%")
    rcv = await readResponse(r)
    dataParsed = rcv[5:-1]
    MFC3Range = {"MFC3": {"range": float(dataParsed)}}
    return MFC3Range


async def MFC2Range(w, r) -> float:
    await writeCommand(w, "$8502%")
    rcv = await readResponse(r)
    dataParsed = rcv[5:-1]
    MFC2Range = {"MFC2": {"range": float(dataParsed)}}
    return MFC2Range


async def MFC1Range(w, r) -> float:
    await writeCommand(w, "$8501%")
    rcv = await readResponse(r)
    dataParsed = rcv[5:-1]
    MFC1Range = {"MFC1": {"range": float(dataParsed)}}
    return MFC1Range


async def RFMaxPower(w, r) -> float:
    await writeCommand(w, "$2A705%")
    rcv = await readResponse(r)
    dataParsed = rcv[7:-1]
    RFMax = {"RFSupply": {"maxPowerForward": float(dataParsed)}}
    return RFMax


async def TunerAutoMode(w, r) -> bool:
    await writeCommand(w, "$89%")
    rcv = await readResponse(r)
    dataParsed = rcv[3:-1]
    tunerMode = {"MBTuner": {"autoMode": bool(dataParsed)}}
    return tunerMode


BROWSER_COMMANDS: dict(str, Callable[[], Any]) = {
    "poll": organize_poll_data,
    "temp": temperature,
    "startup": startup_CTL,
    "setMFCs": set_mfcs,
    "tunerModeSet": TunerModeSet,
    "RFSet": RFSet,
    "MBSet": MBSet,
    "executeRecipe": execute_recipe,
    "loadRecipe": load_recipe,
    "init": init_stage_all,
    "load": load_stage,
    "reset": reset_3_axis_ctl,
    "stop": stop_motors,
    "get_abort_code": get_abort_code,
    "light": set_light_tower,
    "tune": tune_matchbox,
    "twospot": two_spot,
    "get_abort_code": get_abort_code,
}


async def execute_browser_(command: str) -> None:
    await BROWSER_COMMANDS[command]()


async def gui_handler(websocket):
    """Returns a Coroutine that manages the websocket connection when a client connects to the server"""
    async for command in websocket:
        await execute_browser_(command)
        """ match command.split(":"): 
            case ["poll"]:
                rcv = await poll_ctl(writer, reader)
                websocket_ready_ctl_data = await organize_poll_data(rcv)
                await websocket.send(json.dumps(websocket_ready_ctl_data))
                if CTL_3_axis_exists == True: 
                    try:
                        rcv = await poll_3_axis(writer, reader)
                        websocket_ready_3_axis_data = await organize_3_axis_poll_data(rcv)
                        await websocket.send(json.dumps(websocket_ready_3_axis_data))
                    except:
                        CTL_status = {"PCBStatus": {"CTL3AxisExists": False} }
                        await websocket.send(json.dumps(CTL_status))
                        CTL_3_axis_exists = False 
            case ["temp"]:
                temp = await temperature(writer, reader)
                await websocket.send(json.dumps(temp))
            case ["startup"]:
                startupDict = await startup_CTL(writer, reader)
                await websocket.send(json.dumps(startupDict))
                #send JSON parsed stuff await websocket.send("startup complete")
            case ["setMFCs", mfc, sp]:
                await set_mfcs(writer, reader, (mfc), (sp))
            case ["tunerModeSet", TMsp]:
                await TunerModeSet(writer, reader, (TMsp))
            case ["RFSet", RFsp]:
                await RFSet(writer, reader, (RFsp))
            case ["MBSet", MBsp]:
                await MBSet(writer, reader, (MBsp))
            case ["executeRecipe", sp]:
                await execute_recipe(writer, reader, (sp))
            case ["loadRecipe", file_name]:
                recipe_data = await load_recipe(writer, reader, (file_name))
                await websocket.send(json.dumps(recipe_data))
            case ["init"]:
                rcv = await init_stage_all(writer, reader)
                await websocket.send(json.dumps(rcv))
            case ["load"]:
                rcv = await load_stage(writer, reader)
                await websocket.send(json.dumps(rcv))
            case ["reset"]:
                await reset_3_axis_ctl(writer, reader)
            case ["stop"]:
                await stop_motors(writer, reader)
            case ["get_abort_code"]:
                rcv = await get_abort_code(writer, reader)
                await websocket.send(json.dumps(rcv))
            case ["light", state]:
                rcv = await set_light_tower(writer, reader, (state))
                await websocket.send(json.dumps(rcv))
            case ["tune", direction]:
                rcv = await tune_matchbox(writer, reader, (direction))
            case ["twospot", state]:
                rcv = await two_spot(writer, reader, (state))
"""


async def main() -> None:
    """
    websocket.serve() starts a websocket server with 3 positional arguments:
        1st - handler is the coroutine that manages the connection,
        when a client connects, websockets calls handler with the connection in argument.
        When handler terminates, websockets closes the connection.

        2nd - defines the network interface where the server can be reached,
        so that other devies on the same network can connect.

        3rd - port to which the server listens.
    """
    async with websockets.serve(gui_handler, "localhost", 5000):
        # context manager for the server - ensures the server shuts down properly when terminating the program
        await asyncio.Future()  # run forever


if __name__ == "__main__":
    """The event loop main() begins as a coroutine function."""
    asyncio.run(main())
    # creates an asyncio event loop
