#!/usr/bin/python
#
# Park script for INDI Dome Scripting Gateway
#
# Arguments: none
# Exit code: 0 for success, 1 for failure
#

import sys
import time
from tools import *

debug("Park")

if readFifo(fifo_root_path+"powerboard/status/board_state") != "OK":
	debug("powerboard controller Error")
	sys.exit(1)

retry = 5
while readFifo(fifo_root_path+"roof/status/board_state") != "Error":
	writeFifo(fifo_root_path+"powerboard/control/1", "0")
	time.sleep(0.5)
	retry = retry-1

	if retry == 0:
		debug("Parking error")
		sys.exit(1)

debug("roof parked (power off)")
sys.exit(0)
