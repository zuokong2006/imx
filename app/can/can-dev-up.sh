#!/bin/sh
ifconfig can0 down

#设置CAN波特率：500Kbit/s
echo 500000 > /sys/devices/platform/FlexCAN.0/bitrate

#获取设置的波特率
echo "can0 bitrate: "
cat /sys/devices/platform/FlexCAN.0/bitrate
echo "bit/s\r\n"

#开启CAN设备
ifconfig can0 up 