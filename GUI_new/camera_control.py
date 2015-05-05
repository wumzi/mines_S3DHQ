import os

class PhotoHandler:
	def __init__(self):
		pass

	def takePicture(self, filename=""):
		os.system("gphoto2 --capture-image-and-download --filename %s" % filename)


	def getParams(self):
		pass




