#!/bin/sh

echo "\r\n********build common.********"
cd $HOME/bb/app/common
pwd
cmake -D TARGET_P=x86 ./
make

echo "\r\n********build socket project.********"
cd $HOME/bb/app/socket
pwd

if [ -f client ]
then 
    rm client
fi

if [ -f server ]
then 
    rm server
fi

cmake -D TARGET_P=x86 ./

make -lpthread


