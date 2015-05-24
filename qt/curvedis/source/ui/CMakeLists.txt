#datte: 2015-01-20
#author: zuokongxiao

#头文件
SET(HEADER_FILES
    mainwindow.h
    )

#源文件
SET(SOURCE_FILES
    mainwindow.cpp
    )

#moc文件Q_OBJECT
set(MOC_FILES
    mainwindow.h
    )

#ui文件
set(UI_FILES
    mainwindow.ui
    )

MESSAGE(STATUS "ui binary dir: " ${CMAKE_CURRENT_BINARY_DIR})

#包含目录
INCLUDE_DIRECTORIES(${QT_INCLUDES}
                    ${CMAKE_CURRENT_BINARY_DIR}
                    ${CMAKE_CURRENT_SOURCE_DIR}
                    ${CMAKE_CURRENT_SOURCE_DIR}/temperature
                    ${CMAKE_CURRENT_SOURCE_DIR}/upgrade
                    ${CMAKE_CURRENT_SOURCE_DIR}/smartcard
                    )

#包含子目录
ADD_SUBDIRECTORY(qcustomplot)
ADD_SUBDIRECTORY(temperature)
ADD_SUBDIRECTORY(upgrade)
ADD_SUBDIRECTORY(smartcard)

#uic
qt4_wrap_ui(UI_OUTPUTS ${UI_FILES})

#MOC meta-object compiler
QT4_WRAP_CPP(MOC_OUTPUTS ${MOC_FILES})
#SOURCE_GROUP("moc_cxx" FILES ${MOC_OUTPUTS})

#编译成一个可执行文件
ADD_LIBRARY(ui ${SOURCE_FILES} ${MOC_OUTPUTS} ${UI_OUTPUTS})

#指明可执行文件需要连接的链接库
TARGET_LINK_LIBRARIES(ui hal temperature qcustomplot upgrade smartcard ${QT_LIBRARIES})
