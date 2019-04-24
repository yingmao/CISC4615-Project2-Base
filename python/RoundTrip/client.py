import socket
import struct
import sys
text = "Hello,World"
num = 123

# Socket for sending packages
s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
ss = struct.pack("!50si",text.encode(),num)
s.sendto(ss,("127.0.0.1",1999))

# Socket for sending packages
s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
s.bind(("0.0.0.0", 1991))
data, addr = s.recvfrom(1024)
str,num = struct.unpack("!50si",data)
str = str.decode("utf-8").replace("\0","")
print("str:%s\nnum:%d" % (str,num))
