
### ParticleSystemComponent

MyCharacter.h
```
public:
...
UPROPERTY(VisibleAnywhere, Category = Effect)
UParticleSystemComponent* Effect;
```

MyCharacter.cpp
```
AMyCharacter::AMyCharacter()
{
...
 Effect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("EFFECT"));
	Effect->SetupAttachment(RootComponent);
	Effect->bAutoActivate = false;
  ...
}


  
```

### Effect 설정 

에디터에서 BP_MyCharacterKwang  오픈

Effect 컴포넌트 선택해서 리소스 변경

![image](https://user-images.githubusercontent.com/29656900/194736145-60130c3e-cbe5-446e-9173-5221f1c3a40b.png)


### Effect Activate

float AMyCharacter::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent,
	class AController* EventInstigator, AActor* DamageCauser)
{
  ...
  Effect->Activate()
  ...
 }
