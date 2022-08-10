
Animation Montage 생성
![image](https://user-images.githubusercontent.com/29656900/183559242-ff2607ac-15bc-49b7-9417-917326af0d59.png)


AssetBrowser 에서 애니메이션 선택해서 더블클릭 or 드래그해서 지정
![image](https://user-images.githubusercontent.com/29656900/183559096-f9fcae33-ec0d-4ccd-8171-92a266981b3e.png)

AnimInstance에 AttackMontage 코드 추가

MyAnimInstance.h
```
private:
...
UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
UAnimMontage* AttackMontage;
```
MyAnimInstance.cpp
```
AMyAnimInstance::AMyAnimInstance()
{
...
  	static ConstructorHelpers::FObjectFinder<UAnimMontage> ATTACK_MONTAGE(TEXT("Game/Blueprints/KwangAttackMontage.KwangAttackMontage"));
	if (ATTACK_MONTAGE.Succeeded())
	{
		AttackMontage = ATTACK_MONTAGE.Object;
	}
}
```
