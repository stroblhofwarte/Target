# TargetClient
Target(Astronomy) is a small WiFi-Handbox to control a telecope mount via INDI. 
This box is running with battery and does not have any wire to the telecope mount. 
This box consists out of a electronic board, a ESP8266 firmware (arduino IDE) and a 
Qt5 application bridging the handbox to the INDI mount. The handbox provides four keys 
for N,S,W and E, one slew speed key with three LED's and a function key which change 
the N,S,W and E key to Focuser 1 in and out and Focuser 2 in and out 
(not tested yet, I don't have any motor focuser).

# Prerequist (Ubuntu)

```
sudo apt install build-essential
sudo apt install cmake
sudo apt install qt5-default
sudo apt install zlib1g-dev
sudo apt install libcfitsio-dev
```
Installation of the INDI lib development package:
```
sudo apt-add-repository ppa:mutlaqja/ppa
sudo apt update
sudo apt install libindi-dev
```

# How to build

```
cd ~/_develop/

git clone https://github.com/stroblhofwarte/Target.git

mkdir -p ~/_develop/build/targetclient

cd ~/_develop/build/targetclient

cmake -DCMAKE_INSTALL_PREFIX=/usr -DCMAKE_BUILD_TYPE=Release ~/_develop/Target/targetclient

make -j4

sudo cp TargetClient <WHERE/EVER/YOU/WANT>
```

# ASCOM TargetBridge

The new Version of the Target_Astronomy_.ino firmware (from 6.01.2021) is supported by the ASCOM TargetBridge. This version of the firmware is
not supported by the INDI driver anymore. 
The ASCOM TargetBridge adds support for a Win10 based observatory setup. With the TargetBridge application the Target handcontroler 
box can be used to control the mount and the focuser together with the imaging software (e.g. N.I.N.A.). To reach this the ASCOM Hub must
be setup for devices not supporting a multiconnection. The ASCOM Hub must be setup only once. After setup the TargetBridge and/or the imaging
software will startup the hub automatically.

For some convenience four focuser positions (for absolute focuser only) can be marked. This can be used to switch the focal plane between a 
imaging setup with CCD and a visual setup easily.

The ASCOM TargetBridge is written in C# (VisualStudio 2019) and requires the ASCOM development libs to compile properly. 
An precompiled msi package will follow soon.
