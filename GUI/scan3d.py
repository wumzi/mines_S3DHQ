from tkinter import *
from tkinter.filedialog import askdirectory
from os import getcwd
from os import listdir
import tkinter.ttk as ttk
from scanner import ScannerHandler
from threading import Thread
import time
from colorpanel import LightFrame
import re
from PIL import Image, ImageTk



class Fenetre(Tk):

	def reset(self):
		
		self.angH["text"]=0
		self.angV["text"]=0

	#Folder where the pictures are taken
	def directory(self):
		self.where = askdirectory()

	#Method lauched by the thread to run a scan
	def scan(self):
		self.scanner=ScannerHandler(folder=self.where,device="/dev/ttyACM0")

		#Initialize subWindow to show the pictures
		self.subWindow = Toplevel(self)
		self.subWindow.destroy()

		threadStatus = Thread(target=self.updateStatus)
		threadStatus.daemon = True
		threadStatus.start()
		self.scanner.run_scan()
		self.scanner.serial.close()

	#Method for the button to run a scan
	def runScan(self):
		if self.fRun:
			self.bscan["text"]="Pause"
			self.fRun=False
			thread = Thread(target=self.scan)
			thread.daemon = True
			thread.start()
		else:
			if self.scanner.get_pause():
				self.scanner.set_pause(False)
				self.bscan["text"]="Pause"
			else:
				self.scanner.set_pause(True)
				self.bscan["text"]="Scan"


	#Method to get regularly the position of the scanner
	
	def updateStatus(self):
		while True:
			time.sleep(0.01)
			#Angular positions
			v = self.scanner.get_last_position()[0]
			h = self.scanner.get_last_position()[1]
			self.angH["text"]= h
			self.angV["text"]= v
			#Scan progression
			self.pBar["value"]=self.scanner.getProgression()
			self.pLabel["text"] = str(round(self.scanner.getProgression(),2)) + " %"
			#Change colors of panels
			self.scanner.set_lights(self.lightFrame1.getValues(),self.lightFrame2.getValues(),self.lightFrame3.getValues())

			#Update the picture (destroy and build the subWindow back)
			if listdir(self.where):
				regex = str(h) + "_" + str(v) + "*_False.jpg$"
				lastPicture = [item for item in listdir(self.where) if re.search(regex,item)]
				if lastPicture:
					pic = lastPicture[0]
					img = Image.open(file=self.where+"/"+pic).subsample(1,1)
					lastImage["image"]=ImageTk.PhotoImage(img)
					if self.subWindow:
						self.subWindow.destroy()
					self.subWindow = Toplevel(self)
					lab = Label(self.subWindow,image=img)
					lab.pack()



	def __init__(self):
		Tk.__init__(self)
		#Pictures folder path
		self.where = getcwd() + "/data"
		#First run in the scan
		self.fRun = True
		
		#Main frame
		mainFrame = Frame(self)
		#Frame for scanning frame and light frame
		f1 = Frame(mainFrame)
		#Frame for progression bar and progression label
		progressionFrame = Frame(mainFrame)
		#Frame for scanner informations (position...)
		infoFrame = Frame(mainFrame)

		# Scanning frame
		scanFrame = Frame(f1)
		# Scanning subframe
		scanSFrame = Frame(scanFrame)
		breset = Button(scanSFrame,text="Reset",command=self.reset)
		self.folderimg = PhotoImage(file="includes/folder.gif").subsample(20,20)
		bdir = Button(scanSFrame,command=self.directory,image=self.folderimg)
		self.bscan = Button(scanFrame,text="Scan",width=8,command=self.runScan)

		#Light frame RGB
		self.lightFrame1 = LightFrame(f1,"Panneau 1")
		self.lightFrame2 = LightFrame(f1,"Panneau 2")
		self.lightFrame3 = LightFrame(f1,"Panneau 3")


		#SCAN 3D HQ
		self.scannerimg = PhotoImage(file="includes/scanner.gif").subsample(1,1)
		title=Label(mainFrame,image=self.scannerimg)


		#Progress Bar
		self.pBar = ttk.Progressbar(progressionFrame,orient="horizontal",mode="determinate")
		self.pLabel = Label(progressionFrame,text="0%")

		#Information Bar
		posXLabel = Label(infoFrame,text="X :")
		self.posX = Label(infoFrame, text = " ")
		posYLabel = Label(infoFrame,text="Y :")
		self.posY = Label(infoFrame, text = " ")
		posZLabel = Label(infoFrame,text="Z :")
		self.posZ = Label(infoFrame, text = " ")
		angHLabel = Label(infoFrame,text="Angle H :")
		self.angH = Label(infoFrame,text=" ")
		angVLabel = Label(infoFrame,text="Angle V :")
		self.angV = Label(infoFrame,text=" ")


		#Layout

		#ScanFrame and LightFrame
		f1.pack(pady=10,fill=X,padx=50)
		#ScanFrame
		breset.pack(side="left")
		bdir.pack(side="right")
		scanSFrame.pack()
		self.bscan.pack(side="left")		
		scanFrame.pack(side="left",padx=10)

		#LightFrame

		self.lightFrame1.pack(side="left",padx=10)
		self.lightFrame2.pack(side="left",padx=10)
		self.lightFrame3.pack(side="left",padx=10)

		#Title SCANNER 3D HQ
		title.pack()

		#Progress Bar
		self.pBar.pack(pady=10,fill=BOTH,padx=10)
		self.pLabel.pack(side="right")
		progressionFrame.pack(padx=20,fill=X)

		#Information Bar

		angHLabel.pack(side="left",ipadx=2)
		self.angH.pack(side="left",ipadx=2)
		angVLabel.pack(side="left",ipadx=2)
		self.angV.pack(side="left",ipadx=2)
		infoFrame.pack()


		#MainFrame
		mainFrame.pack(pady=10, padx=10)

	
if __name__ == '__main__':
	f = Fenetre()
	f.mainloop()

