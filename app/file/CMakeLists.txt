#dir: app/file
#datte: 2014-06-13
#author: zuokongxiao

#工程名称
PROJECT(file)

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

#使用命令ADD_SUBDIRECTORY指明本项目包含一个子目录common
#ADD_SUBDIRECTORY(common)

#向工程添加头文件搜索路径
INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/../include)

#将当前目录中的源文件名称赋值给变量DIR_SRCS
AUX_SOURCE_DIRECTORY(. DIR_SRCS)

#指示变量DIR_SRCS中的源文件需要编译成一个名称为file_test的可执行文件
ADD_EXECUTABLE(file_test ${DIR_SRCS})

#指明可执行文件file_test需要连接一个名为common的链接库
TARGET_LINK_LIBRARIES(file_test ${PROJECT_SOURCE_DIR}/../common/libcommon.a)