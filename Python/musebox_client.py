import cv2
import zmq
import numpy as np
import json
import base64

def main():
    # open camera
    context = zmq.Context()
    socket = context.socket(zmq.PUB)
    try:
        socket.bind("tcp://*:9696")
    except zmq.error.ZMQError as e:
        print(e)
        
    socket2 = context.socket(zmq.SUB)
    socket2.connect("tcp://127.0.0.1:5556")
    socket2.setsockopt_string(zmq.SUBSCRIBE, str(''))

    message = {
        "topic": "FaceDetection",
        "clientId": "1",
        "publisherQueue": "tcp://*:5556"
    }

    camera = cv2.VideoCapture(0)
    while True:
        # Capture the video frame
        ret, frame = camera.read()
        ret, image = cv2.imencode('.png', frame)
        encoded = base64.b64encode(image)
        message["image"] = str(encoded, "utf-8")
        socket.send_string(json.dumps(message))
        
        # waiting for response from server
        message = json.loads(socket2.recv())
        print(message)

    # After the loop release the cap object
    camera.release()


if __name__ == '__main__':
    main()
