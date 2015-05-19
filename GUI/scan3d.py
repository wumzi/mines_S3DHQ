from tkinter import *
from tkinter.filedialog import askdirectory
from os import getcwd
import tkinter.ttk as ttk
from scanner import ScannerHandler
from threading import Thread



class Fenetre(Tk):

	def reset(self):
		self.posX["text"]=0
		self.posY["text"]=0
		self.posZ["text"]=0
		self.angH["text"]=0
		self.angV["text"]=0

	#Folder where the pictures are taken
	def directory(self):
		self.where = askdirectory()

	#Thread to to the scan
	""""
	class Scan(Thread):

		def __init(self,where,red,green,blue):
			Thread.__init__(self)
			self.where=where
			self.red=red
			self.green=green
			self.blue=blue

		def run(self):
			self.scanner=ScannerHandler(folder=self.where,rgb=(int(self.red),int(self.green),int(self.blue)))
			self.scanner.run_scan()
			self.scanner.serial.close()
	"""
	#Method lauched by the thread to run a scan
	def scan(self):
		self.scanner=ScannerHandler(folder=self.where,rgb=(int(self.redSBox.get()),int(self.greenSBox.get()),int(self.blueSBox.get())))
		self.scanner.run_scan()
		self.scanner.serial.close()

	#Method for the button to run a scan
	def runScan(self):
		if self.fPause:
			self.bscan["text"]="Pause"
			self.fPause=False
			self.thread = Thread(target=self.scan)
			self.thread.start()
		else:
			if self.scanner.get_pause():
				self.scanner.set_pause(False)
				self.bscan["text"]="Pause"
			else:
				self.scanner.set_pause(True)
				self.bscan["text"]="Scan"

	def __init__(self):
		Tk.__init__(self)
		#Pictures folder path
		self.where = getcwd() + "/data"
		#First pause in the scan
		self.fPause = True
		
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
		lightFrame = Frame(f1)
		#Light frame label
		lightLabel = Label(lightFrame,text="Contrôle des LED")
		#Frame for the colors
		colorsFrame = Frame(lightFrame)
		#Frame for the values of RBG
		valuesFrame = Frame(lightFrame)

		#Labels for the colors
		redLabel = Label(colorsFrame,text="Rouge")
		greenLabel = Label(colorsFrame,text="Vert")
		blueLabel = Label(colorsFrame,text="Bleu")
		#Spinboxes for the values of RGB
		self.redSBox = Spinbox(valuesFrame, from_=0, to=255,width=3)
		self.greenSBox = Spinbox(valuesFrame, from_=0, to=255,width=3)
		self.blueSBox = Spinbox(valuesFrame, from_=0, to=255,width=3)


		#SCAN 3D HQ
		self.scannerimg = PhotoImage(file="includes/scanner.gif").subsample(1,1)
		title=Label(mainFrame,image=self.scannerimg)


		#Progress Bar
		pBar = ttk.Progressbar(progressionFrame,orient="horizontal",mode="determinate")
		pLabel = Label(progressionFrame,text="0%")

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
		lightLabel.pack(side="top")
		redLabel.pack()
		greenLabel.pack()
		blueLabel.pack()
		colorsFrame.pack(side="left")

		self.redSBox.pack()
		self.greenSBox.pack()
		self.blueSBox.pack()
		valuesFrame.pack(side="right")

		lightFrame.pack(side="right",padx=10)

		#Title SCANNER 3D HQ
		title.pack()

		#Progress Bar
		pBar.pack(pady=10,fill=BOTH,padx=10)
		pLabel.pack(side="right")
		progressionFrame.pack(padx=20,fill=X)

		#Information Bar
		posXLabel.pack(side="left",ipadx=2)
		self.posX.pack(side="left",ipadx=2)
		posYLabel.pack(side="left",ipadx=2)
		self.posY.pack(side="left",ipadx=2)
		posZLabel.pack(side="left",ipadx=2)
		self.posZ.pack(side="left",ipadx=2)
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

