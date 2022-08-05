# GameMode

Blueprint Reference
![image](https://user-images.githubusercontent.com/29656900/183051418-bcab9f2c-97f9-4316-8d57-338a8958892f.png)

- MyProjectGameModeBase.h 
```
public:
	AMyProjectGameModeBase();
```
- MyProjectGameMode.cpp
```

#include "MyProjectGameModeBase.h"
#include "MyCharacter.h"

AMyProjectGameModeBase::AMyProjectGameModeBase()
{
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/BP_MyCharacter.BP_MyCharacter_C"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
```

![image](https://user-images.githubusercontent.com/29656900/183054003-b4c798cd-0af0-4281-9f33-08819dd2c08c.png)

![image](https://user-images.githubusercontent.com/29656900/183054255-535c7432-b5e4-42e0-9d2c-edad580d22b8.png)


### World Setting
![image](https://user-images.githubusercontent.com/29656900/183053860-4d35259a-02f5-49ff-8a98-3bad10901a62.png)


![image](https://user-images.githubusercontent.com/29656900/183053792-11900c7e-e983-49ac-97c1-4b452eac373f.png)
