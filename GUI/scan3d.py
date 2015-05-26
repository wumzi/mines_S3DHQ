from tkinter import *
from tkinter.filedialog import askdirectory
from os import getcwd
import tkinter.ttk as ttk
from scanner import ScannerHandler
from threading import Thread
import time
from colorpanel import LightFrame



class Fenetre(Tk):

	def reset(self):
		
		self.angH["text"]=0
		self.angV["text"]=0

	#Folder where the pictures are taken
	def directory(self):
		self.where = askdirectory()

	#Method lauched by the thread to run a scan
	def scan(self):
		red = int(self.redSBox.get()) % 255
		green = int(self.greenSBox.get()) % 255
		blue = int(self.blueSBox.get()) % 255
		self.scanner=ScannerHandler(folder=self.where,device="/dev/ttyACM0")
		threadStatus = Thread(target=self.updateStatus)
		threadStatus.start()
		self.scanner.run_scan()
		self.scanner.serial.close()

	#Method for the button to run a scan
	def runScan(self):
		if self.fRun:
			self.bscan["text"]="Pause"
			self.fRun=False
			thread = Thread(target=self.scan)
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
			self.angH["text"]=self.scanner.get_last_position()[0]
			self.angV["text"]=self.scanner.get_last_position()[1]
			#Scan progression
			self.pBar["value"]=self.scanner.getProgression()
			self.pLabel["text"] = str(round(self.scanner.getProgression(),2)) + " %"
			#Change colors of panels
			self.scanner.set_lights(lightFrame1.getValues(),lightFrame2.getValues(),lightFrame3.getValues())


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
		lightFrame1 = LightFrame(f1,"Panneau 1")
		lightFrame2 = LightFrame(f1,"Panneau 2")
		lightFrame3 = LightFrame(f1,"Panneau 3")


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

		lightFrame1.pack(side="left",padx=10)
		lightFrame2.pack(side="left",padx=10)
		lightFrame3.pack(side="left",padx=10)

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

