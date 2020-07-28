#!/bin/bash

sudo systemctl stop serial-getty@ttyAMA0.service
sudo systemctl disable serial-getty@ttyAMA0.service
if [ -z "$(grep "enable_uart=1" /boot/config.txt)" ]; then
    echo "enable_uart=1" >> /boot/config.txt
fi
if [ -z "$(grep "dtoverlay=pi3-miniuart-bt" /boot/config.txt)" ]; then
    echo "dtoverlay=pi3-miniuart-bt" >> /boot/config.txt
fi
sudo sed -i 's/console=serial0,115200 //g' /boot/config.txt
