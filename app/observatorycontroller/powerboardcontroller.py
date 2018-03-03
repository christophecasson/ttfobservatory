#!/usr/bin/python

import os
import errno
import time
from datetime import datetime
import sys
import signal
import serial








serialport = str(sys.argv[1])
serialportbaudrate = 9600

board_state = False

#handle debug log
def debug(text):
	print "[ powerboardcontroller.py - " + str(datetime.now()) + " ] \t" + text 

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
	
	WriteFifo_status("board_state", "ERR\r\n")
	WriteFifo_status("board_vin", "ERR\r\n")
	WriteFifo_status(1, "ERR\r\n")
	WriteFifo_status(2, "ERR\r\n")
	WriteFifo_status(3, "ERR\r\n")
	WriteFifo_status(4, "ERR\r\n")
	WriteFifo_status(5, "ERR\r\n")
	WriteFifo_status(6, "ERR\r\n")
	
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
				WriteFifo_status("board_state", "O")
				WriteFifo_status("board_vin", item[2])
				WriteFifo_status(1, item[3])
				WriteFifo_status(2, item[4])
				WriteFifo_status(3, item[5])
				WriteFifo_status(4, item[6])
				WriteFifo_status(5, item[7])
				WriteFifo_status(6, item[8])
			else:
				WriteFifo_status("board_state", "E")
				WriteFifo_status("board_vin", "E")
				WriteFifo_status(1, "E")
				WriteFifo_status(2, "E")
				WriteFifo_status(3, "E")
				WriteFifo_status(4, "E")
				WriteFifo_status(5, "E")
				WriteFifo_status(6, "E")
				
	return



def sendCmd(cmd):
	serialWrite(cmd+"\r")

		

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
	


fifo_root_path 	= "/home/astro/fifo/"
fifo_board_path = fifo_root_path + "powerboard/"
fifo_status_path = fifo_board_path + "status/"
fifo_control_path = fifo_board_path + "control/"


def createFifos():
	mkdir(fifo_status_path)
	mkdir(fifo_control_path)

	mkfifo(fifo_status_path + "board_state")
	mkfifo(fifo_status_path + "board_vin")
	mkfifo(fifo_status_path + "1")
	mkfifo(fifo_status_path + "2")
	mkfifo(fifo_status_path + "3")
	mkfifo(fifo_status_path + "4")
	mkfifo(fifo_status_path + "5")
	mkfifo(fifo_status_path + "6")

	mkfifo(fifo_control_path + "1")
	mkfifo(fifo_control_path + "2")
	mkfifo(fifo_control_path + "3")
	mkfifo(fifo_control_path + "4")
	mkfifo(fifo_control_path + "5")
	mkfifo(fifo_control_path + "6")

def deleteFifos():
	rmfile(fifo_status_path + "board_state")
	rmfile(fifo_status_path + "board_vin")
	rmfile(fifo_status_path + "1")
	rmfile(fifo_status_path + "2")
	rmfile(fifo_status_path + "3")
	rmfile(fifo_status_path + "4")
	rmfile(fifo_status_path + "5")
	rmfile(fifo_status_path + "6")

	rmfile(fifo_control_path + "1")
	rmfile(fifo_control_path + "2")
	rmfile(fifo_control_path + "3")
	rmfile(fifo_control_path + "4")
	rmfile(fifo_control_path + "5")
	rmfile(fifo_control_path + "6")

	rmdir(fifo_status_path)
	rmdir(fifo_control_path)
	rmdir(fifo_board_path)




def ReadFifo_control(i):
	fifo_path = fifo_control_path + str(i)
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
		debug("sendCmd(\"" + cmd + "\")")
		sendCmd(cmd)
	elif data == "1":
		cmd = str(i) + " ON"
		debug("sendCmd(\"" + cmd + "\")")
		sendCmd(cmd)

	os.close(pipe)
#	time.sleep(0.01)
	return


def WriteFifo_status(i, data):
	fifo_path = fifo_status_path + str(i)
	try:
		pipe = os.open(fifo_path, os.O_WRONLY | os.O_NONBLOCK)
		os.write(pipe, data)
	except:
		return

	os.close(pipe)
	return



connect()
createFifos()

while True:
	for i in range(1,7):
		ReadFifo_control(i)
		
	updateStatus()

disconnect()
deleteFifos()
