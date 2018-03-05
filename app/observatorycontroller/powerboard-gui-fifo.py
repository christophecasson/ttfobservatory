#!/usr/bin/python

from appJar import gui
from threading import Thread
import time
import sys
import os
import errno
from datetime import datetime
import signal




#handle debug log
def debug(text):
	print "[ powerboard-gui.py - " + str(datetime.now()) + " ] \t" + text 

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



btn_state = {		"1":"-",
			"2":"-",
			"3":"-",
			"4":"-",
			"5":"-",
			"6":"-"
}

last_btn_state = {	"1":"-",
			"2":"-",
			"3":"-",
			"4":"-",
			"5":"-",
			"6":"-"
}


board_state = "Init..."
last_board_state = "Init..."
board_vin = "Init..."
last_board_vin = "Init..."


closeapp = False



fifo_root_path 	= "/home/astro/fifo/"
fifo_board_path = fifo_root_path + "powerboard/"
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
	global btn_state

	while closeapp == False:
		board_state = readFifo("board_state")
		board_vin = readFifo("board_vin")

		if board_state == "OK":
			for name in btn_state:
				btn_state[name] = readFifo(name)

		StateUpdater()
		time.sleep(0.1)

#GUI updater
def StateUpdater():

	global last_btn_state
	global last_board_state
	global last_board_vin

	if last_board_vin != board_vin:
		app.setLabel("l_Vin", "Vin="+board_vin)
	
	if board_state != "OK":
		if board_state != last_board_state:
			last_board_state = board_state
			app.setLabel("l_BoardStatus", "Board " + board_state)
			app.setBg("#500000")
			for name in btn_state:
				setButtonUnknown("btn_out"+name)

	else:
		if board_state != last_board_state:
			last_board_state = board_state
			app.setLabel("l_BoardStatus", "Board " + board_state)
			app.setBg("#202020")
			for name in btn_state:
				last_btn_state[name] = not btn_state[name]

		for name in btn_state:
			if btn_state[name] != last_btn_state[name]:
				last_btn_state[name] = btn_state[name]
				if btn_state[name] == "0":
					setButtonOFF("btn_out"+name)
				elif btn_state[name] == "1":
					setButtonON("btn_out"+name)
				else:
					setButtonUnknown("btn_out"+name)


# handle button events
def press(button):
	for name in btn_state:
		if button == "btn_out"+name:
			if btn_state[name] == "0":
				writeFifo(name, "1")
			elif btn_state[name] == "1":
				writeFifo(name, "0")
			else:
				pass



# create GUI
app = gui("Power Board", "225x600", handleArgs=False)
app.setBg("#202020")
app.setFg("red")
app.setFont(24)



app.addLabel("l_out1", "1: Roof", 1, 0)
app.setLabelAlign("l_out1", "left")
app.addButton("btn_out1", press, 1, 1)
setButtonUnknown("btn_out1")

app.addLabel("l_out2", "2: Mount", 2, 0)
app.setLabelAlign("l_out2", "left")
app.addButton("btn_out2", press, 2, 1)
setButtonUnknown("btn_out2")

app.addLabel("l_out3", "3: DSLR", 3, 0)
app.setLabelAlign("l_out3", "left")
app.addButton("btn_out3", press, 3, 1)
setButtonUnknown("btn_out3")

app.addLabel("l_out4", "4:", 4, 0)
app.setLabelAlign("l_out4", "left")
app.addButton("btn_out4", press, 4, 1)
setButtonUnknown("btn_out4")

app.addLabel("l_out5", "5: Flat", 5, 0)
app.setLabelAlign("l_out5", "left")
app.addButton("btn_out5", press, 5, 1)
setButtonUnknown("btn_out5")

app.addLabel("l_out6", "6: ", 6, 0)
app.setLabelAlign("l_out6", "left")
app.addButton("btn_out6", press, 6, 1)
setButtonUnknown("btn_out6")


app.addLabel("l_BoardStatus")
app.addLabel("l_Vin")
app.getLabelWidget("l_Vin").config(font="Courier 18")



#start update thread
app.thread(updateStatus)

# start the GUI
app.go()

#close update thread
closeapp = True




