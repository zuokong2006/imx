#dir: /imx/qt/plotter
#datte: 2014-12-26
#author: zuokongxiao
#discription: cmake文件

#工程名称
PROJECT(plotter)

#CMAKE版本
CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

#QT
FIND_PACKAGE(Qt4 COMPONENTS QtCore QtGui REQUIRED)
INCLUDE(${QT_USE_FILE})

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

#头文件
SET(HEADER_FILES 
    plotter.h
    )

#源文件
SET(SOURCE_FILES 
    plotter.cpp
    main.cpp
    )

#moc文件Q_OBJECT
set(MOC_FILES 
    plotter.h
    )

#
set(RESOURCES_FILES
    plotter.qrc
    )

#向工程添加头文件搜索路径
INCLUDE_DIRECTORIES(${QT_INCLUDES} ${CMAKE_CURRENT_BINARY_DIR} ${CMAKE_CURRENT_SOURCE_DIR})

#MOC
QT4_WRAP_CPP(MOC_OUTPUTS ${MOC_FILES})
SOURCE_GROUP("moc_cxx" FILES ${MOC_OUTPUTS})

qt4_add_resources(RCC_OUTPUTS ${RESOURCES_FILES})

#指示变量DIR_SRCS中的源文件需要编译成可执行文件
ADD_EXECUTABLE(plotter 
               ${SOURCE_FILES}
               ${MOC_FILES}
               ${MOC_OUTPUTS}
               ${RCC_OUTPUTS}
               )

#指明可执行文件需要连接的链接库
TARGET_LINK_LIBRARIES(plotter ${QT_LIBRARIES})


