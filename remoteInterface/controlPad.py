import pyudev

def getDevices():
    udev = pyudev.Context()
    devices = []

    for d in udev.list_devices(subsystem="input", ID_MODEL="Wireless_Gamepad_F710"):
        if d.device_node != None and d.device_node.startswith("/dev/input/event"):
            devices += [d]

    return devices

if __name__ == "__main__":
    print(getDevices())
