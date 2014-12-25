#!/bin/sh

cd ./
pwd

if [ -f serialport ]
then 
    rm serialport
fi

cmake -D TARGET_P=arm ./

make


