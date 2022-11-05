import asyncio 

async def handler(reader, writer):
    # Read data until we get a end of cmd symbol (#)
    data = await reader.read()
    message = data.decode()
    addr = writer.get_extra_info('peername')

    print(f"Received {message!r} from {addr!r}")

    print(f"Send: {message!r}")
    writer.write(data)
    await writer.drain()
    # This is a flow control method that interacts with the underlying IO write 
    # buffer. When the size of the buffer reaches the high watermark,
    #  drain() blocks until the size of the buffer is drained down to the low '
    # watermark and writing can be resumed. 
    # When there is nothing to wait for, the drain() returns immediately.

    print("Close the connection")
    writer.close()

async def main():
    server = await asyncio.start_server(
           handler, '127.0.0.1', '/dev/ttyUSB0')

    addrs = ', '.join(str(sock.getsockname()) for sock in server.sockets)
    print(f'Serving on {addrs}')

    async with server:
        await server.serve_forever()


asyncio.run(main())
#from ServerProtocol import *

#async def main():
    # Get a reference to the event loop as we plan to use
    # low-level APIs.
#    loop = asyncio.get_running_loop()
#    server = await loop.create_server(
#        lambda: EchoServerProtocol(),
#        '127.0.0.1', 8888)

   # Context manager for the server. 
#    async with server:
#        await server.serve_forever()        

#    on_con_lost = loop.create_future()
#    message = 'hello world'

#    transport, protocol = await loop.create_connection(
#            lambda: EchoClientProtocol(message, on_con_lost),
#                '127.0.0.1', 8888)
    # Wait until the protocol signals that the connection
    # is lost and close the transport.
#    try:
#        await on_con_lost
#    finally:
#        transport.close()

#if __name__ == "__main__":
#    asyncio.run(main())

