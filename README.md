# TargetClient
Target(Astronomy) is a small WiFi-Handbox to control a telecope mount via INDI. 
This box is running with battery and does not have any wire to the telecope mount. 
This box consists out of a electronic board, a ESP8266 firmware (arduino IDE) and a 
Qt5 application bridging the handbox to the INDI mount. The handbox provides four keys 
for N,S,W and E, one slew speed key with three LED's and a function key which change 
the N,S,W and E key to Focuser 1 in and out and Focuser 2 in and out 
(not tested yet, I don't have any motor focuser).


# How to build

'''
cd ~/_develop/

git clone https://github.com/stroblhofwarte/Target.git

mkdir -p ~/_develop/build/targetclient

cd ~/_develop/build/targetclient

cmake -DCMAKE_INSTALL_PREFIX=/usr -DCMAKE_BUILD_TYPE=Release ~/_develop/Target/targetclient

make -j4

sudo make install
'''
