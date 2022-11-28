#!/usr/python

from websockets import connect
import asyncio


async def hello(uri):
    async with connect(uri) as websocket:
        while True:
            o_p = await websocket.recv()
            print(o_p)

asyncio.run(hello("ws://192.168.1.1:288"))