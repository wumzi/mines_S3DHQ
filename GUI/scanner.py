#!/usr/bin/env python3
# -*- coding: utf-8 -*-

import os

from camera_control import PhotoHandler
from serial_handler import SerialHandler

FILENAMETPL = "{}_{}_{}_{}_{}.jpg"

class ScannerHandler():
	def __init__(self, rgb=(255, 255, 255), folder="data", device="/dev/ttyACM0", baudrate="9600"):
		if not os.path.exists(folder):
			os.mkdir(folder)
		os.chdir(folder)

		self.already_photographed = [(int(x) for x in filename[:2].split()) for filename in os.listdir()]

		self.camera = PhotoHandler("")
		self.serial = SerialHandler(device, baudrate)

		self.last_pos = (0, 0)

		self.panneau1 = (0, 0, 0)
		self.panneau2 = (0, 0, 0)
		self.panneau3 = (0, 0, 0)
		self.set_lights(rgb, self.panneau2, self.panneau3)



	def take_picture(self, real_z, z, x, laser=False, filenametpl=FILENAMETPL):
		self.serial.laser(laser)

		filename = filenametpl.format(z, x, real_z, x, laser)
		self.camera.take_picture(filename)


	def run_scan(self):
		for z in [z for z in range(91) if not z % 30]:
			for x in [x for x in range(360) if not x % 6]:
				if (z, x) in self.already_photographed:
					continue

				real_z, x = self.reach_position(z, x)
				self.last_pos = (real_z, x)

				print("Taking picture for pos {} {}".format(z, x))
				self.take_picture(real_z, z, x, laser=False)
				self.take_picture(real_z, z, x, laser=True)

				self.already_photographed.append((z, x))


	def reach_position(self, z, x):
		real_z = self.serial.commandeMoteurs(z, x)

		return real_z, x


	def set_lights(self, panneau1=(0, 0, 0), panneau2=(0, 0, 0), panneau3=(0, 0, 0)):
		if self.panneau1 != panneau1:
			self.serial.panneau(0, *panneau1)
		if self.panneau2 != panneau2:
			self.serial.panneau(1, *panneau2)
		if self.panneau3 != panneau3:
			self.serial.panneau(2, *panneau3)
		self.panneau1, self.panneau2, self.panneau3 = panneau1, panneau2, panneau3
		return self.serial.mesureLumiere()

	def getlastpos(self):
		return self.last_pos


if __name__ == '__main__':
	import sys

	a = ScannerHandler()
	#a.set_lights(panneau1 = (200, 100, 200))
	a.run_scan()
	a.serial.close()