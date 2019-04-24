import socket
import sys
import struct
s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
s.bind(("0.0.0.0", 1999))
data, addr = s.recvfrom(1024)
str,num = struct.unpack("!50si",data)
str = str.decode("utf-8").replace("\0","")
print("str:%s\nnum:%d" % (str,num))
