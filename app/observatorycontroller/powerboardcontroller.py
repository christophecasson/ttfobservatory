#!/usr/bin/python

import os
import errno
import time
from datetime import datetime
import sys
import signal
import serial



fifo_root_path 	= "/home/astro/fifo/"
fifo_board_path = fifo_root_path + "powerboard/"
fifo_status_path = fifo_board_path + "status/"
fifo_control_path = fifo_board_path + "control/"

status = {	"board_state":"Connecting",
		"board_vin":"Connecting", 
		"1":"Connecting",
		"2":"Connecting", 
		"3":"Connecting", 
		"4":"Connecting", 
		"5":"Connecting", 
		"6":"Connecting"
}  

control = {	"1":"",
		"2":"", 
		"3":"", 
		"4":"", 
		"5":"", 
		"6":""
}  




serialport = str(sys.argv[1])
serialportbaudrate = 9600


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
	status["1"] = "Reconnecting"
	status["2"] = "Reconnecting"
	status["3"] = "Reconnecting"
	status["4"] = "Reconnecting"
	status["5"] = "Reconnecting"
	status["6"] = "Reconnecting"
	
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
			if len(item) != 10:
				return

			board_state = True
				
			if item[0] != '@':
				board_state = False

			if item[9] != '$':
				board_state = False

			if item[1] != 'O':
				board_state = False

			if board_state == True:
				status["board_state"] = "OK"
				status["board_vin"] = item[2] + "mV"
				status["1"] = item[3]
				status["2"] = item[4]
				status["3"] = item[5]
				status["4"] = item[6]
				status["5"] = item[7]
				status["6"] = item[8]
			else:
				status["board_state"] = "Error"
				status["board_vin"] = "Error"
				status["1"] = "Error"
				status["2"] = "Error"
				status["3"] = "Error"
				status["4"] = "Error"
				status["5"] = "Error"
				status["6"] = "Error"
				
	return




		

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



def sendCmd(cmd):
	debug("sendCmd(\"" + cmd + "\")")
	serialWrite(cmd+"\r")


def ReadFIFOs_control():
	for name in control:
		fifo_path = fifo_control_path + name
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

			if lastitem == "0":
				sendCmd(name + " OFF")
			elif lastitem == "1":
				sendCmd(name + " ON")



def WriteFIFOs_status():
	for name in status:
		try:
			fifo_path = fifo_status_path + name
			pipe = os.open(fifo_path, os.O_WRONLY | os.O_NONBLOCK)
			os.write(pipe, status[name] + "\n")
			os.close(pipe)		
		except:
			#debug("except writing " + status[name] + " to " + fifo_path)
			pass

		#cmd = "echo \"" + status[name] + "\\c\" > " + fifo_path
		#pipe = os.open(fifo_path, os.O_RDONLY | os.O_NONBLOCK)
		#os.system(cmd)
		#os.close(pipe)





connect()
createFifos()
debug("Init done!")

while True:
	updateStatus()	
	WriteFIFOs_status()
	ReadFIFOs_control()
	time.sleep(0.1)












