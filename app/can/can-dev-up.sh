#!/bin/sh
ifconfig can0 down
echo 500000 > /sys/devices/platform/FlexCAN.0/bitrate
echo "can0 bitrate: "
cat /sys/devices/platform/FlexCAN.0/bitrate
echo "bit/s\r\n"
ifconfig can0 up 