#!/usr/bin/env python3
# -*- coding: utf-8 -*-

import os

from camera_control import CameraHandler
from serial_handler import SerialHandler

FILENAMETPL = "{}_{}_{}_{}.jpg"

class ScannerHandler():
	def __init__(self, rgb=(255, 255, 255), folder="data", device="/dev/ttyACM0", baudrate="9600"):
		if not os.path.exists(folder):
			os.mkdir(folder)
		os.chdir(folder)

		self.camera = CameraHandler(model="canonEOS")
		self.serial = SerialHandler(device, baudrate)

		self.last_position = (0, 0)

		self.panneau1 = self.panneau2 = self.panneau3 =(0, 0, 0)
		self.set_lights(rgb, rgb, rgb)


	def take_picture(self, filename, laser=False):
		self.serial.laser(laser)

		self.camera.take_picture(filename.format(laser))


	def run_scan(self, filenametpl=FILENAMETPL):
		self.already_photographed = list(set([tuple([int(x) for x in filename.split('_')[:2]]) for filename in os.listdir()]))
		
		for z in [z for z in range(91) if not z % 30]:
			for x in [x for x in range(360) if not x % 30]:
				if (z, x) in self.already_photographed:
					continue

				real_z, x = self.reach_position(z, x)
				self.last_position = (real_z, x)

				filename = filenametpl.format(z, x, real_z, "{}")
				self.take_picture(filename, laser=False)
				self.take_picture(filename, laser=True)

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


	def get_last_position(self):
		return self.last_position



if __name__ == '__main__':
	import sys

	a = ScannerHandler()
	a.set_lights(panneau1 = (200, 100, 200))
	a.run_scan()
	a.serial.close()