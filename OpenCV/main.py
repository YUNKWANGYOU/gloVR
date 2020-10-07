import cv2
import numpy as np
import time
import math
import socket
import threading

global status
global cxcyCount,cxcyCount2

cxcyCount = 0
cxcyCount2 = 0

UDP_IP = "127.0.0.1"
UDP_PORT = 5065
UDP2_PORT = 8000

pyToUnity = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
unityToPy = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
unityToPy.bind((UDP_IP,UDP2_PORT))

class UDPHandler(threading.Thread) :
    def __init__(self):
        threading.Thread.__init__(self)
        self.daemon = True
    def run(self):
        while(1) :
            global status
            status,addr = unityToPy.recvfrom(200)

            if list(status) == [115] :
                print("START")
            elif list(status) == [49] :
                print("BACK")
            elif list(status) == [101] :
                print("END")
            time.sleep(1)

udp = UDPHandler()
udp.start()

# Open Camera object
cap = cv2.VideoCapture(0)
cap2 = cv2.VideoCapture(1)

# Decrease frame size
cap.set(3, 500)
cap.set(4, 450)

cap2.set(3,500)
cap2.set(4,450)

def nothing():
    pass

# Creating a window for HSV track bars
cv2.namedWindow('HSV_TrackBar')
cv2.namedWindow('HSV_TrackBar2')

# Starting with 100's to prevent error while masking
h, s, v = 100, 100, 100
h2,s2,v2 = 100, 100, 100

# Creating track bar
cv2.createTrackbar('h', 'HSV_TrackBar', 0, 179, nothing)
cv2.createTrackbar('s', 'HSV_TrackBar', 0, 255, nothing)
cv2.createTrackbar('v', 'HSV_TrackBar', 0, 255, nothing)

cv2.createTrackbar('h2', 'HSV_TrackBar2', 0, 179, nothing)
cv2.createTrackbar('s2', 'HSV_TrackBar2', 0, 255, nothing)
cv2.createTrackbar('v2', 'HSV_TrackBar2', 0, 255, nothing)

while (1):
    if list(status) == [115] :

        print(status)

        # Capture frames from the camera
        ret, frame = cap.read()
        ret2, frame2 = cap2.read()

        # Blur the image
        blur = cv2.blur(frame, (3, 3))
        blur2 = cv2.blur(frame2,(3,3))

        # Convert to HSV color space
        hsv = cv2.cvtColor(blur, cv2.COLOR_BGR2HSV)
        hsv2 = cv2.cvtColor(blur2,cv2.COLOR_BGR2HSV)

        # Create a binary image with where white will be skin colors and rest is black
        mask = cv2.inRange(hsv, np.array([45,65,65]), np.array([75,255, 255]))
        mask2 = cv2.inRange(hsv2, np.array([45,65,65]), np.array([75,255, 255]))
        #mask2 = cv2.inRange(hsv, np.array([2, 50, 50]), np.array([15, 255, 255]))

        # Kernel matrices for morphological transformation
        kernel_square = np.ones((11, 11), np.uint8)
        kernel_ellipse = cv2.getStructuringElement(cv2.MORPH_ELLIPSE, (5, 5))

        kernel_square2 = np.ones((11, 11), np.uint8)
        kernel_ellipse2 = cv2.getStructuringElement(cv2.MORPH_ELLIPSE, (5, 5))

        # Perform morphological transformations to filter out the background noise
        dilation = cv2.dilate(mask, kernel_ellipse, iterations=1)
        erosion = cv2.erode(dilation, kernel_square, iterations=1)
        median = cv2.medianBlur(dilation, 5)
        ret, thresh = cv2.threshold(median, 127, 255, 0)

        dilation2 = cv2.dilate(mask2, kernel_ellipse2, iterations=1)
        erosion2 = cv2.erode(dilation2, kernel_square2, iterations=1)
        median2 = cv2.medianBlur(dilation2, 5)
        ret2, thresh2 = cv2.threshold(median2, 127, 255, 0)


        # Find contours of the filtered frame

        contours, hierarchy = cv2.findContours(median, cv2.RETR_TREE, cv2.CHAIN_APPROX_SIMPLE)
        contours2, hierarchy2 = cv2.findContours(median2, cv2.RETR_TREE, cv2.CHAIN_APPROX_SIMPLE)

        # Find Max contour area (Assume that hand is in the frame)
        kernel = cv2.getStructuringElement(cv2.MORPH_ELLIPSE, (5, 5))
        mask = cv2.morphologyEx(mask, cv2.MORPH_CLOSE, kernel, 1)

        kernel2 = cv2.getStructuringElement(cv2.MORPH_ELLIPSE, (5, 5))
        mask2 = cv2.morphologyEx(mask2, cv2.MORPH_CLOSE, kernel2, 1)

        max_area = 100
        max_area2 = 100
        ci = 0
        ci2 = 0

        for i in range(len(contours)):
            cnt = contours[i]
            area = cv2.contourArea(cnt)
            if (area > max_area):
                max_area = area
                ci = i

        for j in range(len(contours2)):
            cnt2 = contours2[j]
            area2 = cv2.contourArea(cnt2)
            if (area2 > max_area2):
                max_area2 = area2
                ci2 = j

            # Largest area contour

        if (not contours) or (not contours2) :
            ret, frame = cap.read()
            ret2, frame2 = cap2.read()

            cv2.imshow("1", frame)
            cv2.imshow("2", frame2)

            # close the output video by pressing 'ESC'
            k = cv2.waitKey(5) & 0xFF
            if k == 27:
                break

            continue
        cnts = contours[ci]
        cnts2 = contours2[ci2]

        # Find convex hull
        hull = cv2.convexHull(cnts)
        hull3 = cv2.convexHull(cnts2)

        # Find convex defects
        hull2 = cv2.convexHull(cnts, returnPoints=False)
        defects = cv2.convexityDefects(cnts, hull2)

        hull4 = cv2.convexHull(cnts2, returnPoints=False)
        defects2 = cv2.convexityDefects(cnts2, hull4)

        # Get defect points and draw them in the original image
        FarDefect = []
        FarDefect2 = []
        if defects is None:
            pass
        else:
            for i in range(defects.shape[0]):
                s, e, f, d = defects[i, 0]
                start = tuple(cnts[s][0])
                end = tuple(cnts[e][0])
                far = tuple(cnts[f][0])
                FarDefect.append(far)

        if defects2 is None:
            pass
        else:
            for j in range(defects2.shape[0]):
                s2, e2, f2, d2 = defects2[j, 0]
                start2 = tuple(cnts2[s2][0])
                end2 = tuple(cnts2[e2][0])
                far2 = tuple(cnts2[f2][0])
                FarDefect2.append(far2)
        # Find moments of the largest contour
        moments = cv2.moments(cnts)
        moments2 = cv2.moments(cnts2)

        # Central mass of first order moments
        # LPF for cx,cy

        """
        1.cx2,cy2에 이전의 cx, cy 저장 (맨 처음에는 같은 값 저장)
        2.cx,cy에 새로 들어오는 값 저장
        """

        if moments['m00'] != 0:
            # 맨 처음에는 이전의 값을 같은 값으로 넣어주기
            if cxcyCount == 0 :

                cx = int(moments['m10'] / moments['m00'])  # cx = M10/M00
                cy = int(moments['m01'] / moments['m00'])  # cy = M01/M00

                cx2 = cx
                cx2 = int(cx2*(0.5) + cx*0.5)
                cy2 = cy
                cy2 = int(cy2*(0.5) + cy*0.5)

                cxcyCount = cxcyCount+1
            # 두 번째 부터 이전 값 저장하고 새로들어오는값이랑 계산해서 필터
            else :
                cx2 = cx
                cy2 = cy
                cx = int(moments['m10'] / moments['m00'])  # cx = M10/M00
                cy = int(moments['m01'] / moments['m00'])  # cy = M01/M00
                cx2 = int(cx2 * (0.5) + cx * 0.5)
                cy2 = int(cy2 * (0.5) + cy * 0.5)

        if moments2['m00'] != 0:
            # 맨 처음에는 이전의 값을 같은 값으로 넣어주기
            if cxcyCount2 == 0 :

                cx3 = int(moments2['m10'] / moments2['m00'])  # cx = M10/M00
                cy3 = int(moments2['m01'] / moments2['m00'])  # cy = M01/M00

                cx4 = cx3
                cx4 = int(cx4*(0.5) + cx3*0.5)
                cy4 = cy3
                cy4 = int(cy4*(0.5) + cy3*0.5)

                print("cx2 = ",cx4)
                print("cy2 = ",cy4)
                cxcyCount2 = cxcyCount2+1
            # 두 번째 부터 이전 값 저장하고 새로들어오는값이랑 계산해서 필터
            else :
                cx4 = cx3
                cy4 = cy3
                cx3 = int(moments2['m10'] / moments2['m00'])  # cx = M10/M00
                cy3 = int(moments2['m01'] / moments2['m00'])  # cy = M01/M00
                cx4 = int(cx4 * (0.5) + cx3 * 0.5)
                cy4 = int(cy4 * (0.5) + cy3 * 0.5)
                print("cx = ", cx4)
                print("cy = ", cy4)


        centerMass = (cx2, cy2)
        centerMass2 = (cx4,cy4)

        # Draw center mass

        cv2.circle(frame, centerMass, 7, [100, 0, 255], 2)
        cv2.circle(frame2, centerMass2, 7, [100, 0, 255], 2)

        font = cv2.FONT_HERSHEY_SIMPLEX
        font2 = cv2.FONT_HERSHEY_SIMPLEX

        cv2.putText(frame, 'Center', tuple(centerMass), font, 2, (255, 255, 255), 2)
        cv2.putText(frame2, 'Center', tuple(centerMass2), font2, 2, (255, 255, 255), 2)

        # Print bounding rectangle
        x, y, w, h = cv2.boundingRect(cnts)
        x2, y2, w2, h2 = cv2.boundingRect(cnts2)
        #img = cv2.rectangle(frame, (x, y), (x + w, y + h), (0, 255, 0), 2)

        ##### Show final image ########
        cv2.imshow("1", frame)
        cv2.imshow("2", frame2)
        ###############################

        cz = cx4

        print(int(cz))
        # close the output video by pressing 'ESC'
        k = cv2.waitKey(5) & 0xFF
        if k == 27:
            break;

        try:

            pyToUnity.sendto((str(cx2)+","+str(cy2)+","+str(cz)).encode(), (UDP_IP, UDP_PORT) )
            print((str(cx2)+","+str(cy2))+","+str(cz))
        except:
            pass
    elif list(status) == [49] :
        pass
    elif list(status) == [101] :
        break

cap.release()
cap2.release()
cv2.destroyAllWindows()
