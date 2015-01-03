#!/bin/sh

cd ./
pwd

if [ -f spreadsheet ]
then 
    rm spreadsheet
fi

cmake TARGET_P=arm ../

make


