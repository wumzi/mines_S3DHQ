#!/usr/bin/env python3
# -*- coding: utf-8 -*-

import os

from camera_control import CameraHandler
from serial_handler import SerialHandler

class ScannerHandler():
	def __init__(self, rgb=(255, 255, 255), folder="data", device="/dev/ttyACM0", baudrate="9600"):
		os.chdir(folder)
		self.already_photographed = [(int(x) for x in filename[:2].split()) for filename in os.listdir()]

		self.camera = PhotoHandler(folder)
		self.serial = SerialHandler(device, baudrate)

		self.set_lights(rgb, rgb, rgb)



	def take_picture(self, z, xlaser=False, filenametpl="{}_{}_{}_{}_{}.jpg"):
		self.serial.laser(laser)

		filename = filenametpl.format(z, x, real_z, real_x, laser)
		self.camera.take_picture(filename)


	def run_scan(self, filenametpl="{}_{}_{}.jpg"):
		for z in [z for z in range(180) if not z % 3]:
			for x in [x for x in range(360) if not x % 3]:
				if (z, x) in already_photographed:
					continue

				real_z, x = self.reach_position(z, x)

				print("Taking picture for pos {} {}".format(z, x))
				self.take_picture(real_z, x, laser=False, filenametpl)
				self.take_picture(real_z, x, laser=True, filenametpl)

				already_photographed.append((z, x))


	def reach_position(self, z, x):
		self.serial.commandeMoteurs(z, x)

		real_z = self.serial.get_real_z()

		return real_z, x


	def set_lights(self, panneau1=(0, 0, 0), panneau2=(0, 0, 0), panneau3=(0, 0, 0)):
		for panneau, rgb in enumerate(kwargs.values()):
			self.serial.panneau(panneau, *rgb)


if __name__ == '__main__':
	import sys