#!/bin/sh

cd ./
pwd

if [ -f gotocelldialog ]
then 
    rm gotocelldialog
fi

cmake TARGET_P=arm ../

make


