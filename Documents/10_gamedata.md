# GameInstance

![image](https://user-images.githubusercontent.com/29656900/186420018-978d55a4-633a-4527-9b97-3408f2f71cef.png)

![image](https://user-images.githubusercontent.com/29656900/186420172-b3b8bd30-67b1-4a12-9f04-302521dff340.png)

![image](https://user-images.githubusercontent.com/29656900/186420262-a5998fe3-b774-4026-b92e-cd61498cc679.png)

프로젝트 세팅의 맵&모드 탭에 있는 GameInstance 항목을 변경
![image](https://user-images.githubusercontent.com/29656900/186420725-1633c23c-ca82-4e35-9271-c663d0d27179.png)



AMyGameInstance.h
```
#include "EngineMinimal.h"
#include "Engine/GameInstance.h"
#include "MyGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class CPPTEST01_API UMyGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	UMyGameInstance();

	virtual void Init() override;
};
```

AMyGameInstance.cpp
```
UMyGameInstance::UMyGameInstance()
{

}

void UMyGameInstance::Init()
{
	Super::Init();
	UE_LOG(LogTemp, Warning, TEXT("UMyGameInstance::Init()"));
}

```

# FMyCharacterData Class
### MyCharacterData.csv 파일
```
Name,Level,MaxMP,Attack,DropExp,NextExp
1,1,100,10,10,30
2,2,200,15,15,80
3,3,300,20,20,150
4,4,400,25,25,250
5,5,500,30,30,450
6,6,600,35,35,-1
```

### MyGameInstanceInstance.h 에 sturct 정의
Name열 데이터는 언리얼 엔진에서 자동으로 키값으로 사용하기 때문에 Name열은 선언에서 제외한다.
```
...
#include "Engine/DataTable.h"
...

USTRUCT(BlueprintType)
struct FMyCharacterData : public FTableRowBase
{
	GENERATED_BODY()

public:
	FMyCharacterData() : Level(1), MaxHP(100.0f), Attack(10.0f), DropExp(10), NextExp(30) {}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	int32 Level;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	float MaxHP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	float Attack;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	int32 DropExp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	int32 NextExp;

};


UCLASS()
class CPPTEST01_API UMyGameInstance : public UGameInstance
{
	GENERATED_BODY()
  ...
```
