from flask import Flask, Response
from flask_cors import CORS, cross_origin

import cv2

app = Flask(__name__)

camera = cv2.VideoCapture(0)  

def gen_frames():  
    while True:
        # Capture frame-by-frame
        success, frame = camera.read()  # read the camera frame
        if not success:
            break
        else:
            ret, jpeg = cv2.imencode('.jpg', frame)
            
            frame = jpeg.tobytes()
        
        yield (b'--frame\r\n'
               b'Content-Type: image/jpeg\r\n\r\n' + frame + b'\r\n\r\n')


@app.route('/video_feed')
@cross_origin()
def video_feed():
    return Response(gen_frames(),
                    mimetype='multipart/x-mixed-replace; boundary=frame')


if __name__ == '__main__':
    app.config['CORS_HEADERS'] = 'Content-Type'
    app.run(debug=True)