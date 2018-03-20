#!/bin/bash

INDI_PORT=7500
CNTRL_FIFO="/home/astro/fifo"


echo "### START BEGIN ###"

echo -n "Checking power board status..."
if [[ $(cat $CNTRL_FIFO/powerboard/status/board_state) != "OK" ]]
then
	echo " [ ERROR ]"
	echo "   -> Powerboard state Error, ABORTING START!"
	exit 1
else
	if [[ $(cat $CNTRL_FIFO/powerboard/status/1) != "0" ]]
	then
		echo " [ ERROR ]"
		echo "   -> powerboard output 1 is ON, ABORTING START"
		exit 11
	fi
	if [[ $(cat $CNTRL_FIFO/powerboard/status/2) != "0" ]]
	then
		echo " [ ERROR ]"
		echo "   -> powerboard output 2 is ON, ABORTING START"
		exit 12
	fi
	if [[ $(cat $CNTRL_FIFO/powerboard/status/3) != "0" ]]
	then
		echo " [ ERROR ]"
		echo "   -> powerboard output 3 is ON, ABORTING START"
		exit 13
	fi

	echo " [ OK ]"
fi



echo -n "Checking indiserver status..."
if [[ $(indi_getprop -p $INDI_PORT > /dev/null 2>&1; echo $?) != 2 ]]  #if indiserver running on $INDI_PORT 
then
	echo " [ ERROR ]"
	echo "   -> INDI server already running on port $INDI_PORT, ABORTING START!"
	exit 2
else
	echo "[ OK ]"
fi



echo -n "Powering ON DSLR..."
echo 1 > /home/astro/fifo/powerboard/control/3
sleep 1
echo 1 > /home/astro/fifo/powerboard/control/3
sleep 1
echo 1 > /home/astro/fifo/powerboard/control/3
sleep 1
if [[ $(cat $CNTRL_FIFO/powerboard/status/3) != "1" ]]
then
	echo " [ ERROR ]"
	echo "   -> error powering ON DSLR, ABORTING START"
	exit 3
else
	echo " [ OK ]"	
fi

sleep 2

echo -n "Starting indiserver..."
# start indiserver as daemon
nohup indiserver -p $INDI_PORT -l /home/astro/indilog -v indi_eqmod_telescope indi_canon_ccd indi_asi_ccd indi_moonlite_focus indi_script_dome indi_wunderground_weather indi_joystick > /dev/null 2>&1 &

sleep 5

if [[ $(indi_getprop -p $INDI_PORT > /dev/null 2>&1; echo $?) = 0 ]]  #if indiserver running on $INDI_PORT 
then
	echo " [ OK ]"
else
	echo " [ ERROR ]"
	echo "   -> INDI server failed to start, ABORTING START!"

	killall indiserver
	sleep 1
	echo 0 > /home/astro/fifo/powerboard/control/3
	sleep 1

	exit 4
fi



sleep 1


echo -n "Loading indi devices parameters..."
indi_setprop -p $INDI_PORT "Dome Scripting Gateway.CONFIG_PROCESS.CONFIG_LOAD=On"
indi_setprop -p $INDI_PORT "EQMod Mount.CONFIG_PROCESS.CONFIG_LOAD=On"
indi_setprop -p $INDI_PORT "Canon DSLR EOS 50D.CONFIG_PROCESS.CONFIG_LOAD=On"
indi_setprop -p $INDI_PORT "ZWO CCD ASI120MM.CONFIG_PROCESS.CONFIG_LOAD=On"
indi_setprop -p $INDI_PORT "MoonLite.CONFIG_PROCESS.CONFIG_LOAD=On"
indi_setprop -p $INDI_PORT "WunderGround.CONFIG_PROCESS.CONFIG_LOAD=On"
indi_setprop -p $INDI_PORT "Joystick.CONFIG_PROCESS.CONFIG_LOAD=On"
echo " [ OK ]"

sleep 1

echo -n "Connecting Device: Joystick..."
indi_setprop -p $INDI_PORT "Joystick.CONNECTION.CONNECT=On"
sleep 1
if [[ $(indi_getprop -p $INDI_PORT -1 "Joystick.CONNECTION.CONNECT") != "On" ]]
then
	echo " [ ERROR ]"
	echo "   -> error Connecting device, ABORTING START"
	killall indiserver
	sleep 1
	echo 0 > /home/astro/fifo/powerboard/control/3
	sleep 1

	exit 30
else
	echo " [ OK ]"
fi

echo -n "Connecting Device: WunderGround..."
indi_setprop -p $INDI_PORT "WunderGround.CONNECTION.CONNECT=On"
sleep 1
if [[ $(indi_getprop -p $INDI_PORT -1 "WunderGround.CONNECTION.CONNECT") != "On" ]]
then
	echo " [ ERROR ]"
	echo "   -> error Connecting device, ABORTING START"
	killall indiserver
	sleep 1
	echo 0 > /home/astro/fifo/powerboard/control/3
	sleep 1

	exit 30
else
	echo " [ OK ]"
fi

echo -n "Connecting Device: MoonLite..."
indi_setprop -p $INDI_PORT "MoonLite.CONNECTION.CONNECT=On"
sleep 1
if [[ $(indi_getprop -p $INDI_PORT -1 "MoonLite.CONNECTION.CONNECT") != "On" ]]
then
	echo " [ ERROR ]"
	echo "   -> error Connecting device, ABORTING START"
	killall indiserver
	sleep 1
	echo 0 > /home/astro/fifo/powerboard/control/3
	sleep 1

	exit 30
else
	echo " [ OK ]"
fi

echo -n "Connecting Device: ZWO CCD ASI120MM..."
indi_setprop -p $INDI_PORT "ZWO CCD ASI120MM.CONNECTION.CONNECT=On"
sleep 1
if [[ $(indi_getprop -p $INDI_PORT -1 "ZWO CCD ASI120MM.CONNECTION.CONNECT") != "On" ]]
then
	echo " [ ERROR ]"
	echo "   -> error Connecting device, ABORTING START"
	killall indiserver
	sleep 1
	echo 0 > /home/astro/fifo/powerboard/control/3
	sleep 1

	exit 30
else
	echo " [ OK ]"
fi

echo -n "Connecting Device: Canon DSLR EOS 50D..."
indi_setprop -p $INDI_PORT "Canon DSLR EOS 50D.CONNECTION.CONNECT=On"
sleep 1
if [[ $(indi_getprop -p $INDI_PORT -1 "Canon DSLR EOS 50D.CONNECTION.CONNECT") != "On" ]]
then
	echo " [ ERROR ]"
	echo "   -> error Connecting device, ABORTING START"
	killall indiserver
	sleep 1
	echo 0 > /home/astro/fifo/powerboard/control/3
	sleep 1

	exit 30
else
	echo " [ OK ]"
fi






echo "Powering ON roof..."
#connect
#indi_setprop -p $INDI_PORT "Dome Scripting Gateway.CONNECTION.CONNECT=On"
echo "Checking roof..."
#roof must be PARKED and CLOSED

echo "Powering Telescope Mount..."
#connect
#indi_setprop -p $INDI_PORT "EQMod Mount.CONNECTION.CONNECT=On"
echo "Checking Telescope Mount..."
#mount must be PARKED (if not parked after power-on, mount may be in dangerous position -> shutdown and warn)

echo "Opening roof..."

echo "Slew mount to HOME..."




echo "Observatory ready!"

sleep 20

echo "shutting down..."
#park mount
#close roof
killall indiserver
echo 0 > /home/astro/fifo/powerboard/control/1
echo 0 > /home/astro/fifo/powerboard/control/2
echo 0 > /home/astro/fifo/powerboard/control/3
echo 0 > /home/astro/fifo/powerboard/control/4
echo 0 > /home/astro/fifo/powerboard/control/5
echo 0 > /home/astro/fifo/powerboard/control/6

echo "### START END ###"
exit 0
