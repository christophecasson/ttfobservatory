#!/bin/bash

Options=""

read -p "Open roof? y/[n]: " openRoof
if [[ $openRoof == "y" ]]
then
    Options="--open"
    read -p "Unpark Mount? y/[n]: " unparkMount
    if [[ $unparkMount == "y" ]]
    then
        Options=" --open --unpark"
    fi
fi

read -p "type \"Start\" to confirm startup: " input

if [[ $input == "Start" ]]
then
    echo "start.sh $Options"
    /home/astro/DEV/ttfobservatory/app/startstopscripts/start.sh $Options
else
    echo "ABORT"
fi

read -p "Press any key to continue... " -n1 -s
