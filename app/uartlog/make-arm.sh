#!/bin/sh

echo "\r\n**************build common.***********"
cd $HOME/imx/app/common
pwd
cmake -D TARGET_P=arm ./
make

echo "\r\n********build uartlog project.********"
cd $HOME/imx/app/uartlog
pwd

if [ -f uartlog ]
then 
    rm uartlog
fi

cmake -D TARGET_P=arm ./

make -lpthread


