#dir: ./app/thermalprinter/thermalprinterlib
#datte: 2014-10-30
#author: zuokongxiao
#discription: 热敏打印机库

#工程名称
PROJECT(thermalprinterlib)

#CMAKE版本
CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

#输出用户定义信息
MESSAGE(STATUS "source dir: " ${PROJECT_SOURCE_DIR})
MESSAGE(STATUS "binary dir: " ${PROJECT_BINARY_DIR})

#编译器设置
IF(${TARGET_P} MATCHES "x86")
 MESSAGE(STATUS "compiler: g++ ")
 SET(CMAKE_CXX_COMPILER g++)
ELSE()
 MESSAGE(STATUS "target_p: arm-linux-g++")
 SET(CMAKE_CXX_COMPILER arm-linux-g++)
ENDIF()

#设置编译器
SET(CMAKE_CXX_FLAGS "-g")
MESSAGE(STATUS "c flags: " ${CMAKE_C_FLAGS})

#向工程添加头文件搜索路径
INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/EasyBMP)

#使用命令ADD_SUBDIRECTORY指明本项目包含一个子目录
ADD_SUBDIRECTORY(EasyBMP)

#将当前目录中的源文件名称赋值给变量DIR_SRCS
AUX_SOURCE_DIRECTORY(. DIR_SRCS)

#指示变量DIR_SRCS中的源文件需要编译成一个可执行文件
ADD_LIBRARY(thermalprinter SHARED ${DIR_SRCS})
TARGET_LINK_LIBRARIES(thermalprinter easybmp)
