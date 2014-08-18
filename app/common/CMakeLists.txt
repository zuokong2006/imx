#dir: app/common
#datte: 2014-06-13
#author: zuokongxiao

#编译器设置
IF(${TARGET_P} MATCHES "x86")
 MESSAGE(STATUS "compiler: gcc ")
 SET(CMAKE_C_COMPILER gcc)
ELSE()
 MESSAGE(STATUS "target_p: arm-linux-gnueabihf-gcc")
 SET(CMAKE_C_COMPILER arm-linux-gnueabihf-gcc)
ENDIF()

#将当前目录中的源文件名称赋值给变量DIR_COMMON_SRCS
#AUX_SOURCE_DIRECTORY(. DIR_COMMON_SRCS)
SET(DIR_COMMON_SRCS debug.c)

#命令ADD_LIBRARY将../common目录中的源文件编译为共享库
ADD_LIBRARY(common STATIC ${DIR_COMMON_SRCS})