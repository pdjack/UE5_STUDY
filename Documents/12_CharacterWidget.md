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

![image](https://user-images.githubusercontent.com/29656900/186856695-77678134-8573-4c67-8797-8d503e34bcc8.png)

PublicDependencyModuleNames 에 UMG 라는 모듈을 추가하면 현재 프로젝트에서 위젯 컴포넌트를 사용할 수 있게 된다. UMG 모듈은 UE4 프로젝트 내 Source -> Runtime -> UMG 폴더에 위치해 있다.

UMG 모듈의 Public/Components 폴더에는 현재 사용 중인 WidgetComponent.h 파일이 있는데, 캐릭터의 구현부에서 이 헤더 파일을 추가해 컴포넌트를 생성하는 코드를 생성한다.


위젯 블루프린트 리소스 레퍼런스 복사 _C 를 붙여서 경로 사용

![image](https://user-images.githubusercontent.com/29656900/186904693-a0295257-db2b-485e-8fd8-9e1732a060db.png)


MyCharacter.cpp
```
...
#include "Components/WidgetComponent.h"
#include "MyCharacterWidget.h"

AMyCharacter::AMyCharacter()
{
    ...

    HPBarWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("HPBARWIDGET"));
    
    ...
    
    HPBarWidget->SetupAttachment(GetMesh());
    
    HPBarWidget->SetRelativeLocation(FVector(0.0f, 0.0f, 180.0f));
    HPBarWidget->SetWidgetSpace(EWidgetSpace::Screen);
    static ConstructorHelpers::FClassFinder<UUserWidget> UI_HUD(TEXT("/Game/UI/CharacterHPBar.CharacterHPBar_C"));
    if (UI_HUD.Succeeded())
    {
    HPBarWidget->SetWidgetClass(UI_HUD.Class);
    HPBarWidget->SetDrawSize(FVector2D(150.0f, 50.0f));
    }
}
```

### UI와의 데이터 연동

이제 캐릭터의 스탯이 변경되면 이를 UI 에 전달해 프로그레스바가 변경되도록 기능을 구현해보자. UI 의 로직은 애님 인스턴스와 유사하게 C++ 클래스에서 미리 만들어 제공할 수 있는데, 위젯 블루프린트가 사용하는 기반 C++ 클래스는 UserWidget 이다. UserWidget 을 상속받은 새로운 클래스를 생성하고 이름을 MyCharacterWidget 으로 정한다.
![image](https://user-images.githubusercontent.com/29656900/186857804-4d555dc7-d5f2-4804-8c3e-e0e25b06713c.png)


해당 클래스는 캐릭터 스탯 정보가 저장돼 있는 CharacterStatComponent와 연동해 캐릭터의 스탯이 변화할 때마다 프로그레스바의 내용을 업데이트할 것이다. 이번에도 상호 의존성을 가지지않게 CharacterStatComponent에 델리게이트를 하나 선언하고 컴포넌트의 HP 값이 변할 때마다 UI 위젯의 값이 자동으로 변경되도록 설계해본다.

CharacterStatComponent.h
```
...
DECLARE_MULTICAST_DELEGATE(FOnHPChangedDelegate);
...

public:
...
	void SetHP(float NewHP);
	float GetHPRatio();
	
	FOnHPChangedDelegate OnHPChanged;
...
```

CharacterStatComponent.cpp
```
void UCharacterStatComponent::SetNewLevel(int32 NewLevel)
{
	auto MyGameInstance = Cast<UMyGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	ensure(nullptr != MyGameInstance);
	CurrentStatData = MyGameInstance->GetMyCharacterData(NewLevel);
	if (nullptr != CurrentStatData)
	{
		Level = NewLevel;
		//CurrentHP = CurrentStatData->MaxHP;
		SetHP(CurrentStatData->MaxHP);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Level (%d) data doesn't exist"), NewLevel);
	}
}

void UCharacterStatComponent::SetDamage(float NewDamage)
{
	ensure(nullptr != CurrentStatData);
	SetHP(FMath::Clamp<float>(CurrentHP - NewDamage, 0.0f, CurrentStatData->MaxHP));
}

void UCharacterStatComponent::SetHP(float NewHP)
{
	CurrentHP = NewHP;
	OnHPChanged.Broadcast();
	if (CurrentHP <= KINDA_SMALL_NUMBER)
	{
		CurrentHP = 0.0f;
		OnHPIsZero.Broadcast();
	}
}

```




델리게이트 로직은 이미 CharacterStatComponent 에서 설정했으므로, UI 에서 캐릭터 컴포넌트에 연결하는 코드를 짜보자. 만약 UI 과 캐릭터가 서로 다른 액터라면 약 포인터를 사용하는 것이 바람직하다.


MyCharacterWidget.h
```
// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EngineMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MyCharacterWidget.generated.h"

/**
 * 
 */
UCLASS()
class CPPTEST01_API UMyCharacterWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void BindCharacterStat(class UCharacterStatComponent* NewCharacterStat);

protected:
	virtual void NativeConstruct() override;
	void UpdateHPWidget();

private:
	TWeakObjectPtr<class UCharacterStatComponent> CurrentCharacterStat;

	UPROPERTY()
	class UProgressBar* HPProgressBar;
};

```

```
#include "MyCharacterWidget.h"
#include "CharacterStatComponent.h"
#include "Components/ProgressBar.h"

void UMyCharacterWidget::BindCharacterStat(UCharacterStatComponent* NewCharacterStat)
{
	ABCHECK(nullptr != NewCharacterStat);

	CurrentCharacterStat = NewCharacterStat;

	NewCharacterStat->OnHPChanged.AddUObject(this, &UCharacterWidget::UpdateHPWidget);
}

void UMyCharacterWidget::NativeConstruct()
{
	Super::NativeConstruct();
	HPProgressBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("PB_HPBAR")));
	ABCHECK(nullptr != HPProgressBar);
	UpdateHPWidget();
}

void UMyCharacterWidget::UpdateHPWidget()
{
	if (CurrentCharacterStat.IsValid())
	{
		if (nullptr != HPProgressBar)
		{
			HPProgressBar->SetPercent(CurrentCharacterStat->GetHPRatio());
		}
	}
}
```


이제 캐릭터의 BeginPlay 함수에서 캐릭터 컴포넌트와 UI 위젯을 연결한다.

MyCharacter.cpp
```
...
#include "MyCharacterWidget.h"
void AMyCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	auto CharacterWidget = Cast<UCharacterWidget>(HPBarWidget->GetUserWidgetObject());
	if(nullptr != CharacterWidget)
	{
		CharacterWidget->BindCharacterStat(CharacterStat);
	}
}
```

위젯블루프린트 부모클래스를 MyCharacterWidget으로 변경한다.
![image](https://user-images.githubusercontent.com/29656900/186911337-a0c7e7d2-2978-4b52-b874-67bc881a2d51.png)
