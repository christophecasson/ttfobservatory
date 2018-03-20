#!/usr/bin/python
#
# Close shutter script for INDI Dome Scripting Gateway
#
# Arguments: none
# Exit code: 0 for success, 1 for failure
#

import sys
import time
from tools import *

debug("Close")

#if readFifo(fifo_root_path+"roof/status/board_state") != "OK":
#	debug("Roof controller Error")
#	sys.exit(1)

retry = 5
while readFifo(fifo_root_path+"roof/status/state") != "CLOSING":
	writeFifo(fifo_root_path+"roof/control/move", "CLOSE")
	time.sleep(0.5)
	retry = retry-1

	if retry == 0:
		debug("Closing error")
		sys.exit(1)

debug("Closing roof")
sys.exit(0)
