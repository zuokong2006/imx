#dir: app/file
#datte: 2014-06-13
#author: zuokongxiao

#向工程添加头文件搜索路径
INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/../include)

#将当前目录中的源文件名称赋值给变量DIR_SRCS
AUX_SOURCE_DIRECTORY(. DIR_SRCS)

#指示变量DIR_SRCS中的源文件需要编译成一个名称为test的可执行文件
ADD_LIBRARY(test ${DIR_SRCS})
