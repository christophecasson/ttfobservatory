#!/usr/bin/python

from appJar import gui
from threading import Thread
import time
import sys
import os
import errno
from datetime import datetime
import signal

from tools import *

BOARDNAME = "roof"



roofstate = {		"state":"-",
			"opened":"-",
			"closed":"-"
}

last_roofstate = {	"state":"-",
			"opened":"-",
			"closed":"-"
}

board_state = "Init..."
last_board_state = "Init..."
board_vin = "Init..."
last_board_vin = "Init..."


closeapp = False




#handle Ctrl-C (SIGINT) and Kill (SIGTERM) properly
def sigint_handler(signum, frame):
	debug("SIGINT received!   Closing connections...")
#	closefifos()
	debug("exit(0)")
	sys.exit(0)

def sigterm_handler(signum, frame):
	debug("SIGTERM received!   Closing connections...")
#	closefifos()
	debug("exit(0)")
	sys.exit(0)
 
signal.signal(signal.SIGINT, sigint_handler)
signal.signal(signal.SIGTERM, sigterm_handler)










#fifo stuff
fifo_board_path = fifo_root_path + BOARDNAME + "/"
fifo_status_path = fifo_board_path + "status/"
fifo_control_path = fifo_board_path + "control/"

def readFifo(i):
	fifo_path = fifo_status_path + str(i)
	try:
		pipe = os.open(fifo_path , os.O_RDONLY )#| os.O_NONBLOCK)
		data = os.read(pipe, 4096)
    		os.close(pipe)
	except OSError as err:
        	if err.errno == 11:
			return
        	else:
            		raise err
   	if data!= '':
		item = data.split()
		lastitem = item[len(item)-1]
		return lastitem
	else:
		return '' 

def writeFifo(i, data):
	fifo_path = fifo_control_path + str(i)
	while True:
		try:
			pipe = os.open(fifo_path, os.O_WRONLY )#| os.O_NONBLOCK)
			os.write(pipe, data + "\n")
			os.close(pipe)
			return
		except:
			debug("except writing " + data + " to " + fifo_path)
			pass





def setButtonON(button):
	app.setButton(button, " ON  ")
	app.setButtonBg(button, "red")
	app.setButtonFg(button, "#202020")
	
def setButtonOFF(button):
	app.setButton(button, " OFF ")
	app.setButtonBg(button, "#202020")
	app.setButtonFg(button, "#AAAAAA")

def setButtonUnknown(button):
	app.setButton(button, "  -  ")
	app.setButtonBg(button, "#808080")
	app.setButtonFg(button, "#DDDDDD")


#thread : data updater
def updateStatus():
	global board_state
	global board_vin
	global roofstate

	while closeapp == False:
		board_state = readFifo("board_state")
		board_vin = readFifo("board_vin")

		for name in roofstate:
			roofstate[name] = readFifo(name)

		StateUpdater()
		time.sleep(0.1)

#GUI updater
def StateUpdater():

	global last_roofstate
	global last_board_state
	global last_board_vin

	if last_board_vin != board_vin:
		app.setLabel("l_Vin", "Vin="+board_vin)
	
	if board_state != "OK":
		if board_state != last_board_state:
			last_board_state = board_state
			app.setLabel("l_BoardStatus", "Board " + board_state)
			app.setBg("#500000")

	else:
		if board_state != last_board_state:
			last_board_state = board_state
			app.setLabel("l_BoardStatus", "Board " + board_state)
			app.setBg("#202020")
	

	for name in roofstate:
		if roofstate[name] != last_roofstate[name]:
			last_roofstate[name] = roofstate[name]
			app.setLabel(name, roofstate[name])
			if name == "state":
				roof_state = roofstate[name]
				if roof_state == "OPENED":
					app.setImage("state_img", "ressources/Opened.png")
				if roof_state == "CLOSED":
					app.setImage("state_img", "ressources/Closed.png")
				if roof_state == "OPENING":
					app.setImage("state_img", "ressources/Opening.png")
				if roof_state == "CLOSING":
					app.setImage("state_img", "ressources/Closing.png")
				if roof_state == "ABORT":
					app.setImage("state_img", "ressources/Idle.png")
				if roof_state == "IDLE":
					app.setImage("state_img", "ressources/Idle.png")


# handle button events
def press(button):
	
	if button == "abort":
		writeFifo("move", "ABORT")
	
	if button == "open":
		writeFifo("move", "OPEN")

	if button == "close":
		writeFifo("move", "CLOSE")



# create GUI
app = gui("Roof", "240x600", handleArgs=False)
app.setBg("#202020")
app.setFg("red")
app.setFont(20)


app.addImage("state_img", "ressources/Idle.png", 1, 0)
app.addLabel("state", " --- ", 1, 1)
app.addLabel("l_opened", "O lock", 2, 0)
app.addLabel("l_closed", "C lock", 2, 1)
app.addLabel("opened", "-", 3, 0)
app.addLabel("closed", "-", 3, 1)
app.addButton("open", press, 5, 0)
app.addButton("close", press, 5, 1)
app.addButton("abort", press)


app.addLabel("l_BoardStatus")
app.addLabel("l_Vin")
app.getLabelWidget("l_Vin").config(font="Courier 14")



#start update thread
app.thread(updateStatus)

# start the GUI
app.go()

#close update thread
closeapp = True




