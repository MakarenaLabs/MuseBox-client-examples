CXX=${CXX:-g++}

result=0 && pkg-config --list-all | grep opencv4 && result=1
if [ $result -eq 1 ]; then
	OPENCV_FLAGS=$(pkg-config --cflags --libs-only-L opencv4)
else
	OPENCV_FLAGS=$(pkg-config --cflags --libs-only-L opencv)
fi

if [ "$1" == "cross-compile" ]; then

echo "*** CROSS COMPILING ***"
echo "*** CROSS COMPILING ***"
echo "*** CROSS COMPILING ***"
echo "*** CROSS COMPILING ***"
echo "*** CROSS COMPILING ***"

$CXX  -I. \
     client.cpp \
     -I/home/makarenalabs/petalinux_sdk_2022.2/sysroots/cortexa72-cortexa53-xilinx-linux/include \
     -lopencv_videoio  \
     -lopencv_imgcodecs \
     -lopencv_highgui \
     -lopencv_imgproc \
     -lopencv_core \
     -lzmq \
     ${OPENCV_FLAGS} \
     -o client

else

$CXX  -I. \
     client.cpp \
     -lopencv_videoio  \
     -lopencv_imgcodecs \
     -lopencv_highgui \
     -lopencv_imgproc \
     -lopencv_core \
     -lzmq \
     ${OPENCV_FLAGS} \
     -o client

fi
