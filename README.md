
## 📝 Table of Contents

- [MuseBox C++ client](#cpp)
- [MuseBox Python Client](#python)
- [NoCodeGUI-DecodedMediaStreams with Python](#nocodegui)

## Prerequisites

In order to run the clients, you should firstly install:
- ZeroMQ (cpp and python)
- OpenCV (cpp and python)
- Websockets (python)
- flask-cors (No Code GUI - python)

## MuseBox C++ client 

This example contains the list of all the available topics that can be run on MuseBox. To use it, you should build the executable file and run it as follows, remember to change row 20 of build_client.sh with the correct path if cross-compiling:

```
cd Cpp
./build_client.sh [cross-compile]
./client topic image|txt_file [image2]
```

## MuseBox Python client 

There are three different python examples featuring both ZeroMQ and Websocket clients. You can simply run them as follows:

```
# zmq client
python3 musebox_client.py
# websocket client
python3 websocket_client.py
```

## NoCodeGUI-DecodedMediaStreams with Python

If you want to stream using your webcam (either on your pc or FPGA board), we have created a Flask app. Run as follows:
```
python3 -m flask run --host=0.0.0.0
```
Meanwhile on the NoCodeGUI, you should add the "Decoded Media Streams" from MuseBox input's list and put the correct address, for example "http://192.168.188.77:5000/video_feed".

## MuseBox documentation

For more informations on MuseBox and the inputs of each topic, you can go to the documentaion site: https://doc.musebox.it/