#datte: 2015-01-10
#author: zuokongxiao

#QT
FIND_PACKAGE(Qt4 COMPONENTS QtCore QtGui REQUIRED)
SET(QT_USE_QTXML TRUE)
SET(QT_USE_QTNETWORK TRUE)
INCLUDE(${QT_USE_FILE})

#执行文件名称
set(EXEC_NAME curvedis)

#向工程添加头文件搜索路径
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}
                    ${CMAKE_CURRENT_BINARY_DIR}
                    ${QT_INCLUDES}
                    ${CMAKE_CURRENT_SOURCE_DIR}/driver
                    ${CMAKE_CURRENT_SOURCE_DIR}/hal
                    ${CMAKE_CURRENT_SOURCE_DIR}/ui
                    )

#包含子目录
ADD_SUBDIRECTORY(driver)
ADD_SUBDIRECTORY(hal)
ADD_SUBDIRECTORY(ui)

#源文件
SET(SOURCE_FILES
    main.cpp
    )

#可执行文件
ADD_EXECUTABLE(${EXEC_NAME}
               ${SOURCE_FILES}
               )

#指明可执行文件需要连接的链接库
TARGET_LINK_LIBRARIES(${EXEC_NAME} driver hal ui)
TARGET_LINK_LIBRARIES(${EXEC_NAME} ${QT_LIBRARIES})




