import asyncio
import websockets

async def handler(websocket):
    while True:
        message = await websocket.recv()
        print(message)

async def main():
    # start a server side websocket with a context manager.
    # the context manager is within the 'with' block
    # this ensures that the server shuts down properly when terminating
    # the program.
    async with websockets.serve(handler, "", 8001):
        await asyncio.Future() #run forever
        
if __name__ == "__main__":
    asyncio.run(main())
