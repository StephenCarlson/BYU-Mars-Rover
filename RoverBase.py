import pygame
from pygame.locals import *
import socket
# import struct
from array import *

# UDP_IP = "192.168.1.143"
# UDP_IP = "127.0.0.1"
# UDP_IP = "192.168.7.2"
# UDP_IP = '127.0.0.255' # Broadcast Loopback
# UDP_IP = '255.255.255.255' # Broadcast Sun Adapter
# UDP_IP = '192.168.0.255' # Broadcast Local Network?
# UDP_IP = '71.195.237.116' # Broadcast
UDP_IP = '192.168.10.130' # Broadcast
# UDP_IP = '192.168.1.149' # Broadcast
# UDP_IP = '192.168.1.112' # Broadcast
# UDP_PORT = 80
UDP_PORT = 27015
MESSAGE = "Rover Test Frame"


FPS = 20

sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
# sock.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1) # Broadcast
# sock.setsockopt(socket.SOL_SOCKET, socket.SO_BROADCAST, 1) # Broadcast
# sock.bind((UDP_IP, UDP_PORT)) # Broadcast

print "UDP target IP:", UDP_IP
print "UDP target port:", UDP_PORT
sock.sendto(MESSAGE, (UDP_IP, UDP_PORT))

# --- Rover variables
allowMouseIntegral = False
gimbal = [0,0]
drive = [0,0]
mainSeqCount = 0
armSeqCount = 0


# Define some colors
BLACK    = (   0,   0,   0)
WHITE    = ( 255, 255, 255)


# This is a simple class that will help us printScr to the screen
# It has nothing to do with the joysticks, just outputing the
# information.
class TextprintScr:
	def __init__(self):
		self.reset()
		self.font = pygame.font.Font(None, 20)

	def printScr(self, screen, textString):
		textBitmap = self.font.render(textString, True, BLACK)
		screen.blit(textBitmap, [self.x, self.y])
		self.y += self.line_height
		
	def reset(self):
		self.x = 10
		self.y = 10
		self.line_height = 15
		
	def indent(self):
		self.x += 10
		
	def unindent(self):
		self.x -= 10
	

pygame.init()

# Set the width and height of the screen [width,height]
size = [500, 300]
screen = pygame.display.set_mode(size)

pygame.display.set_caption("BYU Rover Control Panel")

#Loop until the user clicks the close button.
done = False

# Used to manage how fast the screen updates
clock = pygame.time.Clock()

# Initialize the joysticks
pygame.joystick.init()
	
# Get ready to printScr
textprintScr = TextprintScr()

# -------- Main Program Loop -----------
while done==False:
	# EVENT PROCESSING STEP
	for event in pygame.event.get(): # User did something
		if event.type == pygame.QUIT: # If user clicked close
			done=True # Flag that we are done so we exit this loop
		
		# Possible joystick actions: JOYAXISMOTION JOYBALLMOTION JOYBUTTONDOWN JOYBUTTONUP JOYHATMOTION
		# if event.type == pygame.JOYBUTTONDOWN:
			# print("Joystick button pressed.")
		# if event.type == pygame.JOYBUTTONUP:
			# print("Joystick button released.")
		if event.type == MOUSEBUTTONDOWN:
			if event.button==1: 
				# print "LMB"
				mouse = pygame.mouse.get_pos()
				if((mouse[0] in range(298, 298+104) and mouse[1] in range(98,98+104)) or allowMouseIntegral):
					allowMouseIntegral = not allowMouseIntegral
					pygame.event.set_grab(allowMouseIntegral)
					pygame.mouse.set_visible(not allowMouseIntegral)
					if(not allowMouseIntegral): pygame.mouse.set_pos((348+int(gimbal[0]/20), 148+int(gimbal[1]/20)))
			if event.button==3 and allowMouseIntegral:
				gimbal = [0,0]
			# if event.button==2:
				# print "MMB"
				
		# if event.type == MOUSEBUTTONUP:			
		# if event.type == KEYDOWN:
		# if event.type == KEYUP:
				
			
	# DRAWING STEP
	# First, clear the screen to white. Don't put other drawing commands
	# above this, or they will be erased with this command.
	screen.fill(WHITE)
	textprintScr.reset()
	
	# --- Capture Mouse and set Gimbal
	mouseX,mouseY = pygame.mouse.get_rel()
	if allowMouseIntegral:
		gimbal[0] += mouseX
		gimbal[1] -= mouseY
	textprintScr.printScr(screen, "Gimbal: {} {}".format(gimbal[0],gimbal[1]))
	# Bug here, the gimbal values painted are not what are being drawn and sent in UDP later.
	# (Beacause of constraint functions later, this being relative add/subtract)
	
	# --- Aquire Key States and set Drive accordingly
	keystate = pygame.key.get_pressed()
	drive = [(keystate[K_d]-keystate[K_a])*1000,(keystate[K_w]-keystate[K_s])*1000]
	

	# Get count of joysticks
	# joystick_count = pygame.joystick.get_count()
	# textprintScr.printScr(screen, "Number of joysticks: {}".format(joystick_count) )
	# textprintScr.indent()
	
	# For each joystick:
	
	# --- Over-ride the Mouse/Keyboard setpoints if joystick is present
	if(pygame.joystick.get_count()==1):
		joystick = pygame.joystick.Joystick(0)
		joystick.init()
		if(joystick.get_numaxes() >=2 ):
			drive[0] = int(joystick.get_axis(0)*1000)
			drive[1] = int(joystick.get_axis(1)*-1000)
		if(joystick.get_numaxes() >=4 ):
			gimbal[0] = int(joystick.get_axis(4)*1000)
			gimbal[1] = int(joystick.get_axis(3)*-1000)
	
	
	# --- Constrain all values to +/- 1000
	for i in range(0,len(gimbal)):
		if gimbal[i] > 1000: gimbal[i]= 1000
		if gimbal[i] < -1000: gimbal[i]= -1000
	
	for i in range(0,len(drive)):
		if drive[i] > 1000: drive[i] = 1000
		if drive[i] < -1000: drive[i] = -1000
		# use built-in max() and min()
	
	
	# --- Draw values
	pygame.draw.rect(screen, (255, 128, 0), (98, 98, 104, 104),2)
	pygame.draw.rect(screen, (0, 0, 255), (148+int(drive[0]/20), 148+int(-drive[1]/20), 4, 4))
	pygame.draw.rect(screen, (0, 0, 0), (298, 98, 104, 104),2)
	pygame.draw.rect(screen, (0, 0, 0), (348+int(gimbal[0]/20), 148+int(-gimbal[1]/20), 4, 4))
	
	# --- Assert UDP packet values
	
	
	BASE_ID     = 0x00
	BRD_ID_MAIN = 0x01
	BRD_ID_ARM  = 0x02
	
	WRITE_BIT   = 0xC0
	
	MSG_DRIVE   = 0x08
	MSG_ARM     = 0x0A
	
	mainSeqCount += 1
	armSeqCount += 1
	
	DRIVE_LEN   = 5
	ARM_LEN     = 5
	
	roverLinkFrame = [0x01,0xC8,(mainSeqCount&0xFF),DRIVE_LEN,(gimbal[0]>>8)&0xFF,gimbal[0]&0xFF,(gimbal[1]>>8)&0xFF,gimbal[1]&0xFF]
	#payload = struct.pack('B'*len(roverLinkFrame),*roverLinkFrame) # Gross way
	#print payload
	payload = array('B')
	payload.fromlist(roverLinkFrame)
	
	sock.sendto(payload, (UDP_IP, UDP_PORT))
	# sock.sendto("Test", (UDP_IP, UDP_PORT))
	
	
	
	# ALL CODE TO DRAW SHOULD GO ABOVE THIS COMMENT
	
	# Go ahead and update the screen with what we've drawn.
	pygame.display.flip()

	# Limit to 20 frames per second
	clock.tick(FPS)

pygame.quit ()



'''

# For each joystick:
	for i in range(joystick_count):
		joystick = pygame.joystick.Joystick(i)
		joystick.init()
	
		textprintScr.printScr(screen, "Joystick {}".format(i) )
		textprintScr.indent()
	
		# Get the name from the OS for the controller/joystick
		name = joystick.get_name()
		textprintScr.printScr(screen, "Joystick name: {}".format(name) )
		
		# Usually axis run in pairs, up/down for one, and left/right for
		# the other.
		axes = joystick.get_numaxes()
		textprintScr.printScr(screen, "Number of axes: {}".format(axes) )
		textprintScr.indent()
		
		for i in range( axes ):
			axis = joystick.get_axis( i )
			textprintScr.printScr(screen, "Axis {} value: {:>6.3f}".format(i, axis) )
		textprintScr.unindent()
			
		buttons = joystick.get_numbuttons()
		textprintScr.printScr(screen, "Number of buttons: {}".format(buttons) )
		textprintScr.indent()

		for i in range( buttons ):
			button = joystick.get_button( i )
			textprintScr.printScr(screen, "Button {:>2} value: {}".format(i,button) )
		textprintScr.unindent()
			
		# Hat switch. All or nothing for direction, not like joysticks.
		# Value comes back in an array.
		hats = joystick.get_numhats()
		textprintScr.printScr(screen, "Number of hats: {}".format(hats) )
		textprintScr.indent()

		for i in range( hats ):
			hat = joystick.get_hat( i )
			textprintScr.printScr(screen, "Hat {} value: {}".format(i, str(hat)) )
		textprintScr.unindent()
		
		textprintScr.unindent()		
		
		payload = repr(joystick.get_axis(0))
		sock.sendto(payload, (UDP_IP, UDP_PORT))
	

'''