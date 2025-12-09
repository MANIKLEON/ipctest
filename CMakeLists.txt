cmake_minimum_required(VERSION 3.10)
project(mqtt_local_test)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_EXTENSIONS OFF)

# ----------------------------
# Hardcoded Paho include paths
# ----------------------------
# Adjust if your headers/libs exist in another location (use 'find /usr/local/include -name async_client.h')
include_directories(/usr/local/include)
link_directories(/usr/local/lib)

# ----------------------------
# Threads required by Paho C++
# ----------------------------
find_package(Threads REQUIRED)

# ----------------------------
# Publisher
# ----------------------------
add_executable(publisher src/publisher.cpp)
target_link_libraries(
    publisher
    PRIVATE
        paho-mqttpp3       # Paho C++ library
        paho-mqtt3a        # Paho C async library
        Threads::Threads   # pthreads
)

# ----------------------------
# Subscriber
# ----------------------------
add_executable(subscriber src/subscriber.cpp)
target_link_libraries(
    subscriber
    PRIVATE
        paho-mqttpp3
        paho-mqtt3a
        Threads::Threads
)
