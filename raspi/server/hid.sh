#!/bin/bash
modprobe libcomposite
cd /sys/kernel/config/usb_gadget/
mkdir -p g1
cd g1
echo 0x1d6b > idVendor # Linux Foundation
echo 0x0104 > idProduct # Multifunction Composite Gadget
echo 0x0100 > bcdDevice # v1.0.0
echo 0x0200 > bcdUSB # USB2
mkdir -p strings/0x409
echo "S/N 000-00" > strings/0x409/serialnumber
echo "example.com" > strings/0x409/manufacturer
echo "Virtual USB Mouse" > strings/0x409/product
N="usb0"
mkdir -p functions/hid.$N
echo 2 > functions/hid.$N/protocol
echo 0 > functions/hid.$N/subclass
echo 4 > functions/hid.$N/report_length
# https://www.instructables.com/id/Use-pcDuino-as-a-Mouse-When-its-connected-to-a-PC-/
# https://roy-n-roy.github.io/Raspberry%20Pi/WebConsole/
echo -ne "\\x05\\x01\\x09\\x02\\xa1\\x01\\x09\\x01\\xa1\\x00\\x05\\x09\\x19\\x01\\x29\\x05\\x15\\x00\\x25\\x01\\x95\\x05\\x75\\x01\\x81\\x02\\x95\\x01\\x75\\x03\\x81\\x01\\x05\\x01\\x09\\x30\\x09\\x31\\x09\\x38\\x16\\x01\\x80\\x26\\xff\\x7f\\x75\\x10\\x95\\x03\\x81\\x06\\xc0\\xc0" > functions/hid.usb0/report_desc
C=1
mkdir -p configs/c.$C/strings/0x409
echo "Config $C: ECM network" > configs/c.$C/strings/0x409/configuration
echo 250 > configs/c.$C/MaxPower
ln -s functions/hid.$N configs/c.$C/
# End functions
ls /sys/class/udc > UDC
