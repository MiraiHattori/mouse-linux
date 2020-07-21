echo -ne "\x1\x0\x0" | sudo tee /dev/hidg0
echo "po"
sleep 1
echo -ne "\x2\x0\x0" | sudo tee /dev/hidg0
echo "po2"
sleep 1
echo -ne "\x0\x0\x0" | sudo tee /dev/hidg0
echo "po3"
