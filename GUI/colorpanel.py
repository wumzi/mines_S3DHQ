from tkinter import *

class LightFrame(Frame):
	def __init__(self,parent,text):
		Frame.__init__(self,parent)

		#Light frame label
		lightLabel = Label(self,text=text)
		
		#Frame for the colors
		colorsFrame = Frame(self)
		#Frame for the values of RBG
		valuesFrame = Frame(self)

		#Labels for the colors
		redLabel = Label(colorsFrame,text="Rouge")
		greenLabel = Label(colorsFrame,text="Vert")
		blueLabel = Label(colorsFrame,text="Bleu")
		#Spinboxes for the values of RGB
		self.redSBox = Spinbox(valuesFrame, from_=0, to=255,width=3)
		self.greenSBox = Spinbox(valuesFrame, from_=0, to=255,width=3)
		self.blueSBox = Spinbox(valuesFrame, from_=0, to=255,width=3)

		lightLabel.pack(side="top")
		redLabel.pack()
		greenLabel.pack()
		blueLabel.pack()
		colorsFrame.pack(side="left")

		self.redSBox.pack()
		self.greenSBox.pack()
		self.blueSBox.pack()
		valuesFrame.pack(side="right")

	def getValues(self):
		red = int(self.redSBox.get()) % 255
		green = int(self.redSBox.get()) % 255
		blue = int(self.redSBox.get()) % 255
		return (red,green,blue)