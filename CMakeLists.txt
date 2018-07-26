cmake_minimum_required(VERSION 3.10)
project(myIPC)

set(CMAKE_CXX_STANDARD 11)

add_executable(myIPC shqSvr.cpp  shmQueue.cpp shmQueue.h )