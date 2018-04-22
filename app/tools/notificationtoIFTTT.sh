#!/bin/sh

jsondata="{\"value1\":\"$*\"}"

curl -X POST -H "Content-Type: application/json" -d "$jsondata" https://maker.ifttt.com/trigger/Observatory1/with/key/_tGV5I33s26wbHNKP1sJm
