cmake_minimum_required(VERSION 3.20)
project(Ex1Task2)

set(CMAKE_CXX_FLAGS -pthread)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_BUILD_TYPE Debug)

add_executable(Ex1Task2 MainTrain.cpp Server.cpp HybridAnomalyDetector.cpp anomaly_detection_util.cpp SimpleAnomalyDetector.cpp timeseries.cpp minCircle.cpp CLI.cpp commands.h)
