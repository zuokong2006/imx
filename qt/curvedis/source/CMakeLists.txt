#datte: 2015-01-10
#author: zuokongxiao

#QT
FIND_PACKAGE(Qt4 COMPONENTS QtCore QtGui REQUIRED)
INCLUDE(${QT_USE_FILE})

#执行文件名称
set(EXEC_NAME curvedis)

#向工程添加头文件搜索路径
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}
                    ${QT_INCLUDES}
                    )

#包含子目录
ADD_SUBDIRECTORY(driver)
#ADD_SUBDIRECTORY(hal)
#ADD_SUBDIRECTORY(ui)

#源文件
SET(SOURCE_FILES 
    main.cpp
    #finddialog.cpp
    )

#可执行文件
ADD_EXECUTABLE(${EXEC_NAME} 
               ${SOURCE_FILES}
               #${MOC_OUTPUTS}
               )

#指明可执行文件需要连接的链接库
TARGET_LINK_LIBRARIES(${EXEC_NAME} ${QT_LIBRARIES})




