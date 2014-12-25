#!/bin/sh

cd ./
pwd

if [ -f hello ]
then 
    rm hello
fi

cmake -D TARGET_P=arm ./

make


