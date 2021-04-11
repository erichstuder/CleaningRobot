import time
import serial
from evdev import Device
import arduino
import controlPad as gamePad

def main():
	print("letse goo")

	arduinos = arduino.getDevices()
	if len(arduinos) == 0:
		print("no Arduino found!")
		return
	if len(arduinos) != 1:
		print("more than one Arduino found!")
		return

	print("Arduino found!")

	mySerial = serial.Serial(None)
	mySerial.timeout = 0
	mySerial.port = arduinos[0].device_node
	mySerial.open()

	controlPads = gamePad.getDevices()
	if len(controlPads) == 0:
		print("no control pad found!")
		return
	if len(controlPads) != 1:
		print("more than one control pad found!")
		return

	controlPad = Device(controlPads[0].device_node)

	while not mySerial.is_open:
		pass
	while True:
		#if getkey() == 't':
		#	print('W')

		# the LED is on the same pin as the direction for motor B!!!
		#mySerial.write("setLedOn\r".encode())
		#time.sleep(0.05)
		#mySerial.write("setLedOff\r".encode())
		#time.sleep(0.05)

		time.sleep(0.2)

		#clear input buffer to prevent crash
		mySerial.reset_input_buffer()

		controlPad.poll()
		#print(controlPad)
		axesName = "ABS_Y"
		if axesName in controlPad.axes:
			#print(axesName + ": " + str(controlPad.axes[axesName]))
			mySerial.write(("speedA " + str(5*controlPad.axes[axesName]) + "\r").encode())
		axesName = "ABS_RY"
		if axesName in controlPad.axes:
			#print(axesName + ": " + str(controlPad.axes[axesName]))
			mySerial.write(("speedB " + str(5*controlPad.axes[axesName]) + "\r").encode())
			#print(text)
		#print("")

if __name__ == "__main__":
	main()
