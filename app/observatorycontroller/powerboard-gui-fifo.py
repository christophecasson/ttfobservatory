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



btn_out1_state = False
btn_out2_state = False
btn_out3_state = False
btn_out4_state = False
btn_out5_state = False
btn_out6_state = False

board_state = False
board_vin = 0

last_btn_out1_state = False
last_btn_out2_state = False
last_btn_out3_state = False
last_btn_out4_state = False
last_btn_out5_state = False
last_btn_out6_state = False

last_board_state = False
last_board_vin = 0

closeapp = False



fifo_root_path 	= "/home/astro/fifo/"
fifo_board_path = fifo_root_path + "powerboard/"
fifo_status_path = fifo_board_path + "status/"
fifo_control_path = fifo_board_path + "control/"


fifo_boardstate = 0
fifo_vin = 0
fifo_1 = 0
fifo_2 = 0
fifo_3 = 0
fifo_4 = 0
fifo_5 = 0
fifo_6 = 0

def openfifos():
	global fifo_boardstate
	global fifo_vin
	global fifo_1
	global fifo_2
	global fifo_3
	global fifo_4
	global fifo_5
	global fifo_6

	fifo_boardstate = os.open(fifo_status_path + "board_state", os.O_RDONLY | os.O_NONBLOCK)
	fifo_vin = os.open(fifo_status_path + "board_vin", os.O_RDONLY | os.O_NONBLOCK)
	fifo_1 = os.open(fifo_status_path + "1", os.O_RDONLY | os.O_NONBLOCK)
	fifo_2 = os.open(fifo_status_path + "2", os.O_RDONLY | os.O_NONBLOCK)
	fifo_3 = os.open(fifo_status_path + "3", os.O_RDONLY | os.O_NONBLOCK)
	fifo_4 = os.open(fifo_status_path + "4", os.O_RDONLY | os.O_NONBLOCK)
	fifo_5 = os.open(fifo_status_path + "5", os.O_RDONLY | os.O_NONBLOCK)
	fifo_6 = os.open(fifo_status_path + "6", os.O_RDONLY | os.O_NONBLOCK)

def closefifos():
	global fifo_boardstate
	global fifo_vin
	global fifo_1
	global fifo_2
	global fifo_3
	global fifo_4
	global fifo_5
	global fifo_6
	
	os.close(fifo_boardstate)
	os.close(fifo_vin)
	os.close(fifo_1)
	os.close(fifo_2)
	os.close(fifo_3)
	os.close(fifo_4)
	os.close(fifo_5)
	os.close(fifo_6)




def readFifo(i):
	fifo_path = fifo_status_path + str(i)
	try:
		fifo = os.open(fifo_path , os.O_RDONLY )#| os.O_NONBLOCK)
#		debug(fifo_path + " opened")
#		time.sleep(0.25)
		data = os.read(fifo, 1)
    		os.close(fifo)
#		debug("OK read [" + data + "] to " + fifo_path)
#		debug("fifo closed")
		#debug("read data from pipe=" + str(fifo) + ") : " + data)
		#time.sleep(0.1)
    		return data
	except OSError as err:
        	if err.errno == 11:
#			debug("errno 11")
			return
        	else:
            		raise err
    




def writeFifo(i, data):
	fifo_path = fifo_control_path + str(i)
	
	for i in range(10):
		cmd = "echo  '" + data + "' > " + fifo_path

	#pipe = os.open(fifo_path, os.O_RDONLY | os.O_NONBLOCK)
		debug("system(" + cmd + ")")
		os.system(cmd)
	#os.close(pipe)	

	#try:
	#	pipe = os.open(fifo_path, os.O_WRONLY | os.O_NONBLOCK)
	#	debug(fifo_path + " opened")
	#	os.write(pipe, data)
	#	debug("OK write " + data + " to " + fifo_path)
	#	os.close(pipe)
	#	debug("fifo closed")
	#except:
	#	debug("except writing " + data + " to " + fifo_path)
	#	return

	return




def StateUpdater():

	global last_btn_out1_state
	global last_btn_out2_state
	global last_btn_out3_state
	global last_btn_out4_state
	global last_btn_out5_state
	global last_btn_out6_state
	global last_board_state
	global last_board_vin

	if last_board_vin != board_vin:
		app.setLabel("l_Vin", str(board_vin)+" mV")
	
	if board_state == False:
		if board_state != last_board_state:
			last_board_state = board_state
			app.setLabel("l_BoardStatus", "BOARD ERROR")
			app.setBg("#500000")
			setButtonUnknown("btn_out1")
			setButtonUnknown("btn_out2")
			setButtonUnknown("btn_out3")
			setButtonUnknown("btn_out4")
			setButtonUnknown("btn_out5")
			setButtonUnknown("btn_out6")

	else:
		if board_state != last_board_state:
			last_board_state = board_state
			app.setLabel("l_BoardStatus", "BOARD OK")
			app.setBg("#202020")
			last_btn_out1_state = not btn_out1_state
			last_btn_out2_state = not btn_out2_state
			last_btn_out3_state = not btn_out3_state
			last_btn_out4_state = not btn_out4_state
			last_btn_out5_state = not btn_out5_state
			last_btn_out6_state = not btn_out6_state
			

		if btn_out1_state != last_btn_out1_state:
			last_btn_out1_state = btn_out1_state
			if btn_out1_state == False:
				setButtonOFF("btn_out1")
			else:
				setButtonON("btn_out1")

		if btn_out2_state != last_btn_out2_state:
			last_btn_out2_state = btn_out2_state
			if btn_out2_state == False:
				setButtonOFF("btn_out2")
			else:
				setButtonON("btn_out2")

		if btn_out3_state != last_btn_out3_state:
			last_btn_out3_state = btn_out3_state
			if btn_out3_state == False:
				setButtonOFF("btn_out3")
			else:
				setButtonON("btn_out3")

		if btn_out4_state != last_btn_out4_state:
			last_btn_out4_state = btn_out4_state
			if btn_out4_state == False:
				setButtonOFF("btn_out4")
			else:
				setButtonON("btn_out4")

		if btn_out5_state != last_btn_out5_state:
			last_btn_out5_state = btn_out5_state
			if btn_out5_state == False:
				setButtonOFF("btn_out5")
			else:
				setButtonON("btn_out5")

		if btn_out6_state != last_btn_out6_state:
			last_btn_out6_state = btn_out6_state
			if btn_out6_state == False:
				setButtonOFF("btn_out6")
			else:
				setButtonON("btn_out6")






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



def updateStatus():

	global app

	global board_state
	global board_vin

	global btn_out1_state
	global btn_out2_state
	global btn_out3_state
	global btn_out4_state
	global btn_out5_state
	global btn_out6_state

	while closeapp == False:

		data = readFifo("board_state")
		if data == "O":
			board_state = True
			#data = readFifo(fifo_boardstate)
			#if data == 'K':
			#	board_state = True
			#else:
			#	board_state = False
		else:
			board_state = False


		if board_state == True:
			
			data = readFifo(1)
			if data == '0':
				btn_out1_state = False
			elif data == '1':
				btn_out1_state = True
			else:
				board_state = False

			data = readFifo(2)
			if data == '0':
				btn_out2_state = False
			elif data == '1':
				btn_out2_state = True
			else:
				board_state = False

			data = readFifo(3)
			if data == '0':
				btn_out3_state = False
			elif data == '1':
				btn_out3_state = True
			else:
				board_state = False

			data = readFifo(4)
			if data == '0':
				btn_out4_state = False
			elif data == '1':
				btn_out4_state = True
			else:
				board_state = False

			data = readFifo(5)
			if data == '0':
				btn_out5_state = False
			elif data == '1':
				btn_out5_state = True
			else:
				board_state = False

			data = readFifo(6)
			if data == '0':
				btn_out6_state = False
			elif data == '1':
				btn_out6_state = True
			else:
				board_state = False


		StateUpdater()
		time.sleep(0.1)







	
	



					


# handle button events
def press(button):

	if button == "btn_out1":
		if btn_out1_state == False:
			writeFifo(1, "1")
		else:
			writeFifo(1, "0")

	if button == "btn_out2":
		if btn_out2_state == False:
			writeFifo(2, "1")
		else:
			writeFifo(2, "0")

	if button == "btn_out3":
		if btn_out3_state == False:
			writeFifo(3, "1")
		else:
			writeFifo(3, "0")

	if button == "btn_out4":
		if btn_out4_state == False:
			writeFifo(4, "1")
		else:
			writeFifo(4, "0")

	if button == "btn_out5":
		if btn_out5_state == False:
			writeFifo(5, "1")
		else:
			writeFifo(5, "0")

	if button == "btn_out6":
		if btn_out6_state == False:
			writeFifo(6, "1")
		else:
			writeFifo(6, "0")





# create a GUI variable called app
app = gui("Power Board", "220x600", handleArgs=False)
app.setBg("#202020")
app.setFg("red")
app.setFont(24)




#app.addButton("Update", press, 0, 1)
#app.setButton("Update", "Update status")


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


#openfifos()
#time.sleep(1)


app.thread(updateStatus)


# start the GUI
app.go()


closeapp = True
#app.stop()




