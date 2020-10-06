from getkey import getkey, keys
import time
import serial

def main():
    print("letse goo")
    mySerial = serial.Serial(None)
    mySerial.timeout = 0
    mySerial.port = "/dev/ttyACM0"
    mySerial.open()
    while not mySerial.is_open:
        pass
    while True:
        #if getkey() == 't':
        #    print('W')
        mySerial.write("setLedOn\r".encode())
        time.sleep(0.1)
        mySerial.write("setLedOff\r".encode())
        time.sleep(0.1)

if __name__ == "__main__":
    main()
