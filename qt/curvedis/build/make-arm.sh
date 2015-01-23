#!/bin/sh

cd ./
pwd

if [ -f curvedis ]
then
    rm curvedis
fi

cmake TARGET_P=arm ../

make


