
Animation Montage 생성
![image](https://user-images.githubusercontent.com/29656900/183559242-ff2607ac-15bc-49b7-9417-917326af0d59.png)


AssetBrowser 에서 애니메이션 선택해서 드래그해서 지정 ( DefaultSlot 영역 쪽에 드래그해야 함)
!![image](https://user-images.githubusercontent.com/29656900/183898089-0caa055e-97c3-4961-afb6-53301e5eba6b.png)

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
  	static ConstructorHelpers::FObjectFinder<UAnimMontage> ATTACK_MONTAGE(TEXT("/Game/Blueprints/KwangAttackMontage.KwangAttackMontage"));
	if (ATTACK_MONTAGE.Succeeded())
	{
		AttackMontage = ATTACK_MONTAGE.Object;
	}
}
```

Animation Bluprint 에 Montage DefaultSlot 추가 및 연결

![image](https://user-images.githubusercontent.com/29656900/183898564-018132ee-8481-438f-8e9e-b8cced9173ad.png)
