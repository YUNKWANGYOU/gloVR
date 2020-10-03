import cv2
import numpy as np
import time
import math
import socket
import threading


#TODO
#onOff = 파이썬 받은 문자열


UDP_IP = "127.0.0.1"
UDP_PORT = 5065



sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

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


# Function to find angle between two vectors
def Angle(v1, v2):
    dot = np.dot(v1, v2)
    x_modulus = np.sqrt((v1 * v1).sum())
    y_modulus = np.sqrt((v2 * v2).sum())
    cos_angle = dot / x_modulus / y_modulus
    angle = np.degrees(np.arccos(cos_angle))
    return angle


# Function to find distance between two points in a list of lists
def FindDistance(A, B):
    return np.sqrt(np.power((A[0][0] - B[0][0]), 2) + np.power((A[0][1] - B[0][1]), 2))


# Creating a window for HSV track bars
cv2.namedWindow('HSV_TrackBar')

# Starting with 100's to prevent error while masking
h, s, v = 100, 100, 100

# Creating track bar
cv2.createTrackbar('h', 'HSV_TrackBar', 0, 179, nothing)
cv2.createTrackbar('s', 'HSV_TrackBar', 0, 255, nothing)
cv2.createTrackbar('v', 'HSV_TrackBar', 0, 255, nothing)

cxcyCount = 0
cxcyCount2 = 0

print("통신성공! ")
while (1):
    # Measure execution time
    start_time = time.time()

    # Capture frames from the camera
    ret, frame = cap.read()
    ret2,frame2 = cap2.read()

    # Blur the image
    blur = cv2.blur(frame, (3, 3))
    blur2 = cv2.blur(frame2, (3, 3))
    # Convert to HSV color space
    hsv = cv2.cvtColor(blur, cv2.COLOR_BGR2HSV)
    hsv2 = cv2.cvtColor(blur2, cv2.COLOR_BGR2HSV)

    # Create a binary image with where white will be skin colors and rest is black
    mask = cv2.inRange(hsv, np.array([45,65,65]), np.array([75,255, 255]))
    mask2 = cv2.inRange(hsv2, np.array([45,65,65]), np.array([75,255, 255]))
    #mask2 = cv2.inRange(hsv, np.array([2, 50, 50]), np.array([15, 255, 255]))

    # Kernel matrices for morphological transformation
    kernel_square = np.ones((11, 11), np.uint8)
    kernel_ellipse = cv2.getStructuringElement(cv2.MORPH_ELLIPSE, (5, 5))

    # Perform morphological transformations to filter out the background noise
    # Dilation increase skin color area
    # Erosion increase skin color area
    dilation = cv2.dilate(mask, kernel_ellipse, iterations=1)
    dilation2 = cv2.dilate(mask2, kernel_ellipse, iterations=1)

    erosion = cv2.erode(dilation, kernel_square, iterations=1)
    erosion2 = cv2.erode(dilation2, kernel_square, iterations=1)
    dilation3 = cv2.dilate(erosion, kernel_ellipse, iterations=1)
    dilation4 = cv2.dilate(erosion, kernel_ellipse, iterations=1)

    filtered = cv2.medianBlur(dilation3, 5)

    kernel_ellipse = cv2.getStructuringElement(cv2.MORPH_ELLIPSE, (5, 5))
    dilation2 = cv2.dilate(filtered, kernel_ellipse, iterations=1)
    median = cv2.medianBlur(dilation, 5)
    median2 = cv2.medianBlur(dilation2, 5)

    ret, thresh = cv2.threshold(median, 127, 255, 0)
    ret2, thresh2 = cv2.threshold(median2, 127, 255, 0)



    # Find contours of the filtered frame

    contours, hierarchy = cv2.findContours(median, cv2.RETR_TREE, cv2.CHAIN_APPROX_SIMPLE)
    contours2, hierarchy2 = cv2.findContours(median2, cv2.RETR_TREE, cv2.CHAIN_APPROX_SIMPLE)


    # Draw Contours
    # cv2.drawContours(frame, cnt, -1, (122,122,0), 3)
    # cv2.imshow('Dilation',median)

    # Find Max contour area (Assume that hand is in the frame)
    kernel = cv2.getStructuringElement(cv2.MORPH_ELLIPSE, (5, 5))
    mask = cv2.morphologyEx(mask, cv2.MORPH_CLOSE, kernel, 1)

    kernel2 = cv2.getStructuringElement(cv2.MORPH_ELLIPSE, (5, 5))
    mask2 = cv2.morphologyEx(mask, cv2.MORPH_CLOSE, kernel2, 1)

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

    for i in range(len(contours2)):
        cnt2 = contours2[i]
        area2 = cv2.contourArea(cnt2)
        if (area2 > max_area2):
            max_area2 = area2
            ci2 = i

        # Largest area contour

    if not contours :
        ret, frame = cap.read()
        cv2.imshow("1", frame)
        cv2.imshow("2",frame2)
        ###############################
        # Print execution time
        # print time.time()-start_time

        # close the output video by pressing 'ESC'
        k = cv2.waitKey(5) & 0xFF
        if k == 27:
            break

        continue

    if not contours2 :
        ret2, frame2 = cap2.read()
        cv2.imshow("1", frame)
        cv2.imshow("2",frame2)
        ###############################
        # Print execution time
        # print time.time()-start_time

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
    hull4 = cv2.convexHull(cnts2, returnPoints=False)
    defects = cv2.convexityDefects(cnts, hull2)
    defects2 = cv2.convexityDefects(cnts2, hull4)

    # Get defect points and draw them in the original image
    FarDefect = []
    FarDefect2 = []

    if defects is None:
        nothing()
    else:
        for i in range(defects.shape[0]):
            s, e, f, d = defects[i, 0]
            start = tuple(cnts[s][0])
            end = tuple(cnts[e][0])
            far = tuple(cnts[f][0])
            FarDefect.append(far)
    if defects2 is None:
        nothing()
    else:
        for i in range(defects2.shape[0]):
            s2, e2, f2, d2 = defects2[i, 0]
            start2 = tuple(cnts2[s2][0])
            end2 = tuple(cnts2[e2][0])
            far2 = tuple(cnts2[f2][0])
            FarDefect2.append(far2)
            #cv2.line(frame, start, end, [0, 255, 0], 1)
            #cv2.circle(frame, far, 10, [100, 255, 255], 3)

    # Find moments of the largest contour
    moments = cv2.moments(cnts)

    moments2 = cv2.moments(cnts2)
    # Central mass of first order moments 이 값 보내주면 될듯!!!!!
    #filteredvalue = filteredvalue * (1 - alpha) + flexvalue * alpha;

    # TODO = LPF for cx,cy

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

    centerMass = (cx2, cy2)

    if moments2['m00'] != 0:
        # 맨 처음에는 이전의 값을 같은 값으로 넣어주기
        if cxcyCount2 == 0 :

            cx3 = int(moments2['m10'] / moments2['m00'])  # cx = M10/M00
            cy3 = int(moments2['m01'] / moments2['m00'])  # cy = M01/M00

            cx4 = cx3
            cx4 = int(cx4*(0.5) + cx3*0.5)
            cy4 = cy3
            cy4 = int(cy4*(0.5) + cy3*0.5)

            cxcyCount2 = cxcyCount2+1
        # 두 번째 부터 이전 값 저장하고 새로들어오는값이랑 계산해서 필터
        else :
            cx4 = cx3
            cy4 = cy3
            cx3 = int(moments2['m10'] / moments2['m00'])  # cx = M10/M00
            cy3 = int(moments2['m01'] / moments2['m00'])  # cy = M01/M00
            cx4 = int(cx4 * (0.5) + cx3 * 0.5)
            cy4 = int(cy4 * (0.5) + cy3 * 0.5)

    centerMass2 = (cx4, cy4)

    # Draw center mass

    cv2.circle(frame, centerMass, 7, [100, 0, 255], 2)
    cv2.circle(frame2, centerMass2, 7, [100, 0, 255], 2)

    font = cv2.FONT_HERSHEY_SIMPLEX
    font2 = cv2.FONT_HERSHEY_SIMPLEX

    cv2.putText(frame, 'Center', tuple(centerMass), font, 2, (255, 255, 255), 2)
    cv2.putText(frame2, 'Center', tuple(centerMass2), font2, 2, (255, 255, 255), 2)

    # Distance from each finger defect(finger webbing) to the center mass
    distanceBetweenDefectsToCenter = []
    distanceBetweenDefectsToCenter2 = []

    for i in range(0, len(FarDefect)):
        x = np.array(FarDefect[i])
        centerMass = np.array(centerMass)
        distance = np.sqrt(np.power(x[0] - centerMass[0], 2) + np.power(x[1] - centerMass[1], 2))
        distanceBetweenDefectsToCenter.append(distance)
    for i in range(0, len(FarDefect2)):
        x2 = np.array(FarDefect2[i])
        centerMass2 = np.array(centerMass2)
        distance2 = np.sqrt(np.power(x[0] - centerMass2[0], 2) + np.power(x[1] - centerMass2[1], 2))
        distanceBetweenDefectsToCenter2.append(distance2)
    # Get an average of three shortest distances from finger webbing to center mass
    sortedDefectsDistances = sorted(distanceBetweenDefectsToCenter)
    AverageDefectDistance = np.mean(sortedDefectsDistances[0:2])
    sortedDefectsDistances2 = sorted(distanceBetweenDefectsToCenter2)
    AverageDefectDistance2 = np.mean(sortedDefectsDistances2[0:2])

    # Get fingertip points from contour hull
    # If points are in proximity of 80 pixels, consider as a single point in the group
    finger = []
    finger2 = []
    for i in range(0, len(hull) - 1):
        if (np.absolute(hull[i][0][0] - hull[i + 1][0][0]) > 80) or (
                np.absolute(hull[i][0][1] - hull[i + 1][0][1]) > 80):
            if hull[i][0][1] < 500:
                finger.append(hull[i][0])
    for i in range(0, len(hull3) - 1):
        if (np.absolute(hull3[i][0][0] - hull3[i + 1][0][0]) > 80) or (
                np.absolute(hull3[i][0][1] - hull3[i + 1][0][1]) > 80):
            if hull3[i][0][1] < 500:
                finger2.append(hull3[i][0])

    # The fingertip points are 5 hull points with largest y coordinates
    finger = sorted(finger, key=lambda x: x[1])
    fingers = finger[0:5]
    finger2 = sorted(finger2, key=lambda x2: x2[1])
    fingers2 = finger2[0:5]

    # Calculate distance of each finger tip to the center mass
    fingerDistance = []
    fingerDistance2 = []
    for i in range(0, len(fingers)):
        distance = np.sqrt(np.power(fingers[i][0] - centerMass[0], 2) + np.power(fingers[i][1] - centerMass[0], 2))
        fingerDistance.append(distance)
    for i in range(0, len(fingers2)):
        distance2 = np.sqrt(np.power(fingers2[i][0] - centerMass2[0], 2) + np.power(fingers2[i][1] - centerMass2[0], 2))
        fingerDistance2.append(distance2)

    # Finger is pointed/raised if the distance of between fingertip to the center mass is larger
    # than the distance of average finger webbing to center mass by 130 pixels
    result = 0
    result2 = 0
    for i in range(0, len(fingers)):
        if fingerDistance[i] > AverageDefectDistance + 130:
            result = result + 1

    for i in range(0, len(fingers2)):
        if fingerDistance2[i] > AverageDefectDistance2 + 130:
            result2 = result2 + 1

    # Print number of pointed fingers
    #cv2.putText(frame, str(result), (100, 100), font, 2, (255, 255, 255), 2)

    # show height raised fingers
    # cv2.putText(frame,'finger1',tuple(finger[0]),font,2,(255,255,255),2)
    # cv2.putText(frame,'finger2',tuple(finger[1]),font,2,(255,255,255),2)
    # cv2.putText(frame,'finger3',tuple(finger[2]),font,2,(255,255,255),2)
    # cv2.putText(frame,'finger4',tuple(finger[3]),font,2,(255,255,255),2)
    # cv2.putText(frame,'finger5',tuple(finger[4]),font,2,(255,255,255),2)
    # cv2.putText(frame,'finger6',tuple(finger[5]),font,2,(255,255,255),2)
    # cv2.putText(frame,'finger7',tuple(finger[6]),font,2,(255,255,255),2)
    # cv2.putText(frame,'finger8',tuple(finger[7]),font,2,(255,255,255),2)

    # Print bounding rectangle
    x, y, w, h = cv2.boundingRect(cnts)

    x2, y2, w2, h2 = cv2.boundingRect(cnts2)
    #img = cv2.rectangle(frame, (x, y), (x + w, y + h), (0, 255, 0), 2)

    #cv2.drawContours(frame, [hull], -1, (255, 255, 255), 2)

    ##### Show final image ########
    cv2.imshow("1", frame)
    cv2.imshow("2",frame2)
    ###############################

    # Print execution time
    # print time.time()-start_time

    # close the output video by pressing 'ESC'
    k = cv2.waitKey(5) & 0xFF
    if k == 27:
        break


    cz = (100-cx-cx2)/(cx+cx2)
    cz = -1*cz
    cz = cz*100
    print("cz = ",cz)
    #try:

        #sock.sendto((str(cx2)+","+str(cy2)).encode(), (UDP_IP, UDP_PORT) )
        #print((str(cx2)+","+str(cy2)))
    #except:
    #    pass

cap.release()
cv2.destroyAllWindows()
