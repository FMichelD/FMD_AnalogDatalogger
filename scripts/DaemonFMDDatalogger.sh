#!/bin/bash

if pgrep -x "FMDDataloggerV2" > /dev/null
then
    echo "Running"
else
    echo "Stopped"
	DISPLAY=:0 /home/arduino/FMDDataloggerV2exe
fi

touch /home/arduino/FooBar2
