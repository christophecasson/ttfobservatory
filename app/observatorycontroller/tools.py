import os
import errno
import time
from datetime import datetime
import sys
import signal
import serial



fifo_root_path  = "/home/astro/fifo/"


#handle debug log
def debug(text):
	print("[ " + os.path.basename(sys.argv[0]) + " - " + str(datetime.now()) + " ] \t" + text)



def mkdir(path):
	try:
		os.stat(path)
		debug("directory exists ( " + path + " )")
	except:
		os.makedirs(path)
		debug("directory created ( " + path + " )")

def mkfifo(path):
	try:
		os.stat(path)
		debug("fifo exists ( " + path + " )")
	except:
		os.mkfifo(path)
		debug("fifo created ( " + path + " )")

def rmfile(path):
	try:
		os.stat(path)
		os.remove(path)
		debug("file deleted ( " + path + " )")
	except:
		debug("error deleting file ( " + path + " )")

def rmdir(path):
	try:
		os.stat(path)
		os.rmdir(path)
		debug("directory deleted ( " + path + " )")
	except:
		debug("error deleting directory ( " + path + " )")



