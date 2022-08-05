# C++ Class 생성

![image](https://user-images.githubusercontent.com/29656900/183031031-80480db5-e13a-44da-b3bc-369858dd9553.png)

![image](https://user-images.githubusercontent.com/29656900/183031165-36f25f16-23ad-4ca3-bc01-bf2b950b9a93.png)

![image](https://user-images.githubusercontent.com/29656900/183031289-1c301f30-fa13-4b97-a353-dccf746f5884.png)

# MyCharacter
코드 수정 :  #include "CoreMinimal.h" => #include "EngineMinimal.h"
![image](https://user-images.githubusercontent.com/29656900/183033033-ecc66c4d-cfbb-41db-999a-d0968b6cad5f.png)

- SpringArm , Camera 멤버 

```
<MyCharacter.h>

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
// Sets default values
AMyCharacter::AMyCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SPRINGARM"));
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("CAMERA"));
	Camera->SetupAttachment(SpringArm);

	SpringArm->SetupAttachment(GetCapsuleComponent());
	SpringArm->TargetArmLength = 400.0f;
	SpringArm->SetRelativeRotation(FRotator(-15.0f, 0.0f, 0.0f));

	
}
```

# Live Coding
- c++ 소스를 수정후에 Unreal Editor 에서 Recompile 을 한다.
![LiveCoding](https://user-images.githubusercontent.com/29656900/183032255-4f489bd9-0151-4578-9bc2-60b971b5bfbd.png)
![image](https://user-images.githubusercontent.com/29656900/183032441-3e94dad3-de06-4cff-9862-d051860537c5.png)

# Blueprint class

![image](https://user-images.githubusercontent.com/29656900/183037970-2255aa0f-4dc4-4711-b9c1-f7acdb683e7d.png)


![image](https://user-images.githubusercontent.com/29656900/183038253-cd4e1769-0366-4435-be8b-0638956d5f73.png)

![image](https://user-images.githubusercontent.com/29656900/183039124-23457649-54be-4036-974d-d1f21683659b.png)


![image](https://user-images.githubusercontent.com/29656900/183038918-4fa9ae5b-3c78-4e1a-bb1f-ef2216b515cb.png)


# Add Content Pack
![image](https://user-images.githubusercontent.com/29656900/183039661-56760c04-b0aa-47ad-a161-cdc2a0a10347.png)
![image](https://user-images.githubusercontent.com/29656900/183039801-f8b5e0a3-70a1-412f-8a40-97efcc78d07a.png)

# Set Skeletal Mesh

![image](https://user-images.githubusercontent.com/29656900/183040983-867d9ade-d980-42fb-9112-1664b8fd228a.png)
회전 -90
![image](https://user-images.githubusercontent.com/29656900/183041150-5b622b25-053f-430a-b06e-824cfe3ac515.png)
위치 조정 Z -88.0
![image](https://user-images.githubusercontent.com/29656900/183041401-0423b9a6-16a3-4a77-8c31-b7d32841042d.png)
Map에 드래그 해서 확인
![image](https://user-images.githubusercontent.com/29656900/183041711-c4b235f7-d64b-4fa3-a7a2-3bb3d4f1e611.png)


