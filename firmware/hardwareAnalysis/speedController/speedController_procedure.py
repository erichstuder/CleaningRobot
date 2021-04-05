send("set connectionType USB_RS232")
send("set VID 2341")
send("set PID 8036")
send("reset")

send("log speedA")
send("log speedB")

while True:
	send("speedA 1")
	send("speedB 1")
	sleep(5)
	send("speedA 6")
	send("speedB 6")
	sleep(5)

send("reset")
sleep(1)

send("exit")
