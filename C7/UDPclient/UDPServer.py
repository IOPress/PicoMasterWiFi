import asyncio

async def main():  
    loop = asyncio.get_running_loop()
    transport, protocol = await loop.create_datagram_endpoint(
                             lambda: asyncio.DatagramProtocol(),
                             local_addr=('0.0.0.0',8080))
    data=b"Hello UDP World"
    for i in range(20):
        transport.sendto(data,addr=("192.168.253.89",8080))
        await asyncio.sleep(1)
    transport.close()  
asyncio.run(main())