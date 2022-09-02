### BehaviorTree

AI 폴더 생성

![image](https://user-images.githubusercontent.com/29656900/188062592-aa2539a0-4b61-49e8-99a7-500d30d8a13a.png)


인공지능 메뉴에서 BB_MyCharacter 이름으로 블랙보드 애셋을, BT_MyCharacter 라는 이름으로 비헤이비어 트리 애셋을 생성
![image](https://user-images.githubusercontent.com/29656900/188062684-ea946ddf-2e2c-4dd0-975d-eeec616f56e1.png)

![image](https://user-images.githubusercontent.com/29656900/188062761-d191efa3-c07e-4f49-9bdc-1876bc6c5fde.png)

블랙보드 : 인공지능의 판단에 사용하는 데이터 집합
비헤이비어 트리 : 블랙보드의 데이터에 기반해 설계한 트리 정보를 저장한 애셋

블랙보드에서는 우리가 사용할 키들을 추가한다. HomePos 와 PatrolPos 는 Vector 로, Target 은 Base Class 를 MyCharacter 로 갖는 Object Type 으로 만든다.

![image](https://user-images.githubusercontent.com/29656900/188063047-eb42c5fb-83b5-4b23-ac24-4c5dec0f3307.png)


BehaviourTree

태스크는 독립적으로 실행될 수 없고, 반드시 컴포짓(Composite) 노드를 거쳐 실행돼야 한다. 컴포짓에는 Selector, Sequence, Simple Parallel 이 있다.

Sequence : 연결된 Task 들이 False 결과가 나올 때까지 왼쪽에서 오른쪽으로 태스크를 계속 실행함
Selector : 지정된 키값이나 조건을 기준으로 조건에 맞는 노드를 실행
Simple Parallel : 자식으로 붙은 태스크들이 동시에 실행됨.  Finish Mode 설정을 통해서 메인 테스크가 완료되면, 서브 트리를 중단시키고 즉시(Immediate) 노드를 완료시킬수도 있고. 서브 트리를 완료할 때까지 지연(Delayed) 시킬지를 설정할 수 있음.


비헤이비어 트리는 5개의 요소로 구성된다. Root, Composite(Sequence, Selector, Simple Parallel), Decorator, Service, Task. 컴포짓은 위에서 설명했으니, 나머지 4개의 기능을 정리해보면 :

Root : 비헤이비어 트리의 시작점. 데코레이터나 서비스를 덧붙일 수 없음. 블랙보드 애셋을 설정할 수 있음.
Composite : Sequence, Selector, Simple Parallel (위에서 설명)
Decorator : 조걸절. 컴포짓이나 태스크에 붙여 분기나 노드가 실행될 것인지를 정의한다. 종류는 16 가지가 있다.
Service : 서비스는 컴포짓 노드에 분기가 실행되는 동안 정해진 빈도에 맞춰서 실행된다. 보통 검사를 하고 그 검사를 바탕으로 블랙보드의 내용을 업데이트하는데 사용된다.
Task : AI 의 이동이나 블랙보드의 값 조정 등의 작업을 한다. 태스크 노드의 종류에는 7 가지가 있다.

![image](https://user-images.githubusercontent.com/29656900/188063332-44c1a513-a885-4e59-a0e6-ef29b0d3c31b.png)