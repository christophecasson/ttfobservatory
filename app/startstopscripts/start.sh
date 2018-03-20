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

	exit 31
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

	exit 32
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

	exit 33
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

	exit 34
else
	echo " [ OK ]"
fi






echo -n "Powering ON roof..."
echo 1 > /home/astro/fifo/powerboard/control/1
sleep 1
echo 1 > /home/astro/fifo/powerboard/control/1
sleep 1
echo 1 > /home/astro/fifo/powerboard/control/1
sleep 1
if [[ $(cat $CNTRL_FIFO/powerboard/status/1) != "1" ]]
then
	echo " [ ERROR ]"
	echo "   -> error powering ON Roof, ABORTING START"
	killall indiserver
	sleep 1
	echo 0 > /home/astro/fifo/powerboard/control/3
	sleep 1
	echo 0 > /home/astro/fifo/powerboard/control/1
	sleep 1
	
	exit 5
else
	echo " [ OK ]"	
fi

sleep 2

echo -n "Connecting Device: Dome Scripting Gateway..."
indi_setprop -p $INDI_PORT "Dome Scripting Gateway.CONNECTION.CONNECT=On"
sleep 1
if [[ $(indi_getprop -p $INDI_PORT -1 "Dome Scripting Gateway.CONNECTION.CONNECT") != "On" ]]
then
	echo " [ ERROR ]"
	echo "   -> error Connecting device, ABORTING START"
	killall indiserver
	sleep 1
	echo 0 > /home/astro/fifo/powerboard/control/3
	sleep 1
	echo 0 > /home/astro/fifo/powerboard/control/1
	sleep 1

	exit 35
else
	echo " [ OK ]"
fi

sleep 1

echo -n "Checking roof..."
#roof must be PARKED and CLOSED
if [[ $(cat $CNTRL_FIFO/roof/status/state) = "CLOSED" ]] && [[ $(indi_getprop -p $INDI_PORT -1 "Dome Scripting Gateway.DOME_PARK.PARK") = "On" ]] && [[ $(indi_getprop -p $INDI_PORT -1 "Dome Scripting Gateway.DOME_SHUTTER.SHUTTER_CLOSE") = "On" ]]
then
	echo " [ OK ]"	
else
	echo " [ ERROR ]"
	echo "   -> Roof is not CLOSED, ABORTING START"
	killall indiserver
	sleep 1
	echo 0 > /home/astro/fifo/powerboard/control/3
	sleep 1
	echo 0 > /home/astro/fifo/powerboard/control/1
	sleep 1
	
	exit 50
fi









echo -n "Powering Telescope Mount..."
echo 1 > /home/astro/fifo/powerboard/control/2
sleep 1
echo 1 > /home/astro/fifo/powerboard/control/2
sleep 1
echo 1 > /home/astro/fifo/powerboard/control/2
sleep 1
if [[ $(cat $CNTRL_FIFO/powerboard/status/2) != "1" ]]
then
	echo " [ ERROR ]"
	echo "   -> error powering ON Telescope Mount, ABORTING START"
	killall indiserver
	sleep 1
	echo 0 > /home/astro/fifo/powerboard/control/3
	sleep 1
	echo 0 > /home/astro/fifo/powerboard/control/1
	sleep 1
	echo 0 > /home/astro/fifo/powerboard/control/2
	sleep 1
	
	exit 6
else
	echo " [ OK ]"	
fi

sleep 2

echo -n "Connecting Device: EQMod Mount..."
indi_setprop -p $INDI_PORT "EQMod Mount.CONNECTION.CONNECT=On"
sleep 1
if [[ $(indi_getprop -p $INDI_PORT -1 "EQMod Mount.CONNECTION.CONNECT") != "On" ]]
then
	echo " [ ERROR ]"
	echo "   -> error Connecting device, ABORTING START"
	killall indiserver
	sleep 1
	echo 0 > /home/astro/fifo/powerboard/control/3
	sleep 1
	echo 0 > /home/astro/fifo/powerboard/control/1
	sleep 1
	echo 0 > /home/astro/fifo/powerboard/control/2
	sleep 1

	exit 36
else
	echo " [ OK ]"
fi

sleep 1

echo -n "Checking Telescope Mount..."
#mount must be PARKED (if not parked after power-on, mount may be in dangerous position -> shutdown and warn)
if [[ $(indi_getprop -p $INDI_PORT -1 "EQMod Mount.TELESCOPE_PARK.PARK") = "On" ]]
then
	echo " [ OK ]"
else
	echo " [ ERROR ]"
	echo "   -> Telescope mount is not PARKED after power on [ it may be in UNKNOWN POSITION ], ABORTING START"
	killall indiserver
	sleep 1
	echo 0 > /home/astro/fifo/powerboard/control/3
	sleep 1
	echo 0 > /home/astro/fifo/powerboard/control/1
	sleep 1
	echo 0 > /home/astro/fifo/powerboard/control/2
	sleep 1
	
	exit 60
fi






sleep 3

echo -n "Opening roof..."
indi_setprop -p $INDI_PORT "Dome Scripting Gateway.DOME_PARK.UNPARK=On"
sleep 1
declare -i timeout=120
while [[ $(indi_getprop -p $INDI_PORT -1 "Dome Scripting Gateway.DOME_PARK.UNPARK") = "On" ]] && [[ $(indi_getprop -p $INDI_PORT -1 "Dome Scripting Gateway.DOME_SHUTTER.SHUTTER_OPEN") = "On" ]]
do
	sleep 1
	timeout=$timeout-1
	if [[ $(cat $CNTRL_FIFO/roof/status/state) != "OPENING" ]]
	then
		echo "OPEN" > $CNTRL_FIFO/roof/control/move
	fi
	if [[ $timeout = 0 ]]
	then
		echo " [ ERROR ]"
		echo "   -> Roof opening Timeout, ABORTING START"

		echo "Shutdown observatory!"
		#launch shutdown script		

		exit 80
	fi
done
echo " [ OK ]"

sleep 3


echo -n "Unparking Telescope Mount..."
if [[ $(indi_getprop -p $INDI_PORT -1 "Dome Scripting Gateway.DOME_PARK.PARK") = "Off" ]] && [[ $(indi_getprop -p $INDI_PORT -1 "Dome Scripting Gateway.DOME_SHUTTER.SHUTTER_CLOSE") = "Off" ]]
then
	echo -n "Roof is Opened, Unparking Mount..."
	indi_setprop -p $INDI_PORT "EQMod Mount.TELESCOPE_PARK.UNPARK=On"
	sleep 1
	if [[ $(indi_getprop -p $INDI_PORT -1 "EQMod Mount.TELESCOPE_PARK.UNPARK") = "On" ]]
	then
		echo " [ OK ]"
	else
		echo " [ Error ]"
		echo "   -> Failed to unpark mount, ABORTING START"
		
		echo "Shutdown observatory!"
		#launch shutdown script		

		exit 82
	fi
else
	echo " [ Error ]"
	echo "   -> Failed to open roof, ABORTING START"

	echo "Shutdown observatory!"
	#launch shutdown script		

		exit 81
fi



echo -n "Slew mount to HOME..."









echo "Observatory ready!"

echo "### START END ###"
exit 0
