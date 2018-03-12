#!/usr/bin/python
#
# Status script for INDI Dome Scripting Gateway
#
# Arguments: file name to save current state and coordinates (parked shutter az)
# Exit code: 0 for success, 1 for failure
#

import sys
import time
from tools import *

script, path = sys.argv

debug("status filepath=" + path)

#if readFifo(fifo_root_path+"powerboard/status/board_state") != "OK":
#	debug("powerboard controller Error")
#	sys.exit(1)

#roofboard_state = readFifo(fifo_root_path+"roof/status/board_state")
#if roofboard_state == "OK":
#	parked = "0"
#elif roofboard_state == "Error":
#	parked = "1"
#else:
#	debug("unable to get dome park state")
#	sys.exit(1)

#roofopened = readFifo(fifo_root_path+"roof/status/opened")
#roofclosed = readFifo(fifo_root_path+"roof/status/closed")
#if roofopened == "1":
#	if roofclosed == "0":
#		shutter = "1"
#	else:
#		shutter = "2"
#if roofclosed == "1":
#	if roofopened == "0":
#		shutter = "0"
#	else:
#		shutter = "2"

parked = "0"
shutter = "0"


debug("Status: [parked=" + parked + "] [shutter=" + shutter + "] [az=0.0]")
status = open(path, 'w')
status.truncate()
status.write(parked + " " + shutter + " 0")
status.close()

sys.exit(0)
