#!/usr/bin/python
#
# Unpark script for INDI Dome Scripting Gateway
#
# Arguments: none
# Exit code: 0 for success, 1 for failure
#

import sys
import time
from tools import *

debug("Unpark")

if readFifo(fifo_root_path+"powerboard/status/board_state") != "OK":
	debug("powerboard controller Error")
	sys.exit(1)

retry = 5
while readFifo(fifo_root_path+"roof/status/board_state") != "OK":
	writeFifo(fifo_root_path+"powerboard/control/1", "1")
	time.sleep(0.5)
	retry = retry-1

	if retry == 0:
		debug("Unparking error")
		sys.exit(1)

debug("roof unparked (power on)")
sys.exit(0)
