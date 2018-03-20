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

retry = 10
while retry > 0: 
    powerboard_state = readFifo(fifo_root_path+"powerboard/status/board_state")
    if powerboard_state == "OK":
        break
    else:
	debug("powerboard controller Error")
        retry = retry-1
        if retry == 0:
            sys.exit(1)

retry = 10
while retry > 0:
    roofboard_state = readFifo(fifo_root_path+"roof/status/state")
    if roofboard_state == "CLOSED":
	parked = "1"
        break
    elif roofboard_state == "OPENED":
	parked = "0"
        break
    elif roofboard_state == "CLOSING":
	parked = "0"
	break
    elif roofboard_state == "OPENING":
	parked = "1"
	break
    else:
	debug("unable to get dome park state")
        retry = retry-1
        if retry == 0:
	    sys.exit(1)

retry = 10
while retry > 0:
    roofopened = readFifo(fifo_root_path+"roof/status/opened")
    if roofopened == "0" or roofopened == "1":
        break
    else:
        debug("unable to get opened lock state")
        retry = retry-1
        if retry == 0:
            sys.exit(1)
    
retry = 10
while retry > 0:
    roofclosed = readFifo(fifo_root_path+"roof/status/closed")
    if roofclosed == "0" or roofclosed == "1":
        break
    else:
        debug("unable to get closed lock state")
        retry = retry-1
        if retry == 0:
            sys.exit(1)

    
    
    
#   | opened | closed | shutter |  
#   |   0    |   0    |   [0]   |  
#   |   0    |   1    |    0    |  
#   |   1    |   0    |    1    |  
#   |   1    |   1    |   [0]   |  
    
if roofopened == "1" and roofclosed == "0":
    shutter = "1"
else:
    shutter = "0"
    

debug("Status: [parked=" + parked + "] [shutter=" + shutter + "] [az=0.0]")
status = open(path, 'w')
status.truncate()
status.write(parked + " " + shutter + " 0")
status.close()

sys.exit(0)
