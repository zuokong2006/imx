#datte: 2015-01-14
#author: zuokongxiao

#FIND_PACKAGE(Qt4 COMPONENTS QtCore QtGui REQUIRED)
#INCLUDE(${QT_USE_FILE})

#头文件
SET(HEADER_FILES
    nodecomm.h
    nodelistener.h
    nodeprotocol.h
    nodeprotocolres.h
    nodeprotocolobject.h
    halcomm.h
    haltemperature.h
    halupgrade.h
    halsmartcard.h
    )

#源文件
SET(SOURCE_FILES
    nodecomm.cpp
    nodeprotocol.cpp
    nodelistener.cpp
    nodeprotocolobject.cpp
    halcomm.cpp
    haltemperature.cpp
    halupgrade.cpp
    halsmartcard.cpp
    )

#moc文件Q_OBJECT
set(MOC_FILES
    nodecomm.h
    nodelistener.h
    )

#包含目录
INCLUDE_DIRECTORIES(./
                    ${CMAKE_CURRENT_SOURCE_DIR}/../driver
                    )

#MOC meta-object compiler
QT4_WRAP_CPP(MOC_OUTPUTS ${MOC_FILES})
#SOURCE_GROUP("moc_cxx" FILES ${MOC_OUTPUTS})

#编译成一个可执行文件
ADD_LIBRARY(hal ${SOURCE_FILES} ${MOC_OUTPUTS})

#指明可执行文件需要连接的链接库
TARGET_LINK_LIBRARIES(hal driver ${QT_LIBRARIES})
