
![image](https://user-images.githubusercontent.com/29656900/183891830-ced4909d-0022-4dfa-b926-6d44cbb46bec.png)



MyCharacter.h
```
public:
...
  void Attack();

  UFUNCTION()
	void OnAttackMontageEnded(UAnimMontage* Montage, bool bInerrupted);
  
  
  private:
  ...
  bool IsAttacking = false;
  
  UPROPERTY()
  class UMyAnimInstance* MyAnim;
```


MyCharacter.cpp
```
...
#include "MyAnimInstance.h"
...

void AMyCharacter::BeginPlay()
{
	Super::BeginPlay();
	MyAnim = Cast<UMyAnimInstance>(GetMesh()->GetAnimInstance());

	MyAnim->OnMontageEnded.AddDynamic(this, &AMyCharacter::OnAttackMontageEnded);
}

void AMyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	...
	PlayerInputComponent->BindAction(TEXT("Attack"), EInputEvent::IE_Pressed, this, &AMyCharacter::Attack);
}

void AMyCharacter::Attack()
{
	if (MyAnim)
	{
		MyAnim->PlayAttackMontage();
		IsAttacking = true;
	}
}

void AMyCharacter::OnAttackMontageEnded(UAnimMontage* Montage, bool bInerrupted)
{
	IsAttacking = false;
}
```

### Montage AnimNotify
Norifies 트랙위에서 마우스 오른쪽 클릭
![image](https://user-images.githubusercontent.com/29656900/183911969-b9e251bf-24a6-4e56-997a-69ec55eb262c.png)

![image](https://user-images.githubusercontent.com/29656900/183912334-0f46bb95-d84e-48af-893d-3b542dabe7ee.png)

공격중 히트 타이밍에 맞게 AttackHitCheck Notify 위치 조정 
![image](https://user-images.githubusercontent.com/29656900/183912475-2d37f9d1-09eb-40ed-9a1a-1b2fb9ae35ec.png)


### AnimInstance 
MyAnimInstance.h
```
public:
...
	UFUNCTION()
	void AnimNotify_AttackHitCheck();
```
MyAnimInstance.cpp
```
void UMyAnimInstance::AnimNotify_AttackHitCheck()
{
	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("AnimNofify_HitCheck"));
}
```

### AttackHitCheck Delegate
MyAnimInstance.h
```
...
DECLARE_MULTICAST_DELEGATE(FOnAttackHitCheckDelegate);

UCLASS()
class PROJECTLK_API UMyAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
public:
...

FOnAttackHitCheckDelegate OnAttackHitCheck;
...
};
```

MyAnimInstance.cpp
```
void UMyAnimInstance::AnimNotify_AttackHitCheck()
{
	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("AnimNofify_HitCheck"));
		
	OnAttackHitCheck.Broadcast();
}
```


MyCharacter.cpp
```
void ALKCharacter::BeginPlay()
{
	Super::BeginPlay();
...
	if (MyAnim)
		MyAnim->OnAttackHitCheck.AddUObject(this, &AMyCharacter::AttackCheck);
...
}
```
### AttackHitCheck
MyCharacter.h
```

void AttackHitCheck();
```
MyCharacter.cpp
```
void AMyCharacter::AttackHitCheck()
{
	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("AttackHitCheck"));
}
```
