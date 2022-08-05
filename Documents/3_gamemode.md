# GameMode

![image](https://user-images.githubusercontent.com/29656900/183051418-bcab9f2c-97f9-4316-8d57-338a8958892f.png)

- MyProjectGameModeBase.h 생성자 추가
```
public:
	AMyProjectGameModeBase();
```

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
