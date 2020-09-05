# gloVR

gloVR software

목차
1. 개요
2. 프로토콜
3. arduino
4. openCV
5. Unity3D

# 1. 개요
gloVR 협업을 위한 깃헙링크
<div>
  <img src = "https://user-images.githubusercontent.com/44994031/92015175-5cb7ae00-ed8b-11ea-8fcc-457e3322e03b.png" width = "400px" height = "400px">
</div>

# 2. 프로토콜

목적 : 프로그램 간 원활한 통신과, 팀원들 간 데이터 형식이 달라 발생하는 문제를 방지

1.Arduino to Unity
|index|0|1|2|3|4|5|6|7|8|9|10|11|12|13|
|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|
|1byte|str|flex1|flex2|flex3|flex4|flex5|AcX|AcY|AcZ|ZyX|ZyY|ZyZ|end|

2.Unity to Arduino
|index|0|1|2|3|4|5|6|
|-|-|-|-|-|-|-|-|
|1byte|str|sv1|sv2|sv3|sv4|sv5|end|

3.Python(openCV) to Unity
not yet


# 3. arduino

목적 : gloVR에 사용되는 아두이노를 성공적으로 제어
1. 유니티에 블루투스 모듈을 통해서 플렉스센서, 자이로센서의 데이터 보내기
2. 유니티로부터 블루투스 모듈을 통해서 서보모터를 제어하기 위한 데이터를 받기

통합 코드/ 서보모터 제어 코드/ 자이로 센서 제어 코드/ 플렉스 센서 제어 코드/ 블루투스 모듈 제어 코드/ 테스트 코드/ 로 나눠짐

사용방법 : 제어코드와 통합코드는 함부로 변경하지 않는다.

테스트 코드에 복붙한 뒤 테스트.

자신이 변경한 코드가 성공적으로 기능을 수행하면 제어코드와 통합코드를 수정

# 4. openCV
목적 : openCV를 통해서 hand detection 을 한 뒤, 그 데이터를 UDP를 통해서 유니티로 전달

통합 프로그램/hand detect 프로그램/ UDP socket 프로그램 / 유윤광 폴더/ 박정무 폴더/ 테스트 프로그램 으로 구성

사용방법 : 테스트 프로그램을 통해 테스트한 뒤 잘 작동하면 각각의 프로그램을 수정

# 5. Unity3D
목적 : 캐치볼 게임 만들기

각종 cs파일로 구성

사용법 : cs파일만 업로드, cs파일의 첫줄에 이 파일의 목적, 역할, unity에 적용하는 방법을 작성
