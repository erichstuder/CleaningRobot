send("set connectionType USB_RS232")
send("set VID 2341")
send("set PID 8036")
send("reset")

send("log driverCurrentA")
send("directRatioMotorA")

while True:
	send("controlledRatioMotorA 1")
	sleep(2)
	send("controlledRatioMotorA 0.3")
	sleep(5)

send("reset")
sleep(1)

send("exit")
