import cv2
import numpy as np
import time
import math
import socket
import threading
UDP_IP = "127.0.0.1"
UDP_PORT = 5065
UDP2_PORT = 8000
pyToUnity = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
unityToPy = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
unityToPy.bind((UDP_IP,UDP2_PORT))
class MyThread(threading.Thread) :
    def __init__(self):
        threading.Thread.__init__(self)
        self.daemon = True
    def run(self):
        while(1) :
            global data
            data,addr = unityToPy.recvfrom(200)

            if list(data) == [49] :
                print("게임나가기 버튼 클릭했음")
            time.sleep(1)
cap = cv2.VideoCapture(0)
cap.set(3, 500)
cap.set(4, 450)
def nothing():
    pass
cv2.namedWindow('HSV_TrackBar')
h, s, v = 100, 100, 100
cv2.createTrackbar('h', 'HSV_TrackBar', 0, 179, nothing)
cv2.createTrackbar('s', 'HSV_TrackBar', 0, 255, nothing)
cv2.createTrackbar('v', 'HSV_TrackBar', 0, 255, nothing)
cxcyCount = 0


while (1):
    # TODO: 창 닫기
    PAUSE = cv2.waitKey(5)& 0xFF
    if PAUSE == 69 or PAUSE == 101 : #'e' or 'E'
        cap.release()
        cv2.destroyAllWindows()
        while 1 :
            START = cv2.waitKey(100)&0xFF
            if START == 83 or START == 115 : #'s' or 'S'
                break

    ret, frame = cap.read()
    blur = cv2.blur(frame, (3, 3))
    hsv = cv2.cvtColor(blur, cv2.COLOR_BGR2HSV)
    mask = cv2.inRange(hsv, np.array([45,65,65]), np.array([75,255, 255]))
    kernel_square = np.ones((11, 11), np.uint8)
    kernel_ellipse = cv2.getStructuringElement(cv2.MORPH_ELLIPSE, (5, 5))
    dilation = cv2.dilate(mask, kernel_ellipse, iterations=1)
    erosion = cv2.erode(dilation, kernel_square, iterations=1)
    median = cv2.medianBlur(dilation, 5)
    ret, thresh = cv2.threshold(median, 127, 255, 0)
    contours, hierarchy = cv2.findContours(median, cv2.RETR_TREE, cv2.CHAIN_APPROX_SIMPLE)
    kernel = cv2.getStructuringElement(cv2.MORPH_ELLIPSE, (5, 5))
    mask = cv2.morphologyEx(mask, cv2.MORPH_CLOSE, kernel, 1)
    max_area = 100
    ci = 0
    for i in range(len(contours)):
        cnt = contours[i]
        area = cv2.contourArea(cnt)
        if (area > max_area):
            max_area = area
            ci = i
    if (not contours):# or (not contours2) :
        ret, frame = cap.read()
        cv2.imshow("1", frame)
        k = cv2.waitKey(5) & 0xFF
        if k == 27:
            break
        continue
    cnts = contours[ci]
    hull = cv2.convexHull(cnts)
    hull2 = cv2.convexHull(cnts, returnPoints=False)
    defects = cv2.convexityDefects(cnts, hull2)
    FarDefect = []
    if defects is None:
        pass
    else:
        for i in range(defects.shape[0]):
            s, e, f, d = defects[i, 0]
            start = tuple(cnts[s][0])
            end = tuple(cnts[e][0])
            far = tuple(cnts[f][0])
            FarDefect.append(far)
    moments = cv2.moments(cnts)
    if moments['m00'] != 0:
        if cxcyCount == 0 :
            cx = int(moments['m10'] / moments['m00'])  # cx = M10/M00
            cy = int(moments['m01'] / moments['m00'])  # cy = M01/M00
            cx2 = cx
            cx2 = int(cx2*(0.5) + cx*0.5)
            cy2 = cy
            cy2 = int(cy2*(0.5) + cy*0.5)
            cxcyCount = cxcyCount+1
        else :
            cx2 = cx
            cy2 = cy
            cx = int(moments['m10'] / moments['m00'])  # cx = M10/M00
            cy = int(moments['m01'] / moments['m00'])  # cy = M01/M00
            cx2 = int(cx2 * (0.5) + cx * 0.5)
            cy2 = int(cy2 * (0.5) + cy * 0.5)
    centerMass = (cx2, cy2)
    cv2.circle(frame, centerMass, 7, [100, 0, 255], 2)
    font = cv2.FONT_HERSHEY_SIMPLEX
    cv2.putText(frame, 'Center', tuple(centerMass), font, 2, (255, 255, 255), 2)
    x, y, w, h = cv2.boundingRect(cnts)
    cv2.imshow("1", frame)
    k = cv2.waitKey(5) & 0xFF
    if k == 27:
        break
    try:
        pyToUnity.sendto((str(cx2)+","+str(cy2)).encode(), (UDP_IP, UDP_PORT) )
        print((str(cx2)+","+str(cy2)))
    except:
        pass
cap.release()
cv2.destroyAllWindows()
