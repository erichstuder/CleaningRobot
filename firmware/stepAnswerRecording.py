import pexpect
import time

child = pexpect.spawn("python3 IT_Client/Client.py")
child.sendline("set connectionType USB_RS232")
child.sendline("set VID 2341")
child.sendline("set PID 8036")
child.sendline("reset")
child.sendline("extMode 1")
child.sendline("log speed1")
child.sendline("log speed2")
child.sendline("log bothPwm")
child.sendline("bothPwm 1")
time.sleep(2)
child.sendline("bothPwm 0")
time.sleep(2)
child.sendline("reset")
child.sendline("exit")
