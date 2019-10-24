import time # Used to control how often to send data packages
import pprint # Used to make console dictionary output nicer
pp = pprint.PrettyPrinter(indent = 2)
import json   # Used to convert dict object 
import re
import os

# Imports for Serial / XBee Communication
import serial
import serial.tools.list_ports
import sys 						# Used for exit()

# For 3D Plotting
import numpy as np
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D

# 0763 is the passcode

max_retry_count = 3
xbox_dict = {
	"send_time":     0,								# Time the command was recieved
	"trans_num":     0,		
	"left_trigger":  0, 							# Between 0-255, scales with hardness -> Scaled 0-100
	"left_bumper":   0,								# T/F, pressed or not -> 1/0
	"right_trigger": 0,								# Between 0-255, scales with hardness -> Scaled 0-100
	"right_bumper":  0,								# T/F, pressed or not -> 1/0
	"left_stick":    (0, 0), 						# -32768-32768, tuple of the {x, y} -> Scaled -100-100
	"d_pad":		 (0, 0),						# -1-1, tuple of x, y of D-pad (-x is left, -y is top)
	"right_stick":	 (0, 0),						# -32768-32768, tuple of the x, y -> Scaled -100-100
	"buttons":		 (0, 0, 0, 0),					# T/F, pressed or not of Y, B, A, X -> 1/0
	"start_button":  0,								# T/F, pressed or not of start button -> 1/0
	"back_button":	 0								# T/F, pressed or not of the back button -> 1/0
}

contentOrder = [("buttons", 2), ("buttons", 1), ("buttons", 3), ("buttons", 0),
				"start_button", "back_button", "left_bumper", "right_bumper",
				"left_trigger", "right_trigger", ("left_stick", 0), ("left_stick", 1),
				("right_stick", 0), ("right_stick", 1), "trans_num"]

valid_ack_string = ["A"]

__location__ = os.path.realpath(os.path.join(os.getcwd(), os.path.dirname(__file__)))

# Converts the button dictionary to a space-separated value list
def dictToString(buttonDict):
	temp_str = " ".join("%s" % str(val) for (key, val) in buttonDict.items()) + '\0'
	temp_str = [char for char in temp_str if char not in "(),"]
	temp_str = ''.join(temp_str)
	return (temp_str)

# Send the dictionary over the XBee Serial connection
def sendDict(buttonDict, ser_port):
	print ("Sending information...")

	# Send to XBee over Serial Comm. Port
	tmp_dict = dict(buttonDict)
	del tmp_dict["trans_num"]
	button_str = dictToString(tmp_dict)
	print (button_str.encode())
	ser_port.write(button_str.encode())
	return ser_port.readline().decode("utf-8")
	# return ser_port.readline()

# Find the port with the XBee on it, return the serial object
def findXbee():
	portFound = False
	portList = list(serial.tools.list_ports.comports())
	for port in portList:
		print (port, '\n', port[0], '\n', port[1], '\n', port[2])
		if "0403" in port[2]:
			print ("Possible XBee found on %s" % port[0])
			if not portFound:
				portFound = True
				portname = port[0]
				print ("Using %s as XBee COM port." % port[0])
			else:
				print ("Ignoring this port, using the first one found.")

	if portFound:
		return (serial.Serial(portname, 9600, timeout=0))	# timeout=0 to avoid hangup on readline()
	else:
		sys.exit("No Serial Port connected.")

# Read the following file, return a list of each line
def readFile(fileName="ServerFile.txt"):
	file = open(os.path.join(__location__, fileName), "r")
	contents = file.read()
	file.close()

	# Return None if the string is empty
	if not bool(re.search(r'\d', contents)):
		return None

	content_list = contents.split("\n")
	content_list = [int(x) for x in content_list]

	return content_list

# Clear the provided file, erasing its contents
def clearFile(fileName="ServerFile.txt"):
	file = open(os.path.join(__location__, fileName), "r+")
	file.truncate(0)
	file.close()

# Change the values in xbox_dict according to what is in fileContents
def parseContents(content_list):
	for line_count, value in enumerate(content_list):
		order = contentOrder[line_count] # Grab what the content is from the ordered list
		if isinstance(order, tuple): # If the content is that of a tuple
			curr_tupe = list(xbox_dict[order[0]])
			curr_tupe[order[1]] = value
			xbox_dict[order[0]] = tuple(curr_tupe)
		else: # Content is not a tuple
			xbox_dict[order] = value

	xbox_dict["send_time"] = int(time.time())

# Loop to read from the Controller and send the data as a dictionary at the provided interval (seconds)
def dictReadLoop(logInterval, xbee):
	# Main loop to read from the XBox Controller
	marked_time = time.time() 	# Initialize the timer for resetting the dict
	
	while True:
		# Read the file, parse the contents
		contents = readFile()
		if contents:
			parseContents(contents)

			# Send the dictionary 
			acked = False
			clearFile()
			timeout_count = 0
			while not acked and timeout_count < max_retry_count:
				if time.time() - marked_time > logInterval:
					ack_string = sendDict(xbox_dict, xbee)
					print ("Ack: \"%s\"\n" % ack_string)
					u, v, w, = 0, 0, 0
					if len(ack_string) > 3:
						u, v, w = ack_string.split(",")
						print (u, v, w)
					x, y, z = zip([0, 0, 0], [0, 0, 0], [0, 0, 0])
					fig = plt.figure()
					ax = fig.add_subplot(111, projection='3d')
					ax.view_init(60, 35)
					ax.set_xlim(-2100, 2100)
					ax.set_ylim(-2100, 2100)
					ax.set_zlim(-2100, 2100)
					ax.quiver(x, y, z, u, v, w, arrow_length_ratio=0.01)
					plt.show()
					plt.pause(1)
					plt.close()
					acked = True if len(ack_string) > 0 else False 
					marked_time = time.time()
					timeout_count += 1



			# if time.time() - marked_time > logInterval:
			# 	clearFile() # Clear the file, initiate the next sequence of the queue
			# 	ack_string = sendDict(xbox_dict, xbee)
			# 	print ("Ack: \"%s\"\n" % ack_string)
			# 	acked = True if len(ack_string) > 0 else False 
			# 	marked_time = time.time()

			# timeout_count = 0
			# while not acked and timeout_count < 10:
			# 	print ("\nNot acked, retry #%i" % timeout_count)
			# 	time.sleep(logInterval * 2)
			# 	ack_string = sendDict(xbox_dict, xbee)
			# 	acked = True if len(ack_string) > 0 else False 
			# 	timeout_count += 1

xbee_port = findXbee()
dictReadLoop(0.10, xbee_port) # 0.15 works very well