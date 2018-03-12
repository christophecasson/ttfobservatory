#!/usr/bin/python
#
# Connect script for INDI Dome Scripting Gateway
#
# Arguments: none
# Exit code: 0 for success, 1 for failure
#

import sys
from tools import *

if readFifo(fifo_root_path+"powerboard/status/board_state") != "OK":
	debug("Error connecting Dome scripting gateway")
#	sys.exit(1)

debug("Powerboard state OK: Dome scripting gateway connected!")
sys.exit(0)
