#!/bin/bash

INDI_PORT=7500
CNTRL_FIFO="/home/astro/fifo"

declare -i timeout=0

echo "### SHUTDOWN BEGIN ###"

../tools/notificationtoIFTTT.sh 'Observatory shutdown started' > /dev/null 2>&1

echo -n "Checking powerboard status..."
timeout=60
while [[ $(cat $CNTRL_FIFO/powerboard/status/board_state) != "OK" ]]
do
    echo -n "+"
    sleep 1
    timeout=$timeout-1
    if [[ $timeout = 0 ]]
    then
	    echo " [ Error ]"
	    echo "   -> Powerboard error, cannot continue. WARNING: OBSERVATORY MAY BE IN UNSAFE POSITION!"
	    exit 1
    fi
done
echo " [ OK ]"


echo -n "Checking indiserver status..."
timeout=60
tempindiserver=0
while [[ $(indi_getprop -p $INDI_PORT > /dev/null 2>&1; echo $?) -eq 2 ]]  #if indiserver not running on $INDI_PORT
do
    echo " [ STOPPED ]"
    killall indiserver > /dev/null 2>&1
    echo -n "   -> Launching temporary indiserver..."
    indiserver -p $INDI_PORT -v indi_eqmod_telescope > /dev/null 2>&1 &
    tempindiserver=1
    sleep 1
    timeout=$timeout-1
    if [[ $timeout = 0 ]]
    then
        echo "      -> Error starting indiserver, cannot continue. WARNING: OBSERVATORY MAY BE IN UNSAFE POSITION!"
	    exit 2
    fi
done
echo " [ RUNNING ]"



echo -n "Loading indi EQMod Mount parameters..."
indi_setprop -p $INDI_PORT "EQMod Mount.CONFIG_PROCESS.CONFIG_LOAD=On"
sleep 3
echo " [ OK ]"

echo -n "Checking Telescope Mount"
# if mount is OFF, power ON and check if Parked
# 	if previously OFF and unparked after power on -> ABORT AND WARN (mount in unknown position)
if [[ $(cat $CNTRL_FIFO/powerboard/status/2) == "0" ]]
then #if mount is OFF
	echo " [ OFF ]"
	echo -n "   -> Power ON Mount to check park state..."
    timeout=60
    while [[ $(cat $CNTRL_FIFO/powerboard/status/2) != "1" ]]
    do
        echo 1 > /home/astro/fifo/powerboard/control/2
        sleep 1
        timeout=$timeout-1
        if [[ $timeout = 0 ]]
        then
            echo " [ Error ]"
	        echo "  -> Error powering Telescope Mount, cannot continue. WARNING: OBSERVATORY MAY BE IN UNSAFE POSITION!"
	        exit 2
        fi
    done
    echo " [ OK ]"

	sleep 1
	echo -n "   -> Connecting EQMod Mount..."
    timeout=60
	while [[ $(indi_getprop -p $INDI_PORT -1 "EQMod Mount.CONNECTION.CONNECT") != "On" ]]
	do
        indi_setprop -p $INDI_PORT "EQMod Mount.CONNECTION.CONNECT=On"
        sleep 1
        timeout=$timeout-1
        if [[ $timeout = 0 ]]
        then
            echo " [ Error ]"
		    echo "      -> Error connecting EQMod Mount, cannot continue. WARNING: OBSERVATORY MAY BE IN UNSAFE POSITION!"
		    exit 3
        fi
    done
	echo " [ OK ]"

	echo -n "   -> Telescope Mount Park state:"
	if [[ $(indi_getprop -p $INDI_PORT -1 "EQMod Mount.TELESCOPE_PARK.PARK") == "On" ]]
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
    timeout=60
	while [[ $(indi_getprop -p $INDI_PORT -1 "EQMod Mount.CONNECTION.CONNECT") != "On" ]]
	do
        indi_setprop -p $INDI_PORT "EQMod Mount.CONNECTION.CONNECT=On"
        sleep 1
        timeout=$timeout-1
        if [[ $timeout = 0 ]]
        then
            echo " [ Error ]"
		    echo "      -> Error connecting EQMod Mount, cannot continue. WARNING: OBSERVATORY MAY BE IN UNSAFE POSITION!"
		    exit 3
        fi
    done
	echo " [ OK ]"


	echo -n "   -> Telescope Mount Park state:"
	if [[ $(indi_getprop -p $INDI_PORT -1 "EQMod Mount.TELESCOPE_PARK.PARK") == "On" ]]
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
	parkRA=`indi_getprop -p $INDI_PORT -1 "EQMod Mount.TELESCOPE_PARK_POSITION.PARK_RA"`
	parkDEC=`indi_getprop -p $INDI_PORT -1 "EQMod Mount.TELESCOPE_PARK_POSITION.PARK_DEC"`
	indi_eval -t 120 -p $INDI_PORT -wo '"EQMod Mount.CURRENTSTEPPERS.RAStepsCurrent"=='$parkRA' && "EQMod Mount.CURRENTSTEPPERS.DEStepsCurrent"=='$parkDEC
	echo " [ OK ]"
fi




echo -n "Checking Telescope Mount..."
if [[ $(indi_getprop -p $INDI_PORT -1 "EQMod Mount.TELESCOPE_PARK.PARK") = "On" ]]
then
        echo " [ OK ]"
        sleep 1
        currentRA=`indi_getprop -p $INDI_PORT -1 "EQMod Mount.CURRENTSTEPPERS.RAStepsCurrent"`
        currentDEC=`indi_getprop -p $INDI_PORT -1 "EQMod Mount.CURRENTSTEPPERS.DEStepsCurrent"`
        parkRA=`indi_getprop -p $INDI_PORT -1 "EQMod Mount.TELESCOPE_PARK_POSITION.PARK_RA"`
        parkDEC=`indi_getprop -p $INDI_PORT -1 "EQMod Mount.TELESCOPE_PARK_POSITION.PARK_DEC"`

        echo -n "   -> RA=$currentRA (parked at $parkRA)"
        if [[ $currentRA -lt $((parkRA+100)) ]] && [[ $currentRA -gt $((parkRA-100)) ]]
        then
                echo " [ RA OK ]"
        else
                echo " [ RA NOT PARKED ]"
                echo "   -> Telescope mount is not PARKED, ABORTING SHUTDOWN"
                exit 61
        fi
        echo -n "   -> DEC=$currentDEC (parked at $parkDEC)"
        if [[ $currentDEC -lt $((parkDEC+100)) ]] && [[ $currentDEC -gt $((parkDEC-100)) ]]
        then
                echo " [ DEC OK ]"
        else
                echo " [ DEC NOT PARKED ]"
                echo "   -> Telescope mount is not PARKED, ABORTING SHUTDOWN"
                exit 62
        fi

        #if [[ $currentRA = $parkRA ]] && [[ $currentDEC = $parkDEC ]]
        if [[ $currentRA -lt $((parkRA+100)) ]] && [[ $currentRA -gt $((parkRA-100)) ]] && [[ $currentDEC -lt $((parkDEC+100)) ]] && [[ $currentDEC -gt $((parkDEC-100)) ]]
        then
                echo "Mount is parked!"
        else
                echo "Mount is not in PARK position, ABORTING START"
                exit 63
        fi

else
        echo " [ ERROR ]"
        echo "   -> Telescope mount is not PARKED, ABORTING SHUTDOWN"
        exit 60
fi

echo -n "Closing roof..."
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



echo -n "Disconnect devices..."
indi_setprop -p $INDI_PORT "Dome Scripting Gateway.CONNECTION.DISCONNECT=On"
indi_setprop -p $INDI_PORT "EQMod Mount.CONNECTION.DISCONNECT=On"
indi_setprop -p $INDI_PORT "Canon DSLR EOS 50D.CONNECTION.DISCONNECT=On"
indi_setprop -p $INDI_PORT "ZWO CCD ASI120MM.CONNECTION.DISCONNECT=On"
indi_setprop -p $INDI_PORT "MoonLite.CONNECTION.DISCONNECT=On"
indi_setprop -p $INDI_PORT "WunderGround.CONNECTION.DISCONNECT=On"
indi_setprop -p $INDI_PORT "Joystick.CONNECTION.DISCONNECT=On"
sleep 1
echo " [ OK ]"






echo -n "Powering OFF ALL powerboard outputs..."

timeout=60
while [[ $(cat $CNTRL_FIFO/powerboard/status/board_state) != "OK" ]]
do
    echo -n "+"
    sleep 1
    timeout=$timeout-1
    if [[ $timeout = 0 ]]
    then
	    echo " [ Error ]"
	    echo "   -> Powerboard error"
	    exit 1
    fi
done

for pwrboardout in '1' '2' '3' '4' '5' '6'
do
    timeout=60
    while [[ $(cat $CNTRL_FIFO/powerboard/status/$pwrboardout) != "0" ]]
    do
        echo 0 > /home/astro/fifo/powerboard/control/$pwrboardout
        sleep 1
        timeout=$timeout-1
        if [[ $timeout = 0 ]]
        then
            echo " [ Error ]"
	        echo "  -> Powerboard error: OUT $pwrboardout not powered OFF"
	        exit 11
        fi
    done
done
echo "[ OK ]"

echo -n "Checking roof state..."
#roof closed lock must be OK
if [[ $(cat $CNTRL_FIFO/roof/status/closed) != "1" ]]
then
	echo " [ Error ]"
	echo "  -> Roof error: Closed lock not enabled"
	exit 20
fi
echo " [ OK ]"


if [ $tempindiserver -eq 1 ]
then
    echo "killing temporary indiserver..."
    killall indiserver > /dev/null 2>&1
fi

echo "Observatory closed and shutdown!"
../tools/notificationtoIFTTT.sh 'Observatory closed and shutdown!' > /dev/null 2>&1

echo "### SHUTDOWN END ###"

exit 0
