send("set connectionType USB_RS232")
send("set VID 2341")
send("set PID 8036")
send("reset")

send("log driverCurrentA")

sleep(1)
send("directRatioMotorA -0.8")
sleep(1)
send("directRatioMotorA 1")
sleep(1)
send("directRatioMotorA 0.0")
sleep(1)

send("reset")
sleep(1)

send("exit")
