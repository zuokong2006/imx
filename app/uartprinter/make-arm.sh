#!/bin/sh

echo "\r\n**************build common.***********"
cd $HOME/imx/app/common
pwd
cmake -D TARGET_P=arm ./
make

echo "\r\n********build uartprinter project.********"
cd $HOME/imx/app/uartprinter
pwd

if [ -f uartprinter ]
then 
    rm uartprinter
fi

cmake -D TARGET_P=arm ./

make -lpthread


