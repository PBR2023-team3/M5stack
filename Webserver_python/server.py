import asyncio
import websockets
import json
import socket

# # async def send_json_data(websocket, path):
# #     # ここでJSONデータを読み込んで送信する処理を実装します
# #     json_data = {"message": "Hello, WebSocket with JSON!"}
# #     await websocket.send(json.dumps(json_data))

# async def main():
#     host_ip = socket.gethostbyname(socket.gethostname())
#     print(f"Server running at ws://{host_ip}:8765")

#     server = await websockets.serve(on_message, host_ip, 8765)
#     print(f"WebSocket server started on ws://{host_ip}:8765")

#     await server.wait_closed()

# async def on_message(websocket, message):
#     print(f"Received raw message: {message}")

# if __name__ == "__main__":
#     asyncio.run(main())

async def on_message(websocket, path):
    async for message in websocket:
        # print(f"Received message: {message}")
        # ここで受け取ったメッセージに対する処理を行います
        try:
            # Try parsing the message as JSON
            data = json.loads(message)
            # print("Parsed JSON data:", data)

            # Check if 'ID' is present in the JSON data to confirm it's a valid message
            enc = json.dumps(data, indent = 2)
            print("JSON:", enc)
            # if 'ID' in data:
            #     print("JSON:", data)

            # else:
            #     print("Invalid JSON data, skipping.")

        except json.JSONDecodeError as e:
            print(f"Error decoding JSON: {e}")
    

if __name__ == "__main__":
    host_ip = socket.gethostbyname(socket.gethostname())
    print(f"Server running at ws://{host_ip}:8765")

    start_server = websockets.serve(on_message, host_ip, 8765)
    print(f"WebSocket server started on ws://{host_ip}:8765")
    asyncio.get_event_loop().run_until_complete(start_server)
    asyncio.get_event_loop().run_forever()
