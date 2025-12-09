MQTT IPC Setup Guide
Overview
This document explains how to set up a complete MQTT-based Inter-Process Communication (IPC) system in C++ using Eclipse Paho C/C++ clients and a Mosquitto broker. It provides installation steps, build instructions, and commands that developers can run on any Linux/WSL system.
1. Prerequisites
Ensure your system has the following installed:
- Git
- CMake (3.10+)
- Build tools (g++, make)
- OpenSSL development libraries
- pkg-config
- Mosquitto + mosquitto-clients

Install Required Packages
Run the following commands (Ubuntu / WSL):
sudo apt update
sudo apt install -y build-essential cmake git libssl-dev pkg-config mosquitto mosquitto-clients
2. Build and Install Paho MQTT C Library
git clone https://github.com/eclipse/paho.mqtt.c.git ~/paho.mqtt.c
cd ~/paho.mqtt.c
cmake -B build -S . -DPAHO_BUILD_SHARED=ON -DPAHO_WITH_SSL=ON
cmake --build build -j
sudo cmake --install build
3. Build and Install Paho MQTT C++ Library
git clone https://github.com/eclipse/paho.mqtt.cpp.git ~/paho.mqtt.cpp
cd ~/paho.mqtt.cpp
cmake -B build -S . -DPAHO_BUILD_SAMPLES=OFF
cmake --build build -j
sudo cmake --install build
sudo ldconfig
4. Example Hardcoded CMakeLists.txt
Use this CMake file to build your MQTT IPC project without needing find_package():

cmake_minimum_required(VERSION 3.10)
project(mqtt_local_test)

set(CMAKE_CXX_STANDARD 17)

include_directories(/usr/local/include)
link_directories(/usr/local/lib)

find_package(Threads REQUIRED)

add_executable(publisher src/publisher.cpp)
target_link_libraries(publisher PRIVATE paho-mqttpp3 paho-mqtt3a Threads::Threads)

add_executable(subscriber src/subscriber.cpp)
target_link_libraries(subscriber PRIVATE paho-mqttpp3 paho-mqtt3a Threads::Threads)

5. Running the MQTT Broker
Start Mosquitto broker:
sudo service mosquitto start

Check status:
sudo service mosquitto status
6. Running Publisher and Subscriber
Open Terminal 1:
./subscriber

Open Terminal 2:
./publisher 1000 1
7. Handling High Message Rates
If you see error 'MQTT error [-12]: No more messages can be buffered', implement backpressure using a sliding window of delivery tokens in your publisher.
8. Testing with Mosquitto CLI Tools
Terminal 1:
mosquitto_sub -t test/local -v

Terminal 2:
mosquitto_pub -t test/local -m "hello world"
9. Troubleshooting
- Header async_client.h not found → Paho C++ library not installed
- Link errors → ensure /usr/local/lib contains paho-mqttpp3 and paho-mqtt3a
- Broker connection errors → verify mosquitto is running

