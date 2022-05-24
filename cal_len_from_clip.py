import threading
import pyperclip
def printit():
	threading.Timer(3.0, printit).start()  
	inp = pyperclip.paste()	
	print("len of "+inp+" is: "+str(len(inp)))

printit()
