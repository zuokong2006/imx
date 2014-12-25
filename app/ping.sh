#!/bin/sh

echo "\r\nping 192.168.1.1"
while [ "1" = "1" ]
do
    ping -i 1 www.baidu.com
done



