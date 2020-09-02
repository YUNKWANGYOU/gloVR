import cv2


def Find(path):
    # 창 이름 설정

    cv2.namedWindow('image')

    # 이미지 파일 읽기

    img = cv2.imread(path, cv2.IMREAD_COLOR)

    # 이미지 보여주기

    cv2.imshow('image', img)

    # 창 esc 끄기

    while True:

        if cv2.waitKey(0) == 27:
            cv2.destroyWindow('image')

            break;

    return