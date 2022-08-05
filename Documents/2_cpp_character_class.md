# C++ Class 생성

![image](https://user-images.githubusercontent.com/29656900/183031031-80480db5-e13a-44da-b3bc-369858dd9553.png)

![image](https://user-images.githubusercontent.com/29656900/183031165-36f25f16-23ad-4ca3-bc01-bf2b950b9a93.png)

![image](https://user-images.githubusercontent.com/29656900/183031289-1c301f30-fa13-4b97-a353-dccf746f5884.png)

# MyCharacter.h
코드 수정 :  #include "CoreMinimal.h" => #include "EngineMinimal.h"
![image](https://user-images.githubusercontent.com/29656900/183033033-ecc66c4d-cfbb-41db-999a-d0968b6cad5f.png)

- SpringArm , Camera 멤버 
MyCharacter.h
```
public :
...
	UPROPERTY(VisibleAnywhere, Category = Camera)
	USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere, Category = Camera)
	UCameraComponent* Camera;
...
```
MyCharacter.cpp
```

```

# Live Coding
- c++ 소스를 수정후에 Unreal Editor 에서 Recompile 을 한다.
![LiveCoding](https://user-images.githubusercontent.com/29656900/183032255-4f489bd9-0151-4578-9bc2-60b971b5bfbd.png)
![image](https://user-images.githubusercontent.com/29656900/183032441-3e94dad3-de06-4cff-9862-d051860537c5.png)

