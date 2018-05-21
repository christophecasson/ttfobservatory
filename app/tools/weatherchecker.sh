#!/bin/bash

INDI_PORT=7500

echo -n "Connecting WunderGround..."
while [[ $(indi_setprop -p $INDI_PORT "WunderGround.CONNECTION.CONNECT=On" > /dev/null 2>&1; echo $?) != 0 ]]
do
	sleep 1
done
echo " [ OK ]"

echo -n "Loading configuration..."
indi_setprop -p $INDI_PORT "WunderGround.CONFIG_PROCESS.CONFIG_LOAD=On"
sleep 2
echo " [ OK ]"

while true
do

    clear
    date

    echo "Checking Weather conditions..."
    #indi_setprop -p $INDI_PORT "WunderGround.CONFIG_PROCESS.CONFIG_LOAD=On"
    indi_setprop -p $INDI_PORT "WunderGround.WEATHER_REFRESH.REFRESH=On"
    sleep 1
    weather_forecast=`indi_getprop -p $INDI_PORT -1 "WunderGround.WEATHER_STATUS.WEATHER_FORECAST"`
    weather_temperature=`indi_getprop -p $INDI_PORT -1 "WunderGround.WEATHER_STATUS.WEATHER_TEMPERATURE"`
    weather_windspeed=`indi_getprop -p $INDI_PORT -1 "WunderGround.WEATHER_STATUS.WEATHER_WIND_SPEED"`
    weather_rainhour=`indi_getprop -p $INDI_PORT -1 "WunderGround.WEATHER_STATUS.WEATHER_RAIN_HOUR"`
    weatherparam_forecast=`indi_getprop -p $INDI_PORT -1 "WunderGround.WEATHER_PARAMETERS.WEATHER_FORECAST"`
    weatherparam_temperature=`indi_getprop -p $INDI_PORT -1 "WunderGround.WEATHER_PARAMETERS.WEATHER_TEMPERATURE"`
    weatherparam_windspeed=`indi_getprop -p $INDI_PORT -1 "WunderGround.WEATHER_PARAMETERS.WEATHER_WIND_SPEED"`
    weatherparam_windgust=`indi_getprop -p $INDI_PORT -1 "WunderGround.WEATHER_PARAMETERS.WEATHER_WIND_GUST"`
    weatherparam_rainhour=`indi_getprop -p $INDI_PORT -1 "WunderGround.WEATHER_PARAMETERS.WEATHER_RAIN_HOUR"`

    timeout=60
    while [[ $weather_forecast =~ ^(Idle|)$ ]]
    do
        echo -n "+"
        echo "   -> WEATHER_FORECAST=$weather_forecast"
        #indi_setprop -p $INDI_PORT "WunderGround.CONFIG_PROCESS.CONFIG_LOAD=On"
        indi_setprop -p $INDI_PORT "WunderGround.WEATHER_REFRESH.REFRESH=On"
        sleep 1
        weather_forecast=`indi_getprop -p $INDI_PORT -1 "WunderGround.WEATHER_STATUS.WEATHER_FORECAST"`
        timeout=$timeout-1
        if [[ $timeout = 0 ]]
        then
            echo " [ Error ]"
	        echo "  -> Error while retrieving weather_forecast data, SHUTDOWN NOW"
            /home/astro/DEV/ttfobservatory/app/startstopscripts/shutdown.sh
            exit 71
        fi
    done

    timeout=60
    while [[ $weather_temperature =~ ^(Idle|)$ ]]
    do
        echo -n "*"
        echo "   -> WEATHER_TEMPERATURE=$weather_temperature"
        #indi_setprop -p $INDI_PORT "WunderGround.CONFIG_PROCESS.CONFIG_LOAD=On"
        indi_setprop -p $INDI_PORT "WunderGround.WEATHER_REFRESH.REFRESH=On"
        sleep 1
        weather_temperature=`indi_getprop -p $INDI_PORT -1 "WunderGround.WEATHER_STATUS.WEATHER_TEMPERATURE"`
        timeout=$timeout-1
        if [[ $timeout = 0 ]]
        then
            echo " [ Error ]"
	        echo "  -> Error while retrieving weather_temperature data, SHUTDOWN NOW"
            /home/astro/DEV/ttfobservatory/app/startstopscripts/shutdown.sh
            exit 72
        fi
    done

    timeout=60
    while [[ $weather_windspeed =~ ^(Idle|)$ ]]
    do
        echo -n "."
        echo "   -> WEATHER_WIND_SPEED=$weather_windspeed"
        #indi_setprop -p $INDI_PORT "WunderGround.CONFIG_PROCESS.CONFIG_LOAD=On"
        indi_setprop -p $INDI_PORT "WunderGround.WEATHER_REFRESH.REFRESH=On"
        sleep 1
        weather_windspeed=`indi_getprop -p $INDI_PORT -1 "WunderGround.WEATHER_STATUS.WEATHER_WIND_SPEED"`
        timeout=$timeout-1
        if [[ $timeout = 0 ]]
        then
            echo " [ Error ]"
	        echo "  -> Error while retrieving weather_windspeed data, SHUTDOWN NOW"
            /home/astro/DEV/ttfobservatory/app/startstopscripts/shutdown.sh
            exit 73
        fi
    done

    timeout=60
    while [[ $weather_rainhour =~ ^(Idle|)$ ]]
    do
        echo -n "-"
        echo "   -> WEATHER_RAIN_HOUR=$weather_rainhour"
        #indi_setprop -p $INDI_PORT "WunderGround.CONFIG_PROCESS.CONFIG_LOAD=On"
        indi_setprop -p $INDI_PORT "WunderGround.WEATHER_REFRESH.REFRESH=On"
        sleep 1
        weather_rainhour=`indi_getprop -p $INDI_PORT -1 "WunderGround.WEATHER_STATUS.WEATHER_RAIN_HOUR"`
        timeout=$timeout-1
        if [[ $timeout = 0 ]]
        then
            echo " [ Error ]"
	        echo "  -> Error while retrieving weather_rainhour data, SHUTDOWN NOW"
            /home/astro/DEV/ttfobservatory/app/startstopscripts/shutdown.sh
	        exit 74
        fi
    done


    weatherparam_temperature=`echo $weatherparam_temperature | awk '{printf("%.2f",$1)}'`
    weatherparam_windspeed=`echo $weatherparam_windspeed | awk '{printf("%.2f",$1)}'`
    weatherparam_windgust=`echo $weatherparam_windgust | awk '{printf("%.2f",$1)}'`
    echo -e "   -> WEATHER_FORECAST    : $weatherparam_forecast \t\t\t[$weather_forecast]"
    echo -e "   -> WEATHER_TEMPERATURE : $weatherparam_temperature C \t\t[$weather_temperature]"
    echo -e "   -> WEATHER_WIND_SPEED  : $weatherparam_windspeed km/h \t\t[$weather_windspeed]"
    echo -e "   -> WEATHER_WIND_GUST   : $weatherparam_windgust km/h"
    echo -e "   -> WEATHER_RAIN_HOUR   : $weatherparam_rainhour mm   \t\t[$weather_rainhour]"
    echo ""

    if [[ $weather_forecast = "Ok" ]] && [[ $weather_temperature = "Ok" ]] && [[ $weather_windspeed = "Ok" ]] && [[ $weather_rainhour = "Ok" ]]
    then
	    echo "Current weather conditions are OK"
    elif [[ $weather_forecast = "Alert" ]] || [[ $weather_temperature = "Alert" ]] || [[ $weather_windspeed = "Alert" ]] || [[ $weather_rainhour = "Alert" ]]
    then
	    echo "Current weather conditions are unsafe, SHUTDOWN NOW"
        ./notificationtoIFTTT.sh 'weatherchecker.sh: Current weather conditions are unsafe, SHUTDOWN NOW' > /dev/null 2>&1
	    echo "Shutdown observatory!"
	    #launch shutdown script
	    /home/astro/DEV/ttfobservatory/app/startstopscripts/shutdown.sh
        exit 70
    else
        echo "Current weather conditions are in the Warning zone!"
        ./notificationtoIFTTT.sh 'weatherchecker.sh: Current weather conditions are in the Warning zone!' > /dev/null 2>&1
    fi

    sleep 60
done
