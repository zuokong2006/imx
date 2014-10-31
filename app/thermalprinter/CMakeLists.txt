#dir: app/thermalprinter
#datte: 2014-10-30
#author: zuokongxiao
#discription: 热敏打印机C++工程

#工程名称
PROJECT(thermalprinter)

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
INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/thermalprinterlib)

#将当前目录中的源文件名称赋值给变量DIR_SRCS
AUX_SOURCE_DIRECTORY(. DIR_SRCS)

#指示变量DIR_SRCS中的源文件需要编译成可执行文件
ADD_EXECUTABLE(thermalprinter thermalprinter)

#指明可执行文件需要连接的链接库
TARGET_LINK_LIBRARIES(thermalprinter ${PROJECT_SOURCE_DIR}/thermalprinterlib/libthermalprinter.so)


