import asyncio
import websockets

async def send_message():
    async with websockets.connect("ws://133.51.94.155:8765") as websocket:
        message = "Hello, WebSocket Server!"
        await websocket.send(message)
        print(f"Sent message: {message}")

if __name__ == "__main__":
    asyncio.get_event_loop().run_until_complete(send_message())
