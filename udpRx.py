

import socket

# UDP_IP = "127.0.0.1"
# UDP_IP = '' # Broadcast
# UDP_IP = '192.168.0.255' # Broadcast
UDP_IP = '0.0.0.0' # Broadcast
# UDP_IP = '66.219.250.246' # Broadcast
# UDP_PORT = 27016
UDP_PORT = 8080

# print '<broadcast>'

sock = socket.socket(socket.AF_INET,socket.SOCK_DGRAM)
# sock.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1) # Broadcast
# sock.setsockopt(socket.SOL_SOCKET, socket.SO_BROADCAST, 1) # Broadcast

sock.bind((UDP_IP, UDP_PORT)) # Only needed for receive?

while True:
	try:
		data, addr = sock.recvfrom(1024) # buffer size is 1024 bytes
		print "Frame:", data
	except (KeyboardInterrupt, SystemExit):
		raise
	
