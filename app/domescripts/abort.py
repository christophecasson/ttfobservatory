#!/usr/bin/python
#
# Abort script for INDI Dome Scripting Gateway
#
# Arguments: none
# Exit code: 0 for success, 1 for failure
#

import sys
import time
from tools import *

debug("Abort")
writeFifo(fifo_root_path+"roof/control/move", "ABORT")
time.sleep(0.1)
writeFifo(fifo_root_path+"roof/control/move", "ABORT")
time.sleep(0.1)
writeFifo(fifo_root_path+"roof/control/move", "ABORT")

sys.exit(0)
