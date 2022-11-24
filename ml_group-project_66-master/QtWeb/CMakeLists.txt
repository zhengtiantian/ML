cmake_minimum_required(VERSION 3.5)

project(Qt_Kraken LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")

find_package(Qt5 COMPONENTS Core WebSockets Network REQUIRED)
find_package(OpenSSL COMPONENTS Crypto REQUIRED)

add_executable(Qt_Kraken
  main.cpp
  kraken_websocket.h
  kraken_websocket.cpp
  price_ladder_structure.h
  price_ladder_structure.cpp
  kraken_api.h
  kraken_api.cpp
  Balance_thread.cpp
  Balance_thread.h
  send_order_thread.cpp
  send_order_thread.h
)

target_link_libraries(Qt_Kraken Qt5::Core Qt5::WebSockets Qt5::Network)
target_link_libraries(Qt_Kraken OpenSSL::Crypto)
target_link_libraries(Qt_Kraken pthread)