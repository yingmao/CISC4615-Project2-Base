import socket
import sys
import struct

# Socket for receiving packages
s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
s.bind(("0.0.0.0", 1999))
data, addr = s.recvfrom(1024)
str,num = struct.unpack("!50si",data)
str = str.decode("utf-8").replace("\0","")
print("str:%s\nnum:%d" % (str,num))
num = 2

# Socket for sending packages
ss = struct.pack("!50si",str.encode(),num)
s.sendto(ss,("127.0.0.1",1991))
