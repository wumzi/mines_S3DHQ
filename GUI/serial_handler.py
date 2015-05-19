#!/usr/bin/env python3
# coding: utf-8

import math
import serial
import threading
import time

CODEACTION = {'posmoteur': 1, 'accelerometre': 6, 'mesure_lumiere': 7, 'panneau_sup': 3, 'laser': 2, 'panneau_lat_gauche': 4, 'moteur': 0, 'panneau_lat_droit': 5}


class FrameBrokenException(Exception):
	pass

class SerialHandler():
	def __init__(self, device="/dev/ttyACM0", baudrate="115200"):
		self.serial = serial.Serial(device, baudrate)
		self.serial.flushInput()
		self.serial.flushOutput()


		self.waiting = True
		self.serial.timeout = 4
		self.listen()
		self.serial.timeout = None

	def listen(self):
		input = self.serial.readline().decode(encoding="utf-8")
		self.waiting = False
		if input:
			print("INPUT : " + input)
			return self.handleMessage(input)

	def sendMessage(self, messageBrut):
		messageBrut = "$" + str(messageBrut) + "/"
		while self.waiting:
			time.sleep(0.01)
		self.waiting = True
		self.serial.write(bytes(messageBrut, encoding="utf-8"))
		print("OUTPUT : " + messageBrut)

		return self.listen()


	def commandeMoteurs(self, positionVerticale=0, positionHorizontale=0):
		"""" Commande angulaire des deux moteurs """
		if not (positionVerticale in range(0,91) and positionHorizontale in range(0,361)):
			raise Exception("Bad positions")

		positionVerticale, positionHorizontale = ('000{}'.format(color)[-3:] for color in (positionVerticale, positionHorizontale))
		resp = self.sendMessage("{}{}{}".format(CODEACTION['moteur'], positionVerticale, positionHorizontale))


		acceleration = self.mesureAccelerometre()
		if acceleration[2] == 0:
			return - acceleration[1]/abs(acceleration[1]) * 90
		return math.atan(acceleration[1] / acceleration[2]) * 180 / math.pi

	def laser(self, status):
		if status not in [True, False]:
			raise Exception("Le statut pour le laser booleen")
		resp = self.sendMessage("{}{}".format(CODEACTION['laser'], int(status)))
		return bool(resp[0])

	def panneau(self, panneau, red, green, blue):
		panneau += 3
		if not (red in range(256) and green in range(256) and blue in range(256)):
			raise Exception("Les couleurs sont codés en RGB de 0 a 255")

		red, green, blue = ('000{}'.format(color)[-3:] for color in (red, green, blue))

		resp = self.sendMessage("%s%s%s%s" % (panneau, red, green, blue))
		return True

	def mesureAccelerometre(self):
		resp = self.sendMessage(CODEACTION["accelerometre"])
		try:
			return [float(x) for x in resp]
		except Exception(e):
			print(e)
			return None


	def mesureLumiere(self):
		resp = self.sendMessage(CODEACTION["mesure_lumiere"])
		try:
			return sum([int(x) for x in resp])
		except Exception(e):
			print(e)
			return None

	def handleMessage(self, message):
		if message[0] != "$" or "/" not in message[-3:]:
			print("ERROR: inconsistent message")
			return
		message = message[1:].split("/")[0]
		elems = message.split("_")

		if elems[0] == "ERROR":
			print("ERROR : " + ' '.join(message))
			return

		return elems[2:]

	def close(self):
		self.serial.close()





if __name__ == '__main__':
	import sys

	if len(sys.argv) > 1:
		device = sys.argv[1]
		handler = SerialHandler(device=device)
	else:
		handler = SerialHandler()

	handler.laser(True)
	handler.mesureAccelerometre()
	handler.panneau(0, 0, 0, 255)
	handler.commandeMoteurs(59, 22)
	handler.mesureAccelerometre()