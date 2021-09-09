import pyudev

def getDevices():
    udev = pyudev.Context()
    devices = []

    for d in  udev.list_devices(subsystem="tty", ID_VENDOR_ID="2341"):
        devices += [d]

    return devices

if __name__ == "__main__":
    print(getDevices())

