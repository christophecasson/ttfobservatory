#!/usr/bin/python

import os
import errno
import time
import sys
import signal
import serial

fifo_status_pwr_path = "/home/astro/DEV/ttfobservatory/app/observatorycontroller/fifo/status/pwr/"
fifo_control_pwr_path = "/home/astro/DEV/ttfobservatory/app/observatorycontroller/fifo/control/pwr/"

serialport = str(sys.argv[1])
serialportbaudrate = 9600

board_state = False


#handle Ctrl-C (SIGINT) and Kill (SIGTERM) properly
def sigint_handler(signum, frame):
	print "SIGINT received!\r\n Closing connections..."
	disconnect()
	print "exit(0)"
	sys.exit(0)

def sigterm_handler(signum, frame):
	print "SIGTERM received!\r\n Closing connections..."
	disconnect()
	print "exit(0)"
	sys.exit(0)
 
signal.signal(signal.SIGINT, sigint_handler)
signal.signal(signal.SIGTERM, sigterm_handler)




def connect():
	global ser
	ser = serial.Serial( port=serialport, baudrate=serialportbaudrate, timeout=5 )
	ser.isOpen()
	print("serial port " + serialport + " connected at " + str(serialportbaudrate) + " bauds")
	ser.write("@") #start automatic status sending on arduino

def disconnect():
	global ser
	if ser.isOpen():
		ser.write("#") #stop automatic status sending on arduino
	ser.close()
	print("serial port disconnected")


def updateStatus():
	datastring = ""
	data = ''
	
	try:
		ser.write("@")
	except:
		print "[Error] sending @\r\n"
		return


	time.sleep(0.25)

	while ser.inWaiting() > 0:
		try:
			data = ser.read(1)
		except:
			print "[Error] reading 1 byte"
			return

		datastring += data

		if data == "@":
			datastring = "@"


		if data == "$":
			#datastring = "@-O-12036-1-1-1-0-1-0-$"
			item = datastring.split('-')

			board_state = True
				
			if item[0] != '@':
				board_state = False

			if item[9] != '$':
				board_state = False

			if item[1] != 'O':
				board_state = False

			if board_state == True:
				WriteFifo_status_pwr("board_state", "OK\r\n")
				WriteFifo_status_pwr("board_vin", item[2] + "\r\n")
				WriteFifo_status_pwr(1, item[3] + "\r\n")
				WriteFifo_status_pwr(2, item[4] + "\r\n")
				WriteFifo_status_pwr(3, item[5] + "\r\n")
				WriteFifo_status_pwr(4, item[6] + "\r\n")
				WriteFifo_status_pwr(5, item[7] + "\r\n")
				WriteFifo_status_pwr(6, item[8] + "\r\n")
			else:
				WriteFifo_status_pwr("board_state", "ERR\r\n")
				WriteFifo_status_pwr("board_vin", "ERR\r\n")
				WriteFifo_status_pwr(1, "ERR\r\n")
				WriteFifo_status_pwr(2, "ERR\r\n")
				WriteFifo_status_pwr(3, "ERR\r\n")
				WriteFifo_status_pwr(4, "ERR\r\n")
				WriteFifo_status_pwr(5, "ERR\r\n")
				WriteFifo_status_pwr(6, "ERR\r\n")
				
	return



def sendCmd(cmd):
	try:
		ser.write(cmd+"\r")
	except:
		print "[Error] sending cmd\r\n"
		

def ReadFifo_control_pwr(i):
	fifo_path = fifo_control_pwr_path + str(i)
	try:
		pipe = os.open(fifo_path , os.O_RDONLY | os.O_NONBLOCK)
		data = os.read(pipe, 1)
	except OSError as err:
        	if err.errno == 11:
			return
        	else:
            		raise err
    	if data == "0":
		cmd = str(i) + " OFF"
		print "sendCmd(\"" + cmd + "\")\r\n"
		sendCmd(cmd)
	elif data == "1":
		cmd = str(i) + " ON"
		print "sendCmd(\"" + cmd + "\")\r\n"
		sendCmd(cmd)

	os.close(pipe)
	time.sleep(0.01)
	return


def WriteFifo_status_pwr(i, data):
	fifo_path = fifo_status_pwr_path + str(i)
	try:
		pipe = os.open(fifo_path, os.O_WRONLY | os.O_NONBLOCK)
		os.write(pipe, data)
	except:
		return

	os.close(pipe)
	return




connect()

while True:
	for i in range(1,7):
		ReadFifo_control_pwr(i)
		
	updateStatus()



disconnect()

