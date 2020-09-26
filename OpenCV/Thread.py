import threading
from time import sleep

def hello_1():
    print("쓰레드1")

def hello_2() :
    print("쓰레드2")

def hello_1_thread():
    thread = threading.Thread(target =  hello_1)
    thread.daemon = True
    thread.start()

if __name__ == '__main__':
    hello_1_thread()
    hello_2()