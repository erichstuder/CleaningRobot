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

import matplotlib.pyplot as plt
import numpy as np
import time
import os
import sys

from IT_Client.helpers.TelegramParser import TelegramParser

# signals = [
# 	#{"name": "enc1",   "lastTelegramTimestamp": 0, "data": [], "time": []},
# 	#{"name": "enc2",   "lastTelegramTimestamp": 0, "data": [], "time": []},
# 	{"name": "speed1", "lastTelegramTimestamp": 0, "data": [], "time": []},
# 	{"name": "speed2", "lastTelegramTimestamp": 0, "data": [], "time": []},
# 	#{"name": "current1", "lastTelegramTimestamp": 0, "data": [], "time": []},
# ]

signalNames = ["speed1", "speed2", "bothPwm"]

if sys.platform.startswith("win"):
    os.system('mode 70,15')
    os.system("title plot")
plt.ion()
plt.style.use('dark_background')

with open('mySession.session', 'rb') as sessionFile:
    data = sessionFile.read()

telegrams = TelegramParser.parseStream(data)

figure = plt.figure(1, figsize=(8, 4))
plt.clf()
plt.grid(linestyle='--')
for signalName in signalNames:
	data = []
	time = []
	for telegram in reversed(telegrams):
		if 'value' in telegram and 'timestamp' in telegram:
			if telegram['valueName'] == signalName:
				data = [telegram['value']] + data
				time = [telegram['timestamp']] + time
	plt.step([x/1e6 for x in time], data, where='post')

figure.canvas.flush_events()

	# lastTelegram = TelegramParser.parseLastValidTelegram(data, signalName)
	# lastTelegramTimestamp = lastTelegram["timestamp"]
	# borderTimestamp = max(signal["lastTelegramTimestamp"], lastTelegramTimestamp - timeWindow)
	# signal["lastTelegramTimestamp"] = lastTelegramTimestamp
	# telegrams = TelegramParser.getTelegramsAfterTimestamp(data, signal["name"], borderTimestamp)
	# if telegrams == None:
	# 	continue

	# for telegram in telegrams:
	# 	if "value" in telegram and "timestamp" in telegram:
	# 		signal["data"] += [telegram["value"]]
	# 		signal["time"] += [telegram["timestamp"]]

	# for index in range(len(signal["time"])):
	# 	if signal["time"][index] > lastTelegramTimestamp - timeWindow:
	# 		del signal["data"][0:max(index-1,0)]
	# 		del signal["time"][0:max(index-1,0)]
	# 		break

	# timeSeconds = [x/1e6 for x in signal["time"]]
	# plt.step(timeSeconds, signal["data"], where="post")





# for telegram in reversed(telegrams):
#     if 'value' in telegram and 'timestamp' in telegram:
#         if telegram['valueName'] == 'current1':
#             voltage_data = [telegram['value']] + voltage_data
#             voltage_time = [telegram['timestamp']] + voltage_time

print('')
print('Messages:')
for telegram in telegrams:
    if telegram['valid'] and telegram['telegramType'] == 'string':
        print(telegram['value'])

print('')
input('press enter to close')
