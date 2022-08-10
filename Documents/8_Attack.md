
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

