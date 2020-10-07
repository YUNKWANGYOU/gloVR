import threading
import socket

class UDPThread(threading.Thread) :
    def __init__(self,UDP_IP,UDP_PORT,UDP2_PORT):
        threading.Thread.__init__(self)
        self.daemon = True
        self.UDP2_PORT = UDP2_PORT
        self.UDP_IP = UDP_IP
        self.UDP_PORT = UDP_PORT
        self.pyToUnity = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
        self.unityToPy = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
        self.status = 0

    def send(x,y):
        self.pyToUnity.sendto((str(x)+","+str(y)).encode(), (self.UDP_IP, self.UDP_PORT) )
        print((str(x)+","+str(y)))

    def run(self):
        self.unityToPy.bind((self.UDP_IP,self.UDP2_PORT))
        while(1) :
            status,addr = self.unityToPy.recvfrom(200)
            if list(status) == [115] :
                print("START")
            elif list(status) == [49] :
                print("BACK")
            elif list(status) == [101] :
                print("END")
            time.sleep(1)
