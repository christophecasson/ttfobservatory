#!/bin/bash

INDI_PORT=7501
CNTRL_FIFO="/home/astro/fifo"


echo "### SHUTDOWN BEGIN ###"

echo -n "kill all running indiserver"
killall indiserver
sleep 2
echo " [ OK ]"

echo -n "Checking powerboard status..."
if [[ $(cat $CNTRL_FIFO/powerboard/status/board_state) = "OK" ]]
then
	echo " [ OK ]"
else
	echo " [ Error ]"
	echo "   -> Powerboard error, cannot continue. WARNING: OBSERVATORY MAY BE IN UNSAFE POSITION!"
	exit 1
fi

echo -n "Starting temporary indiserver..."
indiserver -p $INDI_PORT -l /home/astro/indilog -v indi_eqmod_telescope
sleep 3
echo " [ OK ]"

echo -n "Loading indi EQMod Mount parameters..."
indi_setprop -p $INDI_PORT "EQMod Mount.CONFIG_PROCESS.CONFIG_LOAD=On"
echo " [ OK ]"

echo -n "Checking Telescope Mount"
# if mount is OFF, power ON and check if Parked
# 	if previously OFF and unparked after power on -> ABORT AND WARN (mount in unknown position)
if [[ $(cat $CNTRL_FIFO/powerboard/status/2) = "0" ]] 
then #if mount is OFF
	echo " [ OFF ]"
	echo "   -> Power ON Mount to check park state..."	
	echo 1 > /home/astro/fifo/powerboard/control/2
	sleep 1
	if [[ $(cat $CNTRL_FIFO/powerboard/status/2) = "1" ]]
	then
		echo " [ OK ]"
	else
		echo " [ Error ]"
		echo "      -> Error powering Telescope Mount, cannot continue. WARNING: OBSERVATORY MAY BE IN UNSAFE POSITION!"
		exit 2
	fi
	sleep 1
	echo -n "   -> Connecting EQMod Mount..."
	indi_setprop -p $INDI_PORT "EQMod Mount.CONNECTION.CONNECT=On"
	sleep 1
	if [[ $(indi_getprop -p $INDI_PORT -1 "EQMod Mount.CONNECTION.CONNECT") != "On" ]]
	then
		echo " [ Error ]"
		echo "      -> Error connecting EQMod Mount, cannot continue. WARNING: OBSERVATORY MAY BE IN UNSAFE POSITION!"
		exit 3
	else
		echo " [ OK ]"
	fi
	echo -n "   -> Telescope Mount Park state:"
	if [[ $(indi_getprop -p $INDI_PORT -1 "EQMod Mount.TELESCOPE_PARK.PARK") = "On" ]]
	then
		echo " [ PARKED ]"	
	else
		echo " [ UNPARKED ]"
		echo "      -> Mount was powered off UNPARKED, it may be in UNKNOWN POSITION"
		echo "      -> cannot continue. WARNING: OBSERVATORY MAY BE IN UNSAFE POSITION!"
		exit 4
	fi

else #if mount is ON
	echo "[ ON ]"
	echo -n "   -> Connecting EQMod Mount..."
	indi_setprop -p $INDI_PORT "EQMod Mount.CONNECTION.CONNECT=On"
	sleep 1
	if [[ $(indi_getprop -p $INDI_PORT -1 "EQMod Mount.CONNECTION.CONNECT") != "On" ]]
	then
		echo " [ Error ]"
		echo "      -> Error connecting EQMod Mount, cannot continue. WARNING: OBSERVATORY MAY BE IN UNSAFE POSITION!"
		exit 5
	else
		echo " [ OK ]"
	fi
	echo -n "   -> Telescope Mount Park state:"
	if [[ $(indi_getprop -p $INDI_PORT -1 "EQMod Mount.TELESCOPE_PARK.PARK") = "On" ]]
	then
		echo " [ PARKED ]"	
	else
		echo " [ UNPARKED ]"
	fi
fi



if [[ $(indi_getprop -p $INDI_PORT -1 "EQMod Mount.TELESCOPE_PARK.PARK") = "On" ]]
then
	echo "Telescope is already PARKED"
else
	echo -n "Park Telescope Mount..."
	indi_setprop -p $INDI_PORT "EQMod Mount.TELESCOPE_PARK.PARK=On"
	sleep 1
	declare -i timeout=120
	while [[ $(indi_getprop -p $INDI_PORT -1 "EQMod Mount.TELESCOPE_PARK.PARK") != "On" ]]
	do
		sleep 1
		timeout=$timeout-1
		if [[ timeout = 0 ]]
		then
			echo " [ Error ]"
			echo "      -> Error parking EQMod Mount, cannot continue. WARNING: OBSERVATORY MAY BE IN UNSAFE POSITION!"
			exit 6
		fi
	done
	echo " [ OK ]"
fi

echo -n "Powering OFF Telescope Mount..."
echo 0 > /home/astro/fifo/powerboard/control/2
sleep 1
echo 0 > /home/astro/fifo/powerboard/control/2
sleep 1
echo 0 > /home/astro/fifo/powerboard/control/2
sleep 1
echo " [ OK ]"




echo -n "Closing roof..."
echo 1 > /home/astro/fifo/powerboard/control/1
sleep 1
echo 1 > /home/astro/fifo/powerboard/control/1
sleep 1
echo 1 > /home/astro/fifo/powerboard/control/1
sleep 1
echo "CLOSE" > $CNTRL_FIFO/roof/control/move
sleep 1
declare -i rooftimeout=120
while [[ $(cat $CNTRL_FIFO/roof/status/state) != "CLOSED" ]]
do
	sleep 1
	rooftimeout=$rooftimeout-1
	if [[ $(cat $CNTRL_FIFO/roof/status/state) != "CLOSING" ]]
	then
		echo "CLOSE" > $CNTRL_FIFO/roof/control/move
	fi
	if [[ $rooftimeout = 0 ]]
	then
		echo " [ Error ]"
		echo "   -> Error closing roof, cannot continue. WARNING: OBSERVATORY MAY BE IN UNSAFE POSITION!"	
		exit 7
	fi
done
echo " [ OK ]"
sleep 1

echo -n "Powering OFF roof..."
echo 0 > /home/astro/fifo/powerboard/control/1
sleep 1
echo 0 > /home/astro/fifo/powerboard/control/1
sleep 1
echo 0 > /home/astro/fifo/powerboard/control/1
sleep 1
echo " [ OK ]"


echo "Stopping indiserver..."
killall indiserver

echo -n "Powering OFF DSLR..."
echo 0 > /home/astro/fifo/powerboard/control/3
sleep 1
echo 0 > /home/astro/fifo/powerboard/control/3
sleep 1
echo 0 > /home/astro/fifo/powerboard/control/3
sleep 1
echo " [ OK ]"

echo -n "Powering OFF ALL powerboard outputs..."
echo 0 > /home/astro/fifo/powerboard/control/1
echo 0 > /home/astro/fifo/powerboard/control/2
echo 0 > /home/astro/fifo/powerboard/control/3
echo 0 > /home/astro/fifo/powerboard/control/4
echo 0 > /home/astro/fifo/powerboard/control/5
echo 0 > /home/astro/fifo/powerboard/control/6
echo " [ OK ]"

echo -n "Checking power board status..."
#powerboard must be OK and all OFF
if [[ $(cat $CNTRL_FIFO/powerboard/status/board_state) != "OK" ]]
then
	echo " [ Error ]"
	echo "  -> Powerboard error"
	exit 10
fi
if [[ $(cat $CNTRL_FIFO/powerboard/status/1) != "0" ]]
then
	echo " [ Error ]"
	echo "  -> Powerboard error: OUT 1 not powered OFF"
	exit 11
fi
if [[ $(cat $CNTRL_FIFO/powerboard/status/2) != "0" ]]
then
	echo " [ Error ]"
	echo "  -> Powerboard error: OUT 2 not powered OFF"
	exit 12
fi
if [[ $(cat $CNTRL_FIFO/powerboard/status/3) != "0" ]]
then
	echo " [ Error ]"
	echo "  -> Powerboard error: OUT 3 not powered OFF"
	exit 13
fi
if [[ $(cat $CNTRL_FIFO/powerboard/status/4) != "0" ]]
then
	echo " [ Error ]"
	echo "  -> Powerboard error: OUT 4 not powered OFF"
	exit 14
fi
if [[ $(cat $CNTRL_FIFO/powerboard/status/5) != "0" ]]
then
	echo " [ Error ]"
	echo "  -> Powerboard error: OUT 5 not powered OFF"
	exit 15
fi
if [[ $(cat $CNTRL_FIFO/powerboard/status/6) != "0" ]]
then
	echo " [ Error ]"
	echo "  -> Powerboard error: OUT 6 not powered OFF"
	exit 16
fi
echo " [ OK ]"


echo -n "Checking roof state..."
#roof closed lock must be OK
if [[ $(cat $CNTRL_FIFO/roof/status/closed) != "1" ]]
then
	echo " [ Error ]"
	echo "  -> Roof error: Closed lock not enabled"
	exit 20
fi
echo " [ OK ]"


echo "Observatory closed and shutdown!" 

echo "### SHUTDOWN END ###"
exit 0
