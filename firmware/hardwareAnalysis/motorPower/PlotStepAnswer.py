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

if __name__ == "__main__":
	sys.path.append(os.path.abspath(os.path.join("../..")))
	from IT_Client.helpers.TelegramParser import TelegramParser

	signalNames = ["driverCurrentA", "directRatioMotorA"]

	if sys.platform.startswith("win"):
		os.system('mode 70,15')
		os.system("title plot")
	plt.ion()
	plt.style.use('dark_background')

	with open(sys.argv[1], 'rb') as sessionFile:
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
		plt.step([x/1e6 for x in time], data, where='post', marker='x')

	figure.canvas.flush_events()

	print('')
	print('Messages:')
	for telegram in telegrams:
		if telegram['valid'] and telegram['telegramType'] == 'string':
			print(telegram['value'])

	print('')
	input('press enter to close')
