#!/bin/sh

cd ./
pwd

if [ -f hello ]
then 
    rm hello
fi

cmake TARGET_P=arm ../

make


