#!/bin/sh

echo "\r\n********build thermalprinter project.********"
cd ./
pwd

if [ -f thermalprinter ]
then 
    rm thermalprinter
fi

cmake -D TARGET_P=arm ./

make


