#!/bin/bash

INDI_PORT=7500

echo -n "Connecting OpenWeatherMap..."
while [[ $(indi_setprop -p $INDI_PORT "OpenWeatherMap.CONNECTION.CONNECT=On" > /dev/null 2>&1; echo $?) != 0 ]]
do
	sleep 1
done
echo " [ OK ]"

echo -n "Loading configuration..."
indi_setprop -p $INDI_PORT "OpenWeatherMap.CONFIG_PROCESS.CONFIG_LOAD=On"
sleep 2
echo " [ OK ]"

while true
do

    clear
    date

    echo "Checking Weather conditions..."
    #indi_setprop -p $INDI_PORT "OpenWeatherMap.CONFIG_PROCESS.CONFIG_LOAD=On"
    indi_setprop -p $INDI_PORT "OpenWeatherMap.WEATHER_REFRESH.REFRESH=On"
    sleep 1
    weather_forecast=`indi_getprop -p $INDI_PORT -1 "OpenWeatherMap.WEATHER_STATUS.WEATHER_FORECAST"`
    weather_temperature=`indi_getprop -p $INDI_PORT -1 "OpenWeatherMap.WEATHER_STATUS.WEATHER_TEMPERATURE"`
    weather_windspeed=`indi_getprop -p $INDI_PORT -1 "OpenWeatherMap.WEATHER_STATUS.WEATHER_WIND_SPEED"`
    weather_rainhour=`indi_getprop -p $INDI_PORT -1 "OpenWeatherMap.WEATHER_STATUS.WEATHER_RAIN_HOUR"`
    weather_snowhour=`indi_getprop -p $INDI_PORT -1 "OpenWeatherMap.WEATHER_STATUS.WEATHER_SNOW_HOUR"`
    weatherparam_forecast=`indi_getprop -p $INDI_PORT -1 "OpenWeatherMap.WEATHER_PARAMETERS.WEATHER_FORECAST"`
    weatherparam_temperature=`indi_getprop -p $INDI_PORT -1 "OpenWeatherMap.WEATHER_PARAMETERS.WEATHER_TEMPERATURE"`
    weatherparam_pressure=`indi_getprop -p $INDI_PORT -1 "OpenWeatherMap.WEATHER_PARAMETERS.WEATHER_PRESSURE"`
    weatherparam_humidity=`indi_getprop -p $INDI_PORT -1 "OpenWeatherMap.WEATHER_PARAMETERS.WEATHER_HUMIDITY"`
    weatherparam_windspeed=`indi_getprop -p $INDI_PORT -1 "OpenWeatherMap.WEATHER_PARAMETERS.WEATHER_WIND_SPEED"`
    weatherparam_rainhour=`indi_getprop -p $INDI_PORT -1 "OpenWeatherMap.WEATHER_PARAMETERS.WEATHER_RAIN_HOUR"`
    weatherparam_snowhour=`indi_getprop -p $INDI_PORT -1 "OpenWeatherMap.WEATHER_PARAMETERS.WEATHER_SNOW_HOUR"`
    weatherparam_cloudcover=`indi_getprop -p $INDI_PORT -1 "OpenWeatherMap.WEATHER_PARAMETERS.WEATHER_CLOUD_COVER"`
    weatherparam_code=`indi_getprop -p $INDI_PORT -1 "OpenWeatherMap.WEATHER_PARAMETERS.WEATHER_CODE"`



    weatherparam_temperature=`echo $weatherparam_temperature | awk '{printf("%.2f",$1)}'`
    weatherparam_windspeed=`echo $weatherparam_windspeed | awk '{printf("%.2f",$1)}'`
    weatherparam_windgust=`echo $weatherparam_windgust | awk '{printf("%.2f",$1)}'`
    echo -e "   -> WEATHER_FORECAST    : $weatherparam_forecast \t\t\t[$weather_forecast]"
    echo -e "   -> WEATHER_TEMPERATURE : $weatherparam_temperature C \t\t[$weather_temperature]"
    echo -e "   -> WEATHER_WIND_SPEED  : $weatherparam_windspeed km/h \t\t[$weather_windspeed]"
    echo -e "   -> WEATHER_RAIN_HOUR   : $weatherparam_rainhour mm   \t\t[$weather_rainhour]"
    echo -e "   -> WEATHER_SNOW_HOUR   : $weatherparam_snowhour mm   \t\t[$weather_snowhour]"
    echo -e "   -> WEATHER_PRESSURE    : $weatherparam_pressure hPa"
    echo -e "   -> WEATHER_HUMIDITY    : $weatherparam_humidity %"
    echo -e "   -> WEATHER_CLOUD_COVER : $weatherparam_cloudcover %"
    echo -e "   -> WEATHER_CODE        : $weatherparam_code"
    echo ""

    if [[ $weather_forecast = "Ok" ]] && [[ $weather_temperature = "Ok" ]] && [[ $weather_windspeed = "Ok" ]] && [[ $weather_rainhour = "Ok" ]] && [[ $weather_snowhour = "Ok" ]]
    then
	    echo "Current weather conditions are OK"
    elif [[ $weather_forecast = "Alert" ]] || [[ $weather_temperature = "Alert" ]] || [[ $weather_windspeed = "Alert" ]] || [[ $weather_rainhour = "Alert" ]] || [[ $weather_snowhour = "Alert" ]]
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

    sleep 300
done
