"""
IT - Internal Tracer
Copyright (C) 2019 Erich Studer

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU Affero General Public License as published
by the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU Affero General Public License for more details.

You should have received a copy of the GNU Affero General Public License
along with this program.  If not, see <https://www.gnu.org/licenses/>.
"""

import matplotlib.pyplot as plt
import numpy as np
import time
import os
import array
import sys

from IT_Client.helpers.TelegramParser import TelegramParser

if sys.platform.startswith("win"):
    os.system('mode 70,15')
    os.system("title LivePlot")
plt.ion()
plt.style.use('dark_background')

print("Starting up, may take a few seconds ...")

voltage_borderTimestamp = 0

voltage_telegrams = []

voltage_lastTelegramTimestamp_old = 0

voltage_data = []

voltage_time = []

timeWindow = 30e6

while True:
	#millis1 = int(round(time.time() * 1000))
	
	with open('mySession.session', 'rb') as sessionFile:
		data = sessionFile.read()

	#print(data)

	voltage_lastTelegram = TelegramParser.parseLastValidTelegram(data, "enc1")
	
	voltage_lastTelegramTimestamp = voltage_lastTelegram["timestamp"]
	voltage_borderTimestamp = max(voltage_lastTelegramTimestamp_old, voltage_lastTelegramTimestamp - timeWindow)
	voltage_lastTelegramTimestamp_old = voltage_lastTelegramTimestamp
	
	voltage_telegrams = TelegramParser.getTelegramsAfterTimestamp(data, "enc1", voltage_borderTimestamp)
	#print(voltage_telegrams)

	if voltage_telegrams == None:
		continue

	for telegram in voltage_telegrams:
		if 'value' in telegram and 'timestamp' in telegram:
			voltage_data += [telegram['value']]
			voltage_time += [telegram['timestamp']]
	
	for index in range(len(voltage_time)):
		if voltage_time[index] > voltage_lastTelegramTimestamp - timeWindow:
			del voltage_data[0:max(index-1,0)]
			del voltage_time[0:max(index-1,0)]
			break
	
	figure = plt.figure(num='LivePlot', figsize=(8, 4))
	voltage_timeSeconds = [x/1e6 for x in voltage_time]
	
	plt.clf()
	plt.step(voltage_timeSeconds, voltage_data, where='post')

	plt.legend(['enc1'], loc='lower left')
	plt.xlabel('time [s]')
	
	plt.grid(linestyle='--')

	figure.canvas.flush_events()
