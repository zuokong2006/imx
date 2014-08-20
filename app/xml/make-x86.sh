#!/bin/sh

echo "\r\n********build common.********"
cd $HOME/bb/app/common
pwd
cmake -D TARGET_P=x86 ./
make

echo "\r\n********build xml project.********"
cd $HOME/bb/app/xml
pwd

if [ -f testxml ]
then 
    rm testxml
fi

if [ -f parsexmlfile ]
then 
    rm parsexmlfile
fi

if [ -f createxmlfile ]
then 
    rm createxmlfile
fi

cmake -D TARGET_P=x86 ./

make -lpthread


