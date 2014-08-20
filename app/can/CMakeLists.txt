#dir: app/can
#datte: 2014-08-19
#author: zuokongxiao
#discription: can���Գ���

#��������
PROJECT(cantest)

#CMAKE�汾
CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

#����û�������Ϣ
MESSAGE(STATUS "source dir: " ${PROJECT_SOURCE_DIR})
MESSAGE(STATUS "binary dir: " ${PROJECT_BINARY_DIR})

#����������
IF(${TARGET_P} MATCHES "x86")
 MESSAGE(STATUS "compiler: gcc ")
 SET(CMAKE_C_COMPILER gcc)
ELSE()
 MESSAGE(STATUS "target_p: arm-fsl-linux-gnueabi-gcc")
 SET(CMAKE_C_COMPILER arm-fsl-linux-gnueabi-gcc)
ENDIF()

#���ñ�����
#SET(CMAKE_THREAD_PREFER_PTHREAD TRUE)
#SET(CMAKE_C_FLAGS "-lpthread")
SET(CMAKE_C_FLAGS "-g")
MESSAGE(STATUS "c flags: " ${CMAKE_C_FLAGS})

#�߳�
#FIND_PACKAGE(Threads REQUIRED)

#ʹ������ADD_SUBDIRECTORYָ������Ŀ����һ����Ŀ¼test
#ADD_SUBDIRECTORY(libxml2)

#�򹤳�����ͷ�ļ�����·��
INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/../include)
#IF(${TARGET_P} MATCHES "x86")
# INCLUDE_DIRECTORIES(/usr/local/include/libxml2)
#ELSE()
# INCLUDE_DIRECTORIES(/usr/arm/lib/libxml2/include/libxml2)
#ENDIF()

#����ǰĿ¼�е�Դ�ļ����Ƹ�ֵ������DIR_SRCS
AUX_SOURCE_DIRECTORY(. DIR_SRCS)

#ָʾ����DIR_SRCS�е�Դ�ļ���Ҫ����ɿ�ִ���ļ�
ADD_EXECUTABLE(cantest cantest)

#ָ����ִ���ļ���Ҫ���ӵ����ӿ�
#TARGET_LINK_LIBRARIES(testxml ${CMAKE_THREAD_LIBS_INIT})
#TARGET_LINK_LIBRARIES(testxml ${PROJECT_SOURCE_DIR}/../common/libcommon.a)
#IF(${TARGET_P} MATCHES "x86")
# TARGET_LINK_LIBRARIES(testxml /usr/local/lib/libxml2.so)
#ELSE()
# TARGET_LINK_LIBRARIES(testxml /usr/arm/lib/libxml2/lib/libxml2.so)
#ENDIF()
TARGET_LINK_LIBRARIES(cantest ${PROJECT_SOURCE_DIR}/../common/libcommon.a)

