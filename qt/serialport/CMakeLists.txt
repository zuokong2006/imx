#dir: /imx/qt/hello
#datte: 2014-12-25
#author: zuokongxiao
#discription: qt hello

#工程名称
PROJECT(serialport)

#CMAKE版本
CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

FIND_PACKAGE(Qt4 COMPONENTS QtCore QtGui REQUIRED)
INCLUDE(${QT_USE_FILE})

MESSAGE(STATUS "QT_USE_FILE: " ${QT_USE_FILE})
MESSAGE(STATUS "QOBJ_HDRS: " ${QOBJ_HDRS})
MESSAGE(STATUS "QT_DEFINITIONS: " ${QT_DEFINITIONS})

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
MESSAGE(STATUS "CMAKE_CXX_COMPILER: " ${CMAKE_CXX_COMPILER})

#向工程添加头文件搜索路径
INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/)
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR} ${CMAKE_CURRENT_SOURCE_DIR})
MESSAGE(STATUS "CMAKE_CURRENT_BINARY_DIR: " ${CMAKE_CURRENT_BINARY_DIR})
MESSAGE(STATUS "CMAKE_CURRENT_SOURCE_DIR: " ${CMAKE_CURRENT_SOURCE_DIR})

#将当前目录中的源文件名称赋值给变量DIR_SRCS
SET(serialport_source ${serialport_source} serialWidget.cpp)
SET(serialport_source ${serialport_source} serialSet.cpp)
SET(serialport_source ${serialport_source} serialTrans.cpp)
SET(serialport_source ${serialport_source} main.cpp)

#main.cpp serialSet.cpp serialTrans.cpp serialWidget.cpp
#serialData.h serialSet.h serialTrans.h serialWidget.h

#AUX_SOURCE_DIRECTORY(. DIR_SRCS)
MESSAGE(STATUS "source files: " ${serialport_source})

#指示变量DIR_SRCS中的源文件需要编译成可执行文件
ADD_EXECUTABLE(serialport serialWidget serialSet serialTrans main)

#指明可执行文件需要连接的链接库
TARGET_LINK_LIBRARIES(serialport ${QT_LIBRARIES})


