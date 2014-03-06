#!/bin/bash
set -e
stty 115200 < /dev/ttyUSB0
stty cs8 -hupcl cread clocal -icrnl -onlcr -echo < /dev/ttyUSB0
stty ignbrk ignpar -ixon < /dev/ttyUSB0
stty -opost < /dev/ttyUSB0
stty -isig -icanon -iexten -echoe -echok < /dev/ttyUSB0
stty -echoctl -echoke < /dev/ttyUSB0
stty min 1 < /dev/ttyUSB0
echo -n "w" > /dev/ttyUSB0
cat /dev/ttyUSB0
# echo -n "x" > /dev/ttyUSB0
