
from appJar import gui
from threading import Thread
import time
import serial
import termios
import sys



roof_state = "UNKNOWN"
board_state = False
board_vin = 0

last_roof_state = "UNKNOWN"
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

	global last_roof_state
	global last_board_state
	global last_board_vin

	if last_board_vin != board_vin:
		app.setLabel("l_Vin", str(board_vin)+" mV")
	
	if board_state == False:
		if board_state != last_board_state:
			last_board_state = board_state
			app.setLabel("l_BoardStatus", "BOARD ERROR")
			app.setBg("#500000")

	else:
		if board_state != last_board_state:
			last_board_state = board_state
			app.setLabel("l_BoardStatus", "BOARD OK")
			app.setBg("#202020")
			

		if roof_state != last_roof_state:
			last_roof_state = roof_state
			app.setLabel("roof_state", roof_state)

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

		








def sendCmd(cmd):
	try:
		ser.write(cmd+"\r")
	except:
		app.stop()




	
	

def updateStatus():

	global app

	global board_state
	global board_vin

	global roof_state

	datastring = ""
	data = ''

	while closeapp == False:

		
		try:
			ser.write("@")
		except:
			app.stop()


		time.sleep(0.25)

		while ser.inWaiting() > 0:
			try:
				data = ser.read(1)
			except:
				app.stop()



			datastring += data

			if data == "@":
				datastring = "@"


			if data == "$":
				#datastring = "@-O-12036-STATE-$"
				item = datastring.split('-')

				board_state = True
				
				if item[0] != '@':
					board_state = False

				if item[4] != '$':
					board_state = False

				if item[1] != 'O':
					board_state = False

				board_vin = item[2]
				
				roof_state = item[3]

				

				#datastring = ""

				StateUpdater()
				#time.sleep(0.1)
				#break


					


# handle button events
def press(button):

	if button == "Open":	
		sendCmd("OPEN")

	if button == "Close":	
		sendCmd("CLOSE")

	if button == "Abort":	
		sendCmd("ABORT")




# create a GUI variable called app
app = gui("Roof controller", "600x600", handleArgs=False)
app.setBg("#202020")
app.setFg("red")
app.setFont(20)


app.addImage("state_img", "ressources/Idle.png", 0, 0)


app.addLabel("roof_state", " --- ", 0, 1)
app.addButton("Open", press, 1, 0)
app.addButton("Close", press, 1, 1)
app.addButton("Abort", press, 1, 2)


app.addLabel("l_serial")
app.setLabel("l_serial", serialport+":"+str(serialportbaudrate))
app.getLabelWidget("l_serial").config(font="Courier 12")


app.addLabel("l_BoardStatus")
app.addLabel("l_Vin")


connect()

app.thread(updateStatus)


# start the GUI
app.go()


closeapp = True
disconnect()

#app.stop()




