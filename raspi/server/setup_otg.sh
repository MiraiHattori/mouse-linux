#!/bin/bash

if [ -z "$(grep dtoverlay=dwc2 /boot/config.txt)" ]; then
    echo "dtoverlay=dwc2" | sudo tee -a /boot/config.txt
fi
if [ -z "$(grep dwc2 /etc/modules)" ]; then
    echo "dwc2" | sudo tee -a /etc/modules
fi
