#!/bin/sh

echo "\r\n**************build common.***********"
cd $HOME/imx/app/common
pwd
cmake -D TARGET_P=arm ./
make

echo "\r\n********build cantest project.********"
cd $HOME/imx/app/uart
pwd

if [ -f uarttest ]
then 
    rm uarttest
fi

cmake -D TARGET_P=arm ./

make -lpthread


