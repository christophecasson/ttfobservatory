#!/usr/bin/python
#
# Open shutter script for INDI Dome Scripting Gateway
#
# Arguments: none
# Exit code: 0 for success, 1 for failure
#

import sys
import time
from tools import *

debug("Open")

if readFifo(fifo_root_path+"roof/status/board_state") != "OK":
	debug("Roof controller Error")
	sys.exit(1)

retry = 5
while readFifo(fifo_root_path+"roof/status/state") != "OPENING":
	writeFifo(fifo_root_path+"roof/control/move", "OPEN")
	time.sleep(0.5)
	retry = retry-1

	if retry == 0:
		debug("Opening error")
		sys.exit(1)

debug("Opening roof")
sys.exit(0)
