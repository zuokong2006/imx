#!/bin/sh

cd ./
pwd

if [ -f finddialog ]
then 
    rm finddialog
fi

cmake TARGET_P=arm ../

make


