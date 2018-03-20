#!/bin/bash

echo "### SHUTDOWN BEGIN ###"

echo "checking indiserver status..."
#if indiserver not running, start it to park telescope

echo "Checking powerboard status..."
#powerboard must be OK, roof and mount must be ON
# if mount is OFF, power ON and check if Parked
# 	if previously OFF and unparked after power on -> ABORT AND WARN (mount in unknown position)

echo "Closing Dust Cap..."

echo "Park Telescope Mount..."

#check if telescope mount is PARKED
echo "Powering OFF Telescope Mount..."


echo "Closing roof..."

#check if roof is closed
echo "Powering OFF roof..."


echo "Stopping indiserver..."


echo "Powering OFF DSLR..."


echo "Checking power board status..."
#powerboard must be OK and all OFF

echo "Checking roof state..."
#roof closed lock must be OK

echo "Observatory closed and shutdown!" 

echo "### SHUTDOWN END ###"
exit 0
