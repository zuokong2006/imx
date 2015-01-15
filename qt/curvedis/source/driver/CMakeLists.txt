#datte: 2015-01-10
#author: zuokongxiao

#将当前目录中的源文件名称赋值给变量DIR_SRCS
AUX_SOURCE_DIRECTORY(. DIR_SRCS)

#指示变量DIR_SRCS中的源文件需要编译成一个可执行文件
ADD_LIBRARY(driver ${DIR_SRCS})
TARGET_LINK_LIBRARIES(driver ${QT_LIBRARIES})
