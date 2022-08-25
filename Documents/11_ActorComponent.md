### CharacterStatComponent

툴>새로운 C++ 클래스.. 메뉴를 선택하고 다이얼로그에서 ActorComponent 를 선택한 후 오브젝트 이름은 CharacterStatComponent 라고 정한다.

![image](https://user-images.githubusercontent.com/29656900/186672608-1475078c-90ae-452d-8944-11469d6f8900.png)
![image](https://user-images.githubusercontent.com/29656900/186673296-93ba47b4-04e6-4577-aa29-b2ebfd615f35.png)


해당 클래스가 컴파일 되면 이제부터는 여타 컴포넌트와 동일하게 액터에 부착할 수 있다. MyCharacter 클래스에 새로운 컴포넌트를 멤버 변수로 선언한다.

MyCharacter.h
```
public:
...
UPROPERTY(VisibleAnywhere, Category=Stat)
class UCharacterStatComponent* CharacterStat;
...
```
MyCharacter.cpp
```
...
#include "CharacterStatComponent.h"
...

AMyCharacter::AMyCharacter()
{
...
  CharacterStat = CreateDefaultSubobject<UCharacterStatComponent>(TEXT("CHARACTERSTAT"));
}
```
### Actor Component 초기화 로직

InitializeComponent 함수는 액터의 PostInitializeComponents 함수가 호출되기 바로 전에 호출된다.

CharacterStatComponent.h
```
#include "EngineMinimal.h"

...

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CPPTEST01_API UCharacterStatComponent : public UActorComponent
{
...
protected:
...
  virtual void InitializeComponent() override;
  
...

```

CharacterStatComponent.cpp
```
// Sets default values for this component's properties
UCharacterStatComponent::UCharacterStatComponent()
{	
	PrimaryComponentTick.bCanEverTick = false;
  	bWantsInitializeComponent = true;

}

UCharacterStatComponent::InitializeComponent()
{
  	Super::InitializeComponent();
}

```

### CharacterStatComponent에 CurrentHP , Level 관련 변수 & 함수 추가 

CharacterStatComponent.h
```
public:
	void SetNewLevel(int32 NewLevel);
private:
	struct FMyCharacterData* CurrentStatData = nullptr;
	
	UPROPERTY(EditInstanceOnly, Category = Stat, Meta=(AllowPrivateAccess = true))
	int32 Level;
	
	UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat, Meta=(AllowPrivateAccess = true))
	float CurrentHP;
```
CharacterStatComponent.cpp
```
...
#include "MyGameInstance.h"

UCharacterStatComponent::UCharacterStatComponent()
{
...
	Level = 1;
}

void UCharacterStatComponent::InitializeComponent()
{
...
	SetNewLevel(Level);
}

void UCharacterStatComponent::SetNewLevel(int32 NewLevel)
{
	auto MyGameInstance = Cast<UMyGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	ensure(nullptr != MyGameInstance);
	CurrentStatData = MyGameInstance->GetMyCharacterData(NewLevel);
	if(nullptr != CurrentStatData)
	{
		Level = NewLevel;
		CurrentHP = CurrentStatData->MaxHP;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Level (%d) data doesn't exist"), NewLevel);
	}
}

```
CurrentHP값은 게임을 시작할때마다 변경되므로, 이 값을 보관하는것은 의미가 없고, Transient 키워드를 추가해 해당 속성을 직렬화에서 제외하는 것이 좋다.

### CharacterStatComponent 에 Damage 관련 코드 추가

CharacterStatComponent.h
```
...

DECLARE_MULTICAST_DELEGATE(FOnHPIsZeroDelegate);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CPPTEST01_API UCharacterStatComponent : public UActorComponent
{
	...

public:
...
	void SetDamage(float NewDamage);
	float GetAttack();
	
	FOnHPIsZeroDelegate OnHPIsZero;
...
```

CharacterStatComponent.cpp
```
void UCharacterStatComponent::SetDamage(float NewDamage)
{
	ensure(nullptr != CurrentStatData);
	CurrentHP = FMath::Clamp<float>(CurrentHP - NewDamage, 0.0f, CurrentStatData->MaxHP);
	if(CurrentHP <= 0.0f)
	{
		OnHPIsZero.Broadcast();
	}
}

float UCharacterStatComponent::GetAttack()
{
	ensure(nullptr != CurrentStatData, 0.0f);
	return CurrentStatData->Attack;
}

```

### MyCharacter

MyCharacter.cpp
```
void AMyCharacter::PostInitializeComponents()
{
...
	CharacterStat->OnHPIsZero.AddLambda([this]() -> void {
		
		UE_LOG(LogTemp, Warning, TEXT("OnHPIsZero"));
		MyAnim->SetAnimDead();

		//FVector Dir = DamageCauser->GetActorLocation() - GetActorLocation();
		//Dir.Z = 0.0f;
		//FQuat LookAtRot = FRotationMatrix::MakeFromX(Dir).ToQuat();
		//SetActorRotation(LookAtRot);

		SetActorEnableCollision(false);

	});
	
}

float AMyCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float FinalDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	
	UE_LOG(LogTemp, Warning, TEXT("ACtor : %s took Damage : %f"), *GetName(), FinalDamage);
	if (FinalDamage > 0.0f)
	{
		CharacterStat->SetDamage(FinalDamage);

		/*MyAnim->SetAnimDead();

		FVector Dir = DamageCauser->GetActorLocation() - GetActorLocation();
		Dir.Z = 0.0f;
		FQuat LookAtRot = FRotationMatrix::MakeFromX(Dir).ToQuat();
		SetActorRotation(LookAtRot);
		SetActorEnableCollision(false);*/
	}
	return FinalDamage;
}


void AMyCharacter::AttackHitCheck()
{
...
	if (bResult)
	{
		if (HitResult.GetActor())
		{
			UE_LOG(LogTemp, Warning, TEXT("Hit ACtor Name : %s"), *HitResult.GetActor()->GetName());

			FDamageEvent DamageEvent;
			HitResult.GetActor()->TakeDamage(CharacterStat->GetAttack(), DamageEvent, GetController(), this);
		}
	}
}
```
