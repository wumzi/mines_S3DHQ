#!/usr/bin/env python3
# coding: utf-8

"""TODO : 	- synchronisation thread pour communication efficace
			- interface de plus haut niveau
			- interface graphique -> Java ? Responsive web bindé sur Django ?
				\-> communication JPython ? Sockets ?
"""

import serial
import threading
import time

CODEACTION = {'accelerometre': 5, 'mesure_lumiere': 6, 'panneau_sup': 2, 'laser': 1, 'panneau_lat_gauche': 3, 'moteur': 0, 'panneau_lat_droit': 4}


class FrameBrokenException(Exception):
	pass

class SerialHandler():
	def __init__(self, device="/dev/ttyACM0", baudrate="9600"):
		self.serial = serial.Serial(device, baudrate)
		self.serial.flush()

		self.listener = threading.Thread(group=None, target=self.listen, name="listen_serial", args=(), kwargs=None)
		self.listener.daemon = True
		self.listener.start()

	def listen(self):
		while True:
			input = self.serial.readline()
			print("INPUT : " + str(input))
			self.handleMessage(input)

	def sendMessage(self, messageBrut):
		messageBrut = str(messageBrut)
		self.serial.write(bytes(messageBrut + "\n", encoding="utf-8"))
		print("OUTPUT : " + messageBrut)

	def commandeMoteurs(self, positionVerticale=0, positionHorizontale=0):
		"""" Commande angulaire des deux moteurs """
		if not (positionVerticale in range(0,360) and positionHorizontale in range(0,360)):
			raise Exception("Les positions doivent etre des entiers compris en 0 et 359")

		positionVerticale, positionHorizontale = ('000{}'.format(color)[-3:] for color in (positionVerticale, positionHorizontale))
		return self.sendMessage("{}{}{}".format(CODEACTION['moteur'], positionVerticale, positionHorizontale))


	def laser(self, status):
		if status not in [True, False]:
			raise Exception("Le statut pour le laser booleen")
		return self.sendMessage("{}{}".format(CODEACTION['laser'], int(status)))

	def laserOn(self):
		return self.laser(True)

	def laserOff(self):
		return self.laser(False)

	def panneau(self, panneau, red, green, blue):
		if not (red in range(256) and green in range(256) and blue in range(256)):
			raise Exception("Les couleurs sont codés en RGB de 0 a 255")

		red, green, blue = ('000{}'.format(color)[-3:] for color in (red, green, blue))

		return self.sendMessage("%s%s%s%s" % (panneau, red, green, blue))

	def mesureAccelerometre(self):
		return self.sendMessage(CODEACTION["accelerometre"])

	def mesureLumiere(self):
		return self.sendMessage(CODEACTION["mesure_lumiere"])


	def handleMessage(self, message):
		pass



if __name__ == '__main__':
	import sys

	if len(sys.argv) > 1:
		device = sys.argv[1]
		handler = SerialHandler(device=device)
	else:
		handler = SerialHandler()

	handler.laserOn()
	time.sleep(5)
	handler.mesureAccelerometre()
	time.sleep(5)
	handler.commandeMoteurs(244, 122)
	time.sleep(5)
	handler.mesureAccelerometre()

	handler.listener.join()