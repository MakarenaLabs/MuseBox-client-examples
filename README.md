
## 📝 Table of Contents

- [MuseBox C++ client](#cpp)
- [MuseBox Python Client](#python)


## Prerequisites

In order to run the clients, you should firstly install:
- ZeroMQ (cpp and python)
- OpenCV (cpp and python)
- Websockets (python)

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

## MuseBox documentation

For more informations on MuseBox and the inputs of each topic, you can go to the documentaion site: https://doc.musebox.it/