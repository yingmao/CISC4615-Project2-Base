import socket
import struct
import sys


text = "Hello,World"
num = 123

s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
ss = struct.pack("!50si",text.encode(),num)
s.sendto(ss,("127.0.0.1",1999))
