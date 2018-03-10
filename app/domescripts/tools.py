import os
import errno
import time
from datetime import datetime
import sys



fifo_root_path  = "/home/astro/fifo/"


#handle debug log
def debug(text):
	print "[ " + os.path.basename(sys.argv[0]) + " - " + str(datetime.now()) + " ] \t" + text


def readFifo(fifo_path):
	try:
		pipe = os.open(fifo_path , os.O_RDONLY )#| os.O_NONBLOCK)
		data = os.read(pipe, 4096)
    		os.close(pipe)
	except OSError as err:
        	if err.errno == 11:
			return
        	else:
            		#raise err
			debug("OSError : " + str(err))
			return ''
   	if data!= '':
		item = data.split()
		lastitem = item[len(item)-1]
		return lastitem
	else:
		return '' 

def writeFifo(fifo_path, data):
	try:
		pipe = os.open(fifo_path, os.O_WRONLY )#| os.O_NONBLOCK)
		os.write(pipe, data + "\n")
		os.close(pipe)
		return 0
	except:
		debug("except writing " + data + " to " + fifo_path)
		return 1
