echo -ne "\x1\0\0\0\0\0\0" | sudo tee /dev/hidg0
echo "po"
sleep 1
echo -ne "\x2\0\0\0\0\0\0" | sudo tee /dev/hidg0
echo "po2"
sleep 1
echo -ne "\0\0\0\0\0\0\0" | sudo tee /dev/hidg0
echo "po3"
