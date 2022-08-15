# Collision

### Add TraceChannel
- Name : Attack
- Default Response : Ignore
![image](https://user-images.githubusercontent.com/29656900/182603030-0e8968d5-5cf4-4ab2-9871-ae354264edc3.png)

Config/DefaultEngine.ini

![image](https://user-images.githubusercontent.com/29656900/182603137-a6a790a0-767a-4328-b86c-1b27f82ecdf0.png)


```
void AMyCharacter::AttackHitCheck()
{
	float AttackRange = 200.0f;
	float AttackRadius = 50.0f;

	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("AMyCharacter::AttackHitCheck()"));

	FHitResult HitResult;
	FCollisionQueryParams Params(NAME_None, false, this);
	bool bResult = GetWorld()->SweepSingleByChannel(
		HitResult,
		GetActorLocation(),
		GetActorLocation() + GetActorForwardVector() * AttackRange ,
		FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel1,
		FCollisionShape::MakeSphere(AttackRadius),
		Params);

#if ENABLE_DRAW_DEBUG

	FVector TraceVec = GetActorForwardVector() * AttackRange;
	FVector Center = GetActorLocation() + TraceVec * 0.5f;
	float HalfHeight = AttackRange * 0.5f + AttackRadius;

	//Capsule 의 Z (Up)를 TraceVec 방향으로 회전 
	FQuat CapsuleRot = FRotationMatrix::MakeFromZ(TraceVec).ToQuat();
	FColor DrawColor = bResult ? FColor::Green : FColor::Red;

	float DebugLifeTime = 5.0f;

	DrawDebugCapsule(GetWorld(),
		Center,
		HalfHeight,
		AttackRadius,
		CapsuleRot,
		DrawColor,
		false,
		DebugLifeTime);

#endif

	if (bResult)
	{
		if (HitResult.GetActor())
		{
			UE_LOG(LogTemp, Warning, TEXT("Hit ACtor Name : %s"), *HitResult.GetActor()->GetName());

			FDamageEvent DamageEvent;
			HitResult.GetActor()->TakeDamage(50.0f, DamageEvent, GetController(),this);
		}
	}

}
```


TakeDamage 받으면 Dead

![image](https://user-images.githubusercontent.com/29656900/182615041-c13245ec-e4db-4f42-a777-1b23c1c19922.png)

MyCharacter.h
```
public:
...
virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent,
		class AController* EventInstigator, AActor* DamageCauser) override;
```
MyCharacter.cpp
```

float ALKCharacter::TakeDamage(float DamageAmount, FDamageEvent const & DamageEvent, AController * EventInstigator, AActor * DamageCauser)
{
	float FinalDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	UE_LOG(LogTemp, Warning, TEXT("Actor : %s took Damage : %f"), *GetName(), FinalDamage);


	return FinalDamage;
}
```

캐릭터BP 를 레벨에 드래그해서 놓은 다음 Attack을 해보자
![image](https://user-images.githubusercontent.com/29656900/184578041-d944355f-9089-4176-942c-88da222cd28c.png)

디버그로 캡슐을 그릴때 붉은 색은 충돌감지가 안됬다는 뜻
TraceChannel 에서 Pawn에 대한 반응을 설정해줘야 한다.
![image](https://user-images.githubusercontent.com/29656900/184578133-d76bf5a7-4657-4794-88ca-40227db4acab.png)
![image](https://user-images.githubusercontent.com/29656900/184578151-fe049c78-84e7-4289-99e4-4bc4143b33f8.png)


![image](https://user-images.githubusercontent.com/29656900/184578098-a813036f-f55a-4eda-9e91-93566ac759fb.png)


# Weapon

MyCharacter.h 
```
public:
UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon)
UStaticMeshComponent* Weapon;
	
```
MyCharacter.cpp
```
AMyCharacter::AMyCharacter()
{
...
	Weapon = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WEAPON"));
	//StaticMesh'/Game/ModularRPGHeroesPolyart/Meshes/Weapons/Axe01SM.Axe01SM'
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SM_WEAPON(TEXT("/Game/ModularRPGHeroesPolyart/Meshes/Weapons/Axe01SM.Axe01SM"));
	if (SM_WEAPON.Succeeded())
	{
		Weapon->SetStaticMesh(SM_WEAPON.Object);
	}
...
}

void AMyCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	MyAnim = Cast<UMyAnimInstance>(GetMesh()->GetAnimInstance());
	MyAnim->OnAttackHitCheck.AddUObject(this, &AMyCharacter::AttackHitCheck);
	
	FName WeaponSocket(TEXT("hand_rSocket"));
	if (GetMesh()->DoesSocketExist(WeaponSocket))
	{

		Weapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, TEXT("hand_rSocket"));
	}
}

```
