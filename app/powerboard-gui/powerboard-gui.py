
# GUI lib
from appJar import gui

# Serial
import serial

import time


btn_out1_state = False
btn_out2_state = False
btn_out3_state = False
btn_out4_state = False
btn_out5_state = False
btn_out6_state = False


serialport = '/dev/cu.wchusbserial141220'
serialportbaudrate = 9600




ser = serial.Serial(
    port=serialport,
    baudrate=serialportbaudrate,
#    parity=serial.PARITY_ODD,
#    stopbits=serial.STOPBITS_TWO,
#    bytesize=serial.SEVENBITS
)

ser.isOpen()







def setButtonON(button):
	app.setButton(button, "ON ")
	app.setButtonBg(button, "red")
	app.setButtonFg(button, "#202020")
    

def setButtonOFF(button):
	app.setButton(button, "OFF")
	app.setButtonBg(button, "#202020")
	app.setButtonFg(button, "#AAAAAA")


def sendCmd(cmd):
	ser.write(cmd+"\r")
	app.setLabel("l_sent", cmd)
	#time.sleep(1)
	#recvCmd()

def recv():
	recvstr = ser.read(ser.inWaiting())
	app.setLabel("l_recv", recvstr)
	return recvstr

def updateVin():
	sendCmd("@")
	data = recv()
	app.setLabel("VinStr", data) 



# handle button events
def press(button):
	global btn_out1_state
	global btn_out2_state
	global btn_out3_state
	global btn_out4_state
	global btn_out5_state
	global btn_out6_state

	if button == "UpdateVin":
		updateVin()

	if button == "btn_out1":
		if btn_out1_state == False:
			setButtonON(button)
			btn_out1_state = True
			sendCmd("1 ON")
		else:
			setButtonOFF(button)
			btn_out1_state = False
			sendCmd("1 OFF")

	if button == "btn_out2":
		if btn_out2_state == False:
			setButtonON(button)
			btn_out2_state = True
			sendCmd("2 ON")
		else:
			setButtonOFF(button)
			btn_out2_state = False
			sendCmd("2 OFF")

	if button == "btn_out3":
		if btn_out3_state == False:
			setButtonON(button)
			btn_out3_state = True
			sendCmd("3 ON")
		else:
			setButtonOFF(button)
			btn_out3_state = False
			sendCmd("3 OFF")

	if button == "btn_out4":
		if btn_out4_state == False:
			setButtonON(button)
			btn_out4_state = True
			sendCmd("4 ON")
		else:
			setButtonOFF(button)
			btn_out4_state = False
			sendCmd("4 OFF")

	if button == "btn_out5":
		if btn_out5_state == False:
			setButtonON(button)
			btn_out5_state = True
			sendCmd("5 ON")
		else:
			setButtonOFF(button)
			btn_out5_state = False
			sendCmd("5 OFF")

	if button == "btn_out6":
		if btn_out6_state == False:
			setButtonON(button)
			btn_out6_state = True
			sendCmd("6 ON")
		else:
			setButtonOFF(button)
			btn_out6_state = False
			sendCmd("6 OFF")




# create a GUI variable called app
app = gui("Power Board", "200x400")
app.setBg("#202020")
app.setFg("red")
app.setFont(20)



app.addLabel("VinStr", 0, 0)
app.setLabel("VinStr", "VinStr")
app.getLabelWidget("VinStr").config(font="Courier 15")
app.addButton("UpdateVin", press, 0, 1)
app.setButton("UpdateVin", "Update Vin")


app.addLabel("l_out1", "1: Roof", 1, 0)
app.setLabelAlign("l_out1", "left")
app.addButton("btn_out1", press, 1, 1)
setButtonOFF("btn_out1")

app.addLabel("l_out2", "2: Mount", 2, 0)
app.setLabelAlign("l_out2", "left")
app.addButton("btn_out2", press, 2, 1)
setButtonOFF("btn_out2")

app.addLabel("l_out3", "3: DSLR", 3, 0)
app.setLabelAlign("l_out3", "left")
app.addButton("btn_out3", press, 3, 1)
setButtonOFF("btn_out3")

app.addLabel("l_out4", "4:", 4, 0)
app.setLabelAlign("l_out4", "left")
app.addButton("btn_out4", press, 4, 1)
setButtonOFF("btn_out4")

app.addLabel("l_out5", "5: Flat", 5, 0)
app.setLabelAlign("l_out5", "left")
app.addButton("btn_out5", press, 5, 1)
setButtonOFF("btn_out5")

app.addLabel("l_out6", "6: ", 6, 0)
app.setLabelAlign("l_out6", "left")
app.addButton("btn_out6", press, 6, 1)
setButtonOFF("btn_out6")



app.addLabel("l_serial")
app.setLabel("l_serial", serialport+":"+str(serialportbaudrate))
app.getLabelWidget("l_serial").config(font="Courier 12")

app.addLabel("l_sent")
app.getLabelWidget("l_sent").config(font="Courier 20")

app.addLabel("l_recv")
app.getLabelWidget("l_recv").config(font="Courier 20")


# start the GUI
app.go()
