#dir: 
#datte: 2015-01-01
#author: zuokongxiao
#discription: qt cmake files

#工程名称
PROJECT(sort)

#CMAKE版本
CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

#QT
FIND_PACKAGE(Qt4 COMPONENTS QtCore QtGui REQUIRED)
INCLUDE(${QT_USE_FILE})

#输出用户定义信息
MESSAGE(STATUS "source dir: " ${PROJECT_SOURCE_DIR})
MESSAGE(STATUS "binary dir: " ${PROJECT_BINARY_DIR})

#编译器设置
if(WIN32)
    add_definitions(-D_CRT_SECURE_NO_DEPRECATE)
else(WIN32)
 IF(${TARGET_P} MATCHES "x86")
  SET(CMAKE_CXX_COMPILER g++)
 ELSE()
  SET(CMAKE_CXX_COMPILER arm-linux-g++)
 ENDIF()
 SET(CMAKE_CXX_FLAGS "-g -Wno-psabi")
endif(WIN32)
MESSAGE(STATUS "compiler: ${CMAKE_CXX_COMPILER}, flags: ${CMAKE_C_FLAGS}")

#执行文件名称
set(EXEC_NAME sortdialog)

#头文件
SET(HEADER_FILES 
    sortdialog.h
    )

#源文件
SET(SOURCE_FILES 
    main.cpp
    sortdialog.cpp
    )

#moc文件Q_OBJECT
set(MOC_FILES 
    sortdialog.h
    )

#ui文件
set(UI_FILES
    sortdialog.ui
    )

#向工程添加头文件搜索路径
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}
                    ${CMAKE_CURRENT_BINARY_DIR}
                    ${QT_INCLUDES}
                    )

#uic
qt4_wrap_ui(UI_OUTPUTS ${UI_FILES})

#MOC meta-object compiler
QT4_WRAP_CPP(MOC_OUTPUTS ${MOC_FILES})
#SOURCE_GROUP("moc_cxx" FILES ${MOC_OUTPUTS})

#可执行文件
ADD_EXECUTABLE(${EXEC_NAME} 
               ${SOURCE_FILES}
               ${MOC_OUTPUTS}
               ${UI_OUTPUTS}
               )

#指明可执行文件需要连接的链接库
TARGET_LINK_LIBRARIES(${EXEC_NAME} ${QT_LIBRARIES})


