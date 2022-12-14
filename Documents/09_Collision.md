# Collision

### Add TraceChannel
- Name : Attack
- Default Response : Ignore
![image](https://user-images.githubusercontent.com/29656900/182603030-0e8968d5-5cf4-4ab2-9871-ae354264edc3.png)

- 프리셋 에서 Pawn에 대한 반응을 Block으로 설정 
![image](https://user-images.githubusercontent.com/29656900/184578133-d76bf5a7-4657-4794-88ca-40227db4acab.png)
![image](https://user-images.githubusercontent.com/29656900/184578151-fe049c78-84e7-4289-99e4-4bc4143b33f8.png)

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



MyCharacter.h
```
public:
...
virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent,
		class AController* EventInstigator, AActor* DamageCauser) override;
```
MyCharacter.cpp
```

float AMyCharacter::TakeDamage(float DamageAmount, FDamageEvent const & DamageEvent, AController * EventInstigator, AActor * DamageCauser)
{
	float FinalDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	UE_LOG(LogTemp, Warning, TEXT("Actor : %s took Damage : %f"), *GetName(), FinalDamage);


	return FinalDamage;
}
```

캐릭터BP 를 레벨에 드래그해서 놓은 다음 Attack을 해보자
![image](https://user-images.githubusercontent.com/29656900/184578041-d944355f-9089-4176-942c-88da222cd28c.png)
![image](https://user-images.githubusercontent.com/29656900/184578098-a813036f-f55a-4eda-9e91-93566ac759fb.png)


### Dead Animation
TakeDamage 받으면 Dead


MyAnimInstace.h
```
public :
	void SetDeadAnim();
private:
...
	UPROPERTY(BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
	bool bDreadAnim;
...
```
MyAnimInstance.cpp
```
void UMyAnimInstace::SetDeadAnim()
{
	bDeadAnim = true;
}

```
### 캐릭터 TakeDamage 함수 
MyCharacter.cpp

```
float AMyCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float FinalDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	UE_LOG(LogTemp, Warning, TEXT("Actor : %s took Damage : %f"), *GetName(), FinalDamage);

	if (MyAnim)
	{
		MyAnim->SetDeadAnim();
	}

	return FinalDamage;
}
```

### 애니메이션 블루프린트
![image](https://user-images.githubusercontent.com/29656900/184580345-02668d2a-8516-4cfc-b1f9-11260ef00d43.png)


### 죽을때 공격자 방향으로 캐릭터 로테이션 & 충돌되지 않도록 설정

```
float AMyCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float FinalDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	UE_LOG(LogTemp, Warning, TEXT("Actor : %s took Damage : %f"), *GetName(), FinalDamage);
	
	if (FinalDamage > 0.0f)
	{
		MyAnim->SetDeadAnim();

		FVector Dir = DamageCauser->GetActorLocation() - GetActorLocation();
		Dir.Z = 0.0f;
		FQuat LookAtRot = FRotationMatrix::MakeFromX(Dir).ToQuat();
		SetActorRotation(LookAtRot);
		SetActorEnableCollision(false);
	}
}
```

