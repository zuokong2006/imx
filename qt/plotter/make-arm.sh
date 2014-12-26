#!/bin/sh

cd ./
pwd

if [ -f plotter ]
then 
    rm plotter
fi

#内部构建
cmake -D TARGET_P=arm ./

make


