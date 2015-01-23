#datte: 2015-01-23
#author: zuokongxiao

#头文件
SET(HEADER_FILES
    temperaturewin.h
    tempsettingwin.h
    )

#源文件
SET(SOURCE_FILES
    temperaturewin.cpp
    tempsettingwin.cpp
    )

#moc文件Q_OBJECT
set(MOC_FILES
    temperaturewin.h
    tempsettingwin.h
    )

#ui文件
set(UI_FILES
    temperaturewin.ui
    tempsettingwin.ui
    )

#包含目录
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}
                    ${CMAKE_CURRENT_BINARY_DIR}
                    ${QT_INCLUDES}
                    ${CMAKE_CURRENT_SOURCE_DIR}/../qcustomplot
                    )

#uic
qt4_wrap_ui(UI_OUTPUTS ${UI_FILES})

#MOC meta-object compiler
QT4_WRAP_CPP(MOC_OUTPUTS ${MOC_FILES})
#SOURCE_GROUP("moc_cxx" FILES ${MOC_OUTPUTS})

#编译成一个可执行文件
ADD_LIBRARY(temperature ${SOURCE_FILES} ${MOC_OUTPUTS} ${UI_OUTPUTS})

#指明可执行文件需要连接的链接库
TARGET_LINK_LIBRARIES(temperature qcustomplot ${QT_LIBRARIES})
