### C++ AIController
AIController 를 상속받아서 MyAIController class 생성
![image](https://user-images.githubusercontent.com/29656900/188049913-4f6f2357-b311-4df2-9b5e-1abae5e35f84.png)


### MyCharacter.cpp
클래스 속성을 AIController 클래스 속성을 AMyAIController 로 정하고 AI 의 생성 옵션을 PlacedInWorldOrSpawned 로 설정한다. 그러면 앞으로 레벨에 배치하거나 새롭게 생성되는 MyCharacter 마다 AMyIController 액터가 생성되며, 플레이어가 조종하는 캐릭터를 제외한 모든 캐릭터는 MYAIController 의 지배를 받는다.
```
...
#include "MyAIController.h"

AMyCharacter::AMyCharacter()
{
    ...
    AIControllerClass = AMyAIController::StaticClass();
    AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}

```

