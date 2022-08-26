### Character Widget UI

UI 폴더 추가

![image](https://user-images.githubusercontent.com/29656900/186852842-fc976f1a-c184-4023-8d72-027d3dd46e9f.png)

Widget Bluprint 생성 하고 이름은 CharacterHPBar
![image](https://user-images.githubusercontent.com/29656900/186852986-77904fe3-ed31-46c6-b0b5-d6fb930d6e2e.png)

![image](https://user-images.githubusercontent.com/29656900/186853236-b69c079b-7250-481f-9993-9f1e1b457734.png)


Fill Screen 에서  Custom 선택

![image](https://user-images.githubusercontent.com/29656900/186854011-b1d9e5ee-6445-470a-9072-653e3b37cd69.png)


Width 150 Height 50

![image](https://user-images.githubusercontent.com/29656900/186854164-f54d2685-63c5-41d6-a4c8-0910cebe2006.png)


VerticalBox , Spacer , Progress Bar, Spacer

![image](https://user-images.githubusercontent.com/29656900/186854874-53ce2555-ffb7-4df7-bc85-c5ebd193bd49.png)


Progress Bar 선택후 세팅

![image](https://user-images.githubusercontent.com/29656900/186855365-9974a2c1-6df7-476c-a6e3-2f6e22048b0d.png)

![image](https://user-images.githubusercontent.com/29656900/186855596-43195c3c-677a-4d5b-b085-dd357988f37f.png)


### 모듈과 빌드 설정

이제 UI 를 캐릭터에 부착해보자. 언리얼은 액터에 UI 위젯을 부착할 수 있도록 UWidgetComponent 클래스를 제공한다.

MyCharacter.h
```
...
UPROPERTY(VisibleAnywhere, Category = UI)
class UWidgetComponent* HPBarWidget;
...
```
HPBarWidget 을 선언했지만, 컴파일하면 '확인할 수 없는 외부 참조' 에러 메시지가 나온다. 이는 현재 프로젝트 설정에 UI 에 관련된 엔진 모듈을 지정하지 않았기 때문이다. [프로젝트명].Build.cs 파일을 보면 현재 사용하는 모듈을 확인할 수 있다. 
UMG 를 추가한다.
![image](https://user-images.githubusercontent.com/29656900/186856695-77678134-8573-4c67-8797-8d503e34bcc8.png)
