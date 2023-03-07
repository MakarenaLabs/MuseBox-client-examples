import asyncio
import websockets
import cv2
import base64
import json
 
async def test():
    async with websockets.connect('ws://127.0.0.1:9696') as websocket:
        message = {
        "topic": "GenderDetection",
        "clientId": "1",
        }

        image = cv2.imread("test.jpg")
        ret, image = cv2.imencode('.png', image)
        encoded = base64.b64encode(image)
        message["image"] = str(encoded, "utf-8")
        message = json.dumps(message)

        await websocket.send(message)
        print("sent")
        response = await websocket.recv()
        print(response)
 
asyncio.get_event_loop().run_until_complete(test())
