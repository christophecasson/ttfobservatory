
from appJar import gui
from threading import Thread
import time
import serial
import termios
import sys



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


serialport = str(sys.argv[1])
serialportbaudrate = 9600



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


def sendCmd(cmd):
	try:
		ser.write(cmd+"\r")
	except:
		print "-----3-----"
		app.stop()




	
	

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

	datastring = ""
	data = ''

	while closeapp == False:

		
		try:
			ser.write("@")
		except:
			print "-----1-----"
			app.stop()


		time.sleep(0.25)

		while ser.inWaiting() > 0:
			try:
				data = ser.read(1)
			except:
				print "-----2-----"
				app.stop()



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

				board_vin = item[2]
				
				if item[3] == '0':
					btn_out1_state = False
				elif item[3] == '1':
					btn_out1_state = True
				else:
					board_state = False

				if item[4] == '0':
					btn_out2_state = False
				elif item[4] == '1':
					btn_out2_state = True
				else:
					board_state = False

				if item[5] == '0':
					btn_out3_state = False
				elif item[5] == '1':
					btn_out3_state = True
				else:
					board_state = False

				if item[6] == '0':
					btn_out4_state = False
				elif item[6] == '1':
					btn_out4_state = True
				else:
					board_state = False

				if item[7] == '0':
					btn_out5_state = False
				elif item[7] == '1':
					btn_out5_state = True
				else:
					board_state = False

				if item[8] == '0':
					btn_out6_state = False
				elif item[8] == '1':
					btn_out6_state = True
				else:
					board_state = False

				#datastring = ""

				StateUpdater()
				#time.sleep(0.1)
				#break


					


# handle button events
def press(button):

	if button == "btn_out1":
		if btn_out1_state == False:
			sendCmd("1 ON")
		else:
			sendCmd("1 OFF")

	if button == "btn_out2":
		if btn_out2_state == False:
			sendCmd("2 ON")
		else:
			sendCmd("2 OFF")

	if button == "btn_out3":
		if btn_out3_state == False:
			sendCmd("3 ON")
		else:
			sendCmd("3 OFF")

	if button == "btn_out4":
		if btn_out4_state == False:
			sendCmd("4 ON")
		else:
			sendCmd("4 OFF")

	if button == "btn_out5":
		if btn_out5_state == False:
			sendCmd("5 ON")
		else:
			sendCmd("5 OFF")

	if button == "btn_out6":
		if btn_out6_state == False:
			sendCmd("6 ON")
		else:
			sendCmd("6 OFF")

	#updateStatus()




# create a GUI variable called app
app = gui("Power Board", "200x600", handleArgs=False)
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



app.addLabel("l_serial")
app.setLabel("l_serial", serialport+":"+str(serialportbaudrate))
app.getLabelWidget("l_serial").config(font="Courier 12")


app.addLabel("l_BoardStatus")
app.addLabel("l_Vin")
#app.setLabel("VinStr", "VinStr")
#app.getLabelWidget("VinStr").config(font="Courier 15")





connect()

app.thread(updateStatus)


# start the GUI
app.go()


closeapp = True
disconnect()

#app.stop()




