#!/bin/bash

INDI_PORT=7500
CNTRL_FIFO="/home/astro/fifo"

declare -i timeout=0

openRoof=false
unparkMount=false

#handling of options
while [ "${1:-}" != "" ]; do
    case "$1" in
      "-o" | "--open")
        openRoof=true
        ;;
      "-u" | "--unpark")
        unparkMount=true
        ;;
    esac
    shift
  done

echo "### START BEGIN ###"

/home/astro/notificationtoIFTTT.sh 'Observatory startup begin' > /dev/null 2>&1

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
timeout=60
while [[ $(cat $CNTRL_FIFO/powerboard/status/board_state) != "OK" ]]
do
    echo -n "+"
    sleep 1
    timeout=$timeout-1
    if [[ $timeout = 0 ]]
    then
	    echo " [ Error ]"
	    echo "   -> Powerboard error, ABORTING START!"
	    exit 2
    fi
done
echo " [ OK ]"



echo -n "Powering ON Focuser..."
timeout=60
while [[ $(cat $CNTRL_FIFO/powerboard/status/4) != "1" ]]
do
    echo 1 > /home/astro/fifo/powerboard/control/4
    sleep 1
    timeout=$timeout-1
    if [[ $timeout = 0 ]]
    then
        echo " [ Error ]"
	    echo "  -> error powering ON Focuser, ABORTING START"
	    exit 3
    fi
done
echo " [ OK ]"

echo -n "Powering ON CCD..."
timeout=60
while [[ $(cat $CNTRL_FIFO/powerboard/status/3) != "1" ]]
do
    echo 1 > /home/astro/fifo/powerboard/control/3
    sleep 1
    timeout=$timeout-1
    if [[ $timeout = 0 ]]
    then
        echo " [ Error ]"
	    echo "  -> error powering ON CCD, ABORTING START"
	    exit 4
    fi
done
echo " [ OK ]"

echo -n "Powering ON Mount..."
timeout=60
while [[ $(cat $CNTRL_FIFO/powerboard/status/2) != "1" ]]
do
    echo 1 > /home/astro/fifo/powerboard/control/2
    sleep 1
    timeout=$timeout-1
    if [[ $timeout = 0 ]]
    then
        echo " [ Error ]"
	    echo "  -> error powering ON Mount, ABORTING START"
	    exit 5
    fi
done
echo " [ OK ]"

echo -n "Powering ON Roof..."
timeout=60
while [[ $(cat $CNTRL_FIFO/powerboard/status/1) != "1" ]]
do
    echo 1 > /home/astro/fifo/powerboard/control/1
    sleep 1
    timeout=$timeout-1
    if [[ $timeout = 0 ]]
    then
        echo " [ Error ]"
	    echo "  -> error powering ON Roof, ABORTING START"
	    exit 6
    fi
done
echo " [ OK ]"


sleep 2

echo 1 > /home/astro/fifo/powerboard/control/1
echo 1 > /home/astro/fifo/powerboard/control/2
echo 1 > /home/astro/fifo/powerboard/control/3
echo 1 > /home/astro/fifo/powerboard/control/4
echo 1 > /home/astro/fifo/powerboard/control/1
echo 1 > /home/astro/fifo/powerboard/control/2
echo 1 > /home/astro/fifo/powerboard/control/3
echo 1 > /home/astro/fifo/powerboard/control/4
echo 1 > /home/astro/fifo/powerboard/control/1
echo 1 > /home/astro/fifo/powerboard/control/2
echo 1 > /home/astro/fifo/powerboard/control/3
echo 1 > /home/astro/fifo/powerboard/control/4



sleep 2

echo -n "Loading indi devices parameters..."
indi_setprop -p $INDI_PORT "Dome Scripting Gateway.CONFIG_PROCESS.CONFIG_LOAD=On"
indi_setprop -p $INDI_PORT "EQMod Mount.CONFIG_PROCESS.CONFIG_LOAD=On"
#indi_setprop -p $INDI_PORT "Canon DSLR EOS 50D.CONFIG_PROCESS.CONFIG_LOAD=On"
indi_setprop -p $INDI_PORT "ZWO CCD ASI1600MM Pro.CONFIG_PROCESS.CONFIG_LOAD=On"
indi_setprop -p $INDI_PORT "ASI EFW.CONFIG_PROCESS.CONFIG_LOAD=On"
indi_setprop -p $INDI_PORT "ZWO CCD ASI120MM.CONFIG_PROCESS.CONFIG_LOAD=On"
indi_setprop -p $INDI_PORT "MoonLite.CONFIG_PROCESS.CONFIG_LOAD=On"
indi_setprop -p $INDI_PORT "Flip Flat.CONFIG_PROCESS.CONFIG_LOAD=On"
indi_setprop -p $INDI_PORT "OpenWeatherMap.CONFIG_PROCESS.CONFIG_LOAD=On"
#indi_setprop -p $INDI_PORT "Joystick.CONFIG_PROCESS.CONFIG_LOAD=On"
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

#echo -n "Connecting Canon DSLR EOS 50D..."
#while [[ $(indi_setprop -p $INDI_PORT "Canon DSLR EOS 50D.CONNECTION.CONNECT=On" > /dev/null 2>&1; echo $?) != 0 ]]
#do
#	sleep 1
#done
#echo " [ OK ]"

echo -n "Connecting ZWO CCD ASI1600MM Pro..."
while [[ $(indi_setprop -p $INDI_PORT "ZWO CCD ASI1600MM Pro.CONNECTION.CONNECT=On" > /dev/null 2>&1; echo $?) != 0 ]]
do
	sleep 1
done
echo " [ OK ]"

echo -n "Connecting ASI EFW..."
while [[ $(indi_setprop -p $INDI_PORT "ASI EFW.CONNECTION.CONNECT=On" > /dev/null 2>&1; echo $?) != 0 ]]
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

echo -n "Connecting Flip Flat..."
while [[ $(indi_setprop -p $INDI_PORT "Flip Flat.CONNECTION.CONNECT=On" > /dev/null 2>&1; echo $?) != 0 ]]
do
	sleep 1
done
indi_setprop -p $INDI_PORT "Flip Flat.FLAT_LIGHT_CONTROL.FLAT_LIGHT_ON=On"
indi_setprop -p $INDI_PORT "Flip Flat.FLAT_LIGHT_INTENSITY.FLAT_LIGHT_INTENSITY_VALUE=10"
sleep 1
indi_setprop -p $INDI_PORT "Flip Flat.FLAT_LIGHT_INTENSITY.FLAT_LIGHT_INTENSITY_VALUE=100"
sleep 1
indi_setprop -p $INDI_PORT "Flip Flat.FLAT_LIGHT_INTENSITY.FLAT_LIGHT_INTENSITY_VALUE=255"
sleep 1
indi_setprop -p $INDI_PORT "Flip Flat.FLAT_LIGHT_CONTROL.FLAT_LIGHT_OFF=On"
echo " [ OK ]"

echo -n "Connecting OpenWeatherMap..."
while [[ $(indi_setprop -p $INDI_PORT "OpenWeatherMap.CONNECTION.CONNECT=On" > /dev/null 2>&1; echo $?) != 0 ]]
do
	sleep 1
done
echo " [ OK ]"

#echo -n "Connecting Joystick..."
#while [[ $(indi_setprop -p $INDI_PORT "Joystick.CONNECTION.CONNECT=On" > /dev/null 2>&1; echo $?) != 0 ]]
#do
#	sleep 1
#done
#echo " [ OK ]"

sleep 2


echo -n "Reloading indi devices parameters..."
indi_setprop -p $INDI_PORT "Dome Scripting Gateway.CONFIG_PROCESS.CONFIG_LOAD=On"
indi_setprop -p $INDI_PORT "EQMod Mount.CONFIG_PROCESS.CONFIG_LOAD=On"
#indi_setprop -p $INDI_PORT "Canon DSLR EOS 50D.CONFIG_PROCESS.CONFIG_LOAD=On"
indi_setprop -p $INDI_PORT "ZWO CCD ASI1600MM Pro.CONFIG_PROCESS.CONFIG_LOAD=On"
indi_setprop -p $INDI_PORT "ASI EFW.CONFIG_PROCESS.CONFIG_LOAD=On"
indi_setprop -p $INDI_PORT "ZWO CCD ASI120MM.CONFIG_PROCESS.CONFIG_LOAD=On"
indi_setprop -p $INDI_PORT "MoonLite.CONFIG_PROCESS.CONFIG_LOAD=On"
indi_setprop -p $INDI_PORT "Flip Flat.CONFIG_PROCESS.CONFIG_LOAD=On"
indi_setprop -p $INDI_PORT "OpenWeatherMap.CONFIG_PROCESS.CONFIG_LOAD=On"
#indi_setprop -p $INDI_PORT "Joystick.CONFIG_PROCESS.CONFIG_LOAD=On"
echo " [ OK ]"


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
indi_setprop -p $INDI_PORT "OpenWeatherMap.CONFIG_PROCESS.CONFIG_LOAD=On"
weather_forecast=`indi_getprop -p $INDI_PORT -1 "OpenWeatherMap.WEATHER_STATUS.WEATHER_FORECAST"`
weather_temperature=`indi_getprop -p $INDI_PORT -1 "OpenWeatherMap.WEATHER_STATUS.WEATHER_TEMPERATURE"`
weather_windspeed=`indi_getprop -p $INDI_PORT -1 "OpenWeatherMap.WEATHER_STATUS.WEATHER_WIND_SPEED"`
weather_rainhour=`indi_getprop -p $INDI_PORT -1 "OpenWeatherMap.WEATHER_STATUS.WEATHER_RAIN_HOUR"`
weather_snowhour=`indi_getprop -p $INDI_PORT -1 "OpenWeatherMap.WEATHER_STATUS.WEATHER_SNOW_HOUR"`

timeout=60
while [[ $weather_forecast =~ ^(Idle|)$ ]]
do
    #echo -n "+"
    echo "   -> WEATHER_FORECAST=$weather_forecast"
    indi_setprop -p $INDI_PORT "OpenWeatherMap.CONFIG_PROCESS.CONFIG_LOAD=On"
    indi_setprop -p $INDI_PORT "OpenWeatherMap.WEATHER_REFRESH.REFRESH=On"
    sleep 1
    weather_forecast=`indi_getprop -p $INDI_PORT -1 "OpenWeatherMap.WEATHER_STATUS.WEATHER_FORECAST"`
    timeout=$timeout-1
    if [[ $timeout = 0 ]]
    then
        echo " [ Error ]"
	    echo "  -> Error while retrieving weather_forecast data, ABORTING START"
	    exit 71
    fi
done

timeout=60
while [[ $weather_temperature =~ ^(Idle|)$ ]]
do
    #echo -n "*"
    echo "   -> WEATHER_TEMPERATURE=$weather_temperature"
    indi_setprop -p $INDI_PORT "OpenWeatherMap.CONFIG_PROCESS.CONFIG_LOAD=On"
    indi_setprop -p $INDI_PORT "OpenWeatherMap.WEATHER_REFRESH.REFRESH=On"
    sleep 1
    weather_temperature=`indi_getprop -p $INDI_PORT -1 "OpenWeatherMap.WEATHER_STATUS.WEATHER_TEMPERATURE"`
    timeout=$timeout-1
    if [[ $timeout = 0 ]]
    then
        echo " [ Error ]"
	    echo "  -> Error while retrieving weather_temperature data, ABORTING START"
	    exit 72
    fi
done

timeout=60
while [[ $weather_windspeed =~ ^(Idle|)$ ]]
do
    #echo -n "."
    echo "   -> WEATHER_WIND_SPEED=$weather_windspeed"
    indi_setprop -p $INDI_PORT "OpenWeatherMap.CONFIG_PROCESS.CONFIG_LOAD=On"
    indi_setprop -p $INDI_PORT "OpenWeatherMap.WEATHER_REFRESH.REFRESH=On"
    sleep 1
    weather_windspeed=`indi_getprop -p $INDI_PORT -1 "OpenWeatherMap.WEATHER_STATUS.WEATHER_WIND_SPEED"`
    timeout=$timeout-1
    if [[ $timeout = 0 ]]
    then
        echo " [ Error ]"
	    echo "  -> Error while retrieving weather_windspeed data, ABORTING START"
	    exit 73
    fi
done

timeout=60
while [[ $weather_rainhour =~ ^(Idle|)$ ]]
do
    #echo -n "-"
    echo "   -> WEATHER_RAIN_HOUR=$weather_rainhour"
    indi_setprop -p $INDI_PORT "OpenWeatherMap.CONFIG_PROCESS.CONFIG_LOAD=On"
    indi_setprop -p $INDI_PORT "OpenWeatherMap.WEATHER_REFRESH.REFRESH=On"
    sleep 1
    weather_rainhour=`indi_getprop -p $INDI_PORT -1 "OpenWeatherMap.WEATHER_STATUS.WEATHER_RAIN_HOUR"`
    timeout=$timeout-1
    if [[ $timeout = 0 ]]
    then
        echo " [ Error ]"
	    echo "  -> Error while retrieving weather_rainhour data, ABORTING START"
	    exit 74
    fi
done

timeout=60
while [[ $weather_snowhour =~ ^(Idle|)$ ]]
do
    #echo -n "-"
    echo "   -> WEATHER_SNOW_HOUR=$weather_snowhour"
    indi_setprop -p $INDI_PORT "OpenWeatherMap.CONFIG_PROCESS.CONFIG_LOAD=On"
    indi_setprop -p $INDI_PORT "OpenWeatherMap.WEATHER_REFRESH.REFRESH=On"
    sleep 1
    weather_snowhour=`indi_getprop -p $INDI_PORT -1 "OpenWeatherMap.WEATHER_STATUS.WEATHER_SNOW_HOUR"`
    timeout=$timeout-1
    if [[ $timeout = 0 ]]
    then
        echo " [ Error ]"
	    echo "  -> Error while retrieving weather_snowhour data, ABORTING START"
	    exit 74
    fi
done



echo "   -> WEATHER_FORECAST=$weather_forecast"
echo "   -> WEATHER_TEMPERATURE=$weather_temperature"
echo "   -> WEATHER_WIND_SPEED=$weather_windspeed"
echo "   -> WEATHER_RAIN_HOUR=$weather_rainhour"
echo "   -> WEATHER_SNOW_HOUR=$weather_snowhour"

if [[ $weather_forecast = "Ok" ]] && [[ $weather_temperature = "Ok" ]] && [[ $weather_windspeed = "Ok" ]] && [[ $weather_rainhour = "Ok" ]] && [[ $weather_snowhour = "Ok" ]]
then
	echo "   -> Current weather conditions are OK"
else
	echo "   -> Current weather conditions are unsafe, ABORTING START"
	echo "Shutdown observatory!"
	#launch shutdown script
	/home/astro/DEV/ttfobservatory/app/startstopscripts/shutdown.sh
	exit 70
fi

sleep 2

if [ $openRoof == true ]
    then
    echo -n "Opening roof..."
    indi_setprop -p $INDI_PORT "Dome Scripting Gateway.DOME_PARK.UNPARK=On"
    sleep 1
    timeout=120
    #while [[ $(indi_getprop -p $INDI_PORT -1 "Dome Scripting Gateway.DOME_PARK.PARK") = "On" ]] ||
    while [[ $(cat $CNTRL_FIFO/roof/status/state) != "OPENED" ]]
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
		    /home/astro/DEV/ttfobservatory/app/startstopscripts/shutdown.sh
		    exit 80
	    fi
    done
    echo " [ OK ]"
    sleep 3

    if [ $unparkMount == true ]
    then
        echo -n "Unparking Telescope Mount..."
        #if [[ $(indi_getprop -p $INDI_PORT -1 "Dome Scripting Gateway.DOME_PARK.UNPARK") = "On" ]] && [[ $(indi_getprop -p $INDI_PORT -1 "Dome Scripting Gateway.DOME_SHUTTER.SHUTTER_OPEN") = "On" ]] &&
        timeout=60
        while [[ $(cat $CNTRL_FIFO/roof/status/state) != "OPENED" ]]
        do
            echo -n "+"
            sleep 1
            timeout=$timeout-1
            if [[ $timeout = 0 ]]
	        then
                echo " [ Error ]"
	            echo "   -> Failed to open roof, ABORTING START"
	            echo "Shutdown observatory!"
	            #launch shutdown script
	            /home/astro/DEV/ttfobservatory/app/startstopscripts/shutdown.sh
	            exit 81
            fi
        done

	    echo "Roof is Opened, Unparking Mount..."
	    indi_setprop -p $INDI_PORT "EQMod Mount.TELESCOPE_PARK.UNPARK=On"
	    sleep 1
        timeout=60
	    while [[ $(indi_getprop -p $INDI_PORT -1 "EQMod Mount.TELESCOPE_PARK.UNPARK") != "On" ]]
        do
            echo -n "+"
            sleep 1
            timeout=$timeout-1
            if [[ $timeout = 0 ]]
	        then
                echo " [ Error ]"
		        echo "   -> Failed to unpark mount, ABORTING START"
		        echo "Shutdown observatory!"
		        #launch shutdown script
		        /home/astro/DEV/ttfobservatory/app/startstopscripts/shutdown.sh
		        exit 82
            fi
        done
        echo " [ OK ]"

	    echo -n "Opening Telescope Cap..."
        indi_setprop -p $INDI_PORT "Flip Flat.FLAT_LIGHT_CONTROL.FLAT_LIGHT_OFF=On"

        indi_setprop -p $INDI_PORT "Flip Flat.CAP_PARK.UNPARK=On"
	    sleep 1
        timeout=60
	    while [[ $(indi_getprop -p $INDI_PORT -1 "Flip Flat.Status.Cover") != "Open" ]]
        do
            echo -n "+"
            sleep 1
            timeout=$timeout-1
            if [[ $timeout = 0 ]]
	        then
                echo " [ Error ]"
		        echo "   -> Failed to uncap telescope, ABORTING START"
		        echo "Shutdown observatory!"
		        #launch shutdown script
		        #/home/astro/DEV/ttfobservatory/app/startstopscripts/shutdown.sh
		        exit 83
            fi
        done

	echo " [ OK ]"
    fi
fi


echo "Set CCD Cooler temperature to -20C"
indi_setprop -p $INDI_PORT "ZWO CCD ASI1600MM Pro.CCD_TEMPERATURE.CCD_TEMPERATURE_VALUE=-20"



echo "Observatory ready!"

/home/astro/notificationtoIFTTT.sh 'Observatory ready!' > /dev/null 2>&1

echo "### START END ###"
exit 0
