#!/bin/bash

INDI_PORT=7500
CNTRL_FIFO="/home/astro/fifo"


echo "### START BEGIN ###"
/home/astro/notificationtoIFTTT.sh 'Observatory startup begin'

echo -n "Checking indiserver status..."
if [[ $(indi_getprop -p $INDI_PORT > /dev/null 2>&1; echo $?) != 0 ]]  #if indiserver not running on $INDI_PORT 
then
	echo " [ ERROR ]"
	echo "   -> INDI server not running on port $INDI_PORT, ABORTING START!"
	exit 1
else
	echo "[ OK ]"
fi

echo -n "Checking power board status..."
if [[ $(cat $CNTRL_FIFO/powerboard/status/board_state) != "OK" ]]
then
	echo " [ ERROR ]"
	echo "   -> Powerboard state Error, ABORTING START!"
	exit 2
else
	echo " [ OK ]"
fi

echo -n "Powering ON Focuser..."
echo 1 > /home/astro/fifo/powerboard/control/4
sleep 1
if [[ $(cat $CNTRL_FIFO/powerboard/status/4) != "1" ]]
then
	echo " [ ERROR ]"
	echo "   -> error powering ON Focuser, ABORTING START"
	exit 3
else
	echo " [ OK ]"	
fi

echo -n "Powering ON DSLR..."
echo 1 > /home/astro/fifo/powerboard/control/3
sleep 1
if [[ $(cat $CNTRL_FIFO/powerboard/status/3) != "1" ]]
then
	echo " [ ERROR ]"
	echo "   -> error powering ON DSLR, ABORTING START"
	exit 4
else
	echo " [ OK ]"	
fi

echo -n "Powering ON Mount..."
echo 1 > /home/astro/fifo/powerboard/control/2
sleep 1
if [[ $(cat $CNTRL_FIFO/powerboard/status/2) != "1" ]]
then
	echo " [ ERROR ]"
	echo "   -> error powering ON Mount, ABORTING START"
	exit 5
else
	echo " [ OK ]"	
fi

echo -n "Powering ON Roof..."
echo 1 > /home/astro/fifo/powerboard/control/1
sleep 1
if [[ $(cat $CNTRL_FIFO/powerboard/status/1) != "1" ]]
then
	echo " [ ERROR ]"
	echo "   -> error powering ON Roof, ABORTING START"
	exit 6
else
	echo " [ OK ]"	
fi



sleep 2




echo -n "Loading indi devices parameters..."
indi_setprop -p $INDI_PORT "Dome Scripting Gateway.CONFIG_PROCESS.CONFIG_LOAD=On"
indi_setprop -p $INDI_PORT "EQMod Mount.CONFIG_PROCESS.CONFIG_LOAD=On"
indi_setprop -p $INDI_PORT "Canon DSLR EOS 50D.CONFIG_PROCESS.CONFIG_LOAD=On"
indi_setprop -p $INDI_PORT "ZWO CCD ASI120MM.CONFIG_PROCESS.CONFIG_LOAD=On"
indi_setprop -p $INDI_PORT "MoonLite.CONFIG_PROCESS.CONFIG_LOAD=On"
indi_setprop -p $INDI_PORT "WunderGround.CONFIG_PROCESS.CONFIG_LOAD=On"
indi_setprop -p $INDI_PORT "Joystick.CONFIG_PROCESS.CONFIG_LOAD=On"
echo " [ OK ]"

sleep 2

echo -n "Connecting Dome Scripting Gateway..."
while [[ $(indi_setprop -p $INDI_PORT "Dome Scripting Gateway.CONNECTION.CONNECT=On" > /dev/null 2>&1; echo $?) != 0 ]]
do
	 sleep 1
done
echo " [ OK ]"

echo -n "Connecting EQMod Mount..."
while [[ $(indi_setprop -p $INDI_PORT "EQMod Mount.CONNECTION.CONNECT=On" > /dev/null 2>&1; echo $?) != 0 ]]
do 
	sleep 1
done
echo " [ OK ]"

echo -n "Connecting Canon DSLR EOS 50D..."
while [[ $(indi_setprop -p $INDI_PORT "Canon DSLR EOS 50D.CONNECTION.CONNECT=On" > /dev/null 2>&1; echo $?) != 0 ]]
do
	sleep 1
done
echo " [ OK ]"

echo -n "Connecting ZWO CCD ASI120MM..."
while [[ $(indi_setprop -p $INDI_PORT "ZWO CCD ASI120MM.CONNECTION.CONNECT=On" > /dev/null 2>&1; echo $?) != 0 ]]
do
	sleep 1
done
echo " [ OK ]"

echo -n "Connecting MoonLite..."
while [[ $(indi_setprop -p $INDI_PORT "MoonLite.CONNECTION.CONNECT=On" > /dev/null 2>&1; echo $?) != 0 ]]
do
	sleep 1
done
echo " [ OK ]"

echo -n "Connecting WunderGround..."
while [[ $(indi_setprop -p $INDI_PORT "WunderGround.CONNECTION.CONNECT=On" > /dev/null 2>&1; echo $?) != 0 ]]
do
	sleep 1
done
echo " [ OK ]"

echo -n "Connecting Joystick..."
while [[ $(indi_setprop -p $INDI_PORT "Joystick.CONNECTION.CONNECT=On" > /dev/null 2>&1; echo $?) != 0 ]]
do
	sleep 1
done
echo " [ OK ]"

sleep 2




echo -n "Checking Telescope Mount..."
#mount must be PARKED (if not parked after power-on, mount may be in dangerous position -> shutdown and warn)
if [[ $(indi_getprop -p $INDI_PORT -1 "EQMod Mount.TELESCOPE_PARK.PARK") = "On" ]]
then
	echo " [ OK ]"
	currentRA=`indi_getprop -p $INDI_PORT -1 "EQMod Mount.CURRENTSTEPPERS.RAStepsCurrent"`
	currentDEC=`indi_getprop -p $INDI_PORT -1 "EQMod Mount.CURRENTSTEPPERS.DEStepsCurrent"`
	parkRA=`indi_getprop -p $INDI_PORT -1 "EQMod Mount.TELESCOPE_PARK_POSITION.PARK_RA"`
	parkDEC=`indi_getprop -p $INDI_PORT -1 "EQMod Mount.TELESCOPE_PARK_POSITION.PARK_DEC"`

	echo -n "   -> RA=$currentRA (parked at $parkRA)"
	if [[ $currentRA = $parkRA ]]
	then
		echo " [ RA OK ]"
	else
		echo " [ RA NOT PARKED ]"
		echo "   -> Telescope mount is not PARKED after power on [ it may be in UNKNOWN POSITION ], ABORTING     START"
		exit 61
	fi
	echo -n "   -> DEC=$currentDEC (parked at $parkDEC)"
	if [[ $currentDEC = $parkDEC ]]
	then
		echo " [ DEC OK ]"
	else
		echo " [ DEC NOT PARKED ]"
		echo "   -> Telescope mount is not PARKED after power on [ it may be in UNKNOWN POSITION ], ABORTING     START"
		exit 62
	fi

	if [[ $currentRA = $parkRA ]] && [[ $currentDEC = $parkDEC ]]
	then
		echo "Mount is parked!"
	else
		echo "Mount is not in PARK position, ABORTING START"
		exit 63
	fi

else
	echo " [ ERROR ]"
	echo "   -> Telescope mount is not PARKED after power on [ it may be in UNKNOWN POSITION ], ABORTING START"
	exit 60
fi




echo "Checking Weather conditions..."
indi_setprop -p $INDI_PORT "WunderGround.WEATHER_REFRESH.REFRESH=On"
sleep 2
indi_setprop -p $INDI_PORT "WunderGround.WEATHER_REFRESH.REFRESH=On"
sleep 2
indi_setprop -p $INDI_PORT "WunderGround.WEATHER_REFRESH.REFRESH=On"
sleep 6
weather_forecast=`indi_getprop -p $INDI_PORT -1 "WunderGround.WEATHER_STATUS.WEATHER_FORECAST"`
weather_temperature=`indi_getprop -p $INDI_PORT -1 "WunderGround.WEATHER_STATUS.WEATHER_TEMPERATURE"`
weather_windspeed=`indi_getprop -p $INDI_PORT -1 "WunderGround.WEATHER_STATUS.WEATHER_WIND_SPEED"`
weather_rainhour=`indi_getprop -p $INDI_PORT -1 "WunderGround.WEATHER_STATUS.WEATHER_RAIN_HOUR"`

echo "   -> WEATHER_FORECAST=$weather_forecast"
echo "   -> WEATHER_TEMPERATURE=$weather_temperature"
echo "   -> WEATHER_WIND_SPEED=$weather_windspeed"
echo "   -> WEATHER_RAIN_HOUR=$weather_rainhour"

if [[ $weather_forecast = "Ok" ]] && [[ $weather_temperature = "Ok" ]] && [[ $weather_windspeed = "Ok" ]] && [[ $weather_rainhour = "Ok" ]]
then
	echo "   -> Current weather conditions are OK"
else
	echo "   -> Current weather conditions are unsafe, ABORTING START"
#	echo "Shutdown observatory!"
#	#launch shutdown script		
#	./shutdown.sh
	exit 70
fi

sleep 2

echo -n "Opening roof..."
indi_setprop -p $INDI_PORT "Dome Scripting Gateway.DOME_PARK.UNPARK=On"
sleep 1
declare -i timeout=120
#while [[ $(indi_getprop -p $INDI_PORT -1 "Dome Scripting Gateway.DOME_PARK.PARK") = "On" ]] ||
while [[ $(cat $CNTRL_FIFO/roof/status/state) != "OPENED" ]] 
do
	sleep 1
	timeout=$timeout-1
#	if [[ $(cat $CNTRL_FIFO/roof/status/state) != "OPENING" ]]
#	then
#		echo "OPEN" > $CNTRL_FIFO/roof/control/move
#	fi
	if [[ $timeout = 0 ]]
	then
		echo " [ ERROR ]"
		echo "   -> Roof opening Timeout, ABORTING START"

		echo "Shutdown observatory!"
		#launch shutdown script		
		./shutdown.sh
		exit 80
	fi
done
echo " [ OK ]"

sleep 3


echo -n "Unparking Telescope Mount..."
#if [[ $(indi_getprop -p $INDI_PORT -1 "Dome Scripting Gateway.DOME_PARK.UNPARK") = "On" ]] && [[ $(indi_getprop -p $INDI_PORT -1 "Dome Scripting Gateway.DOME_SHUTTER.SHUTTER_OPEN") = "On" ]] && 
if [[ $(cat $CNTRL_FIFO/roof/status/state) = "OPENED" ]]
then
	echo "Roof is Opened, Unparking Mount..."
	indi_setprop -p $INDI_PORT "EQMod Mount.TELESCOPE_PARK.UNPARK=On"
	sleep 3
	if [[ $(indi_getprop -p $INDI_PORT -1 "EQMod Mount.TELESCOPE_PARK.UNPARK") = "On" ]]
	then
		echo " [ OK ]"
	else
		echo " [ Error ]"
		echo "   -> Failed to unpark mount, ABORTING START"
		
		echo "Shutdown observatory!"
		#launch shutdown script		
		./shutdown.sh
		exit 82
	fi
else
	echo " [ Error ]"
	echo "   -> Failed to open roof, ABORTING START"

	echo "Shutdown observatory!"
	#launch shutdown script		
	./shutdown.sh
	exit 81
fi



#echo -n "Slew mount to HOME..."
#targetRA=15.44
#targetDEC=77.47

#indi_setprop -p $INDI_PORT "EQMod Mount.ON_COORD_SET.SLEW=On"
#indi_setprop -p $INDI_PORT "EQMod Mount.EQUATORIAL_EOD_COORD.RA;DEC=$targetRA;$targetDEC"
#indi_eval -t 60 -p $INDI_PORT -wo 'abs("EQMod Mount.EQUATORIAL_EOD_COORD.RA"-'$targetRA')<0.05 && abs("EQMod Mount.EQUATORIAL_EOD_COORD.DEC"-'$targetDEC')<0.05'
#echo " [ OK ]"
#sleep 1








echo "Observatory ready!"

/home/astro/notificationtoIFTTT.sh 'Observatory ready!'

echo "### START END ###"
exit 0
