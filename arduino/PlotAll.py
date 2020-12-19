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

"""
plt.axis([0, 10, 0, 1])

for i in range(10):
    y = np.random.random()
    plt.scatter(i, y)
    plt.pause(0.05)

plt.show()
"""

if sys.platform.startswith("win"):
    os.system('mode 70,15')
    os.system("title plot")
plt.ion()

plt.style.use('dark_background')
# while True:
with open('mySession.session', 'rb') as sessionFile:
    data = sessionFile.read()

# data = b'\x12\x15' + data + b'\x12\x17'

telegrams = TelegramParser.parseStream(data)

voltage_data = []
voltage_time = []

for telegram in reversed(telegrams):
    if 'value' in telegram and 'timestamp' in telegram:
        if telegram['valueName'] == 'voltage':
            voltage_data = [telegram['value']] + voltage_data
            voltage_time = [telegram['timestamp']] + voltage_time

print('')
print('Messages:')
for telegram in telegrams:
    if telegram['valid'] and telegram['telegramType'] == 'string':
        print(telegram['value'])

time.sleep(1)

"""
startDelimiter = b'\xAA'
endDelimiter = b'\xBB'
dataSplitted1 = [e + b'\xBB' for e in data.split(b'\xBB') if e]
# print(dataSplitted1[0;0])


dataSplitted2 = []
for a in dataSplitted1:
    dataSplitted2 += [e + endDelimiter for e in a.split(b'\xbb') if e]
"""
figure = plt.figure(1, figsize=(8, 4))
plt.clf()
plt.step([x/1e6 for x in voltage_time], voltage_data, where='post')
# time.sleep(1)

# plt.draw()
# plt.show(block=False)
plt.grid(linestyle='--')
figure.canvas.flush_events()

print('')
input('press enter to close')
# plt.pause(0.001)

# time.sleep(1)
# print('plotted')
"""
    plt.figure(2)
    plt.clf()

    myData = []
    for n in range(min(len(desiredValue_data), len(plantOut_data))):
        myData += [abs(desiredValue_data[n] - plantOut_data[n])]
    plt.step(range(len(myData)), myData, where='post')
    plt.draw()
    plt.pause(0.001)
"""
"""
t = np.arange(0.0, 2.0, 0.01)
s1 = np.sin(2*np.pi*t)
s2 = np.sin(4*np.pi*t)

plt.figure(1)
ax1 = plt.subplot(211)
ax1.grid()
plt.plot(t, s1)
ax2 = plt.subplot(212, sharex=ax1)
ax2.grid()
# plt.plot(t, 2*s1)
plt.plot([1, 2, 3], [5, 0, 2], '-*')

plt.show()
"""
