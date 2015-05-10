#!/usr/bin/env python3
# coding: utf-8

import os
import gphoto2 as gp

class PhotoHandler:
	def __init__(self, model=None, folder="data"):
		if not os.path.exists(folder):
			os.mkdir(folder)
		self.folder = folder
		self.model = model

		if self.model == "canonEOS":
			gp.check_result(gp.use_python_logging())
			self.context = gp.gp_context_new()
			self.camera = gp.check_result(gp.gp_camera_new())
			gp.check_result(gp.gp_camera_init(self.camera, self.context))


	def take_picture(self, filename="default.jpg"):
		#Unix alternative with gphoto

		#os.system("gphoto2 --capture-image-and-download --filename %s" % filename)
		target = os.path.join(self.folder, filename)

		if self.model == "canonEOS":
			file_path = gp.check_result(gp.gp_camera_capture(self.camera, gp.GP_CAPTURE_IMAGE, self.context))
			camera_file = gp.check_result(gp.gp_camera_file_get(self.camera, file_path.folder, file_path.name, gp.GP_FILE_TYPE_NORMAL, self.context))
			gp.check_result(gp.gp_file_save(camera_file, target))
		else:
			file = open(target, "w")
			file.write("piche")
			file.close()
			print("Photo {} taken".format(filename))

	def get_params(self):
		pass


if __name__ == '__main__':
	p = PhotoHandler(folder="/tmp")
	p.takePicture()