import pygame
import socket

# UDP_IP = "192.168.1.143"
# UDP_IP = "127.0.0.1"
# UDP_IP = "192.168.7.2"
# UDP_IP = '127.0.0.255' # Broadcast Loopback
# UDP_IP = '255.255.255.255' # Broadcast Sun Adapter
# UDP_IP = '192.168.0.255' # Broadcast Local Network?
# UDP_IP = '71.195.237.116' # Broadcast
UDP_IP = '192.168.10.131' # Broadcast
# UDP_PORT = 80
UDP_PORT = 27015
MESSAGE = "Rover Test Frame"

FPS = 20

sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
sock.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1) # Broadcast
sock.setsockopt(socket.SOL_SOCKET, socket.SO_BROADCAST, 1) # Broadcast
# sock.bind((UDP_IP, UDP_PORT)) # Broadcast

print "UDP target IP:", UDP_IP
print "UDP target port:", UDP_PORT
sock.sendto(MESSAGE, (UDP_IP, UDP_PORT))

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
size = [500, 700]
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
		if event.type == pygame.JOYBUTTONDOWN:
			print("Joystick button pressed.")
		if event.type == pygame.JOYBUTTONUP:
			print("Joystick button released.")
			

	# DRAWING STEP
	# First, clear the screen to white. Don't put other drawing commands
	# above this, or they will be erased with this command.
	screen.fill(WHITE)
	textprintScr.reset()

	# Get count of joysticks
	joystick_count = pygame.joystick.get_count()

	textprintScr.printScr(screen, "Number of joysticks: {}".format(joystick_count) )
	textprintScr.indent()
	
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
		

	
	# ALL CODE TO DRAW SHOULD GO ABOVE THIS COMMENT
	
	# Go ahead and update the screen with what we've drawn.
	pygame.display.flip()

	# Limit to 20 frames per second
	clock.tick(FPS)
	
# Close the window and quit.
# If you forget this line, the program will 'hang'
# on exit if running from IDLE.
pygame.quit ()