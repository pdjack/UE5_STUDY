
### MyCharacter.h
```
public:
...
UFUNCTION(Server, Reliable, WithValidation)
	void ServerAttack();

	UFUNCTION(NetMulticast, Reliable)
	void MulticastPlayAttackMontage();
```

### MyCharacter.cpp
```
void AMyCharacter::Attack()
{
	/*if (MyAnim)
	{
		MyAnim->PlayAttackMontage();
		IsAttacking = true;
	}*/
	ServerAttack();	
}

void AMyCharacter::ServerAttack_Implementation()
{	
	//if (IsDead()) return;
	if (MyAnim)
	{
		//MyAnim->PlayAttackMontage();
		MulticastPlayAttackMontage();
		IsAttacking = true;
		
	}
}

bool AMyCharacter::ServerAttack_Validate()
{
	return true;
}

void AMyCharacter::MulticastPlayAttackMontage_Implementation()
{
	if (MyAnim)
		MyAnim->PlayAttackMontage();
}

```



### CharacterStatComponent.h
```
private:
...
UPROPERTY(ReplicatedUsing = OnRep_CurrentHP, Transient, VisibleInstanceOnly, Category = Stat, Meta = (AllowPrivateAccess = true))
	float CurrentHP;

	UFUNCTION()
	void OnRep_CurrentHP();
```
### CharacterStatComponent.cpp
```
#include "Net/UnrealNetwork.h"
...

void UCharacterStatComponent::OnRep_CurrentHP()
{
	OnHPChanged.Broadcast();
	if (CurrentHP < KINDA_SMALL_NUMBER)
	{
		CurrentHP = 0.0f;
		OnHPIsZero.Broadcast();
	}
}
```
