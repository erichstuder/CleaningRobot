import time
import serial
from evdev import Device
import arduino

def main():
    print("letse goo")

    arduinos = arduino.getDevices()
    if len(arduinos) == 0:
        print("no Arduino found!")
        return
    if len(arduinos) != 1:
        print("more than one Arduino found!")
        return

    mySerial = serial.Serial(None)
    mySerial.timeout = 0
    mySerial.port = arduinos[0].device_node
    mySerial.open()

    controlPad = Device("/dev/input/event5")

    while not mySerial.is_open:
        pass
    while True:
        #if getkey() == 't':
        #    print('W')

        # the LED is on the same pin as the direction for motor B!!!
        #mySerial.write("setLedOn\r".encode())
        #time.sleep(0.05)
        #mySerial.write("setLedOff\r".encode())
        #time.sleep(0.05)

        time.sleep(0.2)

        controlPad.poll()
        #print(controlPad)
        #print(controlPad.axes)
        axesName = "ABS_Y"
        if axesName in controlPad.axes:
            #print(axesName + ": " + str(controlPad.axes[axesName]))
            mySerial.write(("setMotorA " + str(controlPad.axes[axesName]) + "\r").encode())
        axesName = "ABS_RY"
        if axesName in controlPad.axes:
            #print(axesName + ": " + str(controlPad.axes[axesName]))
            mySerial.write(("setMotorB " + str(controlPad.axes[axesName]) + "\r").encode())
        #print("")

if __name__ == "__main__":
    main()
