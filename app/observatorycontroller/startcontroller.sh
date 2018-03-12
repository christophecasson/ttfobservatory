#!/bin/sh

./powerboard-controller.py /dev/serial/by-path/pci-0000:00:1a.7-usb-0:6.4:1.0-port0 &
./roof-controller.py /dev/serial/by-path/pci-0000:00:1a.7-usb-0:6.3:1.0-port0 &

