#dir: app/i2c
#datte: 2014-07-15
#author: zuokongxiao

#工程名称
PROJECT(i2c)

#CMAKE版本
CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

#输出用户定义信息
MESSAGE(STATUS "source dir: " ${PROJECT_SOURCE_DIR})
MESSAGE(STATUS "binary dir: " ${PROJECT_BINARY_DIR})

#编译器设置
IF(${TARGET_P} MATCHES "x86")
 MESSAGE(STATUS "compiler: gcc ")
 SET(CMAKE_C_COMPILER gcc)
ELSE()
 MESSAGE(STATUS "target_p: arm-linux-gnueabihf-gcc")
 SET(CMAKE_C_COMPILER arm-linux-gnueabihf-gcc)
ENDIF()

#
#SET(CMAKE_THREAD_PREFER_PTHREAD TRUE)
SET(CMAKE_C_FLAGS "-g")
#SET(CMAKE_C_FLAGS_DEBUG "-lpthread")
#SET(CMAKE_C_FLAGS_RELEASE "-lpthread")
MESSAGE(STATUS "c flags: " ${CMAKE_C_FLAGS})

#
FIND_PACKAGE(Threads REQUIRED)

#使用命令ADD_SUBDIRECTORY指明本项目包含一个子目录test
#ADD_SUBDIRECTORY(test)

#向工程添加头文件搜索路径
INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/../include)
#INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/test)

#将当前目录中的源文件名称赋值给变量DIR_SRCS
AUX_SOURCE_DIRECTORY(. DIR_SRCS)

#指示变量DIR_SRCS中的源文件需要编译成一个名称为pthread_test的可执行文件
#ADD_EXECUTABLE(pthread_test ${DIR_SRCS})
ADD_EXECUTABLE(i2c i2c)

#指明可执行文件需要连接的链接库
TARGET_LINK_LIBRARIES(i2c ${PROJECT_SOURCE_DIR}/../common/libcommon.a)
#TARGET_LINK_LIBRARIES(pthread_test test)
#TARGET_LINK_LIBRARIES(pthread_test ${CMAKE_THREAD_LIBS_INIT})


