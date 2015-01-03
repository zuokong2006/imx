#!/bin/sh

cd ./
pwd

if [ -f sortdialog ]
then 
    rm sortdialog
fi

cmake TARGET_P=arm ../

make


