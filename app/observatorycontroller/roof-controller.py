#!/usr/bin/python

import os
import errno
import time
from datetime import datetime
import sys
import signal
import serial

from tools import *

BOARDNAME = "roof"



status = {	"board_state":"Connecting",
		"board_vin":"Connecting", 
		"state":"Connecting",
		"opened":"Connecting",
		"closed":"Connecting"
}  

control = {	"move":""
}  



#handle Ctrl-C (SIGINT) and Kill (SIGTERM) properly
def sigint_handler(signum, frame):
	debug("SIGINT received!   Closing connections...")
	deleteFifos()
	disconnect()
	debug("exit(0)")
	sys.exit(0)

def sigterm_handler(signum, frame):
	debug("SIGTERM received!   Closing connections...")
	deleteFifos()
	disconnect()
	debug("exit(0)")
	sys.exit(0)

signal.signal(signal.SIGINT, sigint_handler)
signal.signal(signal.SIGTERM, sigterm_handler)



#fifo stuff
fifo_board_path = fifo_root_path + BOARDNAME + "/"
fifo_status_path = fifo_board_path + "status/"
fifo_control_path = fifo_board_path + "control/"

def createFifos():
	mkdir(fifo_status_path)
	mkdir(fifo_control_path)

	for name in status:
		mkfifo(fifo_status_path + name)

	for name in control:
		mkfifo(fifo_control_path + name)

def deleteFifos():
	for name in status:
		rmfile(fifo_status_path + name)	

	for name in control:
		rmfile(fifo_control_path + name)	

	rmdir(fifo_status_path)
	rmdir(fifo_control_path)
	rmdir(fifo_board_path)

def ReadFIFO_control_move():
	fifo_path = fifo_control_path + "move"
	try:
		pipe = os.open(fifo_path, os.O_RDONLY | os.O_NONBLOCK)
		data = os.read(pipe, 4096)
		os.close(pipe)
	except OSError as err:
		if err.errno == 11:
			return
		else:
			raise err
	if data != '':
		item = data.split()
		lastitem = item[len(item)-1]

		sendCmd(lastitem)


def WriteFIFOs_status():
	for name in status:
		try:
			fifo_path = fifo_status_path + name
			pipe = os.open(fifo_path, os.O_WRONLY | os.O_NONBLOCK)
			os.write(pipe, status[name] + "\n")
			os.close(pipe)		
		except:
			pass




#serial port stuff
serialport = str(sys.argv[1])
serialportbaudrate = 9600

def connect():
	global ser
	
	debug("Connecting " + serialport + " ")
	
	while True:
		try:
			sys.stdout.write(".")
   			sys.stdout.flush()

			ser = serial.Serial( port=serialport, baudrate=serialportbaudrate, timeout=5 )
			ser.isOpen()
			
			print ""
			debug("serial port " + serialport + " connected at " + str(serialportbaudrate) + " bauds")
			serialWrite("@") #start automatic status sending on arduino
			return

		except serial.SerialException:
			WriteFIFOs_status()
			time.sleep(1)	
	
def disconnect():
	try:
		global ser
		if ser.isOpen():
			serialWrite("#") #stop automatic status sending on arduino
		ser.close()
		debug("serial port disconnected")
	except:
		return

def reconnect():
	debug("reconnecting serial port...")
	
	status["board_state"] = "Reconnecting"
	status["board_vin"] = "Reconnecting"
	status["state"] = "Reconnecting"
	status["opened"] = "Reconnecting"
	status["closed"] = "Reconnecting"
	
	try: 
		ser.close()
	except:
		pass

	connect()

def serialRead(len):
	try:
		data = ser.read(len)
		return data
	except serial.SerialException:
		debug("Error reading " + str(len) + " byte")
		reconnect()

def serialWrite(data):
	try:
		ser.write(data)
		return
	except serial.SerialException:
		debug("Error sending " + str(data) )
		reconnect()

def sendCmd(cmd):
	debug("sendCmd(\"" + cmd + "\")")
	serialWrite(cmd+"\r")













def updateStatus():
	datastring = ""
	data = ''
	
	serialWrite("@")
#	time.sleep(0.25)

	while ser.inWaiting() > 0:
		data = serialRead(1)
		datastring += data

		if data == "@":
			datastring = "@"

		if data == "$":
			#datastring = "@-O-12036-STATE-0-0-$"
			item = datastring.split('-')
			if len(item) != 7:
				return

			board_state = True
				
			if item[0] != '@':
				board_state = False

			if item[6] != '$':
				board_state = False

			if item[1] != 'O':
				board_state = False

			if board_state == True:
				status["board_state"] = "OK"
			else:
				status["board_state"] = "Error"
			
			status["board_vin"] = item[2] + "mV"
			status["state"] = item[3]
			status["opened"] = item[4]
			status["closed"] = item[5]
	return








#main code
connect()
createFifos()
debug("Init done!")

while True:
	updateStatus()	
	WriteFIFOs_status()
	ReadFIFO_control_move()
	time.sleep(0.1)

