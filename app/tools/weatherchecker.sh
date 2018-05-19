#!/bin/bash

INDI_PORT=7500

echo -n "Connecting WunderGround..."
while [[ $(indi_setprop -p $INDI_PORT "WunderGround.CONNECTION.CONNECT=On" > /dev/null 2>&1; echo $?) != 0 ]]
do
	sleep 1
done
echo " [ OK ]"


while true
do

    echo "Checking Weather conditions..."
    indi_setprop -p $INDI_PORT "WunderGround.CONFIG_PROCESS.CONFIG_LOAD=On"
    weather_forecast=`indi_getprop -p $INDI_PORT -1 "WunderGround.WEATHER_STATUS.WEATHER_FORECAST"`
    weather_temperature=`indi_getprop -p $INDI_PORT -1 "WunderGround.WEATHER_STATUS.WEATHER_TEMPERATURE"`
    weather_windspeed=`indi_getprop -p $INDI_PORT -1 "WunderGround.WEATHER_STATUS.WEATHER_WIND_SPEED"`
    weather_rainhour=`indi_getprop -p $INDI_PORT -1 "WunderGround.WEATHER_STATUS.WEATHER_RAIN_HOUR"`

    timeout=60
    while [[ $weather_forecast =~ ^(Idle|Busy)$ ]]
    do
        #echo -n "+"
        echo "   -> WEATHER_FORECAST=$weather_forecast"
        indi_setprop -p $INDI_PORT "WunderGround.CONFIG_PROCESS.CONFIG_LOAD=On"
        indi_setprop -p $INDI_PORT "WunderGround.WEATHER_REFRESH.REFRESH=On"
        sleep 1
        weather_forecast=`indi_getprop -p $INDI_PORT -1 "WunderGround.WEATHER_STATUS.WEATHER_FORECAST"`
        timeout=$timeout-1
        if [[ $timeout = 0 ]]
        then
            echo " [ Error ]"
	        echo "  -> Error while retrieving weather_forecast data, SHUTDOWN NOW"
	        #exit 71
        fi
    done

    timeout=60
    while [[ $weather_temperature =~ ^(Idle|Busy)$ ]]
    do
        #echo -n "*"
        echo "   -> WEATHER_TEMPERATURE=$weather_temperature"
        indi_setprop -p $INDI_PORT "WunderGround.CONFIG_PROCESS.CONFIG_LOAD=On"
        indi_setprop -p $INDI_PORT "WunderGround.WEATHER_REFRESH.REFRESH=On"
        sleep 1
        weather_temperature=`indi_getprop -p $INDI_PORT -1 "WunderGround.WEATHER_STATUS.WEATHER_TEMPERATURE"`
        timeout=$timeout-1
        if [[ $timeout = 0 ]]
        then
            echo " [ Error ]"
	        echo "  -> Error while retrieving weather_temperature data, SHUTDOWN NOW"
	        #exit 72
        fi
    done

    timeout=60
    while [[ $weather_windspeed =~ ^(Idle|Busy)$ ]]
    do
        #echo -n "."
        echo "   -> WEATHER_WIND_SPEED=$weather_windspeed"
        indi_setprop -p $INDI_PORT "WunderGround.CONFIG_PROCESS.CONFIG_LOAD=On"
        indi_setprop -p $INDI_PORT "WunderGround.WEATHER_REFRESH.REFRESH=On"
        sleep 1
        weather_windspeed=`indi_getprop -p $INDI_PORT -1 "WunderGround.WEATHER_STATUS.WEATHER_WIND_SPEED"`
        timeout=$timeout-1
        if [[ $timeout = 0 ]]
        then
            echo " [ Error ]"
	        echo "  -> Error while retrieving weather_windspeed data, SHUTDOWN NOW"
	        #exit 73
        fi
    done

    timeout=60
    while [[ $weather_rainhour =~ ^(Idle|Busy)$ ]]
    do
        #echo -n "-"
        echo "   -> WEATHER_RAIN_HOUR=$weather_rainhour"
        indi_setprop -p $INDI_PORT "WunderGround.CONFIG_PROCESS.CONFIG_LOAD=On"
        indi_setprop -p $INDI_PORT "WunderGround.WEATHER_REFRESH.REFRESH=On"
        sleep 1
        weather_rainhour=`indi_getprop -p $INDI_PORT -1 "WunderGround.WEATHER_STATUS.WEATHER_RAIN_HOUR"`
        timeout=$timeout-1
        if [[ $timeout = 0 ]]
        then
            echo " [ Error ]"
	        echo "  -> Error while retrieving weather_rainhour data, SHUTDOWN NOW"
	        #exit 74
        fi
    done



    echo "   -> WEATHER_FORECAST=$weather_forecast"
    echo "   -> WEATHER_TEMPERATURE=$weather_temperature"
    echo "   -> WEATHER_WIND_SPEED=$weather_windspeed"
    echo "   -> WEATHER_RAIN_HOUR=$weather_rainhour"

    if [[ $weather_forecast = "Ok" ]] && [[ $weather_temperature = "Ok" ]] && [[ $weather_windspeed = "Ok" ]] && [[ $weather_rainhour = "Ok" ]]
    then
	    echo "   -> Current weather conditions are OK"
    else
	    echo "   -> Current weather conditions are unsafe, SHUTDOWN NOW"
	    echo "Shutdown observatory!"
	    #launch shutdown script
	    /home/astro/DEV/ttfobservatory/app/startstopscripts/shutdown.sh
	    exit 70
    fi

    sleep 60
done
