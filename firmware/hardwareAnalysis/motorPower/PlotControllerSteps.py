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

if __name__ == "__main__":
	sys.path.append(os.path.abspath(os.path.join("../..")))
	from IT_Client.helpers.TelegramParser import TelegramParser


	if sys.platform.startswith("win"):
		os.system('mode 70,15')
		os.system("title LivePlot")
	plt.ion()
	plt.style.use('dark_background')

	print("Starting up, may take a few seconds ...")

	timeWindow = 10e6

	signals = [
		{"name": "driverCurrentA", "lastTelegramTimestamp": 0, "data": [], "time": []},
		{"name": "directRatioMotorA", "lastTelegramTimestamp": 0, "data": [], "time": []},
	]

	myFile = sys.argv[1]
	while not os.path.isfile(myFile):
		pass

	figure = plt.figure(num="ControllerSteps", figsize=(8, 4))

	try:
		while True:
			with open(myFile, "rb") as sessionFile:
				data = sessionFile.read()

			plt.clf()

			borderTimestamp = 0
			telegrams = []

			for signal in signals:
				lastTelegram = TelegramParser.parseLastValidTypeValueTelegram(data, signal["name"])
				if lastTelegram == None:
					continue
				lastTelegramTimestamp = lastTelegram["timestamp"]
				borderTimestamp = max(signal["lastTelegramTimestamp"], lastTelegramTimestamp - timeWindow)
				signal["lastTelegramTimestamp"] = lastTelegramTimestamp
				telegrams = TelegramParser.getTypeValueTelegramsAfterTimestamp(data, signal["name"], borderTimestamp)

				for telegram in telegrams:
					if "value" in telegram and "timestamp" in telegram:
						signal["data"] += [telegram["value"]]
						signal["time"] += [telegram["timestamp"]]

				for index in range(len(signal["time"])):
					if signal["time"][index] > lastTelegramTimestamp - timeWindow:
						del signal["data"][0:max(index-1,0)]
						del signal["time"][0:max(index-1,0)]
						break

				timeSeconds = [x/1e6 for x in signal["time"]]
				plt.step(timeSeconds, signal["data"], where="post")

			#plt.legend(signalNames, loc="lower left")
			plt.xlabel("time [s]")

			plt.grid(linestyle="--")

			figure.canvas.flush_events()
	except Exception as e:
		raise e
		print(e)
		print("good bye")
