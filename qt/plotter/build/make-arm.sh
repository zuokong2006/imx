#!/bin/sh

cd ./
pwd

if [ -f plotter ]
then 
    rm plotter
fi

#外部构建，取消-D
cmake TARGET_P=arm ../

make


